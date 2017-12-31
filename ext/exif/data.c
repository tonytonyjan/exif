#include "data.h"
#include "ruby.h"

#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-log.h>
#include <stdint.h>
#include <time.h>

extern VALUE rb_mExif, rb_eNotReadable, rb_eIFDNotFound;
extern const char *exif_entry_to_ivar(ExifEntry *entry);

VALUE rb_cExifData;
static const char *ifd_name_mapping[] = {"ifd0", "ifd1", "exif", "gps",
                                         "interoperability"};
static const char *attr_readers[] = {"ifds",
                                     "aperture_value",
                                     "artist",
                                     "battery_level",
                                     "bits_per_sample",
                                     "brightness_value",
                                     "cfa_pattern",
                                     "cfa_repeat_pattern_dim",
                                     "color_space",
                                     "components_configuration",
                                     "compressed_bits_per_pixel",
                                     "compression",
                                     "contrast",
                                     "copyright",
                                     "custom_rendered",
                                     "date_time",
                                     "date_time_digitized",
                                     "date_time_original",
                                     "device_setting_description",
                                     "digital_zoom_ratio",
                                     "document_name",
                                     "exif_ifd_pointer",
                                     "exif_version",
                                     "exposure_bias_value",
                                     "exposure_index",
                                     "exposure_mode",
                                     "exposure_program",
                                     "exposure_time",
                                     "file_source",
                                     "fill_order",
                                     "flash",
                                     "flash_energy",
                                     "flash_pix_version",
                                     "fnumber",
                                     "focal_length",
                                     "focal_length_in_35mm_film",
                                     "focal_plane_resolution_unit",
                                     "focal_plane_x_resolution",
                                     "focal_plane_y_resolution",
                                     "gain_control",
                                     "gamma",
                                     "gps_altitude",
                                     "gps_altitude_ref",
                                     "gps_area_information",
                                     "gps_date_stamp",
                                     "gps_dest_bearing",
                                     "gps_dest_bearing_ref",
                                     "gps_dest_distance",
                                     "gps_dest_distance_ref",
                                     "gps_dest_latitude",
                                     "gps_dest_latitude_ref",
                                     "gps_dest_longitude",
                                     "gps_dest_longitude_ref",
                                     "gps_differential",
                                     "gps_dop",
                                     "gps_img_direction",
                                     "gps_img_direction_ref",
                                     "gps_info_ifd_pointer",
                                     "gps_latitude",
                                     "gps_latitude_ref",
                                     "gps_longitude",
                                     "gps_longitude_ref",
                                     "gps_map_datum",
                                     "gps_measure_mode",
                                     "gps_processing_method",
                                     "gps_satellites",
                                     "gps_speed",
                                     "gps_speed_ref",
                                     "gps_status",
                                     "gps_time_stamp",
                                     "gps_track",
                                     "gps_track_ref",
                                     "gps_version_id",
                                     "image_description",
                                     "image_length",
                                     "image_resources",
                                     "image_unique_id",
                                     "image_width",
                                     "inter_color_profile",
                                     "interoperability_ifd_pointer",
                                     "interoperability_index",
                                     "interoperability_version",
                                     "iptc_naa",
                                     "iso_speed_ratings",
                                     "jpeg_interchange_format",
                                     "jpeg_interchange_format_length",
                                     "jpeg_proc",
                                     "light_source",
                                     "make",
                                     "maker_note",
                                     "max_aperture_value",
                                     "metering_mode",
                                     "model",
                                     "new_cfa_pattern",
                                     "new_subfile_type",
                                     "oecf",
                                     "orientation",
                                     "padding",
                                     "photometric_interpretation",
                                     "pixel_x_dimension",
                                     "pixel_y_dimension",
                                     "planar_configuration",
                                     "primary_chromaticities",
                                     "print_image_matching",
                                     "reference_black_white",
                                     "related_image_file_format",
                                     "related_image_length",
                                     "related_image_width",
                                     "related_sound_file",
                                     "resolution_unit",
                                     "rows_per_strip",
                                     "samples_per_pixel",
                                     "saturation",
                                     "scene_capture_type",
                                     "scene_type",
                                     "sensing_method",
                                     "sharpness",
                                     "shutter_speed_value",
                                     "software",
                                     "spatial_frequency_response",
                                     "spectral_sensitivity",
                                     "strip_byte_counts",
                                     "strip_offsets",
                                     "sub_ifds",
                                     "sub_sec_time",
                                     "sub_sec_time_digitized",
                                     "sub_sec_time_original",
                                     "subject_area",
                                     "subject_distance",
                                     "subject_distance_range",
                                     "subject_location",
                                     "tiff_ep_standard_id",
                                     "time_zone_offset",
                                     "transfer_function",
                                     "transfer_range",
                                     "user_comment",
                                     "white_balance",
                                     "white_point",
                                     "x_resolution",
                                     "xml_packet",
                                     "xp_author",
                                     "xp_comment",
                                     "xp_keywords",
                                     "xp_subject",
                                     "xp_title",
                                     "y_resolution",
                                     "ycbcr_coefficients",
                                     "ycbcr_positioning",
                                     "ycbcr_sub_sampling"};

