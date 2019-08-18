#ifndef PROJEKTI_H
#define PROJEKTI_H

typedef struct {
    char name[20]; // name of player
    int hp; // hitpoints of player
    int exp; // experiences of player
    char weapon[30]; // weapon of Player
    int weaponMaxDamage; // maximum damage of weapon
} Player;

typedef struct {
    Player *players; // dynamic array of all players
    int playerCount; // keeps track of player count
} Game;

/* Public functions in main.c */
int doCommand(Game *game, char *buffer);
char** splitCommand(const char *buffer);
void freeArgumentArray(char** argumentArray);
void freeAll(Game *game);
int countArguments(char **argumentArray);
int compareAlpha(const void* a, const void* b);
int compareExp(const void* a, const void* b);
void attackPlayer(char **argumentArray, Game *game);
void createPlayer(char **argumentArray, Game *game);
void printPlayers(Game *game);
void writeToFile(Game *game, char** argumentArray);
void loadFromFile(Game *game, char** argumentArray);

#endif