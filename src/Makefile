LIB=liblightshow.a

.PHONY: all clean

all: $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(LIB)

SRCS= \
	Lexer.cpp \
	Parser.cpp

OBJS= \
	Lexer.o \
	Parser.o

$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@

%.o: %.cpp
	g++ -g -O -c $<

Lexer.cpp: Lexer.h
Parser.cpp: Parser.h

