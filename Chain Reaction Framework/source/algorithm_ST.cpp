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

int minimax(Player player, int num[5][6], char col[5][6], int depth, bool FindMin);//player: find minimum; enemy: find maximum
void copyBoard(Board board, int num[5][6], char col[5][6]);
char Winner(char col[5][6]);
void ccBoard(int[5][6], char[5][6], int[5][6], char[5][6]);
bool check_explode(int[5][6], queue<int>&, queue<int>&);

int limit[5][6];


void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    
    int row, col;
    int num_board[5][6];
    char col_board[5][6];
    int bestScore = __INT_MAX__;
    int score;

    //set critical value
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            limit[i][j] = 8;
        }
    }
    for(int i = 0; i < 5; i++){
        limit[i][0] = 5;
        limit[i][5] = 5;
    }
    for(int i = 0; i < 6; i++){
        limit[0][i] = 5;
        limit[4][i] = 5;
    }
    limit[0][0] = 3;
    limit[0][5] = 3;
    limit[4][0] = 3;
    limit[4][6] = 3;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            //Is the cell available?
            if(board.get_cell_color(i, j) == player.get_color() || board.get_cell_color(i, j) == 'w'){
                copyBoard(board, num_board, col_board);
                num_board[i][j] += 1;
                col_board[i][j] = player.get_color();
                score = minimax(player, num_board, col_board, 5, true);  
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
    queue<int> boomRow;
    queue<int> boomCul;
    while(check_explode(num, boomRow, boomCul)){
        while(!boomRow.empty()){
            char color;
            int i = boomRow.front();
            int j = boomCul.front();

            if(FindMin){
                color = player.get_color();
            }
            else{
                if(player.get_color() == 'r'){
                    color = 'b';
                }
                else{
                    color = 'r';
                }
            }

            num[i][j] -= limit[i][j];
            if(num[i][j] == 0) col[i][j] = 'w';
            
            if( i + 1 < 5){
                num[i + 1][j] += 1;
                col[i + 1][j] = color;
            }
            if( j + 1 < 6){
                num[i][j + 1] += 1;
                col[i][j + 1] = color;
            }
            if( i - 1 >= 0){
                num[i - 1][j] += 1;
                col[i - 1][j] = color;
            }
            if( j - 1 >= 0){
                num[i][j - 1] += 1;
                col[i][j - 1] = color;
            }
            if( i + 1 < 5 && j - 1 >= 0){
                num[i + 1][j - 1] += 1;
                col[i + 1][j - 1] = color;
            }
            if( i - 1 >= 0 && j - 1 >= 0){
                num[i - 1][j - 1] += 1;
                col[i - 1][j - 1] = color;
            }
            if( i + 1 < 5 && j + 1 < 6){
                num[i + 1][j + 1] += 1;
                col[i + 1][j + 1] = color;
            }
            if( i - 1 >= 0 && j + 1 < 6){
                num[i - 1][j + 1] += 1;
                col[i - 1][j + 1] = color;
            }

            boomRow.pop();
            boomCul.pop();
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
    else{
        int score;
        int bestScore = -__INT_MAX__;
        char color;

        if(player.get_color() == 'r'){
            color = 'b';
        }
        else{
            color = 'r';
        }

        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 6; j++){
                if(col[i][j] == 'w' || col[i][j] == color){
                    int new_num[5][6];
                    char new_col[5][6];
                    ccBoard(num, col, new_num, new_col);
                    new_num[i][j] += 1;
                    new_col[i][j] = color;
                    score = minimax(player, num, col, depth - 1, true);
                    bestScore = max(score, bestScore); 
                }
            }
        }
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

bool check_explode(int num[5][6], queue<int> &row, queue<int> &cul){
    bool v = false;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            if(num[i][j] >= limit[i][j]){
                row.push(i);
                cul.push(j);
                v = true;
            }
        }
    }
    return v;
}