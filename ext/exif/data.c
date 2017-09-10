#include "data.h"
#include "libjpeg/jpeg-data.h"
#include "ruby.h"

#include <libexif/exif-data.h>
#include <time.h>

extern VALUE rb_mExif, rb_eError, rb_eNotReadble, rb_eIFDNotFound, rb_eUnknownDataType;
extern const char* exif_entry_to_ivar(ExifEntry* entry);

VALUE rb_cData;

static const char* attrs[] = {"aperture_value", "artist", "battery_level", "bits_per_sample", "brightness_value", "cfa_pattern", "cfa_repeat_pattern_dim", "color_space", "components_configuration", "compressed_bits_per_pixel", "compression", "contrast", "copyright", "custom_rendered", "date_time", "date_time_digitized", "date_time_original", "device_setting_description", "digital_zoom_ratio", "document_name", "exif_ifd_pointer", "exif_version", "exposure_bias_value", "exposure_index", "exposure_mode", "exposure_program", "exposure_time", "file_source", "fill_order", "flash", "flash_energy", "flash_pix_version", "fnumber", "focal_length", "focal_length_in_35mm_film", "focal_plane_resolution_unit", "focal_plane_x_resolution", "focal_plane_y_resolution", "gain_control", "gamma", "gps_altitude", "gps_altitude_ref", "gps_area_information", "gps_date_stamp", "gps_dest_bearing", "gps_dest_bearing_ref", "gps_dest_distance", "gps_dest_distance_ref", "gps_dest_latitude", "gps_dest_latitude_ref", "gps_dest_longitude", "gps_dest_longitude_ref", "gps_differential", "gps_dop", "gps_img_direction", "gps_img_direction_ref", "gps_info_ifd_pointer", "gps_latitude", "gps_latitude_ref", "gps_longitude", "gps_longitude_ref", "gps_map_datum", "gps_measure_mode", "gps_processing_method", "gps_satellites", "gps_speed", "gps_speed_ref", "gps_status", "gps_time_stamp", "gps_track", "gps_track_ref", "gps_version_id", "image_description", "image_length", "image_resources", "image_unique_id", "image_width", "inter_color_profile", "interoperability_ifd_pointer", "interoperability_index", "interoperability_version", "iptc_naa", "iso_speed_ratings", "jpeg_interchange_format", "jpeg_interchange_format_length", "jpeg_proc", "light_source", "make", "maker_note", "max_aperture_value", "metering_mode", "model", "new_cfa_pattern", "new_subfile_type", "oecf", "orientation", "padding", "photometric_interpretation", "pixel_x_dimension", "pixel_y_dimension", "planar_configuration", "primary_chromaticities", "print_image_matching", "reference_black_white", "related_image_file_format", "related_image_length", "related_image_width", "related_sound_file", "resolution_unit", "rows_per_strip", "samples_per_pixel", "saturation", "scene_capture_type", "scene_type", "sensing_method", "sharpness", "shutter_speed_value", "software", "spatial_frequency_response", "spectral_sensitivity", "strip_byte_counts", "strip_offsets", "sub_ifds", "sub_sec_time", "sub_sec_time_digitized", "sub_sec_time_original", "subject_area", "subject_distance", "subject_distance_range", "subject_location", "tiff_ep_standard_id", "time_zone_offset", "transfer_function", "transfer_range", "user_comment", "white_balance", "white_point", "x_resolution", "xml_packet", "xp_author", "xp_comment", "xp_keywords", "xp_subject", "xp_title", "y_resolution", "ycbcr_coefficients", "ycbcr_positioning", "ycbcr_sub_sampling"};

static VALUE new(VALUE self, VALUE file_path);
static VALUE dump(VALUE self);
static void each_content(ExifContent *ec, void *user_data);
static void each_entry(ExifEntry *, void *user_data);
static VALUE exif_entry_to_value(ExifEntry *);

void init_data(){
  int length;

  rb_cData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  length = sizeof(attrs) / sizeof(char*);

  for(int i = 0; i < length; ++i) rb_define_attr(rb_cData, attrs[i], 1, 0);
  rb_define_singleton_method(rb_cData, "new", new, 1);
  rb_define_method(rb_cData, "dump", dump, 0);
}

