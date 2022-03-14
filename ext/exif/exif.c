#include "data.h"
#include "ruby.h"

VALUE rb_mExif, rb_eError, rb_eNotReadable, rb_eIFDNotFound,
    rb_eUnknownDataType;

VALUE rb_exif_const_missing(VALUE klass, VALUE name) {
  const char not_readble[] = "NotReadble";
  if (strcmp(rb_id2name(SYM2ID(name)), not_readble) == 0) {
    rb_warn("constant NotReadble is deprecated and will be removed in the "
            "future, use NotReadable instead.");
    return rb_eNotReadable;
  }
  return rb_call_super(1, &name);
}

void Init_exif(void) {
  rb_mExif = rb_define_module("Exif");

  rb_eError = rb_define_class_under(rb_mExif, "Error", rb_eStandardError);
  rb_eNotReadable = rb_define_class_under(rb_mExif, "NotReadable", rb_eError);
  rb_eIFDNotFound = rb_define_class_under(rb_mExif, "IFDNotFound", rb_eError);
  rb_eUnknownDataType =
      rb_define_class_under(rb_mExif, "UnknownDataType", rb_eError);
  rb_define_singleton_method(rb_mExif, "const_missing", rb_exif_const_missing,
                             1);
  rb_define_const(rb_mExif, "LIBEXIF_VERSION", rb_str_new2(LIBEXIF_VERSION));
  init_data();
}
