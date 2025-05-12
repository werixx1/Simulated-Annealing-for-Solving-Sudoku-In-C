#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h> 
#include "load_sudoku.h"
#include "utils.h"

typedef struct 
{
    int row;
    int col;
} SudokuCell;

int calculate_cost(int** board, int size)
{
    int cost = 0;
    for (int row = 0; row < size; row++)
    {
        int count[10] = {0}; // array to sum how many repetitions are of each numbers (1-9) in a row
        for (int col = 0; col < size; col++)
        {
            count[board[row][col]]++; // counting repetitions of each number in a row (++ for each)
        }
        for (int num = 1; num <= size; num++)
        {
            if (count[num] > 1) // if a num occurs more than once
            {
                cost += count[num] - 1; // adds the number of how many times more a num occured than it should
                                       // (like for three twos in a row or col 
                                      // it will add 2 because they appeared 2 times more than its allowed)
            }
        }
    }

    // the same thing but for columns
    for (int col = 0; col < size; col++)
    {
        int count[10] = {0};
        for (int row = 0; row < size; row++)
        {
            count[board[row][col]]++;
        }
        for (int num = 1; num <= size; num++)
        {
            if(count[num] > 1)
            {
                cost += count[num] - 1;
            }
        }
    }
    return cost;
}

// function that fills empty cells with random nums (but without repetitions in each
//                                                   3x3 block)
void fill_board_with_nums(int** board, int size)
{
    int square = (int)sqrt(size); // size of a block

    // iterating through 3x3 blocks
    for (int row_3x3 = 0; row_3x3 < square; row_3x3++)
    {
        for (int col_3x3 = 0; col_3x3 < square; col_3x3++)
        {
            int start_row = row_3x3 * square;
            int start_col = col_3x3 * square;

            int missing_nums[9]; 
            int missing_counter = 0;

            // looking for missing numbers in a certain block
            for (int num = 1; num <= size; num++)
            {
                bool found = false;

                // check if number 'num' is already in a block
                for (int i = 0; i < square; i++)
                {
                    for (int j = 0; j < square; j++)
                    {
                        if (board[start_row + i][start_col + j] == num)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
                // if not add it to missing_nums
                if (!found)
                {
                    missing_nums[missing_counter++] = num;
                }
            }

            // fisher–yates shuffle
            for (int i = missing_counter - 1; i > 0; i--)
            {
                int j = rand() % (i + 1);
                int temp = missing_nums[i];
                missing_nums[i] = missing_nums[j];
                missing_nums[j] = temp;
            }

            int index_missing = 0;

            // putting missing nums in empty places (wheres 0)
            for (int i = 0; i < square; i++)
            {
                for (int j = 0; j < square; j++)
                {
                    if(board[start_row + i][start_col + j] == 0)
                    {
                        board[start_row + i][start_col + j] = missing_nums[index_missing++];
                    }
                }
            }
        }
    }
}

// function that generates new board state by swapping random cells in a random block
void generate_neighbor(int** board, int size, int** new_board)
{
    int square = (int)sqrt(size);

    copy_board(board, new_board, size);

    // choosing random block to edit
    int block_row = rand() % square;
    int block_col = rand() % square;
    // and their coordinates
    int start_row = block_row * square;
    int start_col = block_col * square;

    SudokuCell cells_that_can_be_swapped[9]; // struct of 9 rows and cols
    int counter = 0;

    // looking for cells that can we swapped (that used to be zeros in og board)
    for (int i = 0; i < square; i++) 
    {
        for (int j = 0; j < square; j++) 
        {
            int row = start_row + i;
            int col = start_col + j;
            if (board[row][col] == 0) // nums that were 0 are not frozen and can be swapped
            {
                // saving cells info to struct
                cells_that_can_be_swapped[counter].row = row;
                cells_that_can_be_swapped[counter].col = col;
                counter++;
            }
        }
    }
    
    if (counter >= 2) // after having two cells to swap
    {
        int random_index = rand() % counter; // choosing random index
        int swap;

        do 
        {
            swap = rand() % counter;  // choosing another (different) random index
        } 
        while (swap == random_index); 
        
        // swapping cells
        int temp = new_board[cells_that_can_be_swapped[random_index].row][cells_that_can_be_swapped[random_index].col];

        new_board[cells_that_can_be_swapped[random_index].row][cells_that_can_be_swapped[random_index].col] 
        = new_board[cells_that_can_be_swapped[swap].row][cells_that_can_be_swapped[swap].col];

        new_board[cells_that_can_be_swapped[swap].row][cells_that_can_be_swapped[swap].col] = temp;
    }
}

int** solve_sudoku_sa(int** board, int size)
{
    srand(time(NULL)); // random nums generator
    
    int **current_board = allocate_board(size);
    int **best_board = allocate_board(size);
    int **neighbor_board = allocate_board(size);
    
    copy_board(board, current_board, size);
    fill_board_with_nums(current_board, size);
    copy_board(current_board, best_board, size); // for the start assuming that the first state
                                                // is the best
    
    // sa parameters
    double T_start = 1000.0;
    double T_end = 0.01;
    double alpha = 0.99;
    int max_iterations = 200000;
    
    double T = T_start; // holds current temp
    int current_cost = calculate_cost(current_board, size);
    int best_cost = current_cost;
    
    int iteration = 0;
    // main sa loop
    while (T > T_end && iteration < max_iterations && best_cost > 0) 
    {
        generate_neighbor(current_board, size, neighbor_board);
        int neightbour_cost = calculate_cost(neighbor_board, size);
        
        int delta_E = neightbour_cost - current_cost; // calculating delta_E
        
        // sa acceptance decision
        // if new state is better than the previous one its accepted
        // if worse its still accepted but with probability depending on temp
        if (delta_E < 0 || (T > 0 && (double)rand() / RAND_MAX < exp(-delta_E / T))) 
        {
            copy_board(neighbor_board, current_board, size); // accept new state
            current_cost = neightbour_cost;
            
            if (current_cost < best_cost) // if new state is the best till now
            {
                copy_board(current_board, best_board, size); // accept it as new best solution
                best_cost = current_cost;
                printf("Iteration: %d, Cost: %d, Temperature: %.2f\n", iteration, best_cost, T);
            }
        }
        
        // cooling
        T *= alpha;
        iteration++;
    }
    
    printf("Final cost: %d\n", best_cost);
    free_board(current_board, size);
    free_board(neighbor_board, size);
    
    return best_board;
}

int main()
{
    int size = 9;
    printf("-------- ORIGINAL BOARD ---------\n");
    int** board =  generate_sudoku(size, 40);
    print_board(size, board);

    printf("\n");
    printf("-------- SOLVED BOARD ---------\n");
    int** solved_board = solve_sudoku_sa(board, size);
    print_board(size, solved_board);

    free_board(board, size);
    free_board(solved_board, size);

    return 1;
}