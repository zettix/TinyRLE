default_target: all

all: TinyRLE_test

TinyRLE.o: TinyRLE.h TinyRLE.cpp
	g++ -g -c -Wall TinyRLE.cpp

TinyRLE_test.o: TinyRLE.o TinyRLE_test.cpp
	g++ -g -c -Wall TinyRLE_test.cpp

TinyRLE_test: TinyRLE.o TinyRLE_test.o
	g++ -g TinyRLE.o TinyRLE_test.o -o TinyRLE_test

a_test.o: a_test.cpp
	g++ -g -c -Wall a_test.cpp

clean:
	rm -f *.o TinyRLE_test
