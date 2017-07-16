/**
 * unittest for `scoreFor`
 * score is calculated by counting the victory points for all the cards in players deck
 */
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "test_helper.h"

int
main ()
{
    struct gameState gs, *gsp;
    int res, expected;
    const int numCurses[] = {-1, -1, 10, 20, 30};
    const int numVictoryCards[] = {-1, -1, 8, 12, 12};
    
    // test no cards in deck should give zero score
    memset(&gs, 0, sizeof(gs)); 
    gs.numPlayers = 2;
    gs.discardCount[0] = 0;
    gs.deckCount[0] = 0;
    gs.handCount[0] = 0;
    expected = 0;

    res = scoreFor(0, &gs); 

    EXPECT_EQUAL(
            "player with 0 cards in deck should have score of 0",
            expected,
            res);

    // cursed player deck has all curses, there are 10 curses in game of 2
    memset(&gs, 0, sizeof(gs));    
    gs.numPlayers = 2;
    gs.deckCount[0] = numCurses[gs.numPlayers];
    expected = -1 * numCurses[gs.numPlayers];
    for (int i = 0; i < numCurses[gs.numPlayers]; i++) {
        gs.deck[0][i] = curse;    
    }

    res = scoreFor(0, &gs);

    EXPECT_EQUAL(
            "all curse deck should be a negative number",
            expected,
            res);

    // get score of assortment of cards in players hand and discard pile
    memset(&gs, 0, sizeof(gs));
    gs.numPlayers = 2;
    gs.deckCount[0] = 0;  // not testing deck this time
    gs.handCount[0] = 5;  // player has 5 hand cards
    gs.hand[0][0] = province; 
    gs.hand[0][1] = province;
    gs.hand[0][2] = duchy;
    gs.hand[0][3] = curse;
    gs.hand[0][4] = curse;
    gs.discardCount[0] = 4;  // and 4 in the discard
    gs.discard[0][0] = province;
    gs.discard[0][1] = estate;
    gs.discard[0][2] = duchy;
    gs.discard[0][3] = curse;

    // get expected score, 3 provinces, 2 duchy, 1 gardens, 2 curse, 1 estate
    expected = 3 * 6 + 2 * 3 + - 3 + 1;

    res = scoreFor(0, &gs);
    
    EXPECT_EQUAL(
            "score is correct with specified cards, 3provinces, 2 duchy, 1 gardens, \n"\
            "2 curse, and 1 estate",
            expected,
            res);

    SUMMARY;
    return 0;
}
