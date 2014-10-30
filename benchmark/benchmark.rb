$: << File.expand_path('../../lib', __FILE__)
require 'benchmark'
require 'exif'
require 'exifr'

N = 500
FILE_PATH = File.expand_path('../../spec/sample.jpg', __FILE__)
exifr = EXIFR::JPEG.new(FILE_PATH)
exif  = Struct.new(:image_width).new(123)#Exif::Data.new(FILE_PATH)
Benchmark.bmbm do |x|
  x.report '[exifr] init' do
    N.times{ EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report '[exif]  init' do
    N.times{ Exif::Data.new(FILE_PATH).image_width }
  end
  x.report '[exifr] get width' do
    N.times{ exifr.width }
  end
  x.report '[exif]  get width' do
    N.times{ exif.image_width }
  end
end
