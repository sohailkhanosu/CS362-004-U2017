/**
 * tests the `isGameOver` function in dominion.h
 * `isGameOver` returns 1 if game is over and 0 if not
 * the game is over when province cards are empty or if
 * any three supply piles are empty
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
    int expected_result;
    int res;
    struct gameState gs;

    // test game should not be over

    expected_result = 0;
    memset(&gs, 0, sizeof(gs));
    // set all supply cards to 8 since this is 
    // the minimum number of victory cards for a game
    for (int i = curse; i < treasure_map + 1; i++) {
       gs.supplyCount[i] = 8; 
    }
    
    res = isGameOver(&gs);

    EXPECT_EQUAL(
            "game should not end with non empty supply piles and non empty province cards",
            expected_result, 
            res);

    // test game should be over if province cards are empty
    expected_result = 1;
    memset(&gs, 0, sizeof(gs));
    for (int i = curse; i < treasure_map + 1; i++) {
        gs.supplyCount[i] = 8;
    }
    gs.supplyCount[province] = 0;

    res = isGameOver(&gs);
    EXPECT_EQUAL(
            "game should be over due to empty province cards",
            expected_result,
            res);

    // test game should be over due to three empty supply piles
    // neither empty should be a province
    expected_result = 1;
    memset(&gs, 0, sizeof(gs));
    for (int i = curse; i < treasure_map + 1; i++) {
        gs.supplyCount[i] = 8;
    }
    gs.supplyCount[curse] = 0;
    gs.supplyCount[treasure_map] = 0;
    gs.supplyCount[ treasure_map / 2 ] = 0; // just pick the middle card whatever it is

    res = isGameOver(&gs);

    EXPECT_EQUAL(
            "game should be over due to three empty supply piles",
            expected_result,
            res);
    
    // test game should be over due to three empty supply piles and one of them is province
    expected_result = 1;
    memset(&gs, 0, sizeof(gs));
    for (int i = curse; i < treasure_map + 1; i++) {
        gs.supplyCount[i] = 8;
    }
    gs.supplyCount[province] = 0;
    gs.supplyCount[curse] = 0;
    gs.supplyCount[adventurer] = 0;
    
    res = isGameOver(&gs);

    EXPECT_EQUAL(
            "game should be over due to three empty supply piles with one of them being province",
            expected_result,
            res);

    SUMMARY;
    return 0;
}
