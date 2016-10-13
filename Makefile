COMPIL = gcc
FLAG = -Wall
NAME = raycaster

all: $(NAME)

json_parser.o : json_parser.h json_parser.c
	$(COMPIL) -c $(FLAG) json_parser.c

$(NAME).o: $(NAME).h $(NAME).c
	$(COMPIL) -c $(FLAG) $(NAME).c -lm

$(NAME): $(NAME).o json_parser.o
	$(COMPIL) $(FLAG) $(NAME).o json_parser.o -o $(NAME) -lm

clean:
	rm *.o $(NAME)
