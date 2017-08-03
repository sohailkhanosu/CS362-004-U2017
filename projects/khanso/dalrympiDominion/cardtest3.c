#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "test_helper.h"
#include "card_test_helper.h"


int stewardEffect (int player, int handPos, struct gameState* gs, int choice1, int choice2, int choice3)
{
    return cardEffect(steward, choice1, choice2, choice3, gs, handPos, NULL);
}

int
main ()
{
    struct gameState gs, modified_gs;
    
    // test 1: choose gain 2 cards by choosing choice 1
    makeTwoPlayerGame(&gs);
    gs.hand[0][0] = steward;
    gs.handCount[0] = 1;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // choose choice 1 and get two cards, hand count should be
    // two more than before
    stewardEffect(0, 0, &modified_gs, 1, 0, 0);

    EXPECT_EQUAL(
            "expect hand count to be two more than before after steward w/ 1",
            gs.handCount[0] + 2 - 1,
            modified_gs.handCount[0]); 

    makeTwoPlayerGame(&gs);
    gs.hand[0][0] = steward;
    gs.handCount[0] = 1;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // choose choice 2 and get two more coins
    stewardEffect(0, 0, &modified_gs, 2, 0, 0);

    EXPECT_EQUAL(
            "expect coins to be two more than before after steward w/ 2",
            gs.coins + 2,
            modified_gs.coins);

    // choice 3, discard 2 cards
    makeTwoPlayerGame(&gs);
    gs.hand[0][0] = steward;
    gs.hand[0][1] = curse;
    gs.hand[0][2] = curse;
    gs.hand[0][3] = province;
    gs.supplyCount[curse] -= 2;
    gs.supplyCount[province] -= 1;
    gs.supplyCount[steward] -= 1;
    gs.handCount[0] = 4;
    memcpy(&modified_gs, &gs, sizeof(gs));

    // choose 2 cards to discard, get rid of curses?
    stewardEffect(0, 0, &modified_gs, 3, 1, 2);

    EXPECT_EQUAL(
            "expect curses to be gone, hand count to be 3 less than before",
            gs.handCount[0] - 3,
            modified_gs.handCount[0]);

    EXPECT_EQUAL(
            "second card should not be a curse card",
            -1,
            modified_gs.hand[0][1]);

    EXPECT_EQUAL(
            "expect discard pile to be only one more than before",
            gs.discardCount[0] + 1,
            modified_gs.discardCount[0]);

    SUMMARY;
    return 0;
}
