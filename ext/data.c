#include <libexif/exif-data.h>
#include <time.h>
#include "data.h"

extern VALUE rb_mExif, rb_eNotReadble, rb_eIFDNotFound, rb_eUnknownDataType;
extern const char* exif_entry_to_ivar(ExifEntry* entry);

VALUE rb_cData;

void init_data(){
  int length;

  rb_cData = rb_define_class_under(rb_mExif, "Data", rb_cObject);
  length = sizeof(attrs) / sizeof(char*);

  for(int i = 0; i < length; ++i) rb_define_attr(rb_cData, attrs[i], 1, 0);
  rb_define_singleton_method(rb_cData, "new", rb_new, 1);
  rb_define_method(rb_cData, "dump", rb_dump, 0);
}

VALUE rb_new(VALUE self, VALUE file_path){
  Check_Type(file_path, T_STRING);

  ExifData* ed;
  VALUE rb_data;

  ed = exif_data_new_from_file(StringValueCStr(file_path));
  if(!ed) rb_raise(rb_eNotReadble, "File not readable or no EXIF data in file.");

  rb_data = Data_Wrap_Struct(self, NULL, exif_data_free, ed);
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

void each_content(ExifContent *ec, void *self_ptr){
  VALUE *self;
  ExifIfd ifd;

  self = (VALUE*)self_ptr;
  ifd = exif_content_get_ifd(ec);
  if(ifd == EXIF_IFD_COUNT) rb_raise(rb_eIFDNotFound, "Con't get IFD.");

  exif_content_foreach_entry(ec, each_entry, self);
}

void each_entry(ExifEntry *entry, void *self_ptr){
  exif_entry_dump(entry, 2);
  VALUE *self, value;
  const char *ivar_name;

  self = (VALUE*)self_ptr;
  ivar_name = exif_entry_to_ivar(entry);
  value = exif_entry_to_value(entry);

  rb_iv_set(*self, ivar_name, value);
}

VALUE exif_entry_to_value(ExifEntry *entry){
  ExifData  *data;
  ExifByteOrder order;
  ExifRational rational;
  ExifSRational srational;
  VALUE ret;
  int i;
  unsigned char size;

  data = entry->parent->parent;
  order = exif_data_get_byte_order(data);
  ret = Qnil;
  size = exif_format_get_size(entry->format);

  switch(entry->format){
  case EXIF_FORMAT_ASCII:
    ret = rb_str_new2((const char *)entry->data);
    break;
  case EXIF_FORMAT_SHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_short(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_short(entry->data, order));
    break;
  case EXIF_FORMAT_SSHORT:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, INT2FIX(exif_get_sshort(entry->data + i*size, order)));
    }else ret = INT2FIX(exif_get_sshort(entry->data, order));
    break;
  case EXIF_FORMAT_RATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++){
        srational = exif_get_srational(entry->data + i*size, order);
        rb_ary_push(ret, rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator)));
      }
    }else{
      srational = exif_get_srational(entry->data, order);
      ret = rb_rational_new(LONG2FIX(srational.numerator), LONG2FIX(srational.denominator));
    }
    break;
  case EXIF_FORMAT_SRATIONAL:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++){
        rational = exif_get_rational(entry->data + i*size, order);
        rb_ary_push(ret, rb_rational_new(LONG2FIX(rational.numerator), LONG2FIX(rational.denominator)));
      }
    }else{
      rational = exif_get_rational(entry->data, order);
      ret = rb_rational_new(LONG2FIX(rational.numerator), LONG2FIX(rational.denominator));
    }
    break;
  case EXIF_FORMAT_BYTE:
  case EXIF_FORMAT_SBYTE:
    ret = rb_str_new((const char *)entry->data, entry->size);
    break;
  case EXIF_FORMAT_LONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, LONG2NUM(exif_get_long(entry->data + i*size, order)));
    }else ret = LONG2NUM(exif_get_long(entry->data, order));
    break;
  case EXIF_FORMAT_SLONG:
    if(entry->components > 1){
      ret = rb_ary_new2(entry->components);
      for(int i = 0; i < entry->components; i++)
        rb_ary_push(ret, LONG2NUM(exif_get_slong(entry->data + i*size, order)));
    }else ret = LONG2NUM(exif_get_slong(entry->data, order));
    break;
  case EXIF_FORMAT_FLOAT:
    // TODO
    break;
  case EXIF_FORMAT_DOUBLE:
    // TODO
    break;
  case EXIF_FORMAT_UNDEFINED:
    // TODO
    break;
  }

  // if(NIL_P(ret)) rb_raise(rb_eUnknownDataType, "Unknown data type");

  return ret;
}