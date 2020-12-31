#include <iostream>
#include <stdlib.h>
#include <time.h>
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

int minimax(Player player, int num[5][6], char col[5][6], int depth, bool FindMin);//player: find minimum; enemy: find maximum
void copyBoard(Board board, int num[5][6], char col[5][6]);
char Winner(char col[5][6]);
void ccBoard(int[5][6], char[5][6], int[5][6], char[5][6]);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    
    int row, col;
    int num_board[5][6];
    char col_board[5][6];
    int bestScore = __INT_MAX__;
    int score;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            //Is the cell available?
            if(board.get_cell_color(i, j) == player.get_color() || board.get_cell_color(i, j) == 'w'){
                copyBoard(board, num_board, col_board);
                num_board[i][j] += 1;
                col_board[i][j] = player.get_color();
                score = minimax(player, num_board, col_board, 10, true);  
                if(score < bestScore){
                    bestScore = score;
                    row = i;
                    col = j;
                }
            }
        }
    }

    index[0] = row;
    index[1] = col;
    
}

void copyBoard(Board board, int num[5][6], char col[5][6]){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            num[i][j] = board.get_orbs_num(i, j);
            col[i][j] = board.get_cell_color(i, j);
        }
    }
}

int minimax(Player player, int num[5][6], char col[5][6], int depth, bool FindMin){
    
    //deal with new board
    int limit[5][6];
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            limit[i][j] = 8;
        }
    }
    

    //if there is a winner
    char winner = Winner(col);
    if(winner != '~'){
        if(winner == player.get_color()){
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
                if(col[i][j] == player.get_color()){
                    sum -= num[i][j];
                }
                else if(col[i][j] != 'w'){
                    sum += num[i][j];
                }
            }
        }
        return sum;
    }
    
    //all possible move
    if(FindMin){
        int score;
        int bestScore = __INT_MAX__;
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 6; j++){
                if(col[i][j] == 'w' || col[i][j] == player.get_color()){
                    int new_num[5][6];
                    char new_col[5][6];
                    ccBoard(num, col, new_num, new_col);
                    new_num[i][j] += 1;
                    new_col[i][j] = player.get_color();
                    score = minimax(player, num, col, depth - 1, false);
                    bestScore = min(score, bestScore); 
                }
            }
        }
        return bestScore;
    }

    return 0;
}

char Winner(char col[5][6]){
    char s = '~';
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 6; j++){
            if(s == '~' && col[i][j] != 'w'){
                s = col[i][j];
                i = -1;
                j = -1;
            }
            else if(s != col[i][j]){
                return '~';
            }
        }
    }
    return s;
}

void ccBoard(int num[5][6], char col[5][6], int c_num[5][6], char c_col[5][6]){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            c_num[i][j] = num[i][j];
            c_col[i][j] = col[i][j];
        }
    }
}