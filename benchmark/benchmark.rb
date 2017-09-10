# frozen_string_literal: true

$LOAD_PATH << File.expand_path('../../lib', __FILE__)
require 'benchmark'
require 'mini_exiftool'
require 'exifr'
require 'exif'

N = 50
FILE_PATH = File.expand_path('../../spec/sample.jpg', __FILE__)
Benchmark.bmbm do |x|
  x.report 'mini_exiftool' do
    N.times { MiniExiftool.new(FILE_PATH).image_width }
  end
  x.report 'exifr' do
    N.times { EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report 'exif' do
    N.times { Exif::Data.new(FILE_PATH).image_width }
  end
end

puts Time.now.utc
