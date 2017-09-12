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

def pirnt_comparison(benchmark)
  puts '-----------------------------------------'
  exif = benchmark.pop
  benchmark.each do |tms|
    puts "#{ (tms.real / exif.real).round } times faster than #{tms.label}"
  end
end
