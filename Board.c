#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "hashset.h"
#include "hashset_itr.h"

#include "Move.h"
#include "Player.h"

const int LOSS = INT_MIN;
const int WIN = INT_MAX;
const int DRAW = 0;

typedef struct Board {
    int **board;
    int size;
    int whitePieceCount;
    int blackPieceCount;
    hashset_t *reachedMoves;
} Board;

Board *createBoard(int size) {
    Board *board = malloc(sizeof(Board));
    board->board = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        board->board[i] = malloc(size * sizeof(int));
    }
    board->size = size;
    board->whitePieceCount = 2;
    board->blackPieceCount = 2;
    board->reachedMoves = hashset_create();

    board->board[size / 2 - 1][size / 2 - 1] = 1;
    board->board[size / 2][size / 2 - 1] = 2;
    board->board[size / 2 - 1][size / 2] = 2;
    board->board[size / 2][size / 2] = 1;

    return board;
}

void getScore(Board b) {
    hashset_t* score =  hashset_create();
}


hashset_t *reachedSpaceMoves(Board *b, Player *p, Move *m) {
    hashset_t *tiles = hashset_create();
    for (int i = 0; i < b->size; i++) {
        for (int j = 0; j < b->size; j++) {
            // Check if the current space is a valid move
            // If it is, add it to the set of reached moves
            if (isValidMove(b, p, i, j)) {
                Move *newMove = createMoveInt(i, j);
                hashset_add(tiles, newMove);
            }
        }
    }
    return tiles;
}


hashset_t* getLegalMovesBlack(Board* b, Player* p) {
    hashset_t* legalMoves = hashset_create(); // Create an empty HashSet

    // Iterate through every position on the board
    for (int i = 0; i < b->size; i++) {
        for (int j = 0; j < b->size; j++) {
            // Create a Move object for the current position
            Move* tempMove = createMoveInt(i, j);
            // Get the set of adjacent enemy pieces for Black
            hashset_t* emenySet = adjacentToEnemyBlack(b, tempMove);

            // If the set is empty, continue to the next iteration
            if(hashset_iterator_has_next(hashset_iterator(emenySet)) == 0){
                continue;
            }
            // If the position on the board is not empty, continue to the next iteration
            if (!(b->board[tempMove->row][tempMove->col] == 0)) {
                continue;
            }

            // Get the set of legal directions for Black
            hashset_t* legalDirections = getLegalDirectionsBlack(b, tempMove, p);
            // If the set of legal directions is not empty and the set of adjacent enemies is not empty, add the Move object to the legalMoves set
        if (hashset_iterator_has_next(hashset_iterator(getLegalDirectionsBlack(b, tempMove, p))) == 1 && hashset_iterator_has_next(hashset_iterator(emenySet)) == 1) {
            hashset_add(legalMoves, tempMove);
        }
        }
    }

    return legalMoves;
}


hashset_t* getLegalMovesWhite(Board b, Player* p) {
hashset_t* legalMoves =hashset_create(); // create empty set
for (int i = 0; i < b.size; i++) { // Double for loop across the board
    for (int j = 0; j < b.size; j++) {
        Move* tempMove = createMoveInt(i, j); // goes to coordinate (i, j)
        hashset_t* enemySet = adjacentToEnemyWhite(b, tempMove); // checks adjacent enemies to White

        if (hashset_iterator_has_next(hashset_iterator(enemySet)) == 0) { // if it is empty, skip
            continue;
        }

        if (b.board[tempMove->row][tempMove->col] != 0) {
            continue;
        }

        // legal direction not empty and enemy set not empty
        if (hashset_iterator_has_next(hashset_iterator(getLegalDirectionsWhite(b, tempMove, p))) == 1 && hashset_iterator_has_next(hashset_iterator(enemySet)) == 1) {
            hashset_add(legalMoves, tempMove);
        }
    }
}

return legalMoves;
}