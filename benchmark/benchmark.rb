# frozen_string_literal: true

$LOAD_PATH << File.expand_path('../../lib', __FILE__)
require 'benchmark'
require 'bundler'
Bundler.setup(:benchmark)
require 'mini_exiftool'
require 'exifr/jpeg'
require 'exif'

def gem_name_with_version(gem)
  "#{gem} (#{Gem.loaded_specs[gem].version})"
end

N = 50
FILE_PATH = File.expand_path('../../test/sample.jpg', __FILE__)
benchmark = Benchmark.bmbm do |x|
  x.report gem_name_with_version('mini_exiftool') do
    N.times { MiniExiftool.new(FILE_PATH).image_width }
  end
  x.report gem_name_with_version('exifr') do
    N.times { EXIFR::JPEG.new(FILE_PATH).width }
  end
  x.report "exif (#{`git rev-parse --short HEAD`.chop!})" do
    N.times { Exif::Data.new(File.open(FILE_PATH)).image_width }
  end
end

puts '-----------------------------------------'
exif = benchmark.pop
benchmark.each do |tms|
  puts "#{ (tms.real / exif.real).round } times faster than #{tms.label}"
end
