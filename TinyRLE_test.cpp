#include <iostream>

#include "TinyRLE.h"

unsigned char testin[9] = {
  0xff, // 1
  0xff, // 2
  0xff, // 3
  0xff, // 4
  0x00, // 5
  0x0f, // 6
  0xff, // 7
  0xff, // 8
  0xff  // 9
};

unsigned char encbuf[200];
unsigned char b2[100];

bool testchars(unsigned char *a, unsigned char *b, int alen) {
  for (int i = 0; i < alen; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

int main(int argc, char **argv) {
  TinyRLE *jack;
  jack = new TinyRLE(testin, encbuf, 9, 'B');
  
  std::cout << "Len: " << jack->GetDataSize() << " \n";
  jack->GetData(0, b2, 9);
  for (int i = 0; i < 9; i++) {
    std::cout << "Byte " << i << " : " << (int) b2[i] << " : " << (int) testin[i] << " \n";
  }
  if (testchars(b2, testin, 9) == true) {
    std::cout << "Success!\n";
  } else {
    std::cout << "Failures!\n";
  }


  // section test:
  jack->GetData(3, b2, 3);
  for (int i = 0; i < 3; i++) {
    std::cout << "Byte " << i << " : " << (int) b2[i] << " : " << (int) testin[i] << " \n";
  }
  if (testchars(b2, testin + 3, 3) == true) {
    std::cout << "Success!\n";
  } else {
    std::cout << "Failures!\n";
  }
}
