CC=gcc
CFLAGS=-Wall -Iinc
LFLAGS=-lpng

TDIR=
TODIR=
ODIR=obj

_TEST = 
TEST = $(patsubst %,$(TODIR)/%.test,$(_TEST))

_OBJ = raytrace.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

raytrace: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o xmas
