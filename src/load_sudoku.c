#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h> 
#include <stdbool.h>

int **allocate_board(int size)
{
    int **board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        board[i] = malloc(size * sizeof(int));
    return board;
}

void free_board(int **board, int size)
{
    for (int i = 0; i < size; i++)
        free(board[i]);
    free(board);
}

void copy_board(int **og_board, int **new_board, int size)
{
    if (og_board == NULL || new_board == NULL || size <= 0) return;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            new_board[i][j] = og_board[i][j];
}

bool unused_in_row(int **board, int size, int row, int num)
{
    for (int col = 0; col < size; col++)
        if (board[row][col] == num) 
            return false;
    return true;
}

bool unused_in_col(int **board, int size, int col, int num)
{
    for (int row = 0; row < size; row++)
        if (board[row][col] == num)
            return false;
    return true;
}

bool unused_in_square(int **board, int row_start, int col_start, int num, int square)
{
    for (int i = 0; i < square; i++)
        for (int j = 0; j < square; j++)
            if (board[row_start + i][col_start + j] == num)
                return false;
    return true;
}

bool is_safe(int **board, int size, int row, int col, int num)
{
    int square = (int)sqrt(size);
    return board[row][col] == 0 &&
           unused_in_row(board, size, row, num) &&
           unused_in_col(board, size, col, num) &&
           unused_in_square(board, row - row % square,
                            col - col % square, num, square);
}

void fill_box(int **board, int row_start, int col_start, int size)
{
    int square = (int)sqrt(size);
    int num;
    for (int i = 0; i < square; i++)
    {
        for (int j = 0; j < square; j++)
        {
            do
            {
                num = rand() % size + 1;
            } while (!unused_in_square(board, row_start, col_start, num, square));
            board[row_start + i][col_start + j] = num;
        }
    }
}

void fill_diagonal(int **board, int size)
{
    int subgrid = (int)sqrt(size);
    for (int i = 0; i < size; i += subgrid)
        fill_box(board, i, i, size);
}

bool fill_remaining(int **board, int size, int row, int col)
{
    if (row >= size)
        return true;

    if (col >= size)
        return fill_remaining(board, size, row + 1, 0);

    if (board[row][col] != 0)
        return fill_remaining(board, size, row, col + 1);

    for (int num = 1; num <= size; num++)
    {
        if (is_safe(board, size, row, col, num))
        {
            board[row][col] = num;
            if (fill_remaining(board, size, row, col + 1))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

void remove_k_digits(int **board, int size, int k)
{
    if (board == NULL) return;
    int count = 0;
    while (count < k)
    {
        const int i = rand() % size;
        const int j = rand() % size;
        if (board[i][j] != 0)
        {
            board[i][j] = 0;
            count++;
        }
    }
}

int **generate_sudoku(int size, int empty_cells)
{
    int **board = allocate_board(size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;

    fill_diagonal(board, size);
    if (!fill_remaining(board, size, 0, 0))
    {
        free_board(board, size);
        return generate_sudoku(size, empty_cells); 
    }

    if (empty_cells > size * size)
        empty_cells = size * size - 1;

    remove_k_digits(board, size, empty_cells);
    return board;
}

