#include "ruby.h"
#include "data.h"

VALUE rb_mExif, rb_eError, rb_eNotReadble, rb_eIFDNotFound, rb_eUnknownDataType;

void Init_exif(void){
  rb_mExif = rb_define_module("Exif");

  rb_eError = rb_define_class_under(rb_mExif, "Error", rb_eStandardError);
  rb_eNotReadble = rb_define_class_under(rb_mExif, "NotReadble", rb_eError);
  rb_eIFDNotFound = rb_define_class_under(rb_mExif, "IFDNotFound", rb_eError);
  rb_eUnknownDataType = rb_define_class_under(rb_mExif, "UnknownDataType", rb_eError);

  init_data();
}
