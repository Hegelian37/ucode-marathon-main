CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

OBJDIR = ./obj
HEADERFILES = ./inc

OBJFILES := ./obj/mx_atoi.o ./obj/mx_isspace.o ./obj/mx_printint.o ./obj/mx_strcmp.o ./obj/mx_strlen.o ./obj/mx_isdigit.o ./obj/mx_printchar.o ./obj/mx_printstr.o ./obj/mx_strcpy.o
SRCFILES := ./src/mx_atoi.c ./src/mx_isspace.c ./src/mx_printint.c ./src/mx_strcmp.c ./src/mx_strlen.c ./src/mx_isdigit.c ./src/mx_printchar.c ./src/mx_printstr.c ./src/mx_strcpy.c

MINILIBMX = minilibmx.a

all: $(MINILIBMX)

$(MINILIBMX): $(OBJFILES)
	ar rcs minilibmx.a $(OBJFILES)

$(OBJFILES): | $(OBJDIR)

$(OBJFILES): $(SRCFILES) $(HEADERFILES)
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_atoi.o -c ./src/mx_atoi.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_isspace.o -c ./src/mx_isspace.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_printint.o -c ./src/mx_printint.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_strcmp.o -c ./src/mx_strcmp.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_strlen.o -c ./src/mx_strlen.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_isdigit.o -c ./src/mx_isdigit.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_printchar.o -c ./src/mx_printchar.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_printstr.o -c ./src/mx_printstr.c -I $(HEADERFILES);
	$(CC) $(CFLAGS) -o $(OBJDIR)/mx_strcpy.o -c ./src/mx_strcpy.c -I $(HEADERFILES);

$(OBJDIR):
	mkdir -p $(OBJDIR);

uninstall: clean
	rm -f $(MINILIBMX);

clean:
	rm -f -r $(OBJDIR);

reinstall: uninstall all
