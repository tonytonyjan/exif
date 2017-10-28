# frozen_string_literal: true

require File.expand_path('../env', __FILE__)

n = 100
file_path = File.expand_path('../../test/images/sample.jpg', __FILE__)
data = IO.binread(file_path)

benchmark = Benchmark.bmbm do |x|
  x.report gem_name_with_version('mini_exiftool') do
    n.times { MiniExiftool.new(StringIO.new(data)).image_width }
  end
  x.report gem_name_with_version('exifr') do
    n.times { EXIFR::JPEG.new(StringIO.new(data)).width }
  end
  x.report "exif (#{`git rev-parse --short HEAD`.chop!})" do
    n.times { Exif::Data.new(data).image_width }
  end
end

print_comparison(benchmark)
