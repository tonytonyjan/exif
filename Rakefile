require "rake/extensiontask"
require "rake/testtask"

Rake::ExtensionTask.new 'exif' do |ext|
  ext.lib_dir = "lib/#{ext.name}"
end

Rake::TestTask.new do |t|
  t.test_files = FileList['test/test*.rb']
  t.verbose = true
end

Gem::PackageTask.new(Gem::Specification.load("#{__dir__}/exif.gemspec")).define

task default: %i[compile test]
