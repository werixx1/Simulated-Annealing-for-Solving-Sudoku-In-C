#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h> 
#include "load_sudoku.h"
#include "utils.h"

//int** solved_sudoku()
//{
//    return board;
//}

int main()
{
    int size = 9;
    int** board =  generate_sudoku(size, 40);
    print_board(size, board);

    return 1;
}