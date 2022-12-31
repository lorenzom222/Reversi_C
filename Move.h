#ifndef MOVE_H
#define MOVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int row;
    int col;
    char colChar;
    int numCaps;
} Move;

int getNumCaps(Move* move);
void setNumCaps(Move* move, int numCaps);

// This is the constructor for the Move class. It takes in a row and a column character
// as arguments and sets the row and column of the Move object. The column character
// is converted to an integer.
Move* createMove(int row, char colChar);

// This is another constructor for the Move class. It takes in a row and a column
// as arguments and sets the row and column of the Move object.
Move* createMoveInt(int row, int col);

void deleteMove(Move* move);

// This function returns a string representation of the Move object.
char* toString(Move* move);

void setColChar(Move* move, char colChar);

// This function returns the row of the Move object.
int getRow(Move* move);

// This function returns the column character of the Move object.
char getColChar(Move* move);

// This function sets the column character of the Move object.
void setColChar(Move* move, char colChar);

// This function sets the row of the Move object.
void setRow(Move* move, int row);

// This function returns the column of the Move object.
int getCol(Move* move);

// This function sets the column of the Move object.
void setCol(Move* move, int col);

// The compareTo function compares two Move objects based on the value of their numCaps member variable.
// Define a compare function for Move objects
int compareMove(const void* a, const void* b);

#endif
