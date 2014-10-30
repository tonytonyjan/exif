#ifndef DATA_H
#define DATA_H 1

#include <libexif/exif-data.h>
#include "ruby.h"

static char *attrs[] = {"aperture_value", "artist", "battery_level", "bits_per_sample", "brightness_value", "cfa_pattern", "cfa_repeat_pattern_dim", "color_space", "components_configuration", "compressed_bits_per_pixel", "compression", "contrast", "copyright", "custom_rendered", "date_time", "date_time_digitized", "date_time_original", "device_setting_description", "digital_zoom_ratio", "document_name", "exif_ifd_pointer", "exif_version", "exposure_bias_value", "exposure_index", "exposure_mode", "exposure_program", "exposure_time", "file_source", "fill_order", "flash", "flash_energy", "flash_pix_version", "fnumber", "focal_length", "focal_length_in_35mm_film", "focal_plane_resolution_unit", "focal_plane_x_resolution", "focal_plane_y_resolution", "gain_control", "gamma", "gps_altitude", "gps_altitude_ref", "gps_area_information", "gps_date_stamp", "gps_dest_bearing", "gps_dest_bearing_ref", "gps_dest_distance", "gps_dest_distance_ref", "gps_dest_latitude", "gps_dest_latitude_ref", "gps_dest_longitude", "gps_dest_longitude_ref", "gps_differential", "gps_dop", "gps_img_direction", "gps_img_direction_ref", "gps_info_ifd_pointer", "gps_latitude", "gps_latitude_ref", "gps_longitude", "gps_longitude_ref", "gps_map_datum", "gps_measure_mode", "gps_processing_method", "gps_satellites", "gps_speed", "gps_speed_ref", "gps_status", "gps_time_stamp", "gps_track", "gps_track_ref", "gps_version_id", "image_description", "image_length", "image_resources", "image_unique_id", "image_width", "inter_color_profile", "interoperability_ifd_pointer", "interoperability_index", "interoperability_version", "iptc_naa", "iso_speed_ratings", "jpeg_interchange_format", "jpeg_interchange_format_length", "jpeg_proc", "light_source", "make", "maker_note", "max_aperture_value", "metering_mode", "model", "new_cfa_pattern", "new_subfile_type", "oecf", "orientation", "padding", "photometric_interpretation", "pixel_x_dimension", "pixel_y_dimension", "planar_configuration", "primary_chromaticities", "print_image_matching", "reference_black_white", "related_image_file_format", "related_image_length", "related_image_width", "related_sound_file", "resolution_unit", "rows_per_strip", "samples_per_pixel", "saturation", "scene_capture_type", "scene_type", "sensing_method", "sharpness", "shutter_speed_value", "software", "spatial_frequency_response", "spectral_sensitivity", "strip_byte_counts", "strip_offsets", "sub_ifds", "sub_sec_time", "sub_sec_time_digitized", "sub_sec_time_original", "subject_area", "subject_distance", "subject_distance_range", "subject_location", "tiff_ep_standard_id", "time_zone_offset", "transfer_function", "transfer_range", "user_comment", "white_balance", "white_point", "x_resolution", "xml_packet", "xp_author", "xp_comment", "xp_keywords", "xp_subject", "xp_title", "y_resolution", "ycbcr_coefficients", "ycbcr_positioning", "ycbcr_sub_sampling"};

void init_data();

static VALUE rb_new(VALUE self, VALUE file_path);
static VALUE rb_dump(VALUE self);
static VALUE rb_value(VALUE self, VALUE key);

static void each_content(ExifContent *ec, void *user_data);
static void each_entry(ExifEntry *, void *user_data);
static VALUE process_value(ExifTag tag, char *buf);
static char* attr_string(ExifIfd ifd, ExifTag tag);

#endif /* DATA_H */
