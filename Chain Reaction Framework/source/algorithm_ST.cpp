#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
//g++ chain_reaction.cpp board.cpp rules.cpp player.cpp algorithm_ST.cpp algorithm_TA.cpp


int minimax(Player player, Board cboard, int depth, bool FindMax, int alpha, int beta, int *row, int *col);
char Winner(Board);
char player_me, player_enemy;
int DEPTH = 5;

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    
    int bestScore = -__INT_MAX__;
    int score;
    Board cboard = board;
    int row, col;

    //define player
    player_me = player.get_color();
    if(player_me == 'r'){
        player_enemy = 'b';
    }
    else{
        player_enemy = 'r';
    }

   minimax(player, board, DEPTH, true, -__INT_MAX__, __INT_MAX__, &row, &col);
   index[0] = row;
   index[1] = col;
}

int minimax(Player player, Board cboard, int depth, bool FindMax, int alpha, int beta, int *row, int *col){
    
    //if there is a winner
    char winner = Winner(cboard);
    if(winner != '~'){
        if(winner == player_me){
            return -__INT_MAX__;
        }
        else
        {
            return __INT_MAX__;
        }
        
    }

    //can't predict all chances
    if(depth == 0){
        int sum = 0;
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 6; j++){
                if(cboard.get_cell_color(i, j) == player_me){
                    sum = sum + cboard.get_orbs_num(i, j);
                }
                else if(cboard.get_cell_color(i, j) == player_enemy){
                    sum = sum - cboard.get_orbs_num(i, j);
                }
            }
        }
        return sum;
    }
    
    //all possible move
    Board tboard = cboard;
    if(FindMax){
        int score;
        int bestScore = -__INT_MAX__;
        bool end = false;
        Player enemy(player_enemy);
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 6; j++){
                if(cboard.place_orb(i, j, &player)){
                    score = minimax(player, cboard, depth - 1, false, alpha, beta, row, col);
                    if(bestScore < score && depth == DEPTH){
                        *row = i;
                        *col = j;
                    }
                    bestScore = max(bestScore, score);
                    alpha = max(alpha, bestScore);
                    cboard = tboard;
                    if(beta <= alpha){
                        end = true;
                        break;
                    }
                }
            }
            if(end){
                break;
            }
        }
        return bestScore;
    }
    else{
        int score;
        int bestScore = __INT_MAX__;
        bool end = false;
        Player enemy(player_enemy);
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 6; j++){
                if(cboard.place_orb(i, j, &enemy)){
                    score = minimax(player, cboard, depth - 1, true, alpha, beta, row, col);
                    bestScore = min(score, bestScore); 
                    beta = min(beta, bestScore);
                    cboard = tboard;
                    if(beta <= alpha){
                        end = true;
                        break;
                    }
                }
            }
            if(end){
                break;
            }
        }
        return bestScore;
    }
}

char Winner(Board cboard){
    char s = '~';
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 6; j++){
            if(s == '~' && cboard.get_cell_color(i, j) != 'w'){
                s = cboard.get_cell_color(i, j);
                i = -1; 
                j = -1;
            }
            else if(s != cboard.get_cell_color(i, j) && cboard.get_cell_color(i, j) != 'w'){
                return '~';
            }
        }
    }
    return s;
}