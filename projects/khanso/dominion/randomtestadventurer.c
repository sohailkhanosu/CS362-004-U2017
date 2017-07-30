#include <stdlib.h>
#include <time.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "random_tester_helper.h"
#include "test_helper.h"

#define NUM_ITERATIONS 10000

extern int adventurerEffect(int, int, struct gameState *);

void
adventure_card_checker(struct gameState *gs, int numTreasures, int cardPos, int should_reuse_discard)
{
    struct gameState modified_gs;
    int player = gs->whoseTurn;

    memcpy(&modified_gs, gs, sizeof(*gs));

    adventurerEffect(player, cardPos, &modified_gs);

    // should also check to see if discard should be shuffled...
    switch(numTreasures) {
        case 0:
            EXPECT_EQUAL(
                    "expect 0 treasures to not change the handcount",
                    gs->handCount[player],
                    modified_gs.handCount[player]);
            break;
        case 1:
            EXPECT_EQUAL(
                    "expect 1 treasures to increase handcount by 1",
                    gs->handCount[player] + 1,
                    modified_gs.handCount[player]);
            break;
        case 2:
            EXPECT_EQUAL(
                    "expect 2 treasures to increase handcount by 2",
                    gs->handCount[player] + 2,
                    modified_gs.handCount[player]);
            break;
            // exactly 2
        default:
            // more than 2
            EXPECT_EQUAL(
                    "expect more than 2 treasures to increase handcount by 2 only",
                    gs->handCount[player] + 2,
                    modified_gs.handCount[player]);
    }

    // if discard shuffling was required...check to make sure it happend
    if (should_reuse_discard) {
        EXPECT_NOT_EQUAL(
                "deck should not be same as before",
                gs->deckCount[player],
                modified_gs.deckCount[player]);
    }
}

int
get_treasures(struct gameState *gs, int player, int *should_reuse_discard)
{
    int numTreasures = 0;
    enum CARD card;
    // check deck 
    for (int i = 0; i < gs->deckCount[player]; i++) {
        card = gs->deck[player][i];
        switch (card) {
            case copper:
            case silver:
            case gold:
                numTreasures++;
                break;
            default:
                continue;
        }
    }

    if (numTreasures == 0) {
        *should_reuse_discard = 1;
    }

    for (int i = 0; i < gs->discardCount[player]; i++) {
        card = gs->discard[player][i];
        switch (card) {
            case copper:
            case silver:
            case gold:
                numTreasures++;
                break;
            default:
                continue;
        }
    }

    return numTreasures;
}

int
main()
{

    int player;
    int numTreasures;
    int deckSize;
    int discardSize;
    int handSize;
    int cardPos;
    struct gameState gs;
    int should_reuse_discard;

    srand(time(NULL));

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        generate_random_game_initialization(&gs);
        player = pick_player(gs.numPlayers);
        deckSize = rand() % MAX_DECK;
        handSize = rand() % MAX_HAND;

        deckSize = fill_deck(&gs, player, deckSize);
        handSize = fill_hand(&gs, player, handSize);
        discardSize = rand() % (deckSize + handSize);
        fill_discard(&gs, player, discardSize);

        should_reuse_discard = 0;
        numTreasures = get_treasures(&gs, player, &should_reuse_discard);
        cardPos = add_card_to_hand(&gs, player, adventurer);
        gs.whoseTurn = player;
        adventure_card_checker(&gs, numTreasures, cardPos, should_reuse_discard);
    }
    
    SUMMARY;
    return 0;
}
