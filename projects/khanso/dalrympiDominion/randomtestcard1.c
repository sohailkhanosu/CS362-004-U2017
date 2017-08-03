/**
 * random tests for council room
*/

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "dominion_helpers.h"
#include "dominion.h"
#include "test_helper.h"
#include "random_tester_helper.h"

#define NUM_ITERATIONS 10000

int councilRoomEffect(int player, int handPos, struct gameState *gs)
{
    return cardEffect(council_room,  0, 0, 0, gs, handPos, NULL);
}

void
council_room_checker(struct gameState *gs, int cardPos)
{

    struct gameState modified_gs;
    int player = gs->whoseTurn;

    memcpy(&modified_gs, gs, sizeof(*gs));

    councilRoomEffect(player, cardPos, &modified_gs);

    // should draw 4 or fewer cards, because deck + discard count may be less than 4
    int cardsToDraw = min(gs->discardCount[player] + gs->deckCount[player], 4);
    int cardsDrawn = modified_gs.handCount[player] - gs->handCount[player];

    EXPECT_EQUAL(
            "number of drawn cards should increase appropriately",
            cardsToDraw,
            cardsDrawn); 
    
    EXPECT_EQUAL(
            "num buys should one be greater than before",
            gs->numBuys + 1,
            modified_gs.numBuys);

    // check the other players
    for (int i = 0; i < gs->numPlayers; i++) {
        if (i == player) {
            continue;
        }

        cardsToDraw = min(gs->discardCount[i] + gs->deckCount[i], 1);
        cardsDrawn = modified_gs.handCount[i] - gs->handCount[i];
        EXPECT_EQUAL(
                "other players should have one more card than before",
                cardsToDraw,
                modified_gs.handCount[i]);
    }
}

int
main()
{

    int player;
    int deckSize;
    int discardSize;
    int handSize;
    int cardPos;
    struct gameState gs;

    srand(time(NULL));


    for (int i = 0; i < NUM_ITERATIONS; i++) {
        generate_random_game_initialization(&gs);
        player = pick_player(gs.numPlayers);

        for (int i = 0; i < gs.numPlayers; i++) {
            deckSize = rand() % (MAX_DECK / gs.numPlayers);
            handSize = rand() % (MAX_HAND / gs.numPlayers);
            deckSize = fill_deck(&gs, i, deckSize);
            handSize = fill_hand(&gs, i, handSize);
            if (deckSize + handSize > 0) {
                discardSize = rand() % (deckSize + handSize);
                fill_discard(&gs, i, discardSize);
            }
        }

        cardPos = add_card_to_hand(&gs, player, council_room);
        gs.whoseTurn = player;
        gs.numBuys = rand() % 10;
        council_room_checker(&gs, cardPos);
    }

    SUMMARY;
    return 0;
}

