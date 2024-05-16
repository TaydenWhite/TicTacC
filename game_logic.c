#include "tac.h"

/* main function */
int main() {
    int choice = 1;
    while (choice == 1) {
        system("clear");
        play_game();
        choice = take_choice(PLAY_AGAIN, 1, 2);
    }
}

/* This function plays one instance of the game */
void play_game() {
    /* initialize board */
    int start_board[BOARD_SIZE] = { 0 };
    board_t *curr = create_board(start_board);
    comp_t *c = NULL;

    /* get computer settings */
    int choice = take_choice(COMPUTER, 1, 2);
    if (choice == 1) {
        c = (comp_t *) malloc(sizeof(comp_t));
        
        choice = take_choice(X_OR_O, 1, 2);
        if (choice == 1) {c->turn = OTURN; c->win = OWIN;}
        if (choice == 2) {c->turn = XTURN; c->win = XWIN;}

        choice = take_choice(DIFFICULTY, 1, 3);
        if (choice == 1) {c->diff = 1;}
        if (choice == 2) {c->diff = 3;}
        if (choice == 3) {c->diff = 9;}
    }

    /* turn loop */
    
    while (!GAME_OVER(curr->gamestate)) {
        play_turn(curr, c);
    }
    system("clear");
    print_board(curr->board);
    /* finish game */
    if (curr->gamestate == XWIN) printf("Congratulations X!\n");
    else if (curr->gamestate == OWIN) printf("Congratulations O!\n");
    else printf("It's a tie!\n");
}

/* This functions plays one turn in the game */
void play_turn(board_t *curr, comp_t *c) {
    system("clear");
    print_board(curr->board);
    if ((c != NULL) && (c->turn == curr->gamestate)) {  
        c_turn(curr, c);
    }
    else {
        int pos = 0;
        do {
            printf("%s\n", state_to_string(curr->gamestate));
            pos = take_choice(POSITION, 1, 9);
        } while ((curr->board)[pos - 1] != 0);

        (curr->board)[pos - 1] = curr->gamestate;
        curr->gamestate = calc_state(curr->board, curr->gamestate);
    }
}

/* This functions prints the current board position */
void print_board(int board[BOARD_SIZE]) {
    for (int b = 0; b < BOARD_SIZE; b++) {
        printf("%s", num_to_piece(board[b]));

        if (b % 3 < 2) printf("|");
        else if (b < 6) printf("\n------\n");
        else printf("\n===================\n");
    }
}

/* This function ensures a valid input for a given message */
int take_choice(const char *message, int min, int max) {
    int read = 0;
    int choice = 0;
    
    do {
        printf("%s", message);
        read = scanf("%d", &choice);    
        printf("===================\n");
        if (read == 0) {while (getchar() != '\n');}
    } while (VALID_INPUT(read, min, max, choice));
    
    return choice;
}

/* This function converts a board position into a printable value */
const char *num_to_piece(int val) {
    switch (val) {
        case 1: return "x";
        case 4: return "o";
        default: return " ";
    }
}

/* This function converts a gamestate into a printable value */
const char *state_to_string(gamestate_t state) {
    switch (state) {
        case XTURN: return "XTURN";
        case OTURN: return "OTURN";
        case XWIN: return "XWIN";
        case OWIN: return "OWIN";
        case TIE: return "TIE";
    }
}

/* 
 * This function checks and returns the state of the board. It takes the board
 * and determines if its an XWIN, OWIN, or a TIE. If it is none of these it
 * returns the opposite of the boards parent state.
 */
gamestate_t calc_state(int board[BOARD_SIZE], gamestate_t parent_state) {
    int board_sum = 0;
    int sum1 = 0;
    int sum2 = 0;

    for (int i = 0; i < 3; i++) {
        sum1 = board[3 * i] + board[3 * i + 1] + board[3 * i + 2];
        sum2 = board[i] + board[i + 3] + board[i + 6]; 
        board_sum += sum1;
        
        if ((sum1 == 3) || (sum2 == 3)) return XWIN;
        if ((sum1 == 12) || (sum2 == 12)) return OWIN;
    }
    sum1 = board[0] + board[4] + board[8];
    sum2 = board[2] + board[4] + board[6];

    if ((sum1 == 3) || (sum2 == 3)) return XWIN;
    if ((sum1 == 12) || (sum2 == 12)) return OWIN;

    //tie logic
    if (board_sum == 21) return TIE;
    if (parent_state == XTURN) return OTURN;
    else return XTURN;
}
