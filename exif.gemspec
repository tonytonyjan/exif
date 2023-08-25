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
  spec.add_development_dependency 'rake', '>= 0.8.1'
  spec.add_development_dependency 'rake-compiler', '~> 1.0.4'
  spec.add_development_dependency 'minitest', '~> 5.11.3'
  spec.metadata['msys2_mingw_dependencies'] = 'libexif'
end
