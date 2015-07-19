// This is a byte encoding, where lengths of 1's and 0's are between
// 8 and 256 dots long, good for fonts between 24 and 4000 point,
// or images with similar attributes.
// The encoding is: A B A B ... with each A as a count of zeros,
// and each B as a count of ones.

#include <iostream>

#include "TinyRLE.h"

// Args:
//   inbuf: Input uncompressed buffer.
//   outbuf: Output compressed buffer.
//   inbuf_size: Size of input buffer.
//   endian: 'L' for little endian, 'B' for big endian.
TinyRLE::TinyRLE(unsigned char *inbuf, unsigned char *outbuf,
    int inbuf_size, char endian) {
  // Simultaneously encode.
  int inbuf_index = 0;
  int outbuf_index = 0;
  int A_count = 0;
  int B_count = 0;
  int bit_select = 0;
  unsigned char inbuf_byte, wb;  // Working Byte
  unsigned char AB_state = 'A';
  endian_ = endian;
  while (inbuf_index < inbuf_size) {
    inbuf_byte = inbuf[inbuf_index];
    for (bit_select = 0; bit_select < 8; bit_select++) {
      // shift left/right for big/small endian.
      if (endian_ == 'L') {
        wb = 0x01 & inbuf_byte;  // 0x80 & c for other endian.
      } else {
        wb = 0x80 & inbuf_byte;
      }
      // wb now has bit in question.
      if (wb != 0x0) { // one
        if (AB_state == 'A') {  // changed: 01
          outbuf[outbuf_index] = A_count;
          outbuf_index++;
          AB_state = 'B';
          B_count = 1;
          A_count = 0;
        } else { // 11
          B_count++;
          if (B_count > 254) { // up to 254 is ok. 255 is time to emit.
            outbuf[outbuf_index] = 0xff;
            outbuf_index++;
            B_count = 1;
          } 
        }
      } else { // zero
        if (AB_state == 'B') {  // changed: 10
          outbuf[outbuf_index] = B_count;
          outbuf_index++;
          AB_state = 'A';
          B_count = 0;
          A_count = 1;
        } else { // 00
          A_count++;
          if (A_count > 254) { // up to 254 is ok. 255 is time to emit.
            outbuf[outbuf_index] = 0xff;
            outbuf_index++;
            A_count = 1;
          } 
        } 
      }
      if (endian_ == 'L') {
        inbuf_byte >>= 1;
      } else {
        inbuf_byte <<= 1;
      }
    }
    inbuf_index++;
  }
  // clean up...
  // in theory we got here somehow with active counters..
  // Always end on 0/1 0/1 2-byte encodings.
  if (AB_state == 'A') {  // emit 0,1
    outbuf[outbuf_index] = A_count;
    outbuf_index++;
    outbuf[outbuf_index] = B_count;
    outbuf_index++;
  } else {  // emit only 1
    outbuf[outbuf_index] = B_count;
    outbuf_index++;
  }
  precious_data = outbuf;
  data_size = outbuf_index;
}


int TinyRLE::GetDataSize() {
 return data_size;
}


void TinyRLE::GetData(int index, unsigned char *outbuf, int outbuf_size) {
  int precious_index = 0;
  int encoded_index = 0;  // count of all bit sums up to precious_index.
  int outbuf_index = 0;
  int bitcount;
  unsigned char wb;
  char AB_state = 'A';
  int remainder;
  int bit_select;

  index *= 8;  // Byte offset.
  
  bitcount = precious_data[precious_index];
  while (encoded_index < index) {   // skip ahead until next count gets to index
		if (bitcount + encoded_index > index) {
      break;
    }
    encoded_index += bitcount;
    precious_index++;
    if (AB_state == 'A') {
       AB_state = 'B';
    } else {
       AB_state = 'A';
    }
    bitcount = precious_data[precious_index];
  }
  // Now we push data a byte at a time, with two counters:
  // // output buffer, and output bit x 8....
  // // precious index
  // // working bitcount
  remainder = encoded_index + bitcount - index;
  while (outbuf_index < outbuf_size) {
    wb = 0x0;
    for (bit_select = 0; bit_select < 8; ++bit_select) {
      while (remainder <= 0) {  // switch AB, get new remainder.
        if (AB_state == 'A') {
          AB_state = 'B';
        } else {
          AB_state = 'A';
        }
        precious_index++;
        remainder = precious_data[precious_index];
      }
      if (endian_ == 'B') {
        wb <<= 1;
        if (AB_state == 'B') {
          wb |= 0x01;
        }
      } else {
        wb >>= 1;
        if (AB_state == 'B') {
          wb |= 0x80;
        }
      }
      remainder--;
    }
    outbuf[outbuf_index++] = wb;
  }
}
