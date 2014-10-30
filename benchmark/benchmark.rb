$: << File.expand_path('../../lib', __FILE__)
require 'benchmark'
require 'exif'
require 'exifr'

N = 500
FILE_PATH = File.expand_path('../../spec/sample.jpg', __FILE__)
Benchmark.bmbm do |x|
  x.report '[exifr] init' do
    N.times{ EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report '[exif]  init' do
    N.times{ Exif::Data.new(FILE_PATH).image_width }
  end
end
