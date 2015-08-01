#include <libexif/exif-data.h>
#include <time.h>
#include "data.h"

extern VALUE rb_mExif, rb_eNotReadble, rb_eIFDNotFound;
extern const char* exif_entry_to_ivar(ExifEntry* ee);

VALUE rb_cData;
static VALUE IFD2SYM[5];

void init_data(){
  int length;

  rb_cData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  IFD2SYM[EXIF_IFD_0]                = INT2NUM(0);
  IFD2SYM[EXIF_IFD_1]                = INT2NUM(1);
  IFD2SYM[EXIF_IFD_EXIF]             = ID2SYM(rb_intern("exif"));
  IFD2SYM[EXIF_IFD_GPS]              = ID2SYM(rb_intern("gps"));
  IFD2SYM[EXIF_IFD_INTEROPERABILITY] = ID2SYM(rb_intern("interoperability"));
  length = sizeof(attrs) / sizeof(char*);

  for(int i = 0; i < length; ++i) rb_define_attr(rb_cData, attrs[i], 1, 0);
  // rb_define_attr(rb_cData, "contents", 1, 0);
  // rb_define_alias(rb_cData, "to_h", "contents");
  rb_define_singleton_method(rb_cData, "new", rb_new, 1);
  // rb_define_method(rb_cData, "dump", rb_dump, 0);
  // rb_define_method(rb_cData, "[]", rb_value, 1);
}

VALUE rb_new(VALUE self, VALUE file_path){
  Check_Type(file_path, T_STRING);
  ExifData *ed = exif_data_new_from_file(StringValueCStr(file_path));
  if(!ed) rb_raise(rb_eNotReadble, "File not readable or no EXIF data in file.");
  VALUE rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
  rb_iv_set(rb_data, "@contents", rb_hash_new());
  exif_data_foreach_content(ed, each_content, &rb_data);
  return rb_data;
}

VALUE rb_dump(VALUE self){
  ExifData *ed;
  Data_Get_Struct(self, ExifData, ed);
  exif_data_dump(ed);
  return Qnil;
}

VALUE rb_value(VALUE self, VALUE key){
  VALUE rb_contents = rb_iv_get(self, "@contents");
  return rb_hash_aref(rb_contents, key);
}

void each_content(ExifContent *ec, void *self_ptr){
  VALUE *self = (VALUE*)self_ptr;
  VALUE rb_contents = rb_iv_get(*self, "@contents");
  Check_Type(rb_contents, T_HASH);
  ExifIfd ifd = exif_content_get_ifd(ec);
  VALUE ifd_name = IFD2SYM[ifd]; //rb_str_new_cstr(exif_ifd_get_name(ifd));
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eIFDNotFound, "Con't get IFD.");
  else rb_hash_aset(rb_contents, ifd_name, rb_hash_new());
  exif_content_foreach_entry(ec, each_entry, self);
}

void each_entry(ExifEntry *ee, void *self_ptr){
  VALUE *self = (VALUE*)self_ptr;
  VALUE rb_contents = rb_iv_get(*self, "@contents");
  ExifIfd ifd = exif_entry_get_ifd(ee);
  const char *attr_name = exif_entry_to_ivar(ee);
  VALUE tag_name = ID2SYM(rb_intern(attr_name + 1));
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
    value = Qnil;//process_value(self, ifd, ee->tag, buf);
  }
  rb_iv_set(*self, attr_name, value);
}