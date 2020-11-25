.PHONY: all test clean

all: test

clean:
	rm -rf tests tests.dSYM

SRCS=TestRunner.cpp

test: $(SRCS) ../src/liblightshow.a
	g++ -std=c++11 -lcppunit -o tests $^
	./tests
