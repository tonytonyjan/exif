# frozen_string_literal: true

require_relative 'lib/exif/version'

Gem::Specification.new do |spec|
  spec.name          = 'exif'
  spec.version       = Exif::VERSION
  spec.authors       = 'Jian Weihang'
  spec.email         = 'tonytonyjan@gmail.com'
  spec.summary       = 'Ruby EXIF reader written in C extension.'
  spec.description   = 'Ruby EXIF reader written in C extension.'
  spec.homepage      = 'https://github.com/tonytonyjan/exif'
  spec.license       = 'MIT'
  spec.files         = Dir['lib/**/*.rb', 'ext/**/*.{h,c}']
  spec.extensions    = ['ext/exif/extconf.rb']
  spec.add_development_dependency 'bundler', '~> 1.7'
  spec.add_development_dependency 'rake', '~> 10.0'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'minitest'
end
