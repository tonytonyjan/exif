## [2.2.3](https://github.com/tonytonyjan/exif/compare/v2.2.2...v2.2.3) (2022-03-11)

### Bug Fixes

- support all libexif versions since v0.6.21.


## [2.2.2](https://github.com/tonytonyjan/exif/compare/v2.2.1...v2.2.2) (2022-03-11)

### Features

- add missing tags ([26f9bac](https://github.com/tonytonyjan/exif/commit/26f9bac108584b2874b8e70edc80a7291f61d5b8))

# [2.1.0](https://github.com/tonytonyjan/exif/compare/v2.0.1...v2.1.0) (2017-10-20)

### Features

- support objects which has `#read` implemented ([641a42a](https://github.com/tonytonyjan/exif/commit/641a42a3be475054bb80f62d8c954c9e487486d4)), closes [#13](https://github.com/tonytonyjan/exif/issues/13)

## [2.0.1](https://github.com/tonytonyjan/exif/compare/v2.0.0...v2.0.1) (2017-10-02)

### Bug Fixes

- handle negative infinity ([c2c578b](https://github.com/tonytonyjan/exif/commit/c2c578bddfe390f69d80904795f7f57f57b085f3))
- handle zero denominator ([0a0ca2e](https://github.com/tonytonyjan/exif/commit/0a0ca2e7c04bc0dd5cbcf8a67b9bacc0bd4696a1)), closes [#12](https://github.com/tonytonyjan/exif/issues/12)

# [2.0.0](https://github.com/tonytonyjan/exif/compare/v1.0.1...v2.0.0) (2017-09-12)

### Bug Fixes

- support ruby 1.9.3 ([e7748ac](https://github.com/tonytonyjan/exif/commit/e7748ac4ec66c01f80fd52f6729dea78e20a8b2f))

### Features

- add Data#[] ([9fbf4d7](https://github.com/tonytonyjan/exif/commit/9fbf4d7f0a4ad1e2405a227ea534fd1f1dd8c2e7))
- add Data#ifds ([648841f](https://github.com/tonytonyjan/exif/commit/648841f8d275a14ac18d4d6c2bd603441ba5482e))
- alias #ifds to #to_h ([5e2c876](https://github.com/tonytonyjan/exif/commit/5e2c87684efb949950bcbdae6f258dde37cb0b7b))
- make Exif::Data#new accept either String or IO instance ([d6af39a](https://github.com/tonytonyjan/exif/commit/d6af39a8727b7c862844d63d55e230d89d9d44ac))
- make fd0's priority higher than fd1 when same tag being used by both ([1db5274](https://github.com/tonytonyjan/exif/commit/1db52747c5e454fe9e534cc371dc7188268446be))
- support undefined type ([1c8e4e8](https://github.com/tonytonyjan/exif/commit/1c8e4e800666a9e69d324940e2adca0f651bbadd))
- typecast all tags ([95c1185](https://github.com/tonytonyjan/exif/commit/95c1185c7d6e073dba71eb07b1fb389648ee5cc0))

### BREAKING CHANGES

- All top-level APIs will seek ifd0 tags first instead of ifd1, to retrieve tags from ifd1, use `Exif::Data#[]`, for example:

  ```ruby
  # to retrieve image width of ifd0
  data.image_width
  data[:ifd0][:image_width]
  # to retrieve image width of ifd1
  data[:ifd1][:image_width]
  ```

- Passing String to Data#new is no longer treated as a file path, instead, it loads as binary data. Therefore, `Data.new(path)` has to be changed to `Data.new(File.open(path))` or `Data.new(IO.read(path))`.
- Instead of returning String, all tags are now typecasted according to TIFF tag. Tags for time like DateTimeOriginal is nolonger typecasted to Time object.
  To convert to Time object, use `Time::strptime` instea, for example:

      ```ruby
      Time.strptime(data.date_time_original, '%Y:%m:%d %H:%M:%S')
      ```
