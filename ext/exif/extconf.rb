# frozen_string_literal: true

require 'mkmf'
$CFLAGS << ' -std=c99 '
pkg_config('libexif')
have_library('exif')
have_header('libexif/exif-data.h')
create_makefile('exif/exif')
