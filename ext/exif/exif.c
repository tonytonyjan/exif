#include "exif.h"
#include "data.h"

VALUE rb_mExif;

void Init_exif(void){
  rb_mExif = rb_define_module("Exif");
  init_data();
}
