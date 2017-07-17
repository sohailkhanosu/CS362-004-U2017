/*
 * helper functions for testing dominion code
 */

#ifndef _CARD_TEST_HELPER_H
#define _CARD_TEST_HELPER_H

#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"

const int kingdomCardsChoices[10] = {adventurer, council_room, feast,   mine,  steward,
                              remodel,    smithy,       village, baron, minion};

// num curses and num victory cards per game
const int numCurses[] = {-1, -1, 10, 20, 30};
const int numVictoryCards[] = {-1, -1, 8, 12, 12};

void makeTwoPlayerGame(struct gameState* gs)
{
    memset(gs, 0, sizeof(*gs));
    gs->numPlayers = 2;

    // initialize all to -1
    for (int i = 0; i < treasure_map + 1; i++) {
        gs->supplyCount[i] = -1;
    }

    // initialize victories
    for (int i = estate; i <= province; i++) {
        gs->supplyCount[i] = numVictoryCards[gs->numPlayers];
    }
    gs->supplyCount[curse] = numCurses[gs->numPlayers];

    // initialize the treasures
    gs->supplyCount[copper] = 60;
    gs->supplyCount[silver] = 40;
    gs->supplyCount[gold] = 20;

    // initialize the kingdoms
    for (int i = 0; i < sizeof(kingdomCardsChoices) / sizeof(kingdomCardsChoices[0]); i++) {
        gs->supplyCount[ kingdomCardsChoices[i] ] = 10;
    }

    // initialize the decks
    for (int i = 0; i < gs->numPlayers; i++) {
        // 7 coppers and 3 estates (not from supply)
        for (int j = 0; j < 7; j++) {
            gs->deck[i][j] = copper;
        }
        gs->supplyCount[copper] -= 7;
        gs->deckCount[i] = 7;
        for (int j = gs->deckCount[i]; j < 10; j++) {
            gs->deck[i][j] = estate;
            gs->deckCount[i]++;
        }

        // hand count is 0 and  discard count is 0
        gs->handCount[i] = 0;
        gs->discardCount[i] = 0;
    }

    // not sure what this is...
    gs->playedCardCount = 0;
}

#endif
