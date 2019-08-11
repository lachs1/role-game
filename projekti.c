#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "projekti.h"

int main()
{
    Game game;
    memset(&game, 0, sizeof(game));
    srand((unsigned)time(NULL)); // seed
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
    char** argumentArray = NULL;
    switch(toupper((int)buffer[0])) {
        case 'A': // create a new player
            argumentArray = splitCommand(buffer);
            createPlayer(argumentArray, game);
            return 1;
        case 'H': // attack
            argumentArray = splitCommand(buffer);
            attackPlayer(argumentArray, game);
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

char** splitCommand(const char *buffer)
{
    char *split = " ";
    char **argumentArray = malloc(sizeof(char*));
    argumentArray[0] = NULL; // empty array has only NULL
    char *next = strstr(buffer, split); // skip command type
    buffer = next + strlen(split);
    int c = 0;

    while(1) {
        argumentArray = realloc(argumentArray, (c + 2) * sizeof(char*));
        argumentArray[c + 1] = NULL;

        char *next = strstr(buffer, split); // find next space

        if (!next) // end of string
            break;

        int length = next - buffer; // length of the part
        argumentArray[c] = calloc(length + 1, sizeof(char));
        strncpy(argumentArray[c], buffer, length);

        buffer = next + strlen(split);
        c++;
    }
    // last part of string
    int length = strlen(buffer);
    argumentArray[c] = calloc(length + 1, sizeof(char));
    strncpy(argumentArray[c], buffer, length);

    return argumentArray;
}

void freeArgumentArray(char **argumentArray)
{
    for (int i = 0; argumentArray[i]; i++) {
        free(argumentArray[i]);
    }
    free(argumentArray);
}

int countArguments(char **argumentArray)
{
    int argumentCount = 0;
    for (int i = 0; argumentArray[i]; i++) {
        argumentCount++;
    }
    return argumentCount;
}

int compareAlpha(const void* a, const void* b)
{
    const Player *playerA = a;
    const Player *playerB = b;
    return strcmp(playerA->name, playerB->name);
}

void attackPlayer(char **argumentArray, Game *game)
{
    int argumentCount = countArguments(argumentArray);

    if (argumentCount == 2) { // attacking another player needs 2 arguments
        Player *tmpAttacker = malloc(sizeof(Player));
        strcpy(tmpAttacker->name, argumentArray[0]); // search key for bsearch

        Player *attacker = bsearch(tmpAttacker, game->players, game->playerCount, sizeof(Player), compareAlpha);
        free(tmpAttacker);

        Player *tmpTarget = malloc(sizeof(Player));
        strcpy(tmpTarget->name, argumentArray[1]);

        Player *target = bsearch(tmpTarget, game->players, game->playerCount, sizeof(Player), compareAlpha);
        free(tmpTarget);

        printf("%s, %s", attacker->name, argumentArray[1]);

        if (attacker && target) {  // both found
            int randomHit = rand() % attacker->weaponMaxDamage + 0;

            if (target->hp >= randomHit) {
                target->hp -= randomHit;
            } else {
                target->hp = 0;
            }

            // Todo: bonus for killing
            int exp = randomHit;
            attacker->exp += exp;

            printf("%s attacked %s with %s by %d damage.\
                    %s has %d hitpoints remaining.\
                    %s gained %d experience points.\n",
                    attacker->name, target->name,
                    attacker->weapon, randomHit,
                    target->name, target->hp,
                    attacker->name, exp
                );
        } else {
            printf("One or more players was not found.\n");
        }
    }
    freeArgumentArray(argumentArray);
}

void createPlayer(char **argumentArray, Game *game)
{
    int argumentCount = countArguments(argumentArray);

    if (argumentCount == 4) { // creating a player needs 4 arguments
        game->players = realloc(game->players, (game->playerCount + 1) * sizeof(Player));
        if (game->players == NULL) {
            freeAll(game);
        }
        strcpy(game->players[game->playerCount].name, argumentArray[0]);
        game->players[game->playerCount].hp = atof(argumentArray[1]);
        strcpy(game->players[game->playerCount].weapon, argumentArray[2]);
        game->players[game->playerCount].weaponMaxDamage = atof(argumentArray[3]);

        game->playerCount++;
    } else {
        printf("Creating a player needs exactly 4 arguments for example: A Bilbo 25 Dagger 8.\n");
    }

    for (int i = 0; i < game->playerCount; i++) {
        printf("%s %d %s %d\n", game->players[i].name, game->players[i].hp, game->players[i].weapon, game->players[i].weaponMaxDamage);
    }

    freeArgumentArray(argumentArray);

}
