#include "data.h"
#include "libjpeg/jpeg-data.h"
#include "ruby.h"

#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>
#include <time.h>
#include <stdint.h>

extern VALUE rb_mExif, rb_eNotReadble, rb_eIFDNotFound;
extern const char* exif_entry_to_ivar(ExifEntry* entry);

VALUE rb_cExifData;
static const char* ifd_name_mapping[] = {"ifd0", "ifd1", "exif", "gps", "interoperability"};
static const char* attr_readers[] = {"ifds", "aperture_value", "artist", "battery_level", "bits_per_sample", "brightness_value", "cfa_pattern", "cfa_repeat_pattern_dim", "color_space", "components_configuration", "compressed_bits_per_pixel", "compression", "contrast", "copyright", "custom_rendered", "date_time", "date_time_digitized", "date_time_original", "device_setting_description", "digital_zoom_ratio", "document_name", "exif_ifd_pointer", "exif_version", "exposure_bias_value", "exposure_index", "exposure_mode", "exposure_program", "exposure_time", "file_source", "fill_order", "flash", "flash_energy", "flash_pix_version", "fnumber", "focal_length", "focal_length_in_35mm_film", "focal_plane_resolution_unit", "focal_plane_x_resolution", "focal_plane_y_resolution", "gain_control", "gamma", "gps_altitude", "gps_altitude_ref", "gps_area_information", "gps_date_stamp", "gps_dest_bearing", "gps_dest_bearing_ref", "gps_dest_distance", "gps_dest_distance_ref", "gps_dest_latitude", "gps_dest_latitude_ref", "gps_dest_longitude", "gps_dest_longitude_ref", "gps_differential", "gps_dop", "gps_img_direction", "gps_img_direction_ref", "gps_info_ifd_pointer", "gps_latitude", "gps_latitude_ref", "gps_longitude", "gps_longitude_ref", "gps_map_datum", "gps_measure_mode", "gps_processing_method", "gps_satellites", "gps_speed", "gps_speed_ref", "gps_status", "gps_time_stamp", "gps_track", "gps_track_ref", "gps_version_id", "image_description", "image_length", "image_resources", "image_unique_id", "image_width", "inter_color_profile", "interoperability_ifd_pointer", "interoperability_index", "interoperability_version", "iptc_naa", "iso_speed_ratings", "jpeg_interchange_format", "jpeg_interchange_format_length", "jpeg_proc", "light_source", "make", "maker_note", "max_aperture_value", "metering_mode", "model", "new_cfa_pattern", "new_subfile_type", "oecf", "orientation", "padding", "photometric_interpretation", "pixel_x_dimension", "pixel_y_dimension", "planar_configuration", "primary_chromaticities", "print_image_matching", "reference_black_white", "related_image_file_format", "related_image_length", "related_image_width", "related_sound_file", "resolution_unit", "rows_per_strip", "samples_per_pixel", "saturation", "scene_capture_type", "scene_type", "sensing_method", "sharpness", "shutter_speed_value", "software", "spatial_frequency_response", "spectral_sensitivity", "strip_byte_counts", "strip_offsets", "sub_ifds", "sub_sec_time", "sub_sec_time_digitized", "sub_sec_time_original", "subject_area", "subject_distance", "subject_distance_range", "subject_location", "tiff_ep_standard_id", "time_zone_offset", "transfer_function", "transfer_range", "user_comment", "white_balance", "white_point", "x_resolution", "xml_packet", "xp_author", "xp_comment", "xp_keywords", "xp_subject", "xp_title", "y_resolution", "ycbcr_coefficients", "ycbcr_positioning", "ycbcr_sub_sampling"};

static VALUE new(VALUE self, VALUE input);
static VALUE dump(VALUE self);
static VALUE brackets(VALUE self, VALUE ifd_symbol);
static void each_content(ExifContent *ec, void *user_data);
static void each_entry(ExifEntry *, void *user_data);
static VALUE exif_entry_to_rb_value(ExifEntry *);

void init_data(){
  int length;

  rb_cExifData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  length = sizeof(attr_readers) / sizeof(char*);
  for(int i = 0; i < length; ++i) rb_define_attr(rb_cExifData, attr_readers[i], 1, 0);
  rb_define_alias(rb_cExifData, "to_h", "ifds");
  rb_define_singleton_method(rb_cExifData, "new", new, 1);
  rb_define_method(rb_cExifData, "dump", dump, 0);
  rb_define_method(rb_cExifData, "[]", brackets, 1);
}

