#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "assignment_2.h"

//3 assumptions as mentioned in the doubts Excel Sheet: numbers are all distinct, non-negative, n ≥ 2

static void init(int space; int matrix[][space], int space)
{
	for(int i = 0; i < space; i++)
	{
		matrix[0][i] = -1;
	}
}

void print_set(int space; int matrix[][space], int space, int c, int r)
{
	for(int i = 0; i < r + 1; i++)
	{
		printf("%d ", matrix[i][c]);
	}
}

void find_second_greatest(int *numbers, int length)
{
	int space = ceil(log2(length))+1;
	int matrix[space][space];
	init(matrix, space);

	int last_pos;
	int elem;
	for(int i = 0; i < length; i++)
	{
		last_pos = 0;
		elem = numbers[i];
		while(1)
		{
			if(matrix[0][last_pos] == -1)
			{
				if(last_pos == 0)
				{
					matrix[0][0] = elem;
				}
				else
				{
					int y = last_pos+1;
					for(int x = 0; x < y; x++)
					{
						matrix[x][last_pos] = matrix[x][last_pos-1];
					}
					matrix[0][last_pos-1] = -1;
				}
				break;
			}
			else
			{
				if(last_pos == 0)
				{
					if(matrix[0][0] > elem)
					{
						printf("%d x %d = %d %d\n", matrix[0][0], elem, matrix[0][0], elem);
						matrix[1][0] = elem;
					}
					else
					{
						printf("%d x %d = %d %d\n", matrix[0][0], elem, elem, matrix[0][0]);
						matrix[1][0] = matrix[0][0];
						matrix[0][0] = elem;
					}
					last_pos++;
				}
				else
				{
					print_set(matrix, space, last_pos, last_pos);
					printf("x ");
					print_set(matrix, space, last_pos-1, last_pos);
					printf("= ");
					if(matrix[0][last_pos] > matrix[0][last_pos-1])
					{
						for(int x = last_pos; x >= 1; x--)
						{
							matrix[x+1][last_pos] = matrix[x][last_pos];
						}
						matrix[1][last_pos] = matrix[0][last_pos-1];
						matrix[0][last_pos-1] = -1;
						last_pos++;
					}
					else
					{
						matrix[1][last_pos] = matrix[0][last_pos];
						matrix[0][last_pos] = matrix[0][last_pos-1];
						for(int x = 1; x < last_pos+1; x++)
						{
							matrix[x+1][last_pos] = matrix[x][last_pos-1];
						}
						matrix[0][last_pos-1] = -1;
						last_pos++;
					}
					print_set(matrix, space, last_pos-1, last_pos);
					printf("\n");
				}
			}
		}
	}
	for(int i = 0; i < space; i++)
	{
		for(int j = 0; j < space; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	// int i = 0;
	// while(matrix[0][i] == -1)
	// {
	// 	i++;
	// }
	// if(i != space - 1)
	// {
	// 	int size = i;
	// 	while(i != space-2)
	// 	{
	// 		if(matrix[0][i+1] == -1)
	// 		{
	// 			i++;
	// 		}
	// 		else
	// 		{
				
	// 		}
	// 	}
	// }
	//find second highest in ith column
}