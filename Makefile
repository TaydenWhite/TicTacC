CC = gcc
CFLAGS = -g -Wall -Werror

SRC = game_logic.c com_logic.c
OBJ = $(SRC:.c=.o)
EXECTUABLE = tac

all: $(EXECTUABLE)

$(EXECTUABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECTUABLE)
