#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int Color; // White = 1, black = 0
} Player;

// Returns the color of the player (1 for white, 0 for black).
int getColor(Player* player);

// Sets the color of the player (1 for white, 0 for black).
void setColor(Player* player, int color);

// Creates and returns a new Player object with the given color (1 for white, 0 for black).
Player* createPlayer(int color);

#endif
