# frozen_string_literal: true

require 'minitest/autorun'
require 'exif'

class TestExif < Minitest::Test
  def test_it_works
    data = Exif::Data.new(File.expand_path('../sample.jpg', __FILE__))
    assert_equal data.model, 'NIKON D600'
    assert_equal data.image_width, 4000
    assert_equal data.bits_per_sample, [8, 8, 8]
    assert_equal data.gps_latitude, [Rational(24, 1), Rational(106_817, 10_000), Rational(0, 1)]
    assert_equal data.gps_time_stamp, [Rational(8, 1), Rational(4, 1), Rational(25, 1)]
    assert_equal data.gps_altitude, Rational(332, 1)
    assert_equal data.pixel_x_dimension, 4000
    assert_equal data.gps_version_id, "\x02\x02\x00\x00"
    assert_equal data.gps_altitude_ref, "\x00"
    assert_equal data.date_time, Time.new(2013, 12, 8, 21, 14, 11)
    assert_equal data.date_time_original, Time.new(2013, 9, 10, 16, 31, 21)
    assert_equal data.gps_date_stamp, Time.new(2013, 9, 10)
  end

  def test_not_readble
    assert_raises Exif::NotReadble do
      Exif::Data.new(File.expand_path('../not_readable.jpg', __FILE__))
    end
  end
end
