#ifndef DATA_H
#define DATA_H 1

#include <libexif/exif-data.h>
#include "ruby.h"

void init_data();

static VALUE rb_new(VALUE self, VALUE file_path);
static VALUE rb_dump(VALUE self);
static VALUE rb_value(VALUE self, VALUE key);

static void each_content(ExifContent *ec, void *user_data);
static void each_entry(ExifEntry *, void *user_data);

#endif /* DATA_H */
