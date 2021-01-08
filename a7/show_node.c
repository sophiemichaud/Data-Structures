#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt.h"

int main(int argc, char *argv[]){

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);
    compute_score();

    for(int i = 1; i < argc; i++){

        int hashLocation = atol(argv[i]);
        print_node(htable[hashLocation]);
    }

    return 0;
}