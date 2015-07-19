#ifndef TinyRLE_H
#define TinyRLE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
//#include "WProgram.h"

#endif

class TinyRLE {
  int data_size;
  char endian_;
  unsigned char *precious_data;
 public:
  TinyRLE (unsigned char* inbuf, unsigned char* outbuf, int inbuf_size,
          char endian);
  TinyRLE (unsigned char* encoded_data, int encoded_data_size) :
      data_size(encoded_data_size), precious_data(encoded_data) {}
  int GetDataSize(void);
  void GetData(int index, unsigned char* outbuf, int outbuf_size);
};

#endif
