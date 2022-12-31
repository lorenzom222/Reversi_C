typedef struct {
    int Color; // White = 1, black = 0
} Player;

int getColor(Player* player) {
    return player->Color;
}

void setColor(Player* player, int color) {
    player->Color = color;
}

Player* createPlayer(int color) {
    Player* player = (Player*) malloc(sizeof(Player));
    player->Color = color;
    return player;
}
