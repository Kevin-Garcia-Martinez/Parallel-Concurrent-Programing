# Sección de variables
PROYECTO=servidor
# Directorio de destino para instalar el ejecutable
DESTDIR=/usr/local/bin
# Banderas del compilador
CFLAGS=-Wall
# Banderas del enlazador
LFLAGS=-lm 
# Compilador de C
CC=gcc

all: $(PROYECTO)

# genera todos los objetos con todos los archivos fuente que encuentres
%.o:%.c
	$(CC) -c $< $(CFLAGS)

# Esta regla utiliza tres dependencias para cumplir el objetivo
$(PROYECTO): $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $^ -o $@  $(LFLAGS)

# Objetivos falsos
.PHONY: clean 

install: all
	# Si no existe el directorio lo crea
	if [ ! -d $(DESTDIR) ] ; then \
			sudo mkdir $(DESTDIR) ; \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

uninstall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean:
    # Se ejecuta dentro de la carpeta en donde nos encontramos
	rm -f *.o  $(PROYECTO) 
