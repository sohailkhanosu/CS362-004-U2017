/**
 * Tests `supplyCount` return total number of a given card that is in supply
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
    int res, expected;
    // test return correct value for a particular card
    
    int theCard = smithy;
    memset(&gs, 0, sizeof(gs));
    for (int i = 0; i < treasure_map + 1; i++) {
        gs.supplyCount[i] = -1;
    }
    expected = 8;
    gs.supplyCount[theCard] = expected;
    
    res = supplyCount(theCard, &gs);

    EXPECT_EQUAL(
            "Get correct number for supply card",
            expected,
            res);

    // all others should be -1"
    for (int i = 0; i < treasure_map + 1; i++) {
        if (i != theCard) {
            res = supplyCount(i, &gs);
            EXPECT_EQUAL(
                    "non set cards should be -1",
                    -1,
                    res);
        }
    }


    SUMMARY;
    return 0;
}
