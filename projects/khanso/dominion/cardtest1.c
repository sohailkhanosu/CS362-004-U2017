/**
 * Unit tests for adventurer card
 * recall that adventurer card has the effect of adding 2 treasure cards to your hand
 * so the hand should contain greater than or the same  amount of cards after invocation
 *
 * tests no treasures, 2 treasures, and 1 treasure.
 */
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "test_helper.h"
#include "card_test_helper.h"

extern int adventurerEffect (int, int, struct gameState*);

int
main ()
{
    struct gameState gs, modified_gs;

    // no treasure
    makeTwoPlayerGame(&gs);
    
    // get rid of initial coppers
    for (int i = 0; i < 7; i++) {
        gs.deck[0][i] = estate;
        gs.supplyCount[estate]--;
    }
    // add adventurer to hand
    gs.handCount[0] = 1;
    gs.hand[0][0] = adventurer;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // player 1 plays adventurer 
    adventurerEffect(0, 0, &modified_gs);

    EXPECT_EQUAL(
            "hand should not contain more cards than before adventurer use",
            gs.handCount[0],
            modified_gs.handCount[0]);


    // two treasures
    makeTwoPlayerGame(&gs);
    // keep two coppers
    for (int i = 0; i < 5; i++) {
        gs.deck[0][i] = estate;
        gs.supplyCount[estate]--;
    }

    // add adventurer to hand
    gs.handCount[0] = 1;
    gs.hand[0][0] = adventurer;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // player 1 plays adventurer
    adventurerEffect(0, 0, &modified_gs);

    EXPECT_EQUAL(
            "hand should contain exactly two more cards than before after "
            "adventurer use",
            gs.handCount[0] + 2,
            modified_gs.handCount[0]);


    // one treasures
    makeTwoPlayerGame(&gs);
    //keep one coppers
    for (int i = 0; i < 6; i++) {
        gs.deck[0][i] = estate;
        gs.supplyCount[estate]--;
    }
    gs.hand[0][0] = adventurer;
    gs.handCount[0] = 1;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // player 1 plays adventurer
    adventurerEffect(0, 0, &modified_gs);

    EXPECT_EQUAL(
            "hand should have only one more card than before",
            gs.handCount[0] + 1,
            modified_gs.handCount[0]);

    SUMMARY;
    return 0;
}
