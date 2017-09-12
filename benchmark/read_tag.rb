# frozen_string_literal: true

require File.expand_path('../env', __FILE__)

n = 1000
file_path = File.expand_path('../../test/sample.jpg', __FILE__)
tags = [:image_width, :image_length, :bits_per_sample, :photometric_interpretation, :make, :model, :orientation, :samples_per_pixel, :x_resolution, :y_resolution, :resolution_unit, :software, :date_time, :compression, :x_resolution, :y_resolution, :resolution_unit, :exposure_time, :exposure_program, :iso_speed_ratings, :exif_version, :date_time_original, :date_time_digitized, :shutter_speed_value, :aperture_value, :exposure_bias_value, :max_aperture_value, :metering_mode, :light_source, :flash, :focal_length, :color_space, :pixel_x_dimension, :pixel_y_dimension, :focal_plane_x_resolution, :focal_plane_y_resolution, :focal_plane_resolution_unit, :sensing_method, :file_source, :scene_type, :cfa_pattern, :custom_rendered, :exposure_mode, :white_balance, :digital_zoom_ratio, :focal_length_in_35mm_film, :scene_capture_type, :gain_control, :contrast, :saturation, :sharpness, :subject_distance_range, :gps_version_id, :gps_latitude_ref, :gps_latitude, :gps_longitude_ref, :gps_longitude, :gps_altitude_ref, :gps_altitude, :gps_time_stamp, :gps_map_datum, :gps_date_stamp]

mini_exiftool = MiniExiftool.new(file_path)
exifr = EXIFR::JPEG.new(file_path)
exif = Exif::Data.new(File.open(file_path))

benchmark = Benchmark.bmbm do |x|
  x.report gem_name_with_version('mini_exiftool') do
    n.times { tags.each { |tag| mini_exiftool[tag] } }
  end
  x.report gem_name_with_version('exifr') do
    n.times { tags.each { |tag| exifr.send(tag) } }
  end
  x.report "exif (#{`git rev-parse --short HEAD`.chop!})" do
    n.times { tags.each { |tag| exif.send(tag) } }
  end
end

pirnt_comparison(benchmark)
