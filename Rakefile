# frozen_string_literal: true

require 'rake/extensiontask'
require 'rake/testtask'

Rake::ExtensionTask.new 'exif' do |ext|
  ext.lib_dir = "lib/#{ext.name}"
end

Rake::TestTask.new do |t|
  t.test_files = FileList['test/test*.rb']
  t.verbose = true
end

spec = Gem::Specification.load(File.expand_path('..//exif.gemspec', __FILE__))
Gem::PackageTask.new(spec).define

task default: [:compile, :test]
