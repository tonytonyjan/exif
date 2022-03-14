# frozen_string_literal: true

require 'mkmf'
$CFLAGS << ' -std=c99 '
pkg_config('libexif')
have_library('exif')
have_header('libexif/exif-data.h')

checking_for 'libexif >= 0.6.22' do
  if try_const(%w[EXIF_TAG_LENS_SPECIFICATION ExifTag], 'libexif/exif-tag.h')
    append_cflags('-D LIBEXIF_0_6_22_OR_HIGHER')
  end
end

checking_for 'libexif >= 0.6.23' do
  if try_const(%w[EXIF_TAG_IMAGE_DEPTH ExifTag], 'libexif/exif-tag.h')
    append_cflags('-D LIBEXIF_0_6_23_OR_HIGHER')
  end
end

libexif_version = pkg_config('libexif', 'modversion') || 'unknown'
append_cflags("-D LIBEXIF_VERSION='\"#{libexif_version}\"'")

create_makefile('exif/exif')
