# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c -o image_editor

pack:
	zip -FSr Nume.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
