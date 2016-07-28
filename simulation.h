#pragma once

#include <cstdlib>
#include <cstdio>
#include "state_machine.h"

using namespace std;


/*
 * payoff for four situations, high bit means own choices, true means C
 * so it is, CC CD DC DD
 */
const int PAYOFF[4] = {3, 0, 5, 1};
const char *SITUATION[4] = {"CC", "CD", "DC", "DD"};

/*
 * This simulate times round with noisy probability between player0 and player1
 * For info_level, 0 means nothing, 1 prints score, 2 prints details
 *
 */
void simulate_ipd(StateMachine *player0, StateMachine *player1,
                  int &score0, int &score1,
                  int times, double noisy, int info_level = 0) {
    score0 = 0; score1 = 0;
    player0->restart();
    player1->restart();
    bool move0, move1;
    int s0, s1;

    // simulate
    for (int i = 0; i < times; ++i) {
        move0 = player0->next_move();
        move1 = player1->next_move();
        s0 = (move0 << 1) + move1;
        s1 = (move1 << 1) + move0;
        if (info_level == 2) {
            printf("R%d:\t%s\n", i, SITUATION[s0]);
        }

        score0 += PAYOFF[s0];
        score1 += PAYOFF[s1];

        player0->move_state(((double) rand()) / RAND_MAX < noisy ? s0 ^ 1 : s0);
        player1->move_state(((double) rand()) / RAND_MAX < noisy ? s1 ^ 1 : s1);
    }

    if (info_level) {
        printf("player0: %d,\tplayer1: %d\n", score0, score1);
    }
}
