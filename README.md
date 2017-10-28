A Ruby EXIF reader written in C. [![Build Status](https://travis-ci.org/tonytonyjan/exif.svg?branch=master)](https://travis-ci.org/tonytonyjan/exif)

# Installation

    $ gem install exif

Please make sure you have installed `libexif`:

    $ brew install libexif             # Homebrew
    $ sudo apt-get install libexif-dev # APT
    $ sudo yum install libexif-devel   # CentOS

# Usage

```ruby
data = Exif::Data.new(IO.read('sample.jpg'))   # load from string
data = Exif::Data.new("Exif\x00\x00II...")     # load raw exif data
data = Exif::Data.new(File.open('sample.jpg')) # load from file
data.model         # => "NIKON D600"
data.image_width   # => 4000
data.gps_longitude # => [(121/1), (76869/2500), (0/1)]
data.date_time     # => "2013:12:08 21:14:11"

# get all entries in an IFD
data[:ifd0]             # => {image_width: 4000, image_length: 2670, ...}
data[:ifd1]             # => {x_resolution: (72/1), y_resolution: (72/1), ...}
data[:exif]             # => exposure_time: (1/125), fnumber: (8/1)}
data[:gps]              # => {gps_version_id: [2, 2, 0, 0], gps_latitude_ref: "N", ...}
data[:interoperability] # => {...}
data.to_h               # => {:ifd0 => {...}, :ifd1 => {...}, :exif => {...}}
data.ifds == data.to_h  # => true
```

`ifd0` takes precedence over `ifd1`, since `ifd0` and `ifd1` share the same domain of tags, sometimes the same tag can be both used inside them, in this case, calling top-level API like `Exif::Data#image_width` will return the value of `ifd0`.

# Typecasting

Tag Type  | Ruby Class
----------|----------
ASCII     | String
BYTE      | Integer
DOUBLE    | Flat
FLOAT     | Float
LONG      | Integer
RATIONAL  | Rational
SHORT     | Integer
UNDEFINED | String

# How fast?

**It's hundreds of times faster than mini_exiftool or exiftool and several times faster than exifr.**

There are similar excellent gems like [exifr](https://github.com/remvee/exifr), [mini_exiftool](https://github.com/janfri/mini_exiftool) and [exiftool](https://github.com/mceachen/exiftool.rb) built in pure Ruby while this one is a C extension.

If you are using JRuby you might want to give exifr, exiftool or mini_exiftool a look, the latter two are commandline wrappers for [ExifTool](http://www.sno.phy.queensu.ca/~phil/exiftool/) by Phil Harvey and support many file formats. The exiftool gem can process multiple files at once while mini_exiftool can modify or copy EXIF data.

# Benchmark

You can find all benchmark files in [/benchmark](benchmark):

Read a single tag from 100 files.
```
$ ruby benchmark/load_file.rb
Rehearsal ---------------------------------------------------------
mini_exiftool (2.9.0)   0.240000   0.110000  22.830000 ( 23.275885)
exiftool (1.2.0)        0.480000   0.130000  22.870000 ( 23.311755)
exifr (1.3.2)           0.120000   0.010000   0.130000 (  0.139821)
exif (49b6835)          0.020000   0.000000   0.020000 (  0.019927)
----------------------------------------------- total: 45.850000sec

                            user     system      total        real
mini_exiftool (2.9.0)   0.240000   0.120000  22.830000 ( 23.267355)
exiftool (1.2.0)        0.490000   0.130000  23.190000 ( 23.678799)
exifr (1.3.2)           0.120000   0.010000   0.130000 (  0.134572)
exif (49b6835)          0.020000   0.000000   0.020000 (  0.018419)
-----------------------------------------
1263 times faster than mini_exiftool (2.9.0)
1286 times faster than exiftool (1.2.0)
7 times faster than exifr (1.3.2)
```

Read many tags from the same file 1000 times.
```
$ ruby benchmark/read_tag.rb
Rehearsal ---------------------------------------------------------
mini_exiftool (2.9.0)   0.390000   0.000000   0.390000 (  0.404067)
exiftool (1.2.0)        0.030000   0.000000   0.030000 (  0.027073)
exifr (1.3.2)           1.220000   0.020000   1.240000 (  1.268672)
exif (49b6835)          0.010000   0.000000   0.010000 (  0.006862)
------------------------------------------------ total: 1.670000sec

                            user     system      total        real
mini_exiftool (2.9.0)   0.410000   0.010000   0.420000 (  0.412627)
exiftool (1.2.0)        0.030000   0.000000   0.030000 (  0.028518)
exifr (1.3.2)           1.180000   0.010000   1.190000 (  1.203806)
exif (49b6835)          0.010000   0.000000   0.010000 (  0.006925)
-----------------------------------------
60 times faster than mini_exiftool (2.9.0)
4 times faster than exiftool (1.2.0)
174 times faster than exifr (1.3.2)
```

## Tag Reference

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

1. Create, update and delete tags.
