#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "state_machine.h"

using namespace std;

/*
 * This initialize the state machine from specific file
 * Return false if any error occur, true otherwise
 */
bool StateMachine::init(char* path, char *file_name) {
    // path
    char pathall[100];
    int l1 = strlen(path), l2 = strlen(file_name);
    strncpy(pathall, path, l1);
    strncpy(pathall + l1, file_name, l2);
    pathall[l1 + l2] = '\0';
    // file open
    FILE *fp = fopen(pathall, "r");
    if (!fp) {
        fprintf(fp, "Failed open file ");
        fputs(file_name, fp);
        fprintf(fp, "\n");
        return false; 
    }

    // info input
    int size = 0;
    char ch;
    char rest[15];
    while (fscanf(fp, "%c", &ch) != EOF) {
        // if state id valid
        if (ch != '0' + size) {
           fprintf(fp, "wrong state id in line %d from file ", size)  ;
           fputs(file_name, fp);
           fprintf(fp, "\n");
           return false;
        }

        // get rest info
        if (!fscanf(fp, "%c%lf%d%d%d%d", &ch, &(states[size].p),
                                              &(states[size].ptr[0]),
                                              &(states[size].ptr[1]),
                                              &(states[size].ptr[2]),
                                              &(states[size].ptr[3]))) {
            fprintf(fp, "wrong format in line %d from file ", size);
            fputs(file_name, fp);
            fprintf(fp, "\n");
            return false;
        }
        fgets(rest, 10, fp);

        // count size
        ++size;
        if (size >= 10) {
            fprintf(fp, "too many states in file ");
            fputs(file_name, fp);
            fprintf(fp, "\n");
            return false;
        }
    }

    // info check
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < 4; ++j) {
            // id check
            if (states[i].ptr[j] < 0 || states[i].ptr[j] >= size) {
                fprintf(fp, "wrong succeed state id from line %d, in file ", i);
                fputs(file_name, fp);
                fprintf(fp, "\n");
                return false;
            }
        }
    }

    fclose(fp);
    return true;
}

/*
 * This restart by reset state to 0
 */
void StateMachine::restart() {
    ptr = 0;
}

/*
 * This return a move relate to the P(C)
 */
bool StateMachine::next_move() {
    double p = ((double) rand()) / RAND_MAX;
    return p >= states[ptr].p;
}

/*
 * This move ptr to succeed state
 * The input s is the previous situation, which is from [0, 4]
 * Its higher bit is his choise, second bit is another player's choice
 */
 void StateMachine::move_state(int s) {
    ptr = states[ptr].ptr[s];
 }
