require "bundler/gem_tasks"
require "rake/extensiontask"
require "rake/testtask"

Rake::ExtensionTask.new 'exif' do |ext|
  ext.lib_dir = "lib/#{ext.name}"
end

Rake::TestTask.new do |t|
  t.test_files = FileList['test/test*.rb']
  t.verbose = true
end

task default: %i[compile test]