VALUE new(VALUE self, VALUE file_path){
  Check_Type(file_path, T_STRING);

  ExifData* ed;
  VALUE rb_data;

  ed = exif_data_new_from_file(StringValueCStr(file_path));
  if(!ed) rb_raise(rb_eNotReadble, "File not readable or no EXIF data in file.");

  rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  exif_data_foreach_content(ed, each_content, &rb_data);
  return rb_data;
}

static VALUE dump(VALUE self){
  ExifData *ed;
  Data_Get_Struct(self, ExifData, ed);
  exif_data_dump(ed);
  return Qnil;
}

static void each_content(ExifContent *ec, void *self_ptr){
  VALUE *self;
  ExifIfd ifd;

  self = (VALUE*)self_ptr;
  ifd = exif_content_get_ifd(ec);
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eIFDNotFound, "Con't get IFD.");

  exif_content_foreach_entry(ec, each_entry, self);
}

static void each_entry(ExifEntry *entry, void *self_ptr){
  VALUE *self, value;
  const char *ivar_name;

  self = (VALUE*)self_ptr;
  ivar_name = exif_entry_to_ivar(entry);
  value = exif_entry_to_value(entry);

  rb_iv_set(*self, ivar_name, value);
}

static VALUE exif_entry_to_value(ExifEntry *entry){
  ExifData  *data;
  ExifByteOrder order;
  ExifRational rational;
  ExifSRational srational;
  VALUE ret;
  int i;
  unsigned char size;
  struct tm tm = {};

  data = entry->parent->parent;
  order = exif_data_get_byte_order(data);
  ret = Qnil;
  size = exif_format_get_size(entry->format);

  switch(entry->format){
  case EXIF_FORMAT_ASCII:
    switch((int)entry->tag){
    case EXIF_TAG_DATE_TIME:
    case EXIF_TAG_DATE_TIME_ORIGINAL:
      if(strptime((const char*)entry->data, "%Y:%m:%d %T", &tm) != NULL)
        ret = rb_time_new(mktime(&tm), 0);
      else rb_raise(rb_eError, "wrong date time format");
      break;
    case EXIF_TAG_GPS_DATE_STAMP:
      if(strptime((const char*)entry->data, "%Y:%m:%d", &tm) != NULL)
        ret = rb_time_new(mktime(&tm), 0);
      else rb_raise(rb_eError, "wrong date time format");
      break;
    default:
      ret = rb_str_new2((const char *)entry->data);
    }
    break;
  case EXIF_FORMAT_SHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_short(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_short(entry->data, order));
    break;
  case EXIF_FORMAT_SSHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_sshort(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_sshort(entry->data, order));
    break;
  case EXIF_FORMAT_RATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++){
        srational = exif_get_srational(entry->data + i*size, order);
        rb_ary_push(ret, rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator)));
      }
    }else{
      srational = exif_get_srational(entry->data, order);
      ret = rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator));
    }
    break;
  case EXIF_FORMAT_SRATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++){
        rational = exif_get_rational(entry->data + i*size, order);
        rb_ary_push(ret, rb_rational_new(LONG2FIX(rational.numerator), LONG2FIX(rational.denominator)));
      }
    }else{
      rational = exif_get_rational(entry->data, order);
      ret = rb_rational_new(LONG2FIX(rational.numerator), LONG2FIX(rational.denominator));
    }
    break;
  case EXIF_FORMAT_BYTE:
  case EXIF_FORMAT_SBYTE:
    ret = rb_str_new((const char *)entry->data, entry->size);
    break;
  case EXIF_FORMAT_LONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, LONG2NUM(exif_get_long(entry->data + i*size, order)));
    }else ret = LONG2NUM(exif_get_long(entry->data, order));
    break;
  case EXIF_FORMAT_SLONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, LONG2NUM(exif_get_slong(entry->data + i*size, order)));
    }else ret = LONG2NUM(exif_get_slong(entry->data, order));
    break;
  case EXIF_FORMAT_FLOAT:
    // TODO
    break;
  case EXIF_FORMAT_DOUBLE:
    // TODO
    break;
  case EXIF_FORMAT_UNDEFINED:
    // TODO
    break;
  }

  // if(NIL_P(ret)) rb_raise(rb_eUnknownDataType, "Unknown data type");

  return ret;
}