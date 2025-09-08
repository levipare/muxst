.POSIX:

VERSION = 0.1

PREFIX = /usr/local

CPPFLAGS += -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE
CFLAGS += -Wall

SRC = muxst.c
OBJ = $(SRC:.c=.o)

BIN = muxst

all: $(BIN)

$(BIN): $(OBJ)

clean:
	rm -f $(BIN) $(OBJ)

install: all
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	
.PHONY: all clean install uninstall
