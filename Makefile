CC = gcc
SDIR = src
IDIR = include
LDIR = lib
ODIR = build/object
CFLAGS = -Wall -g -I$(IDIR) -L$(LDIR)
LINKERS = -lm -lraylib

all: object main

object:
	@mkdir -p build/object

_DEPS = raylib.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o build/$@ $^ $(CFLAGS)  $(LINKERS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
