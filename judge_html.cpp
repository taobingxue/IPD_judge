#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <getopt.h>
#include "state_machine.h"
#include "simulation.h"

using namespace std;

// comman line info
char *file_name;
int r, t;
double n;
// players
typedef struct MyPlayer {
    char name[50];
    StateMachine *state_machine;
    int score;
    double a_score;
} player_t;
vector<player_t> player;
// 
char path[50] = "./files/";

/*
 *  show usage
 */
void show_usage_exit() {
    printf("Usage: \n");
    printf("\t./judge [-h] -f <F> -r <R> -t <T> [-n <N>] [-p <P>]");
    printf("\t\t-h: print out this message.\n");
    printf("\t\t-f <F>: string F is the input file.\n");
    printf("\t\t-r <R>: R(>0) is how many rounds for each game.\n");
    printf("\t\t-t <T>: T(>0) is how many games between each pair of player.\n");
    printf("\t\t-n <N>: N(>=0) is the probability of the noisy info default by 0.\n"); 
    printf("\t\t-p <P>: string of folder path to files.\n"); 
    exit(0);
}

/*
 * Pass the command line
 */
void pass(int argc, char** argv) {
    file_name = NULL;
    r = 0; t = 0; n = 0;
    char ch;

    while ((ch = getopt(argc, argv, "hf:r:t:n:p:")) != -1) {
        if (ch == 'f') {
            file_name = optarg;
        } else if (ch == 'r') {
            r = atoi(optarg);
        } else if (ch == 't') {
            t = atoi(optarg);
        } else if (ch == 'n') {
            n = atof(optarg);
        } else if (ch == 'p') {
            strcpy(path, optarg);
//            path = optarg;
        }else {
            show_usage_exit();
        }
    }

    if (file_name == NULL || r == 0 || t == 0) {
        show_usage_exit();
    }
}

int main(int argc, char** argv) {
    pass(argc, argv);
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("invalid input file.\n");
        return 0;
    }

    // init player
    char s[50];
    player_t new_player;
    while (fgets(s, 100, fp)) {
        int len = strlen(s);
        while (s[len-1] == '\r' || s[len-1] == '\n') {
            s[--len] = '\0';
        }

        StateMachine *tmp = new StateMachine();
        if (tmp->init(path, s)) {
            new_player.state_machine = tmp;
            strncpy(new_player.name, s, len);
            new_player.name[len] = '\0';
            new_player.score = 0;
            player.push_back(new_player);
        } else {
            delete(tmp);
        }
    }

   // simulate
    int s0, s1, l = player.size();
    for (int i = 0; i < l; ++i) {
        for (int j = i+1; j < l; ++j) {
            for (int k = 0; k < t; ++k) {
                simulate_ipd(player[i].state_machine, player[j].state_machine,
                             s0, s1, r, n);
                player[i].score += s0;
                player[j].score += s1;
            }
        }
        player[i].a_score = (player[i].score + 0.0) / t / (l - 1);
    }

    // sort
    for (int i = 0; i < l; ++i) {
        int p = i;
        for (int j = i + 1; j < l; ++j) {
            if (player[j].a_score > player[p].a_score) {
                p = j;
            }
        }
        
        if (p != i) {
            new_player = player[i]; player[i] = player[p]; player[p] = new_player;
        }
    }

    // output & release
    for (int i = 0; i < l; ++i) {
/*		
  <tr>
    <td align="middle">2</td>
    <td align="middle">EXAMPLE-DTRIGGER.txt</td>
    <td align="middle">184.94167</td>
    <td align="middle">00:00 July 27</td>
  </tr>
*/
		
        printf("  <tr>\n    <td align=\"middle\">%d</td>\n", i+1);
		printf("    <td align=\"middle\">%s</td>\n", player[i].name);
		printf("    <td align=\"middle\">%lf</td>\n", player[i].a_score);
		printf("    <td align=\"middle\">00:00 July 27</td>\n  </tr>\n");

        delete(player[i].state_machine);
    }

    return 0;
}
