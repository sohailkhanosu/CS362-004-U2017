/*
 * contains helpers for random testing
 */

#ifndef _RANDOM_TESTER_HELPER_H
#define _RANDOM_TESTER_HELPER_H

#include <stdlib.h>
#include <string.h>
#include "dominion.h"

#define min(a, b) ((a) < (b)) ? (a) : (b);

// num curses and num victory cards per game per number of players
const int numCurses[] = {-1, -1, 10, 20, 30};
const int numVictoryCards[] = {-1, -1, 8, 12, 12};
const int kingdomCardsChoices[10] = {
    adventurer, council_room, feast,   mine,  steward,
    remodel,    smithy,       village, baron, minion};


/** sets up a standard deck */
void
setup_initial_decks(struct gameState *gs)
{
    for (int i = 0; i < gs->numPlayers; i++) {
        for (int j = 0; j < 7; j++) {
            gs->deck[i][j] = copper;
        }
        gs->supplyCount[copper] -= 7;
        gs->deckCount[i] = 7;
        for (int j = gs->deckCount[i]; j < 7 + 3; j++) {
            gs->deck[i][j] = estate;
            gs->deckCount[i]++;
        }
    }
}

void
generate_random_game_initialization(struct gameState *gs)
{
    memset(gs, 0, sizeof(*gs));

    // pick random players between 2 and 4
    gs->numPlayers = (rand() % (MAX_PLAYERS - 1)) + 2;

    // set supplies
    for (int i = 0; i < treasure_map + 1; i++) {
        gs->supplyCount[i] = -1;
    }

    for (int i = estate; i <= province; i++) {
        gs->supplyCount[i] = numVictoryCards[gs->numPlayers];
    }
    gs->supplyCount[curse] = numCurses[gs->numPlayers];
    gs->supplyCount[copper] = 60;
    gs->supplyCount[silver] = 40;
    gs->supplyCount[gold] = 20;

    // initialize kingdom cards
    for (int i = 0; i < sizeof(kingdomCardsChoices) / sizeof(kingdomCardsChoices[0]); i++) {
        gs->supplyCount[ kingdomCardsChoices[i] ] = 10;
    }

}

int
pick_player(int numPlayers)
{
    return rand() % numPlayers;
}

/**
 * randomly chooses a location in hand to place a card,
 * existing card will be deleted and added back to supply
 */
int
add_card_to_hand(struct gameState *gs, int player, enum CARD card)
{
    int numCardsInHand = gs->handCount[player];
    int position;


    if (numCardsInHand > 0) {
        // add existing back to supply
        position = rand() % numCardsInHand;
        gs->supplyCount[gs->hand[player][position]]++;

    } else {
        position = 0;    
    }

    // remove chosen from supply
    gs->supplyCount[card]--;
    gs->hand[player][position] = card;
    gs->handCount[player]++;

    return position;
}


static int all_supplies_exhausted(int *supplyCards, int total)
{
    int all_exhausted = 1;
    for (int i = 0; all_exhausted && i <= total; i++) {
        if (supplyCards[i] > 0) {
            all_exhausted = 0;
        }
    }

    return all_exhausted;
}

int
fill_deck(struct gameState *gs, int player, int deckSize)
{
    int currentSize = gs->deckCount[player];
    int numCards = treasure_map + 1;
    int card;

    int i = currentSize;
    while (i < deckSize) {
        if (all_supplies_exhausted(gs->supplyCount, treasure_map)) {
            break;
        }

        card = rand() % numCards; 

        if (gs->supplyCount[card] <= 0)
            continue;
        else {
            gs->deck[player][i++] = card;
            gs->deckCount[player]++;
            gs->supplyCount[card]--;     
        }
    }

    return gs->deckCount[player];
}

int
fill_hand(struct gameState *gs, int player, int handSize)
{
    int currentSize = gs->handCount[player];
    int numCards = treasure_map + 1;
    int card;

    int i = currentSize;
    while (i < handSize) {
        if (all_supplies_exhausted(gs->supplyCount, treasure_map)) {
            break;
        }

        card = rand() % numCards; 

        if (gs->supplyCount[card] <= 0)
            continue;
        else {
            gs->hand[player][i++] = card;
            gs->handCount[player]++;
            gs->supplyCount[card]--;     
        }
    }

    return gs->handCount[player];
}

/**
 * fills discard with cards from deck and hand
 * modifies gamestate to reflect new cards for given player
 */
int
fill_discard(struct gameState *gs, int player, int discardSize)
{
    int i = gs->discardCount[player];
    int handOrDeck = 0;
    int position;
    while (i < discardSize) {
        handOrDeck = rand() % 2;

        switch(handOrDeck) {
            case 0:
                if (gs->deckCount[player] <= 0)
                    continue;
                // from deck
                // move last card to fill in the hole left by moving card to discard
                position = rand() % (gs->deckCount[player]);
                gs->discard[player][gs->discardCount[player]++] = gs->deck[player][position];
                gs->deck[player][position] = gs->deck[player][gs->deckCount[player] - 1];
                gs->deck[player][gs->deckCount[player] - 1] = -1;
                gs->deckCount[player]--;
                break;
            default:
                if (gs->handCount[player] <= 0)
                    continue;
                position = rand() % (gs->handCount[player]);
                gs->discard[player][gs->discardCount[player]++] = gs->hand[player][position];
                gs->hand[player][position] = gs->hand[player][gs->handCount[player] - 1];
                gs->hand[player][gs->handCount[player] - 1] = -1;
                gs->handCount[player]--;
        }
        i++;
    }

    return gs->discardCount[player];
}

#endif