VALUE new(VALUE self, VALUE input){
  ExifData* ed;
  VALUE read_data;

  switch (TYPE(input)) {
  case T_STRING:
    read_data = input;
    break;
  case T_FILE:
    read_data = rb_funcall(input, rb_intern("read"), 0);
    break;
  default:
    rb_raise(rb_eTypeError, "wrong argument type %s (expected String or IO)", rb_obj_classname(input));
  }

	ExifLoader *loader = exif_loader_new();
	exif_loader_write(loader, (unsigned char*)RSTRING_PTR(read_data), RSTRING_LEN(read_data));
	ed = exif_loader_get_data(loader);
	exif_loader_unref (loader);
  if(!ed) rb_raise(rb_eNotReadble, "File not readable or no EXIF data in file.");


  VALUE rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  rb_iv_set(rb_data, "@ifds", rb_hash_new());
  exif_data_foreach_content(ed, each_content, &rb_data);
  return rb_data;
}

static VALUE dump(VALUE self){
  ExifData *ed;
  Data_Get_Struct(self, ExifData, ed);
  exif_data_dump(ed);
  return Qnil;
}

static VALUE brackets(VALUE self, VALUE ifd_symbol){
  return rb_hash_aref(rb_iv_get(self, "@ifds"), ifd_symbol);
}

static void each_content(ExifContent *ec, void *self_ptr){
  VALUE *self;
  ExifIfd ifd;

  ifd = exif_content_get_ifd(ec);
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eIFDNotFound, "Con't get IFD.");
  rb_hash_aset(rb_iv_get(*(VALUE*)self_ptr, "@ifds"), ID2SYM(rb_intern(ifd_name_mapping[ifd])), rb_hash_new());
  exif_content_foreach_entry(ec, each_entry, self_ptr);
}

static void each_entry(ExifEntry *entry, void *self_ptr){
  VALUE value;
  const char *ivar_name;

  ivar_name = exif_entry_to_ivar(entry);
  value = exif_entry_to_rb_value(entry);
  rb_hash_aset(
    rb_hash_aref(
      rb_iv_get(*(VALUE*)self_ptr, "@ifds"),
      ID2SYM(rb_intern(ifd_name_mapping[exif_entry_get_ifd(entry)]))
    ),
    ID2SYM(rb_intern(ivar_name + 1)),
    value
  );
  rb_iv_set(*(VALUE*)self_ptr, ivar_name, value);
}

static VALUE exif_entry_to_rb_value(ExifEntry *entry){
  ExifData  *data;
  ExifByteOrder order;
  ExifRational rational;
  ExifSRational srational;
  VALUE ret;
  size_t len, i;
  unsigned char size;
  struct tm tm = {};

  data = entry->parent->parent;
  order = exif_data_get_byte_order(data);
  ret = Qnil;
  size = exif_format_get_size(entry->format);

  switch(entry->format){
  case EXIF_FORMAT_UNDEFINED:
    ret = rb_str_new((const char*)entry->data, entry->size);
    break;
  case EXIF_FORMAT_BYTE:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for (i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX((uint8_t)entry->data[i]));
    }else ret = INT2FIX((uint8_t)entry->data[0]);
  case EXIF_FORMAT_SBYTE:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
  		for (i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX((int8_t)entry->data[i]));
    }else ret = INT2FIX((int8_t)entry->data[0]);
    break;
  case EXIF_FORMAT_SHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_short(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_short(entry->data, order));
    break;
  case EXIF_FORMAT_SSHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_sshort(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_sshort(entry->data, order));
    break;
  case EXIF_FORMAT_LONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(i = 0; i < entry->components; i++)
        rb_ary_push(ret, ULONG2NUM(exif_get_long(entry->data + i*size, order)));
    }else ret = ULONG2NUM(exif_get_long(entry->data, order));
    break;
  case EXIF_FORMAT_SLONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(i = 0; i < entry->components; i++)
        rb_ary_push(ret, LONG2NUM(exif_get_slong(entry->data + i*size, order)));
    }else ret = LONG2NUM(exif_get_slong(entry->data, order));
    break;
  case EXIF_FORMAT_ASCII:
    ret = rb_str_new2((const char *)entry->data);
    break;
  case EXIF_FORMAT_RATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(i = 0; i < entry->components; i++){
        rational = exif_get_rational(entry->data + i * size, order);
        rb_ary_push(ret, rb_rational_new(ULONG2NUM(rational.numerator), ULONG2NUM(rational.denominator)));
      }
    } else {
      rational = exif_get_rational(entry->data, order);
      ret = rb_rational_new(ULONG2NUM(rational.numerator), ULONG2NUM(rational.denominator));
    }
    break;
  case EXIF_FORMAT_SRATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++){
        srational = exif_get_srational(entry->data + i * size, order);
        rb_ary_push(ret, rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator)));
      }
    } else {
      srational = exif_get_srational(entry->data, order);
      ret = rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator));
    }
    break;
  case EXIF_FORMAT_DOUBLE:
  case EXIF_FORMAT_FLOAT:
    ret = rb_float_new(*(double*)entry->data);
    break;
  }

  return ret;
}
