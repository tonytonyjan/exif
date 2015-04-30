# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'exif/version'

Gem::Specification.new do |spec|
  spec.name          = "exif"
  spec.version       = Exif::VERSION
  spec.authors       = ["Jian Weihang"]
  spec.email         = ["tonytonyjan@gmail.com"]
  spec.summary       = %q{Ruby EXIF reader written in C extension.}
  spec.description   = %q{Ruby EXIF reader written in C extension.}
  spec.homepage      = "https://github.com/tonytonyjan/exif"
  spec.license       = "MIT"
  spec.files         = Dir['lib/**/*.rb', 'ext/**/*.{h,c}']
  spec.extensions    = ["ext/extconf.rb"]
  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec"
end
