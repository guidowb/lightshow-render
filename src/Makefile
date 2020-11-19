LIB=liblightshow.a

.PHONY: all

all: $(LIB)

SRCS= \
	Parser.cpp

OBJS= \
	Parser.o

$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@

%.o: %.cpp
	g++ -g -O -c $<

src/Parser.cpp: src/Parser.h