static VALUE new (VALUE self, VALUE input);
static VALUE dump(VALUE self);
static VALUE brackets(VALUE self, VALUE ifd_symbol);
static void each_content(ExifContent *ec, void *user_data);
static void each_entry(ExifEntry *, void *user_data);
static VALUE exif_entry_to_rb_value(ExifEntry *);
static VALUE rational_to_num(ExifRational rational);
static VALUE srational_to_num(ExifSRational srational);

static void exif_corrupt_log_func(ExifLog *log, ExifLogCode code,
                                  const char *domain, const char *format,
                                  va_list args, void *data) {
  if (code == EXIF_LOG_CODE_CORRUPT_DATA) {
    unsigned int *corrupt = (unsigned int *)data;
    *corrupt = 1;
  }
}

void init_data() {
  int length;

  rb_cExifData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  length = sizeof(attr_readers) / sizeof(char *);
  for (int i = 0; i < length; ++i)
    rb_define_attr(rb_cExifData, attr_readers[i], 1, 0);
  rb_define_alias(rb_cExifData, "to_h", "ifds");
  rb_define_singleton_method(rb_cExifData, "new", new, 1);
  rb_define_method(rb_cExifData, "dump", dump, 0);
  rb_define_method(rb_cExifData, "[]", brackets, 1);
}

VALUE new (VALUE self, VALUE input) {
  ExifData *ed;

  if (TYPE(input) != T_STRING && !rb_respond_to(input, rb_intern("read")))
    rb_raise(rb_eTypeError, "wrong argument type %s (expected String or IO)",
             rb_obj_classname(input));

  if (TYPE(input) == T_STRING) {
    unsigned int corrupt = 0;
    ExifLog *log = exif_log_new();
    exif_log_set_func(log, exif_corrupt_log_func, (void *)&corrupt);
    ed = exif_data_new();
    exif_data_log(ed, log);
    exif_data_load_data(ed, (unsigned char *)RSTRING_PTR(input),
                        (uint32_t)RSTRING_LEN(input));
    exif_log_unref(log);
    if (corrupt) {
      exif_data_free(ed);
      ed = NULL;
    }
  } else {
    ExifLoader *loader = exif_loader_new();
    VALUE buffer;
    while (1) {
      buffer = rb_funcall(input, rb_intern("read"), 1, INT2FIX(1024));
      if (TYPE(buffer) == T_NIL)
        break;
      if (!exif_loader_write(loader, (unsigned char *)RSTRING_PTR(buffer),
                             (uint32_t)RSTRING_LEN(buffer)))
        break;
    }
    ed = exif_loader_get_data(loader);
    exif_loader_unref(loader);
  }
  if (!ed)
    rb_raise(rb_eNotReadable, "File not readable or no EXIF data in file.");

  VALUE rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  rb_iv_set(rb_data, "@ifds", rb_hash_new());
  exif_data_foreach_content(ed, each_content, &rb_data);
  return rb_data;
}

static VALUE dump(VALUE self) {
  ExifData *ed;
  Data_Get_Struct(self, ExifData, ed);
  exif_data_dump(ed);
  return Qnil;
}

static VALUE brackets(VALUE self, VALUE ifd_symbol) {
  return rb_hash_aref(rb_iv_get(self, "@ifds"), ifd_symbol);
}

static void each_content(ExifContent *ec, void *self_ptr) {
  ExifIfd ifd;

  ifd = exif_content_get_ifd(ec);
  if (ifd == EXIF_IFD_COUNT)
    rb_raise(rb_eIFDNotFound, "Con't get IFD.");
  rb_hash_aset(rb_iv_get(*(VALUE *)self_ptr, "@ifds"),
               ID2SYM(rb_intern(ifd_name_mapping[ifd])), rb_hash_new());
  exif_content_foreach_entry(ec, each_entry, self_ptr);
}

