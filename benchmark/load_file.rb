# frozen_string_literal: true

require File.expand_path('../env', __FILE__)

n = 100
file_path = File.expand_path('../../test/images/sample.jpg', __FILE__)

benchmark = Benchmark.bmbm do |x|
  x.report gem_name_with_version('mini_exiftool') do
    n.times { MiniExiftool.new(file_path).image_width }
  end
  x.report gem_name_with_version('exiftool') do
    n.times { Exiftool.new(file_path)[:image_width] }
  end
  x.report gem_name_with_version('exifr') do
    n.times { EXIFR::JPEG.new(file_path).width }
  end
  x.report "exif (#{`git rev-parse --short HEAD`.chop!})" do
    n.times { File.open(file_path, 'rb') { |f| Exif::Data.new(f).image_width } }
  end
end

print_comparison(benchmark)
