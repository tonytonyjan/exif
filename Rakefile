require "bundler/gem_tasks"
require "rake/extensiontask"
require 'rspec/core/rake_task'

Rake::ExtensionTask.new("exif") do |ext|
  ext.lib_dir = "lib/exif"
end

RSpec::Core::RakeTask.new(:spec)

task default: :spec