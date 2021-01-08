#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt.h"

int main(int argc, char *argv[]){

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);

    int numInitialized = 0;

    for(int i = 0; i < HSIZE; i++){
        
        struct BoardNode *tempNode = &htable[i];

        if(tempNode ->init == 1){
            numInitialized++;
        }
    }

    printf("%d\n", numInitialized);

    return 0;
}