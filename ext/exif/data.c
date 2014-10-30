#include <libexif/exif-data.h>
#include "data.h"

extern VALUE rb_mExif;
VALUE rb_cData;
static VALUE IFD2SYM[5];

void init_data(){
  rb_cData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  IFD2SYM[0] = INT2NUM(0);                            // EXIF_IFD_0
  IFD2SYM[1] = INT2NUM(1);                            // EXIF_IFD_1
  IFD2SYM[2] = ID2SYM(rb_intern("exif"));             // EXIF_IFD_EXIF
  IFD2SYM[3] = ID2SYM(rb_intern("gps"));              // EXIF_IFD_GPS
  IFD2SYM[4] = ID2SYM(rb_intern("interoperability")); // EXIF_IFD_INTEROPERABILITY
  rb_define_singleton_method(rb_cData, "new", rb_new, 1);
  rb_define_method(rb_cData, "dump", rb_dump, 0);
  rb_define_method(rb_cData, "[]", rb_value, 1);
}

static VALUE rb_new(VALUE self, VALUE file_path){
  Check_Type(file_path, T_STRING);
  ExifData *ed = exif_data_new_from_file(StringValueCStr(file_path));
  if(!ed) rb_raise(rb_eRuntimeError, "File not readable or no EXIF data in file.");
  VALUE rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  rb_define_attr(self, "contents", 1, 0);
  VALUE rb_h = rb_hash_new();
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

static VALUE rb_value(VALUE self, VALUE key){
  VALUE rb_contents = rb_iv_get(self, "@contents");
  return rb_hash_aref(rb_contents, key);
}

static void each_content(ExifContent *ec, void *rb_h_ptr){
  VALUE *rb_h = (VALUE*)rb_h_ptr;
  ExifIfd ifd = exif_content_get_ifd(ec);
  VALUE ifd_name = IFD2SYM[ifd]; //rb_str_new_cstr(exif_ifd_get_name(ifd));
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eRuntimeError, "Con't get IFD.");
  else rb_hash_aset(*rb_h, ifd_name, rb_hash_new());
  exif_content_foreach_entry(ec, each_entry, rb_h_ptr);
}

static void each_entry(ExifEntry *ee, void *rb_h_ptr){
  VALUE *rb_h = (VALUE*)rb_h_ptr;
  ExifIfd ifd = exif_entry_get_ifd(ee);
  VALUE tag_name = rb_str_new_cstr(exif_tag_get_name_in_ifd(ee->tag, ifd));
  rb_funcall(rb_mExif, rb_intern("underscore!"), 1, tag_name);
  tag_name = ID2SYM(rb_intern_str(tag_name));
  char buf[500];
  exif_entry_get_value(ee, buf, sizeof(buf));
  VALUE value;
  switch(ee->format){
  // case EXIF_FORMAT_BYTE:
  //   break;
  // case EXIF_FORMAT_ASCII:
  //   break;
  case EXIF_FORMAT_SHORT:
    value = INT2NUM(atoi(buf));
    break;
  case EXIF_FORMAT_LONG:
    value = INT2NUM(atol(buf));
    break;
  // case EXIF_FORMAT_RATIONAL:
  //   break;
  // case EXIF_FORMAT_SBYTE:
  //   break;
  // case EXIF_FORMAT_UNDEFINED:
  //   break;
  // case EXIF_FORMAT_SSHORT:
  //   break;
  // case EXIF_FORMAT_SLONG:
  //   break;
  // case EXIF_FORMAT_SRATIONAL:
  //   break;
  case EXIF_FORMAT_FLOAT:
    value = rb_float_new(atof(buf));
    break;
  case EXIF_FORMAT_DOUBLE:
    value = rb_float_new(atof(buf));
    break;
  default:
    value = rb_str_new_cstr(buf);
  }
  rb_hash_aset(rb_hash_aref(*rb_h, IFD2SYM[ifd]), tag_name, value);
  if(NIL_P(rb_hash_aref(*rb_h, tag_name))) rb_hash_aset(*rb_h, tag_name, value);
}