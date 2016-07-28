#pragma once

#include <cstdlib>
#include <cstdio>

using namespace std;

/*
 * This structure save a state in the state machine
 */
typedef struct stState {
public:
    // probability for C
    double p;
    // index of pointers
    int ptr[4];
} State;

/*
 * This class save and simulate the statemachines
 */
class StateMachine {
public:
    StateMachine(): name(NULL),
                    size(-1),
                    ptr(-1) { 
    }

    bool init(char *path, char *file_name);
    void restart();
    bool next_move();
    void move_state(int s);

private:
    // name of the file
    char *name;
    // size of the state machine
    int size;
    // pointer to current state
    int ptr;
    // states
    State states[10];
};
