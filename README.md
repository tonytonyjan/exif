Ruby EXIF reader written in C extension.

# Installation

    $ gem install exif

Please make sure you have installed `libexif` first.

# Usage

```ruby
data = Exif::Data.new('sample.jpg')
data.model        # => "NIKON D600"
data.image_width  # => 4000
data.gps_longitude # => 121.51246
data.date_time    # => 2013-12-08 21:14:11 0800

# get all entries in an IFD
data[0]                 # => {image_width: 4000, image_length: 2670, ...}
data[1]                 # => {x_resolution: "72", y_resolution: "72", ...}
data[:exif]             # => exposure_time: "1/125 sec.", f_number: "f/8.0"}
data[:gps]              # => {gps_version_id: "2.2.0.0", gps_latitude_ref: "N", ...}
data[:interoperability] # => {...}
data.to_h               # => {0 => {...}, 1 => {...}, :exif => {...}}
```

# How fast?

There is another excellent work called [exifr](https://github.com/remvee/exifr) made by [@remvee](https://github.com/remvee). That's pure Ruby while this one is C extension. If you program JRuby, you may want to choose exifr, otherwise you can try this gem for speed purpose, it's about 8 times faster. A small benchmark shows below:

```ruby
require 'benchmark'
require 'exif'
require 'exifr'
N = 500
FILE_PATH = 'sample.jpg'
Benchmark.bmbm do |x|
  x.report '[exifr] init' do
    N.times{ EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report '[exif]  init' do
    N.times{ Exif::Data.new(FILE_PATH).image_width }
  end
end
```

```
$ ruby benchmark/benchmark.rb
Rehearsal ------------------------------------------------
[exifr] init   0.810000   0.020000   0.830000 (  0.840701)
[exif]  init   0.090000   0.010000   0.100000 (  0.099700)
--------------------------------------- total: 0.930000sec

                   user     system      total        real
[exifr] init   0.810000   0.020000   0.830000 (  0.830644)
[exif]  init   0.090000   0.010000   0.100000 (  0.095148)
```

## Tag Rreference

- aperture_value
- artist
- battery_level
- bits_per_sample
- brightness_value
- cfa_pattern
- cfa_repeat_pattern_dim
- color_space
- components_configuration
- compressed_bits_per_pixel
- compression
- contrast
- copyright
- custom_rendered
- date_time
- date_time_digitized
- date_time_original
- device_setting_description
- digital_zoom_ratio
- document_name
- exif_ifd_pointer
- exif_version
- exposure_bias_value
- exposure_index
- exposure_mode
- exposure_program
- exposure_time
- file_source
- fill_order
- flash
- flash_energy
- flash_pix_version
- fnumber
- focal_length
- focal_length_in_35mm_film
- focal_plane_resolution_unit
- focal_plane_x_resolution
- focal_plane_y_resolution
- gain_control
- gamma
- gps_altitude
- gps_altitude_ref
- gps_area_information
- gps_date_stamp
- gps_dest_bearing
- gps_dest_bearing_ref
- gps_dest_distance
- gps_dest_distance_ref
- gps_dest_latitude
- gps_dest_latitude_ref
- gps_dest_longitude
- gps_dest_longitude_ref
- gps_differential
- gps_dop
- gps_img_direction
- gps_img_direction_ref
- gps_info_ifd_pointer
- gps_latitude
- gps_latitude_ref
- gps_longitude
- gps_longitude_ref
- gps_map_datum
- gps_measure_mode
- gps_processing_method
- gps_satellites
- gps_speed
- gps_speed_ref
- gps_status
- gps_time_stamp
- gps_track
- gps_track_ref
- gps_version_id
- image_description
- image_length
- image_resources
- image_unique_id
- image_width
- inter_color_profile
- interoperability_ifd_pointer
- interoperability_index
- interoperability_version
- iptc_naa
- iso_speed_ratings
- jpeg_interchange_format
- jpeg_interchange_format_length
- jpeg_proc
- light_source
- make
- maker_note
- max_aperture_value
- metering_mode
- model
- new_cfa_pattern
- new_subfile_type
- oecf
- orientation
- padding
- photometric_interpretation
- pixel_x_dimension
- pixel_y_dimension
- planar_configuration
- primary_chromaticities
- print_image_matching
- reference_black_white
- related_image_file_format
- related_image_length
- related_image_width
- related_sound_file
- resolution_unit
- rows_per_strip
- samples_per_pixel
- saturation
- scene_capture_type
- scene_type
- sensing_method
- sharpness
- shutter_speed_value
- software
- spatial_frequency_response
- spectral_sensitivity
- strip_byte_counts
- strip_offsets
- sub_ifds
- sub_sec_time
- sub_sec_time_digitized
- sub_sec_time_original
- subject_area
- subject_distance
- subject_distance_range
- subject_location
- tiff_ep_standard_id
- time_zone_offset
- transfer_function
- transfer_range
- user_comment
- white_balance
- white_point
- x_resolution
- xml_packet
- xp_author
- xp_comment
- xp_keywords
- xp_subject
- xp_title
- y_resolution
- ycbcr_coefficients
- ycbcr_positioning
- ycbcr_sub_sampling

# TODO

1. Support reading from String.