#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "state_machine.h"
#include "simulation.h"

using namespace std;

// comman line info
char *file_name;
int r, t;
double n;
bool html;

// players
typedef struct MyPlayer {
    char name[50];
    char date[50];
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
    printf("\t./judge [-h] -f <F> -r <R> -t <T> [-n <N>] [-p <P>] [-H]");
    printf("\t\t-h: print out this message.\n");
    printf("\t\t-f <F>: string F is the input file.\n");
    printf("\t\t-r <R>: R(>0) is how many rounds for each game.\n");
    printf("\t\t-t <T>: T(>0) is how many games between each pair of player.\n");
    printf("\t\t-n <N>: N(>=0) is the probability of the noisy info default by 0.\n"); 
    printf("\t\t-p <P>: string of folder path to files.\n");
    printf("\t\t-H: print out rank info with html format.\n");
    exit(0);
}

/*
 * Pass the command line
 */
void pass(int argc, char** argv) {
    file_name = NULL;
    r = 0; t = 0; n = 0;
    html = false;
    char ch;

    while ((ch = getopt(argc, argv, "hHf:r:t:n:p:")) != -1) {
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
        } else if (ch == 'H') {
            html = true;
        } else {
            show_usage_exit();
        }
    }

    if (file_name == NULL || r == 0 || t == 0) {
        show_usage_exit();
    }
}

/*
 * Remove \n\r in the end
 */
void trunc(char *str) {
    int len = strlen(str);
    while (len && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[--len] = '\0';
    }
}

int main(int argc, char** argv) {
    srand((unsigned int) time(NULL));

    pass(argc, argv);
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("invalid input file.\n");
        return 0;
    }

    // init player
    char s[50];
    char date[50];
    player_t new_player;
    int num_valid = 0;  // valid player

    while (fgets(s, 100, fp)) {
        fgets(date, 100, fp);
        trunc(s);
        trunc(date);
        // new player
        StateMachine *tmp = new StateMachine();
        strncpy(new_player.name, s, strlen(s));
        new_player.name[strlen(s)] = '\0';
        strncpy(new_player.date, date, strlen(date));
        new_player.date[strlen(date)] = '\0';
        // if valid
        if (tmp->init(path, s)) {
            new_player.score = 0;
            new_player.state_machine = tmp;
            ++num_valid;
        } else {
            new_player.a_score = -1;
            new_player.state_machine = NULL;
            delete(tmp);
        }
        player.push_back(new_player);
    }

   // simulate
    int s0, s1, l = player.size();
    for (int i = 0; i < l; ++i) {
        if (!player[i].state_machine) {
            continue;
        }
        for (int j = i+1; j < l; ++j) {
            if (!player[j].state_machine) {
                continue; 
            }
            for (int k = 0; k < t; ++k) {
                simulate_ipd(player[i].state_machine, player[j].state_machine,
                             s0, s1, r, n);
                player[i].score += s0;
                player[j].score += s1;
            }
        }
        player[i].a_score = (player[i].score + 0.0) / t / (num_valid - 1);
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
        if (html) {
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
    		printf("    <td align=\"middle\">%s</td>\n  </tr>\n", player[i].date);
        } else {
            printf("%10d%30s%15.5lf\n", i + 1, player[i].name, player[i].a_score);
        }
        delete(player[i].state_machine);
    }

    return 0;
}
