#ifndef TAC_H
#define TAC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GAME_OVER(state) (((state == TIE) || (state == XWIN) || (state == OWIN)) ? 1 : 0)
#define VALID_INPUT(read, min, max, choice) (read == 0) || (choice < min) || (choice > max) ? 1 : 0 

#define PLAY_AGAIN "Would you like to play again?\n1. Yes\n2. Exit\nChoice: "
#define COMPUTER "Would you like to play vs a computer?\n1. Yes\n2. No\nChoice: "
#define X_OR_O "Would you like to play as X or O?\n1. X\n2. O\nChoice: "
#define DIFFICULTY "Choose a difficulty\n1. Low\n2. Medium\n3. High\nChoice: "
#define POSITION "Where would you like to go (1-9): "
#define BOARD_SIZE (9)

typedef enum gamestate {
    XTURN = 1,
    OTURN = 4,
    XWIN,
    OWIN,
    TIE
} gamestate_t;

typedef struct computer {
    gamestate_t turn;
    gamestate_t win;
    int diff;
} comp_t;

typedef struct board {
    int board[BOARD_SIZE];
    gamestate_t gamestate;
    int num_children;
    struct board **nexts;
} board_t;

/* function prototypes */
 
/* main */
int main();

/* game logic */
void play_game();
void play_turn(board_t *, comp_t *);
void print_board(int *);

/* computer logic */
void c_turn(board_t *, comp_t *);
gamestate_t calc_state(int *, gamestate_t);
int calc_children(int *);
void calc_board(board_t *, int *, int);

/* tree functions */
board_t *create_board(int *);
void build_tree(board_t *, int);
void delete_tree(board_t *);
float board_strength(board_t *, comp_t *);

/* helper functions */
int take_choice(const char *, int, int);
const char *num_to_piece(int);
const char *state_to_string(gamestate_t);

#endif // TAC_H