#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "projekti.h"

int main()
{
    Game game;
    memset(&game, 0, sizeof(game));
    int gameOn = 1;
    while(gameOn) {
        char buffer[80];
        printf("command >");
        fgets(buffer, sizeof(buffer), stdin);
        gameOn = doCommand(&game, buffer);
	}
    freeAll(&game);
    return 0;
}

void freeAll(Game *game)
{
    if (game->players)
        free(game->players);
}

/* Parse command given by user
 * 
 * Parameters:
 * game: game struct
 * buffer: one line read from user
 * 
 * Returns: 1, if game continues; 0 if game ends
 */
int doCommand(Game *game, char *buffer)
{
    switch(toupper((int)buffer[0])) {
        case 'A': // create a new player
            printf("Create a new player.\n");
            createPlayer(buffer, game);
            return 1;
        case 'H': // attack
            printf("Attack player.\n");
            return 1;
        case 'L': // print all players
            printf("Print player information.\n");
            return 1;
        case 'W': // write to file
            printf("Write to file.\n");
            return 1;
        case 'O': // open from file
            printf("Open game from file.\n");
            return 1;
        case 'Q': // quit
            printf("Quit game.\n");
            return 0; // game over
        default:
            printf("Input error.\n");
            return 1; // game continues
    }
}

Player *createPlayer(const char *buffer, Game *game)
{
    const char *splitChar = " ";
    char **argumentArray = NULL;
    int spaceCount = 0;
    char *tmp = malloc(strlen(buffer) * sizeof(char) + 1);
    char *startLocation;
    char *token;

    strcpy(tmp, buffer);
    startLocation = tmp;
    
    do {
        token = strstr(tmp, splitChar);
        if (token) {
            *token = '\0';
            argumentArray = realloc(argumentArray, sizeof(char *) * ++spaceCount);
            if (argumentArray == NULL) { // memory alloc failed
                return NULL;
            }
            argumentArray[spaceCount - 1] = malloc(strlen(tmp) * sizeof(char) + 1);
            strcpy(argumentArray[spaceCount - 1], tmp);
            tmp = token + strlen(splitChar);
        }
    } while (token != NULL);

    argumentArray = realloc(argumentArray, sizeof(char *) * (spaceCount + 1));
    if (argumentArray == NULL) { // memory alloc failed
        return NULL;
    }

    argumentArray[spaceCount] = malloc(strlen(tmp) * sizeof(char) + 1);
    strcpy(argumentArray[spaceCount], tmp);

    //argumentArray = realloc(argumentArray, sizeof(char *) * (spaceCount + 1));
    //argumentArray[spaceCount] = NULL;

    free(startLocation);

    if (spaceCount == 4) {

        game->players = realloc(game->players, (game->playerCount + 1) * sizeof(Player));
        if (game->players == NULL) {
            free(game->players);
        }
        strcpy(game->players[game->playerCount].name, argumentArray[1]);
        game->players[game->playerCount].hp = atof(argumentArray[2]);
        strcpy(game->players[game->playerCount].weapon, argumentArray[3]);
        game->players[game->playerCount].weaponMaxDamage = atof(argumentArray[4]);

        game->playerCount++;

    }

    //return argumentArray;

    for (int i = 0; i < game->playerCount; i++) {
        printf("%s\n", game->players[i].name);
    }

    free(argumentArray);

    return NULL;
}
