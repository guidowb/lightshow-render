.PHONY: all lib test clean

all: lib test

clean:
	(cd src; make clean)
	(cd test; make clean)

lib:
	(cd src; make all)

test:
	(cd test; make all)
