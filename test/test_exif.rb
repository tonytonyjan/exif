# frozen_string_literal: true

require 'minitest/autorun'
require 'exif'

class TestExif < Minitest::Test
  module Shared
    def test_byte
      assert_equal 0, data.gps_altitude_ref
      assert_equal [2, 2, 0, 0], data.gps_version_id
    end

    def test_short
      assert_equal 4000, data.image_width
      assert_equal 2670, data.image_length
      assert_equal [8, 8, 8], data.bits_per_sample
      assert_equal 2, data.photometric_interpretation
      assert_equal 1, data.orientation
      assert_equal 3, data.samples_per_pixel
      assert_equal 2, data.resolution_unit
      assert_equal 6, data.compression
      assert_equal 3, data.exposure_program
      assert_equal 250, data.iso_speed_ratings
      assert_equal 2, data.metering_mode
      assert_equal 9, data.light_source
      assert_equal 16, data.flash
      assert_equal 1, data.color_space
      assert_equal 3, data.focal_plane_resolution_unit
      assert_equal 2, data.sensing_method
      assert_equal 0, data.custom_rendered
      assert_equal 0, data.exposure_mode
      assert_equal 1, data.white_balance
      assert_equal 70, data.focal_length_in_35mm_film
      assert_equal 0, data.scene_capture_type
      assert_equal 0, data.gain_control
      assert_equal 0, data.contrast
      assert_equal 0, data.saturation
      assert_equal 0, data.sharpness
      assert_equal 0, data.subject_distance_range
    end

    def test_long
      assert_equal 4000, data.pixel_x_dimension
      assert_equal 2670, data.pixel_y_dimension
    end

    def test_ascii
      assert_equal 'NIKON CORPORATION', data.make
      assert_equal 'NIKON D600', data.model
      assert_equal 'Adobe Photoshop CS6 (Macintosh)', data.software
      assert_equal '2013:12:08 21:14:11', data.date_time
      assert_equal '2013:09:10 16:31:21', data.date_time_original
      assert_equal '2013:09:10 16:31:21', data.date_time_digitized
      assert_equal '90', data.sub_sec_time_original
      assert_equal '90', data.sub_sec_time_digitized
      assert_equal 'N', data.gps_latitude_ref
      assert_equal 'E', data.gps_longitude_ref
      assert_equal 'WGS-84', data.gps_map_datum
      assert_equal '2013:09:10', data.gps_date_stamp
    end

    def test_rational
      # assert_equal Rational(72, 1), data.x_resolution
      # assert_equal Rational(72, 1), data.y_resolution
      assert_equal Rational(1, 125), data.exposure_time
      assert_equal Rational(8, 1), data.fnumber
      assert_equal Rational(870723, 125000), data.shutter_speed_value
      assert_equal Rational(6, 1), data.aperture_value
      assert_equal Rational(-1, 3), data.exposure_bias_value
      assert_equal Rational(3, 1), data.max_aperture_value
      assert_equal Rational(70, 1), data.focal_length
      assert_equal Rational(54886891, 32768), data.focal_plane_x_resolution
      assert_equal Rational(54886891, 32768), data.focal_plane_y_resolution
      assert_equal Rational(1, 1), data.digital_zoom_ratio
      assert_equal [Rational(24, 1), Rational(106817, 10000), Rational(0, 1)], data.gps_latitude
      assert_equal [Rational(121, 1), Rational(76869, 2500), Rational(0, 1)], data.gps_longitude
      assert_equal Rational(332, 1), data.gps_altitude
      assert_equal [Rational(8, 1), Rational(4, 1), Rational(25, 1)], data.gps_time_stamp
    end

    def test_double
      # TODO
    end

    def test_float
      # TODO
    end

    def data
      raise NotImplementedError
    end
  end

  class TestLoadFromString < Minitest::Test
    include Shared

    def data
      @data ||= Exif::Data.new(IO.read(File.expand_path('../sample.jpg', __FILE__), mode: 'rb'))
    end
  end

  class TestLoadFromIO < Minitest::Test
    include Shared

    def data
      @data ||= Exif::Data.new(File.open(File.expand_path('../sample.jpg', __FILE__)))
    end
  end

  def test_not_readble
    assert_raises(Exif::NotReadble) { Exif::Data.new 'not readable' }
    assert_raises Exif::NotReadble do
      Exif::Data.new(File.open(File.expand_path('../not_readable.jpg', __FILE__)))
    end
  end

  def test_type_error
    assert_raises TypeError do
      Exif::Data.new(123)
    end
  end
end
