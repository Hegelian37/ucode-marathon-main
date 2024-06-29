.PHONY: all clean install uninstall

TARGET = snake_game

all: $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

game.o: src/game.c
	$(CC) $(CFLAGS) -c src/game.c -o game.o

snake.o: src/snake.c
	$(CC) $(CFLAGS) -c src/snake.c -o snake.o

$(TARGET): main.o game.o snake.o
	$(CC) $(CFLAGS) $^ -lSDL2 -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

install:
	install $(TARGET) $(PREFIX)

uninstall:
	rm $(PREFIX)/$(TARGET)
