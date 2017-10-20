# frozen_string_literal: true

require 'minitest/autorun'
require 'exif'

class TestExif < Minitest::Test
  module Shared
    def test_brackets
      assert_equal data.ifds[:ifd0], data[:ifd0]
      assert_equal data.ifds[:ifd1], data[:ifd1]
      assert_equal data.ifds[:exif], data[:exif]
      assert_equal data.ifds[:gps], data[:gps]
      assert_equal data.ifds[:interoperability], data[:interoperability]
    end

    def test_to_h
      assert_equal data.ifds, data.to_h
    end

    def test_ifds
      assert_equal({
        image_width: 4000,
        image_length: 2670,
        bits_per_sample: [8, 8, 8],
        photometric_interpretation: 2,
        make: 'NIKON CORPORATION',
        model: 'NIKON D600',
        orientation: 1,
        samples_per_pixel: 3,
        x_resolution: Rational(300, 1),
        y_resolution: Rational(300, 1),
        resolution_unit: 2,
        software: 'Adobe Photoshop CS6 (Macintosh)',
        date_time: '2013:12:08 21:14:11'
      }, data.ifds[:ifd0])

      assert_equal({
        compression: 6,
        x_resolution: Rational(72, 1),
        y_resolution: Rational(72, 1),
        resolution_unit: 2
      }, data.ifds[:ifd1])

      assert_equal({
        exposure_time: Rational(1, 125),
        fnumber: Rational(8, 1),
        exposure_program: 3,
        iso_speed_ratings: 250,
        exif_version: '0230',
        date_time_original: '2013:09:10 16:31:21',
        date_time_digitized: '2013:09:10 16:31:21',
        shutter_speed_value: Rational(870723, 125000),
        aperture_value: Rational(6, 1),
        exposure_bias_value: Rational(-1, 3),
        max_aperture_value: Rational(3, 1),
        metering_mode: 2,
        light_source: 9,
        flash: 16,
        focal_length: Rational(70, 1),
        sub_sec_time_original: '90',
        sub_sec_time_digitized: '90',
        color_space: 1,
        pixel_x_dimension: 4000,
        pixel_y_dimension: 2670,
        focal_plane_x_resolution: Rational(54886891, 32768),
        focal_plane_y_resolution: Rational(54886891, 32768),
        focal_plane_resolution_unit: 3,
        sensing_method: 2,
        file_source: "\x03",
        scene_type: "\x01",
        new_cfa_pattern: "\x02\x00\x02\x00\x00\x01\x01\x02",
        custom_rendered: 0,
        exposure_mode: 0,
        white_balance: 1,
        digital_zoom_ratio: Rational(1, 1),
        focal_length_in_35mm_film: 70,
        scene_capture_type: 0,
        gain_control: 0,
        contrast: 0,
        saturation: 0,
        sharpness: 0,
        subject_distance_range: 0,
        flash_pix_version: '0100'
      }, data.ifds[:exif])

      assert_equal({
        gps_version_id: [2, 2, 0, 0],
        gps_latitude_ref: 'N',
        gps_latitude: [Rational(24, 1), Rational(106817, 10000), Rational(0, 1)],
        gps_longitude_ref: 'E',
        gps_longitude: [Rational(121, 1), Rational(76869, 2500), Rational(0, 1)],
        gps_altitude_ref: 0,
        gps_altitude: Rational(332, 1),
        gps_time_stamp: [Rational(8, 1), Rational(4, 1), Rational(25, 1)],
        gps_map_datum: 'WGS-84',
        gps_date_stamp: '2013:09:10'
      }, data.ifds[:gps])

      assert_equal({}, data.ifds[:interoperability])
    end

    def test_undefined
      assert_equal '0230', data.exif_version
      assert_equal "\x03", data.file_source
      assert_equal "\x01", data.scene_type
      assert_equal "\x02\x00\x02\x00\x00\x01\x01\x02", data.new_cfa_pattern
      assert_equal '0100', data.flash_pix_version
    end

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
      assert_equal Rational(300, 1), data.x_resolution
      assert_equal Rational(300, 1), data.y_resolution
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
      @data ||= Exif::Data.new(IO.read(File.expand_path('../images/sample.jpg', __FILE__), mode: 'rb'))
    end
  end

  class TestLoadFromIO < Minitest::Test
    include Shared

    def data
      @data ||= Exif::Data.new(File.open(File.expand_path('../images/sample.jpg', __FILE__)))
    end
  end

  class TestLoadFromStringIO < Minitest::Test
    include Shared

    def data
      str = StringIO.new(IO.read(File.expand_path('../images/sample.jpg', __FILE__), mode: 'rb'))
      @data ||= Exif::Data.new(str)
    end
  end

  class TestLoadFromTempfil < Minitest::Test
    include Shared

    def data
      tmpfile = Tempfile.new('foo')
      tmpfile.write(IO.read(File.expand_path('../images/sample.jpg', __FILE__), mode: 'rb'))
      tmpfile.rewind
      @data ||= Exif::Data.new(tmpfile)
    end
  end

  def test_not_readable
    assert_raises(Exif::NotReadable) { Exif::Data.new 'not readable' }
    assert_raises Exif::NotReadable do
      Exif::Data.new(File.open(File.expand_path('../images/not_readable.jpg', __FILE__)))
    end
  end

  def test_depreacted_not_readble
    assert_output(nil, /NotReadble is deprecated/) do
      assert_equal Exif::NotReadable, Exif::NotReadble
    end
  end

  def test_type_error
    assert_raises TypeError do
      Exif::Data.new(123)
    end
  end

  def test_nan
    data = Exif::Data.new(IO.read(File.expand_path('../images/nan.jpg', __FILE__)))
    assert data.exposure_index.nan?
  end

  def test_infinity
    data = Exif::Data.new(IO.read(File.expand_path('../images/infinity.jpg', __FILE__)))
    assert_equal Float::INFINITY, data.exposure_index
    assert_equal -Float::INFINITY, data.exposure_bias_value
  end
end
