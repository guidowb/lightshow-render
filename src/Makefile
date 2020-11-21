LIB=liblightshow.a

.PHONY: all clean

all: $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(LIB)

SRCS= \
	Lexer.cpp \
	Parser.cpp \
	Compiler.cpp

OBJS= \
	Lexer.o \
	Parser.o \
	Compiler.o

$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@

%.o: %.cpp
	g++ -g -O -c $<

Lexer.cpp: Lexer.h
Parser.cpp: Parser.h Lexer.h
Compiler.cpp: Compiler.h Parser.h Render.h
