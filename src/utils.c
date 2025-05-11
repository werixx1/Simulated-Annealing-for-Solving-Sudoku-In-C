#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h> 
#include <stdbool.h>

void print_board(int size, int** board)
{
    const int square = (int)sqrt(size); 

    printf("    ");
    for (int j = 0; j < size; j++) {
        if (j % square == 0 && j != 0)
            printf("  "); 
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < size; i++)
        {
        if (i % square == 0 && i != 0)
            {
            printf("    ");
            for (int k = 0; k < size; k++)
                {
                if (k % square == 0 && k != 0)
                    printf("+"); 
                printf("---");
            }
            printf("\n");
        }

        printf("%2d  ", i + 1);
        for (int j = 0; j < size; j++)
            {
            if (j % square == 0 && j != 0)
                printf("| ");  
            if (board[i][j] == 0)
                printf(" . ");
            else
                printf("%2d ", board[i][j]);
            }
        printf("\n");
    }
}
