require "bundler/gem_tasks"
require "rake/extensiontask"
require 'rspec/core/rake_task'

Rake::ExtensionTask.new("exif_ext") do |ext|
  ext.ext_dir = "ext"
end

RSpec::Core::RakeTask.new(:spec)

task default: [:compile, :spec]