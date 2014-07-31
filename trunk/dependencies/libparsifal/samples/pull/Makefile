APP = pull1
## install root location:
PREFIX=/usr/local
LIBS = -L/usr/lib -lparsifal
OBJ = pull1.o xmlreader.o
CC = gcc
CPPFLAGS = -I/usr/include
CFLAGS = -Wall -g -Wno-pointer-sign

BIN_INSTALL_DIR=$(PREFIX)/bin

$(APP): $(OBJ)
	$(CC) $(OBJ) -o $(APP) $(LIBS)

install: $(APP)
	install -D -m555 $(APP) $(BIN_INSTALL_DIR)/$(APP)

uninstall:
	rm -f $(BIN_INSTALL_DIR)/$(APP)

clean:
	rm -f $(OBJ)

.PHONY: all all-before all-after clean clean-custom

all: all-before $(APP) all-after

xmlreader.o: xmlreader.c
	$(CC) -c xmlreader.c -o xmlreader.o $(CFLAGS)

pull1.o: pull1.c
	$(CC) -c pull1.c -o pull1.o $(CFLAGS)

