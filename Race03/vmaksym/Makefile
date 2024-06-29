CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -Iinc

BINARY = way_home

HEADERFILES = $(wildcard inc/*.h)
SOURCEFILES = $(wildcard src/*.c)

all: $(BINARY)

$(BINARY): $(HEADERFILES) $(SOURCEFILES)
	$(CC) $(CFLAGS) -o $@ $(SOURCEFILES)

uninstall:
	rm -rf $(BINARY)
clean:
	rm -rf $(BINARY)

reinstall: uninstall all
