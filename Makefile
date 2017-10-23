#USO: Reemplazar solamente los valores de las primeras 3 lineas:
#	En EXEC escribir el nombre del TDA/TP esperado
#	En CFILES escribir los archivos.c con los que se trabaja
#	En HFILES escribir los archivos.h con los que se trabaja

#Tiene 2 funcionalidades:
#	make run: Corre el programa
#	make valgrind: Chequea perdida de memoria

EXEC = hash
CFILES = main.c lista.c hash.c pruebas_catedra.c testing.c
HFILES = hash.h lista.h testing.h

CC = gcc
CFLAGS = -g -std=c99 -Wall
CFLAGS += -Wconversion -Wno-sign-conversion -Werror -Wbad-function-cast -Wshadow
CFLAGS += -Wpointer-arith -Wunreachable-code -Wformat=2
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

all: $(EXEC)

run: all
	./$(EXEC)

valgrind: all
	valgrind $(VFLAGS) ./$(EXEC)
	
$(EXEC): $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $(EXEC)

