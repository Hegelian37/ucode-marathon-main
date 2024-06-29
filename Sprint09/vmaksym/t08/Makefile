CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

BINARY = calc

HEADERFILES = ./inc
SRCFILES = ./src

all: $(BINARY)

$(BINARY): $(SRCFILES) | $(HEADERFILES)
	$(CC) $(CFLAGS) $^/*.c -o $@ -I $(HEADERFILES)

uninstall: clean
	rm -f $(BINARY)

clean:

reinstall: uninstall all
