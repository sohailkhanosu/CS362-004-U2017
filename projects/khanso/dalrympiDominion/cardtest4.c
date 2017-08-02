/**
 * unittest for council room effect
 * council room gives player 4 cards and 1 more buy and 
 * makes other players draw 1 card from deck
 *
 */
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "test_helper.h"
#include "card_test_helper.h"


extern int councilRoomEffect(int, int, struct gameState*);

int
main ()
{
    struct gameState gs, modified_gs;

    // test use council room
    makeTwoPlayerGame(&gs);
    
    // player 1 has council room
    gs.hand[0][0] = council_room;
    gs.handCount[0] = 1;
    memcpy(&modified_gs, &gs, sizeof(gs));
    
    councilRoomEffect(0, 0, &modified_gs);

    EXPECT_EQUAL(
            "player 1 should have 4 more cards in deck than before",
            gs.handCount[0] + 4 - 1,
            modified_gs.handCount[0]);

    EXPECT_EQUAL(
            "player 1 should have 1 more buy",
            gs.numBuys + 1,
            modified_gs.numBuys);

    EXPECT_EQUAL(
            "player 2 should have 1 more card in hand than before",
            gs.handCount[1] + 1,
            modified_gs.handCount[1]);

    SUMMARY;
    return 0;
}
