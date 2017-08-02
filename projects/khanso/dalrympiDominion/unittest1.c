/**
 * unit test for the dominion.h function `whoseTurn`
 * 
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
    struct gameState gs;
    int actualTurn;
    int res;
    
    // clear the game state
    memset(&gs, 0, sizeof(gs));
    actualTurn = 1;
    gs.numPlayers = 2;
    gs.whoseTurn = actualTurn; 
    
    res = whoseTurn(&gs);

    EXPECT_EQUAL("whoseturn returns correct player", actualTurn, res);
    SUMMARY;
    return 0;
}
