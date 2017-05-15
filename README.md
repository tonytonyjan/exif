Ruby EXIF reader written in C extension. [![Build Status](https://travis-ci.org/tonytonyjan/exif.svg?branch=master)](https://travis-ci.org/tonytonyjan/exif)

# Installation

    $ gem install exif

Please make sure you have installed `libexif` first:

    $ brew install libexif             # Homebrew
    $ sudo apt-get install libexif-dev # APT
    $ sudo yum install libexif-devel   # CentOS

# Usage

```ruby
data = Exif::Data.new('sample.jpg')
data.model         # => "NIKON D600"
data.image_width   # => 4000
data.gps_longitude # => 121.51246
data.date_time     # => 2013-12-08 21:14:11 0800

# get all entries in an IFD
data[0]                 # => {image_width: 4000, image_length: 2670, ...}
data[1]                 # => {x_resolution: "72", y_resolution: "72", ...}
data[:exif]             # => exposure_time: "1/125 sec.", f_number: "f/8.0"}
data[:gps]              # => {gps_version_id: "2.2.0.0", gps_latitude_ref: "N", ...}
data[:interoperability] # => {...}
data.to_h               # => {0 => {...}, 1 => {...}, :exif => {...}}
```

# How fast?

There are some other excellent works called [exifr](https://github.com/remvee/exifr) by [@remvee](https://github.com/remvee), and [mini_exiftool](https://github.com/janfri/mini_exiftool) by [@janfri](https://github.com/janfri). They're built in pure Ruby while this one is C extension.

If you program JRuby, you may want to choose exifr or mini_exiftool, the latter lets you get the full power of [Exiftool](http://www.sno.phy.queensu.ca/~phil/exiftool/) written by Phil Harvey since it's a command-line wrapper, otherwise you can try this gem for speed purpose. **It's about 8 times faster than exifr and 1200 times than that of mini_exiftool.**

A small benchmark shows below:

```ruby
require 'benchmark'
require 'mini_exiftool'
require 'exifr'
require 'exif'

N = 50
FILE_PATH = File.expand_path('../../spec/sample.jpg', __FILE__)
Benchmark.bmbm do |x|
  x.report 'mini_exiftool' do
    N.times{ MiniExiftool.new(FILE_PATH).image_width }
  end
  x.report 'exifr' do
    N.times{ EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report 'exif' do
    N.times{ Exif::Data.new(FILE_PATH).image_width }
  end
end
```

```
$ ruby benchmark/benchmark.rb
Rehearsal -------------------------------------------------
mini_exiftool   0.190000   0.050000  12.130000 ( 12.293019)
exifr           0.080000   0.000000   0.080000 (  0.087728)
exif            0.010000   0.000000   0.010000 (  0.006422)
--------------------------------------- total: 12.220000sec

                    user     system      total        real
mini_exiftool   0.190000   0.050000  12.330000 ( 12.448299)
exifr           0.080000   0.010000   0.090000 (  0.078003)
exif            0.010000   0.000000   0.010000 (  0.005781)

2015-08-03 01:15:01 UTC
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
2. Create, update and delete tags.
