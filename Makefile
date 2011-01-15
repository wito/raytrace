CC=gcc
CFLAGS=-Wall -Iinc -std=c99
LFLAGS=-lpng

TDIR=
TODIR=
ODIR=obj

_TEST = 
TEST = $(patsubst %,$(TODIR)/%.test,$(_TEST))

_OBJ = raytrace.o vector.o matrix.o primitive.o sphere.o plane.o material.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

raytrace: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o raytrace out.png
