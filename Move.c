// This is the Move class in C. It represents a move in a game of Reversi.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int row;
    int col;
    char colChar;
    int numCaps;
} Move;

int getNumCaps(Move* move) {
return move->numCaps;
}

void setNumCaps( Move* move, int numCaps) {
move->numCaps = numCaps;
}

// This is the constructor for the Move class. It takes in a row and a column character
// as arguments and sets the row and column of the Move object. The column character
// is converted to an integer.
Move* createMove(int row, char colChar) {
    Move* move = malloc(sizeof(Move));
    move->row = row;

    switch (colChar) {
        case 'a':
            move->col = 0;
            break;
        case 'b':
            move->col = 1;
            break;
        case 'c':
            move->col = 2;
            break;
        case 'd':
            move->col = 3;
            break;
        case 'e':
            move->col = 4;
            break;
        case 'f':
            move->col = 5;
            break;
        case 'g':
            move->col = 6;
            break;
        case 'h':
            move->col = 7;
            break;
    }

    return move;
}

void deleteMove(Move* move) {
    free(move);
}



// This is another constructor for the Move class. It takes in a row and a column
// as arguments and sets the row and column of the Move object.
Move* createMove(int row, int col) {
    Move* move = malloc(sizeof(Move));
    move->row = row;
    move->col = col;
    return move;
}

// This function returns a string representation of the Move object.
char* toString( Move* move) {
char* s = malloc(sizeof(char) * 30);
sprintf(s, "( Row:%d, Col:%d )", move->row, move->col);
return s;
}





void setColChar( Move* move, char colChar) {
move->colChar = colChar;
}

// This is the default constructor for the Move class. It creates a new Move object
// with default values for the row and column.
// Move* createMoveEmpty() {
//     return (Move*)malloc(sizeof(Move));
// }

// This function returns the row of the Move object.
int getRow( Move* move) {
return move->row;
}

// This function returns the column character of the Move object.
char getColChar( Move* move) {
return move->colChar;
}
// This function sets the column character of the Move object.
void setColChar( Move* move, char colChar) {
    move->colChar = colChar;
}
// This function sets the row of the Move object.
void setRow( Move* move, int row) {
    move->row = row;
}
// This function returns the column of the Move object.
int getCol( Move* move) {
    return move->col;
}
// This function sets the column of the Move object.
void setCol( Move* move, int col) {
    move->col = col;
}

// The compareTo function compares two Move objects based on the value of their numCaps member variable.
// Define a compare function for Move objects
int compareMove(const void* a, const void* b) {
    // Cast the void pointers to pointers to Move objects
    const Move* moveA = (const Move*)a;
    const Move* moveB = (const Move*)b;

    // Compare the numCaps fields of the two Move objects
    if (moveA->numCaps > moveB->numCaps) {
        return 1;
    }
    else if (moveA->numCaps < moveB->numCaps) {
        return -1;
    }
    else {
        return 0;
    }
}