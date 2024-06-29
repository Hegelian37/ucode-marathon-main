CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

BINARY = mx_cp

HEADERFILES = ./inc
SRCFILES = ./src

all: $(BINARY)

$(BINARY): $(SRCFILES) | $(HEADERFILES)
	$(CC) $(CFLAGS) $^/*.c -o $@ -I $(HEADERFILES)

uninstall:
	rm -rf $(BINARY)

clean:
	rm -rf $(BINARY)

reinstall: uninstall all