static void each_entry(ExifEntry *entry, void *self_ptr) {
  VALUE value;
  const char *ivar_name;

  ivar_name = exif_entry_to_ivar(entry);
  value = exif_entry_to_rb_value(entry);
  rb_hash_aset(rb_hash_aref(rb_iv_get(*(VALUE *)self_ptr, "@ifds"),
                            ID2SYM(rb_intern(
                                ifd_name_mapping[exif_entry_get_ifd(entry)]))),
               ID2SYM(rb_intern(ivar_name + 1)), value);
  if (TYPE(rb_ivar_defined(*(VALUE *)self_ptr, rb_intern(ivar_name))) ==
      T_FALSE) {
    rb_iv_set(*(VALUE *)self_ptr, ivar_name, value);
  }
}

#define TYPECAST(value1, value2)                                               \
  do {                                                                         \
    if (entry->components > 1) {                                               \
      ret = rb_ary_new2(entry->components);                                    \
      for (i = 0; i < entry->components; i++)                                  \
        rb_ary_push(ret, value1);                                              \
    } else                                                                     \
      ret = value2;                                                            \
  } while (0)

#define TYPECAST_BYTE(c_to_rb, type)                                           \
  TYPECAST(c_to_rb((type)entry->data[i]), c_to_rb((type)entry->data[0]))

#define TYPECAST_EXIF(c_to_rb, exif_get)                                       \
  TYPECAST(c_to_rb(exif_get_##exif_get(entry->data + i * size, order)),        \
           c_to_rb(exif_get_##exif_get(entry->data, order)))

static VALUE exif_entry_to_rb_value(ExifEntry *entry) {
  ExifData *data;
  ExifByteOrder order;
  VALUE ret;
  size_t i;
  unsigned char size;

  data = entry->parent->parent;
  order = exif_data_get_byte_order(data);
  ret = Qnil;
  size = exif_format_get_size(entry->format);

  switch (entry->format) {
  case EXIF_FORMAT_UNDEFINED:
    ret = rb_str_new((const char *)entry->data, entry->size);
    break;
  case EXIF_FORMAT_BYTE:
    TYPECAST_BYTE(INT2FIX, uint8_t);
  case EXIF_FORMAT_SBYTE:
    TYPECAST_BYTE(INT2FIX, int8_t);
    break;
  case EXIF_FORMAT_SHORT:
    TYPECAST_EXIF(INT2FIX, short);
    break;
  case EXIF_FORMAT_SSHORT:
    TYPECAST_EXIF(INT2FIX, sshort);
    break;
  case EXIF_FORMAT_LONG:
    TYPECAST_EXIF(ULONG2NUM, long);
    break;
  case EXIF_FORMAT_SLONG:
    TYPECAST_EXIF(LONG2NUM, slong);
    break;
  case EXIF_FORMAT_ASCII:
    ret = rb_str_new2((const char *)entry->data);
    break;
  case EXIF_FORMAT_RATIONAL:
    TYPECAST_EXIF(rational_to_num, rational);
    break;
  case EXIF_FORMAT_SRATIONAL:
    TYPECAST_EXIF(srational_to_num, srational);
    break;
  case EXIF_FORMAT_DOUBLE:
  case EXIF_FORMAT_FLOAT:
    ret = rb_float_new(*(double *)entry->data);
    break;
  }

  return ret;
}

#undef TYPECAST
#undef TYPECAST_BYTE
#undef TYPECAST_EXIF

VALUE rational_to_num(ExifRational rational) {
  if (rational.numerator == 0 && rational.denominator == 0)
    return DBL2NUM(NAN);
  else if (rational.denominator == 0)
    return DBL2NUM(INFINITY);
  else
    return rb_rational_new(ULONG2NUM(rational.numerator),
                           ULONG2NUM(rational.denominator));
}

VALUE srational_to_num(ExifSRational srational) {
  if (srational.numerator == 0 && srational.denominator == 0)
    return DBL2NUM(NAN);
  else if (srational.denominator == 0)
    return DBL2NUM(srational.numerator > 0 ? INFINITY : -INFINITY);
  else
    return rb_rational_new(LONG2NUM(srational.numerator),
                           LONG2NUM(srational.denominator));
}
