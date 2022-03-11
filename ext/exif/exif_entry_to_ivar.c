#include <libexif/exif-entry.h>

const char *exif_entry_to_ivar(ExifEntry *ee) {
  ExifIfd ifd;
  ExifTag tag;

  ifd = exif_entry_get_ifd(ee);
  tag = ee->tag;

  switch ((int)tag) {
  case EXIF_TAG_INTEROPERABILITY_INDEX: /* EXIF_TAG_GPS_LATITUDE_REF */
    return ifd == EXIF_IFD_GPS ? "@gps_latitude_ref"
                               : "@interoperability_index";
    break;
  case EXIF_TAG_INTEROPERABILITY_VERSION: /* EXIF_TAG_GPS_LATITUDE */
    return ifd == EXIF_IFD_GPS ? "@gps_latitude" : "@interoperability_version";
    break;
  case EXIF_TAG_NEW_SUBFILE_TYPE:
    return "@new_subfile_type";
    break;
  case EXIF_TAG_IMAGE_WIDTH:
    return "@image_width";
    break;
  case EXIF_TAG_IMAGE_LENGTH:
    return "@image_length";
    break;
  case EXIF_TAG_BITS_PER_SAMPLE:
    return "@bits_per_sample";
    break;
  case EXIF_TAG_COMPRESSION:
    return "@compression";
    break;
  case EXIF_TAG_PHOTOMETRIC_INTERPRETATION:
    return "@photometric_interpretation";
    break;
  case EXIF_TAG_FILL_ORDER:
    return "@fill_order";
    break;
  case EXIF_TAG_DOCUMENT_NAME:
    return "@document_name";
    break;
  case EXIF_TAG_IMAGE_DESCRIPTION:
    return "@image_description";
    break;
  case EXIF_TAG_MAKE:
    return "@make";
    break;
  case EXIF_TAG_MODEL:
    return "@model";
    break;
  case EXIF_TAG_STRIP_OFFSETS:
    return "@strip_offsets";
    break;
  case EXIF_TAG_ORIENTATION:
    return "@orientation";
    break;
  case EXIF_TAG_SAMPLES_PER_PIXEL:
    return "@samples_per_pixel";
    break;
  case EXIF_TAG_ROWS_PER_STRIP:
    return "@rows_per_strip";
    break;
  case EXIF_TAG_STRIP_BYTE_COUNTS:
    return "@strip_byte_counts";
    break;
  case EXIF_TAG_X_RESOLUTION:
    return "@x_resolution";
    break;
  case EXIF_TAG_Y_RESOLUTION:
    return "@y_resolution";
    break;
  case EXIF_TAG_PLANAR_CONFIGURATION:
    return "@planar_configuration";
    break;
  case EXIF_TAG_RESOLUTION_UNIT:
    return "@resolution_unit";
    break;
  case EXIF_TAG_TRANSFER_FUNCTION:
    return "@transfer_function";
    break;
  case EXIF_TAG_SOFTWARE:
    return "@software";
    break;
  case EXIF_TAG_DATE_TIME:
    return "@date_time";
    break;
  case EXIF_TAG_ARTIST:
    return "@artist";
    break;
  case EXIF_TAG_WHITE_POINT:
    return "@white_point";
    break;
  case EXIF_TAG_PRIMARY_CHROMATICITIES:
    return "@primary_chromaticities";
    break;
  case EXIF_TAG_SUB_IFDS:
    return "@sub_ifds";
    break;
  case EXIF_TAG_TRANSFER_RANGE:
    return "@transfer_range";
    break;
  case EXIF_TAG_JPEG_PROC:
    return "@jpeg_proc";
    break;
  case EXIF_TAG_JPEG_INTERCHANGE_FORMAT:
    return "@jpeg_interchange_format";
    break;
  case EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH:
    return "@jpeg_interchange_format_length";
    break;
  case EXIF_TAG_YCBCR_COEFFICIENTS:
    return "@ycbcr_coefficients";
    break;
  case EXIF_TAG_YCBCR_SUB_SAMPLING:
    return "@ycbcr_sub_sampling";
    break;
  case EXIF_TAG_YCBCR_POSITIONING:
    return "@ycbcr_positioning";
    break;
  case EXIF_TAG_REFERENCE_BLACK_WHITE:
    return "@reference_black_white";
    break;
  case EXIF_TAG_XML_PACKET:
    return "@xml_packet";
    break;
  case EXIF_TAG_RELATED_IMAGE_FILE_FORMAT:
    return "@related_image_file_format";
    break;
  case EXIF_TAG_RELATED_IMAGE_WIDTH:
    return "@related_image_width";
    break;
  case EXIF_TAG_RELATED_IMAGE_LENGTH:
    return "@related_image_length";
    break;
  case EXIF_TAG_CFA_REPEAT_PATTERN_DIM:
    return "@cfa_repeat_pattern_dim";
    break;
  case EXIF_TAG_CFA_PATTERN:
    return "@cfa_pattern";
    break;
  case EXIF_TAG_BATTERY_LEVEL:
    return "@battery_level";
    break;
  case EXIF_TAG_COPYRIGHT:
    return "@copyright";
    break;
  case EXIF_TAG_EXPOSURE_TIME:
    return "@exposure_time";
    break;
  case EXIF_TAG_FNUMBER:
    return "@fnumber";
    break;
  case EXIF_TAG_IPTC_NAA:
    return "@iptc_naa";
    break;
  case EXIF_TAG_IMAGE_RESOURCES:
    return "@image_resources";
    break;
  case EXIF_TAG_EXIF_IFD_POINTER:
    return "@exif_ifd_pointer";
    break;
  case EXIF_TAG_INTER_COLOR_PROFILE:
    return "@inter_color_profile";
    break;
  case EXIF_TAG_EXPOSURE_PROGRAM:
    return "@exposure_program";
    break;
  case EXIF_TAG_SPECTRAL_SENSITIVITY:
    return "@spectral_sensitivity";
    break;
  case EXIF_TAG_GPS_INFO_IFD_POINTER:
    return "@gps_info_ifd_pointer";
    break;
  case EXIF_TAG_ISO_SPEED_RATINGS:
    return "@iso_speed_ratings";
    break;
  case EXIF_TAG_OECF:
    return "@oecf";
    break;
  case EXIF_TAG_TIME_ZONE_OFFSET:
    return "@time_zone_offset";
    break;
  case EXIF_TAG_EXIF_VERSION:
    return "@exif_version";
    break;
  case EXIF_TAG_DATE_TIME_ORIGINAL:
    return "@date_time_original";
    break;
  case EXIF_TAG_DATE_TIME_DIGITIZED:
    return "@date_time_digitized";
    break;
  case EXIF_TAG_COMPONENTS_CONFIGURATION:
    return "@components_configuration";
    break;
  case EXIF_TAG_COMPRESSED_BITS_PER_PIXEL:
    return "@compressed_bits_per_pixel";
    break;
  case EXIF_TAG_SHUTTER_SPEED_VALUE:
    return "@shutter_speed_value";
    break;
  case EXIF_TAG_APERTURE_VALUE:
    return "@aperture_value";
    break;
  case EXIF_TAG_BRIGHTNESS_VALUE:
    return "@brightness_value";
    break;
  case EXIF_TAG_EXPOSURE_BIAS_VALUE:
    return "@exposure_bias_value";
    break;
  case EXIF_TAG_MAX_APERTURE_VALUE:
    return "@max_aperture_value";
    break;
  case EXIF_TAG_SUBJECT_DISTANCE:
    return "@subject_distance";
    break;
  case EXIF_TAG_METERING_MODE:
    return "@metering_mode";
    break;
  case EXIF_TAG_LIGHT_SOURCE:
    return "@light_source";
    break;
  case EXIF_TAG_FLASH:
    return "@flash";
    break;
  case EXIF_TAG_FOCAL_LENGTH:
    return "@focal_length";
    break;
  case EXIF_TAG_SUBJECT_AREA:
    return "@subject_area";
    break;
  case EXIF_TAG_TIFF_EP_STANDARD_ID:
    return "@tiff_ep_standard_id";
    break;
  case EXIF_TAG_MAKER_NOTE:
    return "@maker_note";
    break;
  case EXIF_TAG_USER_COMMENT:
    return "@user_comment";
    break;
  case EXIF_TAG_SUB_SEC_TIME:
    return "@sub_sec_time";
    break;
  case EXIF_TAG_SUB_SEC_TIME_ORIGINAL:
    return "@sub_sec_time_original";
    break;
  case EXIF_TAG_SUB_SEC_TIME_DIGITIZED:
    return "@sub_sec_time_digitized";
    break;
  case EXIF_TAG_XP_TITLE:
    return "@xp_title";
    break;
  case EXIF_TAG_XP_COMMENT:
    return "@xp_comment";
    break;
  case EXIF_TAG_XP_AUTHOR:
    return "@xp_author";
    break;
  case EXIF_TAG_XP_KEYWORDS:
    return "@xp_keywords";
    break;
  case EXIF_TAG_XP_SUBJECT:
    return "@xp_subject";
    break;
  case EXIF_TAG_FLASH_PIX_VERSION:
    return "@flash_pix_version";
    break;
  case EXIF_TAG_COLOR_SPACE:
    return "@color_space";
    break;
  case EXIF_TAG_PIXEL_X_DIMENSION:
    return "@pixel_x_dimension";
    break;
  case EXIF_TAG_PIXEL_Y_DIMENSION:
    return "@pixel_y_dimension";
    break;
  case EXIF_TAG_RELATED_SOUND_FILE:
    return "@related_sound_file";
    break;
  case EXIF_TAG_INTEROPERABILITY_IFD_POINTER:
    return "@interoperability_ifd_pointer";
    break;
  case EXIF_TAG_FLASH_ENERGY:
    return "@flash_energy";
    break;
  case EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE:
    return "@spatial_frequency_response";
    break;
  case EXIF_TAG_FOCAL_PLANE_X_RESOLUTION:
    return "@focal_plane_x_resolution";
    break;
  case EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION:
    return "@focal_plane_y_resolution";
    break;
  case EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT:
    return "@focal_plane_resolution_unit";
    break;
  case EXIF_TAG_SUBJECT_LOCATION:
    return "@subject_location";
    break;
  case EXIF_TAG_EXPOSURE_INDEX:
    return "@exposure_index";
    break;
  case EXIF_TAG_SENSING_METHOD:
    return "@sensing_method";
    break;
  case EXIF_TAG_FILE_SOURCE:
    return "@file_source";
    break;
  case EXIF_TAG_SCENE_TYPE:
    return "@scene_type";
    break;
  case EXIF_TAG_NEW_CFA_PATTERN:
    return "@new_cfa_pattern";
    break;
  case EXIF_TAG_CUSTOM_RENDERED:
    return "@custom_rendered";
    break;
  case EXIF_TAG_EXPOSURE_MODE:
    return "@exposure_mode";
    break;
  case EXIF_TAG_WHITE_BALANCE:
    return "@white_balance";
    break;
  case EXIF_TAG_DIGITAL_ZOOM_RATIO:
    return "@digital_zoom_ratio";
    break;
  case EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM:
    return "@focal_length_in_35mm_film";
    break;
  case EXIF_TAG_SCENE_CAPTURE_TYPE:
    return "@scene_capture_type";
    break;
  case EXIF_TAG_GAIN_CONTROL:
    return "@gain_control";
    break;
  case EXIF_TAG_CONTRAST:
    return "@contrast";
    break;
  case EXIF_TAG_SATURATION:
    return "@saturation";
    break;
  case EXIF_TAG_SHARPNESS:
    return "@sharpness";
    break;
  case EXIF_TAG_DEVICE_SETTING_DESCRIPTION:
    return "@device_setting_description";
    break;
  case EXIF_TAG_SUBJECT_DISTANCE_RANGE:
    return "@subject_distance_range";
    break;
  case EXIF_TAG_IMAGE_UNIQUE_ID:
    return "@image_unique_id";
    break;
  case EXIF_TAG_GAMMA:
    return "@gamma";
    break;
  case EXIF_TAG_PRINT_IMAGE_MATCHING:
    return "@print_image_matching";
    break;
  case EXIF_TAG_PADDING:
    return "@padding";
    break;
  case EXIF_TAG_GPS_VERSION_ID:
    return "@gps_version_id";
    break;
  case EXIF_TAG_GPS_LONGITUDE_REF:
    return "@gps_longitude_ref";
    break;
  case EXIF_TAG_GPS_LONGITUDE:
    return "@gps_longitude";
    break;
  case EXIF_TAG_GPS_ALTITUDE_REF:
    return "@gps_altitude_ref";
    break;
  case EXIF_TAG_GPS_ALTITUDE:
    return "@gps_altitude";
    break;
  case EXIF_TAG_GPS_TIME_STAMP:
    return "@gps_time_stamp";
    break;
  case EXIF_TAG_GPS_SATELLITES:
    return "@gps_satellites";
    break;
  case EXIF_TAG_GPS_STATUS:
    return "@gps_status";
    break;
  case EXIF_TAG_GPS_MEASURE_MODE:
    return "@gps_measure_mode";
    break;
  case EXIF_TAG_GPS_DOP:
    return "@gps_dop";
    break;
  case EXIF_TAG_GPS_SPEED_REF:
    return "@gps_speed_ref";
    break;
  case EXIF_TAG_GPS_SPEED:
    return "@gps_speed";
    break;
  case EXIF_TAG_GPS_TRACK_REF:
    return "@gps_track_ref";
    break;
  case EXIF_TAG_GPS_TRACK:
    return "@gps_track";
    break;
  case EXIF_TAG_GPS_IMG_DIRECTION_REF:
    return "@gps_img_direction_ref";
    break;
  case EXIF_TAG_GPS_IMG_DIRECTION:
    return "@gps_img_direction";
    break;
  case EXIF_TAG_GPS_MAP_DATUM:
    return "@gps_map_datum";
    break;
  case EXIF_TAG_GPS_DEST_LATITUDE_REF:
    return "@gps_dest_latitude_ref";
    break;
  case EXIF_TAG_GPS_DEST_LATITUDE:
    return "@gps_dest_latitude";
    break;
  case EXIF_TAG_GPS_DEST_LONGITUDE_REF:
    return "@gps_dest_longitude_ref";
    break;
  case EXIF_TAG_GPS_DEST_LONGITUDE:
    return "@gps_dest_longitude";
    break;
  case EXIF_TAG_GPS_DEST_BEARING_REF:
    return "@gps_dest_bearing_ref";
    break;
  case EXIF_TAG_GPS_DEST_BEARING:
    return "@gps_dest_bearing";
    break;
  case EXIF_TAG_GPS_DEST_DISTANCE_REF:
    return "@gps_dest_distance_ref";
    break;
  case EXIF_TAG_GPS_DEST_DISTANCE:
    return "@gps_dest_distance";
    break;
  case EXIF_TAG_GPS_PROCESSING_METHOD:
    return "@gps_processing_method";
    break;
  case EXIF_TAG_GPS_AREA_INFORMATION:
    return "@gps_area_information";
    break;
  case EXIF_TAG_GPS_DATE_STAMP:
    return "@gps_date_stamp";
    break;
  case EXIF_TAG_GPS_DIFFERENTIAL:
    return "@gps_differential";
    break;
  #ifdef LIBEXIF_0_6_22_OR_HIGHER
  case EXIF_TAG_BODY_SERIAL_NUMBER:
    return "@body_serial_number";
    break;
  case EXIF_TAG_CAMERA_OWNER_NAME:
    return "@camera_owner_name";
    break;
  case EXIF_TAG_COMPOSITE_IMAGE:
    return "@composite_image";
    break;
  case EXIF_TAG_GPS_H_POSITIONING_ERROR:
    return "@gps_h_positioning_error";
    break;
  case EXIF_TAG_LENS_MAKE:
    return "@lens_make";
    break;
  case EXIF_TAG_LENS_MODEL:
    return "@lens_model";
    break;
  case EXIF_TAG_LENS_SERIAL_NUMBER:
    return "@lens_serial_number";
    break;
  case EXIF_TAG_LENS_SPECIFICATION:
    return "@lens_specification";
    break;
  case EXIF_TAG_SOURCE_EXPOSURE_TIMES_OF_COMPOSITE_IMAGE:
    return "@source_exposure_times_of_composite_image";
    break;
  case EXIF_TAG_SOURCE_IMAGE_NUMBER_OF_COMPOSITE_IMAGE:
    return "@source_image_number_of_composite_image";
    break;
  #endif
  #ifdef LIBEXIF_0_6_23_OR_HIGHER
  case EXIF_TAG_IMAGE_DEPTH:
    return "@image_depth";
    break;
  case EXIF_TAG_ISO_SPEED:
    return "@iso_speed";
    break;
  case EXIF_TAG_ISO_SPEEDLatitudeYYY:
    return "@iso_speedlatitudeyyy";
    break;
  case EXIF_TAG_ISO_SPEEDLatitudeZZZ:
    return "@iso_speedlatitudezzz";
    break;
  case EXIF_TAG_OFFSET_TIME:
    return "@offset_time";
    break;
  case EXIF_TAG_OFFSET_TIME_DIGITIZED:
    return "@offset_time_digitized";
    break;
  case EXIF_TAG_OFFSET_TIME_ORIGINAL:
    return "@offset_time_original";
    break;
  case EXIF_TAG_RECOMMENDED_EXPOSURE_INDEX:
    return "@recommended_exposure_index";
    break;
  case EXIF_TAG_SENSITIVITY_TYPE:
    return "@sensitivity_type";
    break;
  case EXIF_TAG_STANDARD_OUTPUT_SENSITIVITY:
    return "@standard_output_sensitivity";
    break;
  #endif
  }
  return 0;
}
