LIB=liblightshow.a

.PHONY: all clean

all: $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(LIB)

SRCS= \
	Lexer.cpp

OBJS= \
	Lexer.o

$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@

%.o: %.cpp
	g++ -g -O -c $<

src/Lexer.cpp: src/Lexer.h


