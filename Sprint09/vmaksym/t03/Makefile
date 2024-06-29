CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

OBJDIR = ./obj

OBJFILES := ./obj/mx_atoi.o ./obj/mx_isspace.o ./obj/mx_printint.o ./obj/mx_strcmp.o ./obj/mx_strlen.o ./obj/mx_isdigit.o ./obj/mx_printchar.o ./obj/mx_printstr.o ./obj/mx_strcpy.o

MINILIBMX = minilibmx.a

all: $(MINILIBMX)

$(MINILIBMX): $(OBJFILES)
	ar rcs $@ $^

$(OBJFILES): | $(OBJDIR)

$(OBJDIR)/%.o: ./src/%.c ./inc
	$(CC) $(CFLAGS) -o $@ -c $< -I ./inc

$(OBJDIR):
	mkdir -p $@

uninstall: clean
	rm -f $(MINILIBMX)

clean:
	rm -r -f $(OBJDIR)

reinstall: uninstall all
