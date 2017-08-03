/*
 * random tests for steward effect
 */

#include <stdlib.h>
#include <time.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "random_tester_helper.h"
#include "test_helper.h"

#define NUM_ITERATIONS 10000

int stewardEffect (int player, int handPos, struct gameState* gs, int choice1, int choice2, int choice3)
{
    return cardEffect(steward, choice1, choice2, choice3, gs, handPos, NULL);
}


void
steward_card_checker(struct gameState *gs, int player, int cardPos, int c1, int c2, int c3)
{
    int res;
    int cardsDrawn;
    int cardsToDraw;
    int cardsToTrash;

    struct gameState modified_gs;

    memcpy(&modified_gs, gs, sizeof(*gs));
    res = stewardEffect(player, cardPos, &modified_gs, c1, c2, c3);

    switch(c1) {
        
        case 1:
            // two cards
            cardsToDraw = min(gs->deckCount[player] + gs->discardCount[player], 2);
            cardsDrawn = modified_gs.handCount[player] - gs->handCount[player];
            EXPECT_EQUAL(
                    "should draw appropriate number of cards",
                    cardsToDraw,
                    cardsDrawn);
            break;
        case 2:
            // two coins
            EXPECT_EQUAL(
                    "should have 2 more coins than before",
                    gs->coins + 2,
                    modified_gs.coins);
            break;

        case 3:
            // trash two
            // can't trash steward
            // both trashed cards cannot be the same card
            // depending on number of trashes, handCount after should be
            // less than three now, with trashing steward as well
            cardsToTrash = min(gs->handCount[player] - 1, 2);
            if (cardsToTrash == 0) {
                EXPECT_EQUAL(
                        "no cards to trash should result in an error",
                        -1,
                        res);
            } else {
                if (cardsToTrash == 2) {
                    if (c2 == cardPos || c3 == cardPos) {
                        EXPECT_EQUAL(
                            "cannot trash steward while its being played",
                            -1,
                            res);
                    } else {
                        EXPECT_EQUAL(
                            "player should have three less cards than before",
                            gs->handCount[player] - 3,
                            modified_gs.handCount[player]);
                    }
                } else {
                    if (c2 == cardPos) {
                        EXPECT_EQUAL(
                            "cannot trash steward while its being played",
                            -1,
                            res);
                    } else if (c3 != -1) {
                        EXPECT_EQUAL(
                            "cannot select a third card to trash if only have 1 other in deck",
                            -1,
                            res);
                    } else {
                        EXPECT_EQUAL(
                            "player should have two less cards than before",
                            gs->handCount[player] - 2,
                            modified_gs.handCount[player]);
                    }
                }
            }
            break;
        default:
            // should return a -1
            EXPECT_EQUAL(
                    "invalid choice should return a -1",
                    -1,
                    res);
    }
}


int
main()
{
    struct gameState gs;
    int player;
    int deckSize;
    int discardSize;
    int handSize;
    int cardPos;
    int choice1;
    int choice2;
    int choice3;

    srand(time(NULL));

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        generate_random_game_initialization(&gs);
        player = pick_player(gs.numPlayers);
        deckSize = rand() % MAX_DECK;
        handSize = rand() % MAX_HAND;

        deckSize = fill_deck(&gs, player, deckSize);
        handSize = fill_hand(&gs, player, handSize);
        discardSize = rand() % (deckSize + handSize);
        discardSize = fill_discard(&gs, player, discardSize);

        cardPos = add_card_to_hand(&gs, player, steward);
        choice1 = rand() %  5; // can only have choices 1,2, and 3, other choices should err
        choice2 = rand() %  gs.handCount[player]; // cannot have card be same as each other
        choice3 = rand() %  gs.handCount[player]; // or be the steward card

        gs.whoseTurn = player;
        steward_card_checker(&gs, player, cardPos, choice1, choice2, choice3);
    }

    SUMMARY;
    return 0;
}
