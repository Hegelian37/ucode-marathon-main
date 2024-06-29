CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -g
LDFLAGS = -lm

BINARY = endgame

HEADERFILES = ./inc
SRCFILES = ./src

SDL_FLAGS = -F resources/framework -I resources/framework/SDL2.framework/include \
        -F resources/framework -I resources/framework/SDL2_image.framework/include \
        -F resources/framework -I resources/framework/SDL2_mixer.framework/include \
        -F resources/framework -I resources/framework/SDL2_ttf.framework/include \
        -L resources/framework -I resources/framework/SDL2.framework/lib \
        -L resources/framework -I resources/framework/SDL2_image.framework \
        -L resources/framework -I resources/framework/SDL2_mixer.framework \
        -L resources/framework -I resources/framework/SDL2_ttf.framework \

all: $(BINARY)

$(BINARY): $(SRCFILES) | $(HEADERFILES)
	$(CC) $(CFLAGS) -c $^/*.c -I $(HEADERFILES) -fPIC
	$(CC) -shared -o $@ *.o $(SDL_FLAGS) $(FRAMEWORKS) $(LDFLAGS)

uninstall:
	rm -rf $(BINARY)

clean:
	rm -rf $(BINARY)

reinstall: uninstall all
