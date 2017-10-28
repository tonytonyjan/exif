<a name="2.2.0"></a>
# [2.2.0](https://github.com/tonytonyjan/exif/compare/v2.1.0...v2.2.0) (2017-10-28)

### Features

* Support parsing raw EXIF data ([
3c0dfe1](https://github.com/tonytonyjan/exif/commit/3c0dfe1)), closes [#16](https://github.com/tonytonyjan/exif/issues/16)

<a name="2.1.0"></a>
# [2.1.0](https://github.com/tonytonyjan/exif/compare/v2.0.1...v2.1.0) (2017-10-20)


### Features

* support objects which has `#read` implemented ([641a42a](https://github.com/tonytonyjan/exif/commit/641a42a)), closes [#13](https://github.com/tonytonyjan/exif/issues/13)

### Deprecations

* `Exif::NotReadble` is renamed to `Exif::NotReadable` and will be removed in the next release.

<a name="2.0.1"></a>
## [2.0.1](https://github.com/tonytonyjan/exif/compare/v2.0.0...v2.0.1) (2017-10-02)


### Bug Fixes

* handle negative infinity ([51dc679](https://github.com/tonytonyjan/exif/commit/51dc679))
* handle zero denominator ([b927674](https://github.com/tonytonyjan/exif/commit/b927674))



<a name="2.0.0"></a>
# [2.0.0](https://github.com/tonytonyjan/exif/compare/v1.0.1...v2.0.0) (2017-09-12)


### Bug Fixes

* support ruby 1.9.3 ([e7748ac](https://github.com/tonytonyjan/exif/commit/e7748ac))


### Features

* add Data#[] ([9fbf4d7](https://github.com/tonytonyjan/exif/commit/9fbf4d7))
* add Data#ifds ([648841f](https://github.com/tonytonyjan/exif/commit/648841f))
* alias #ifds to #to_h ([5e2c876](https://github.com/tonytonyjan/exif/commit/5e2c876))
* make Exif::Data#new accept either String or IO instance ([d6af39a](https://github.com/tonytonyjan/exif/commit/d6af39a))
* make fd0's priority higher than fd1 when same tag being used by both ([1db5274](https://github.com/tonytonyjan/exif/commit/1db5274))
* support undefined type ([1c8e4e8](https://github.com/tonytonyjan/exif/commit/1c8e4e8))
* typecast all tags ([95c1185](https://github.com/tonytonyjan/exif/commit/95c1185))


### BREAKING CHANGES

* All top-level APIs will seek ifd0 tags first instead of ifd1, to retrieve tags from ifd1, use `Exif::Data#[]`, for example:

    ```ruby
    # to retrieve image width of ifd0
    data.image_width
    data[:ifd0][:image_width]
    # to retrieve image width of ifd1
    data[:ifd1][:image_width]
    ```
* Passing String to Data#new is no longer treated as a file path, instead, it loads as binary data. Therefore, `Data.new(path)` has to be changed to `Data.new(File.open(path))` or `Data.new(IO.read(path))`.
* Instead of returning String, all tags are now typecasted according to TIFF tag. Tags for time like DateTimeOriginal is nolonger typecasted to Time object.
To convert to Time object, use `Time::strptime` instea, for example:

    ```ruby
    Time.strptime(data.date_time_original, '%Y:%m:%d %H:%M:%S')
    ```
