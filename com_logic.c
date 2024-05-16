#include "tac.h"

/* 
 * This function runs a computer computer turn. It takes the current board
 * and the computer info. It builds a tree based on the computers difficulty
 * level, then recursively grades the strength of each turn, updating curr
 * to be the strongest of its children boards.
 */
void c_turn(board_t *curr, comp_t *c) {
    srand(time(NULL));
    printf("%s - Computer\nWhere would you like to go (1-9): ", state_to_string(curr->gamestate));
    /* generate tree */
    build_tree(curr, calc_children(curr->board) - c->diff);
    /* get strength of possible turns */
    float max = -99;
    int maxi = 0;
    for (int i = 0; i < curr->num_children; i++) {
            float bs = board_strength((curr->nexts)[i], c);
            if (bs > max) {max = bs; maxi = i;}
            if ((bs == max) && (rand() % 2 + 1 == 1)) {max = bs; maxi = i;}
    }

    if ((curr->num_children == 9) && (c->diff == 9)) {maxi = 4;}
    
    int index = 0;
    for (int j = 0; j < BOARD_SIZE; j++) {
        if ((curr->board)[j] == 0) {
            if (index == maxi) {printf("%d\n", j + 1); (curr->board)[j] = curr->gamestate;}
            index ++;
        }
    }
    
    for (int k = 0; k < curr->num_children; k++) {
        delete_tree(curr->nexts[k]);
    }
    free(curr->nexts);
    curr->gamestate = calc_state(curr->board, curr->gamestate);
    sleep(1);
}

/* 
 * This function creates, allocates, and returns a board node with a given
 * board layout. It copies the board but sets all other fields to a default
 * value. 
 */
board_t *create_board(int board[BOARD_SIZE]) {
    board_t *new_board = (board_t *) malloc(sizeof(board_t));
    memcpy(new_board->board, board, BOARD_SIZE * sizeof(int));

    new_board->num_children = 0;
    new_board->gamestate = XTURN;
    new_board->nexts = NULL;

    return new_board;
}

/*
 * This function builds a board tree of a certain height based on a threshold
 * child_thresh is made by subtracting the difficulty level of the computer
 * from the current board's number of children.
 */
void build_tree(board_t *root, int child_thresh) {
    int zeros = calc_children(root->board);
    if (zeros == child_thresh) return;
     
    if (!GAME_OVER(root->gamestate)) {
        root->num_children = zeros;
        root->nexts = (board_t **) malloc(root->num_children * sizeof(board_t *));
    }

    for (int n = 0; n < root->num_children; n++) {
        int child_board[BOARD_SIZE]= { 0 };
        calc_board(root, child_board, n);
        (root->nexts)[n] = create_board(child_board);
        ((root->nexts)[n])->gamestate = calc_state(child_board, root->gamestate);
        build_tree((root->nexts)[n], child_thresh);
    }
}

/* This function deletes a board tree. */
void delete_tree(board_t *root) {
    if (root->nexts != NULL) {
        for (int i = 0; i < root->num_children; i++) {
            delete_tree((root->nexts)[i]);
        }
        free(root->nexts);
    }
    free(root);
}

/*
 * This function returns the strength of a given board state. A board is a 1
 * if it is a win, or it leads to a win. It is a -1 if it is a loss or leads 
 * to a loss. Otherwise, it returns the average of its children boards scores.
 */
float board_strength(board_t *root, comp_t *c) {
    /* base cases */
    if (GAME_OVER(root->gamestate)) {
        if (root->gamestate == c->win) return 1;
        else if (root->gamestate == TIE) return 0;
        else return -1;
    }
    if (root->num_children == 0) return 0;

    float avg = 0;
    for (int i = 0; i < root->num_children; i++) {
           float bs = board_strength((root->nexts)[i], c);
        if ((root->gamestate == c->turn) && (bs == 1)) return 1; 
        if ((root->gamestate != c->turn) && (bs == -1)) return -1; 
        avg += bs;
    }
    return avg  / root->num_children;
}

/* 
 * This function takes the current board node, a board to fill, and the
 * index of the zero that is being changed. It essentially makes fill_board
 * the indexth child of the root.
 */
void calc_board(board_t *root, int *fill_board, int index) {
    int zero = 0;
    for (int  i = 0; i < BOARD_SIZE; i++) {
        if ((root->board)[i] == 0) {
            if (zero == index) {fill_board[i] = root->gamestate;}
            zero ++;
        }
        else {fill_board[i] = (root->board)[i];}
    }
}

/* This function returns the number of children for a given board. */
int calc_children(int board[BOARD_SIZE]) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i ++) {
        if (board[i] == 0) {count ++;}
    }
    return count;
}