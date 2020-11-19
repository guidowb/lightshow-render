.PHONY: all lib test

all: lib test

lib:
	(cd src; make)

test:
	(cd test; make)
