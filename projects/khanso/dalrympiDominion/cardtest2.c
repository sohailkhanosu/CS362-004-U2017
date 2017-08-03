/*
 * unit tests for smithy card: it gives player 3 more cards in their hand
 * testing strategy focuses on 0 cards in deck, 1 card in deck
 * 3 cards in deck and finally 4 cards in deck
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "test_helper.h"
#include "card_test_helper.h"

extern int fSmithy(struct gameState*, int, int);

int smithyEffect(int player, int handPos, struct gameState *gs)
{
    return fSmithy(gs, player, handPos);
}

int
main ()
{
    const char* msgLess = "deck should have 2 or fewer more cards than before";
    const char* msgMore = "deck should have 3 more cards than before";

    struct gameState gs, modified_gs;

    // test 0 cards
    int expected;
    for (int i = 0; i <= 4; i++) {
        makeTwoPlayerGame(&gs);
        gs.hand[0][0] = smithy;
        gs.handCount[0] = 1;
        gs.deckCount[0] = i;
        memcpy(&modified_gs, &gs, sizeof(gs));

        smithyEffect(0, 0, &modified_gs);

        // for more than three cards in deck, only three should be added
        // to hand minus one due to discard of smithy
        expected = i <= 3 ? gs.handCount[0] + i - 1 : gs.handCount[0] + 3 - 1;
        EXPECT_EQUAL(
                i < 3 ? msgLess : msgMore,
                expected,
                modified_gs.handCount[0]);
    }

    SUMMARY;
    return 0;
}
