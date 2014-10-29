#include <libexif/exif-data.h>
#include "data.h"

extern VALUE rb_mExif;
VALUE rb_cData;
static VALUE IFD2SYM[5];

void init_data(){
  rb_cData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  rb_define_singleton_method(rb_cData, "new", rb_new, 1);
  rb_define_method(rb_cData, "dump", rb_dump, 0);
  IFD2SYM[EXIF_IFD_0] = ID2SYM(rb_intern("ifd_0"));
  IFD2SYM[EXIF_IFD_1] = ID2SYM(rb_intern("ifd_1"));
  IFD2SYM[EXIF_IFD_EXIF] = ID2SYM(rb_intern("exif"));
  IFD2SYM[EXIF_IFD_GPS] = ID2SYM(rb_intern("gps"));
  IFD2SYM[EXIF_IFD_INTEROPERABILITY] = ID2SYM(rb_intern("interoperability"));
}

static VALUE rb_new(VALUE self, VALUE file_path){
  Check_Type(file_path, T_STRING);
  ExifData *ed = exif_data_new_from_file(StringValueCStr(file_path));
  if(!ed) rb_raise(rb_eRuntimeError, "File not readable or no EXIF data in file.");
  VALUE rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  rb_define_attr(self, "contents", 1, 0);
  VALUE rb_h = rb_hash_new();
  rb_hash_set_ifnone(rb_h, rb_hash_new());
  exif_data_foreach_content(ed, each_content, &rb_h);
  rb_iv_set(rb_data, "@contents", rb_h);
  return rb_data;
}

static VALUE rb_dump(VALUE self){
  ExifData *ed;
  Data_Get_Struct(self, ExifData, ed);
  exif_data_dump(ed);
  return Qnil;
}

static void each_content(ExifContent *ec, void *rb_h_ptr){
  VALUE *rb_h = (VALUE*)rb_h_ptr;
  ExifIfd ifd = exif_content_get_ifd(ec);
  VALUE symbol = IFD2SYM[ifd];
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eRuntimeError, "Con't get IFD.");
  else rb_hash_aset(*rb_h, symbol, rb_hash_new());
  exif_content_foreach_entry(ec, each_entry, rb_h_ptr);
}

static void each_entry(ExifEntry *ee, void *rb_h_ptr){
  VALUE *rb_h = (VALUE*)rb_h_ptr;
  ExifIfd ifd = exif_entry_get_ifd(ee);
  const char *name = exif_tag_get_name_in_ifd(ee->tag, ifd);
  char buf[500];
  exif_entry_get_value(ee, buf, sizeof(buf));
  rb_hash_aset(rb_hash_aref(*rb_h, IFD2SYM[ifd]), rb_str_new_cstr(name), rb_str_new_cstr(buf));
}