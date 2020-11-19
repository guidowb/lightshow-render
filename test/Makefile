.PHONY: test

SRCS= \
	TestRunner.cpp \
	ParserTests.cpp

test: $(SRCS) ../src/lightshow.a
	g++ -std=c++11 -lcppunit -o tests $^
	./tests
