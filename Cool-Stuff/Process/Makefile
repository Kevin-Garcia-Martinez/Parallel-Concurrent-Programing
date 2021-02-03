# project name
PROJECT=procesos
# compiler flags
CFLAGS=-Wall
# linker flags
LFLAGS=-lm
# compiler
C=gcc


all: $(PROJECT)

%.o:%.c
	$(C) -c $< $(CFLAGS)

$(PROJECT): proc1.o
	$(C) $< -o $@ $(LFLAGS)

clean:
	rm -f *.o $(PROJECT)


