# frozen_string_literal: true

require File.expand_path('../lib/exif/version', __FILE__)

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
  spec.add_development_dependency 'bundler', '~> 1.16.1'
  spec.add_development_dependency 'rake', '~> 12.3.0'
  spec.add_development_dependency 'rake-compiler', '~> 1.0.4'
  spec.add_development_dependency 'minitest', '~> 5.10.3'
end
