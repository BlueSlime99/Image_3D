EXEC=raytracer.exe
SOURCES=json.c  menu.c main.c scene3D.c color.c bmp.c raytracer.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall
 
.PHONY: default clean
 
default: $(EXEC)

json.o: json.c json.h
color.o : color.c color.h  json.h
bmp.o : bmp.c bmp.h  color.h
scene3D.o: scene3D.c scene3D.h  json.h color.h
menu.o: menu.c menu.h
main.o: main.c json.h  menu.h
raytracer.o: raytracer.c raytracer.h scene3D.h color.h json.h

 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ -lm

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~

