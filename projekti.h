#ifndef PROJEKTI_H
#define PROJEKTI_H

typedef struct {
    char name[20]; // name of player
    float hp; // hitpoints of player
    float exp; // experiences of player
    char weapon[30]; // weapon of Player
    float weaponMaxDamage; // maximum damage of weapon
} Player;

typedef struct {
    Player *players; // dynamic array of all players
    int playerCount;
} Game;

/* Public functions in main.c */
int doCommand(Game *game, char *buffer);
void freeAll(Game *game);
void *createPlayer(const char *buffer, Game *game);

#endif