.PHONY: test

SRCS= \
	TestRunner.cpp \
	ParserTests.cpp

test: $(SRCS) ../src/liblightshow.a
	g++ -std=c++11 -lcppunit -o tests $^
	./tests
