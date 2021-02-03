# Sección de variables
PROYECTO=procesos
# Directorio de destino para instalar el ejecutable
DESTDIR=/usr/local/bin
# Banderas del compilador
CFLAGS=-Wall
# Banderas del enlazador
LFLAGS=-lm
# Compilador de C
CC=gcc

# Sección de dependias y objetivos
all: $(PROYECTO)
# Generamos todos los archivos objeto .o con todos los archivos .c del directorio actual
%.o:%.c
	$(CC) -c $< $(CFLAGS)
# Para todos los archivos.c en el directorio actual cambiamos su extensión a archivos.o
$(PROYECTO): $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $^ -o $@  $(LFLAGS)

# Sección de objetivos falsos
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
	rm -f *.o $(PROYECTO) 
