#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt.h"

/* goes through the entire hash table and set the init variable in each structure to 0 */
void init_boards(){

    //loops through each node in the htable and sets init to 0
    //setting init to 0 indicates that that element of the hash table is empty
    for(int i = 0; i < HSIZE; i++){
        struct BoardNode *tempNode = &htable[i];
        tempNode -> init = 0;
    }
}

/* returns the number of markers (X's and O's) on the board */
int depth( Board board ){

    //loops over the string and counts the number of letters that are either X's or O's
    int numMarkers = 0;

    for(int i = 0; i < BSIZE; i++){
        if(board[i] == 'X' || board[i] == 'O'){
            numMarkers++;
        }
    }
    return numMarkers;
}

/* returns the status of the game */
char winner( Board board ){

    char winnerChar = '\0';

    //goes through the values in WINS array and checks if all three indices have the same value in them

    for(int i = 0; i < 8; i++){

        int winsSet[3];
        for(int j = 0; j < 3; j++){

            winsSet[j]= WINS[i][j];

        }

        int indexes[3] = {pos2idx[winsSet[0]], pos2idx[winsSet[1]], pos2idx[winsSet[2]]};

        if((board[indexes[0]] == board[indexes[1]]) && (board[indexes[1]] == board[indexes[2]])){

            //detects if X has wthe game in one of eight ways
            if(board[indexes[0]] == 'X'){
                winnerChar = 'X';
            }

            //detects if O has won the game in one of eight ways
            if(board[indexes[0]] == 'O'){
                winnerChar = 'O';
            }
        }
    }

    //detects if the game is over and no one has won
    if(depth(board) == 9){
        winnerChar = '-';
    }
    //if none of the above are satisified, then produce a question mark
    else if(winnerChar == '\0'){
        winnerChar = '?';
    }

    return winnerChar;
}

/* determines whose turn it is */
char turn( Board board ){

    //uses modular division to determine whose turn it should be 
    char turnChar = '\0';
    int depthNum = depth(board);
    int mod = depthNum % 2;

    if(depthNum < 9){
        
        if(mod == 0){
            turnChar = 'X';
        }
        else if(mod == 1){
            turnChar = 'O';
        }
    }
    else if(depthNum == 9){
        turnChar = '-';
    }

    return turnChar;
}

/* computes the hash index of the board and sets the variables in the structure */
void init_board( Board board ){

    //determines the hash index of the board
    int hashIndex = board_hash(board);

    //initializes a temporary BoardNode at that hash index
    struct BoardNode *tempNode = &htable[hashIndex];

    //set the value of init equal to 1
    tempNode -> init = 1;

    //turn is set to what the turn function returns
    tempNode -> turn = turn(board);

    //depth is set to what the depth function returns
    tempNode -> depth = depth(board);

    //board is set to the argument board by calling the strcpy function
    strcpy(tempNode -> board, board);

    //winner is set to what the winner function returns
    tempNode -> winner = winner(board);
}

/* creates and joins all the nodes of all the possible tic-tac-toe games */
void join_graph( Board board ){

    for(int i = 0; i < 9; i++){

        int charPosition = pos2idx[i];

        //for each position, if there is already an X or and O in that position
        //store a value of -1 in the move array in the appropriate structure of the htable (with board_hash function)
        if(board[charPosition] == 'X' || board[charPosition] == 'O'){
            int hashNum = board_hash(board);
            struct BoardNode *tempNode = &htable[hashNum];
            tempNode -> move[i] = -1;
        }

        //if the index contains a number, a copy is made of the board
        //a marker (X or O) is determined and placed by using the turn and pos2idx functions
        //the hash value of the new board is computed and stored in the move array
        if(board[charPosition] >= '0' && board[charPosition] <= '8'){

            Board boardCopy;
            strcpy(boardCopy, board);
            char marker = turn(boardCopy);
            boardCopy[charPosition] = marker;

            int originalHashNum = board_hash(board);
            int hashNum = board_hash(boardCopy);
            struct BoardNode *tempNode = &htable[originalHashNum];
            tempNode -> move[i] = hashNum;

            //then, the hash table is checked to determine if it already contains an entry (init != 0) for the new board string
            struct BoardNode *newNode = &htable[hashNum];

            //if the hash table already contains an entry at this location, loop to the next move position
            //if there is no entry, create one by calling init_board and recursively calling join_graph
            if(newNode -> init == 0){
                init_board(boardCopy);
                join_graph(boardCopy);
            }
        } 
    }
}

/* assigns a score to each entry in the htable that has an init value of 1 */
void compute_score(){

    htable[-1] -> init = 0;
    //change back to HSIZE
    for(int i = 0; i < HSIZE; i++){
        struct BoardNode *tempNode = &htable[i];

        if(winner(tempNode -> board) == '?'){
            break;
        }
        else if(winner(tempNode -> board) == 'X'){
            tempNode -> score = 1; 
        }
        else if(winner(tempNode -> board) == 'O'){
            tempNode -> score = -1;
        }
        else if(winner(tempNode -> board) == '-'){
            tempNode -> score = 0;
        }
        else{
            if(turn(tempNode -> board) == 'X'){

                int maxChildScore = 0;
                for(int j = 9; j >= 0; j--){

                    struct BoardNode *tempNode2 = &htable[tempNode -> move[j]];

                    if(tempNode2 -> score > maxChildScore){
                        maxChildScore = tempNode2 -> score;
                    }

                }
                tempNode -> score = maxChildScore;
            }
            else if(turn(tempNode -> board) == 'O'){

                int minChildScore = 0;
                for(int j = 9; j >= 0; j--){

                    struct BoardNode *tempNode2 = &htable[tempNode -> move[j]];

                    if(tempNode2 -> score < minChildScore){
                        minChildScore = tempNode2 -> score;
                    }
                }
                tempNode -> score = minChildScore;
            }
        }
    }

}

int best_move( int board ){

    int bestMove = 0;

    struct BoardNode *tempNode = &htable[board];

    for(int i = 0; i < 9; i++){

        struct BoardNode *tempNode2 = &htable[tempNode -> move[i]];

        if((tempNode2 -> score) > bestMove){
            bestMove = tempNode -> score;
        }
    }
    return bestMove;
}