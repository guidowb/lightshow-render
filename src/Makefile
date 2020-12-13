LIB=liblightshow.a

.PHONY: all clean

all: $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(LIB)

SRCS= \
	Lexer.cpp \
	Parser.cpp \
	Compiler.cpp \
	Renderers.cpp \
	Canvasses.cpp \
	LightShow.cpp

OBJS= \
	Lexer.o \
	Parser.o \
	Compiler.o \
	Renderers.o \
	Canvasses.o \
	LightShow.o

$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@

%.o: %.cpp
	g++ -g -O -c $<

Lexer.cpp: Lexer.h
Parser.cpp: Parser.h Lexer.h LightShow.h
Compiler.cpp: Compiler.h Parser.h LightShow.h Renderers.h
Renderers.cpp: Renderers.h LightShow.h
Canvasses.cpp: Canvasses.h LightShow.h
LightShow.cpp: LightShow.h Compiler.h