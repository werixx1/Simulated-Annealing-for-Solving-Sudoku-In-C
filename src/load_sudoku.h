#ifndef LOAD_SUDOKU_H
#define LOAD_SUDOKU_H

#include <stdbool.h>

// functions stolen and modified from previous project hehe
int **allocate_board(int size);
void free_board(int **board, int size);
void copy_board(int **og_board, int **new_board, int size);
bool unused_in_row(int **board, int size, int row, int num);
bool unused_in_col(int **board, int size, int col, int num);
bool unused_in_square(int **board, int row_start, int col_start, int num, int square);
bool is_safe(int **board, int size, int row, int col, int num);
void fill_box(int **board, int row_start, int col_start, int size);
void fill_diagonal(int **board, int size);
bool fill_remaining(int **board, int size, int row, int col);
void remove_k_digits(int **board, int size, int k);
int **generate_sudoku(int size, int empty_cells);

#endif