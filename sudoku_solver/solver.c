#include <stdio.h>
#include "solver.h"

bool find_next_empty(int[][9], int*, int*);
bool isLegal(int[][9],int num,int row,int col); // is legal for now to place this number at these coordinates
void printGrid(int[][9]);
bool solve(int grid[][9]);

bool findSolution(int grid[][9])
{
  printGrid(grid);

  if (solve(grid))
  {
    printf("\nthe solution is : \n");
    printGrid(grid);
    return true;
  }else
   {
    printf("\nbad puzzle, unsolvable\n");
    return false;
   }
}

bool solve(int grid[][9])
{
   int row;
   int col;
   int* p_row = &row;
   int* p_col = &col;

   if (!find_next_empty(grid, p_row, p_col)) // if all spots are filled already, we are done
     return true;

   for ( int i = 1; i <= 9; ++i )
   {
      if (isLegal(grid,i,row, col)) // possible fit for this spot
      {
	grid[row][col] = i;// tenativley place this number
        if (solve(grid)) return true; // if is solves with the number in, we are good
	 grid[row][col] = 0; // undo the tenative placement
      }
   }

    return false; // back track to try again, or if was top of stack, we failed to solve.
}

bool isLegal(int grid[][9], int num, int row, int col)
{
  int boxY = (row / 3) * 3;
  int boxX = (col / 3) * 3;
  int rowA = ((row + 2) % 3) + boxY;
  int rowB = ((row + 4) % 3) + boxY;
  int colA = ((col + 2) % 3) + boxX;
  int colB = ((col + 4) % 3) + boxX;
  // iterate the row and coulums and check for a duplicate
  for (int i = 0; i < 9; ++i)
  {
    if(grid[row][i] == num) return false;
    if(grid[i][col] == num) return false;
  }
   
  // check each box for a duplicate, we only need to test the 4 corners in each box
   
   if (grid[rowA][colA] == num) return false;
   if (grid[rowA][colB] == num) return false;
   if (grid[rowB][colA] == num) return false;
   if (grid[rowB][colB] == num) return false;
  
   return true;
}

bool find_next_empty(int grid[][9], int* row, int* col)
{
  for (int y = 0; y < 9; ++y)
  {
    for (int x = 0; x < 9; ++x)
    {
      if (grid[y][x] == 0)
       {
         *row = y;
         *col = x;
         return true;
       }
    }
  }
  return false;
}

void printGrid(int grid[][9])
{
   int num;
   printf("\n");
  for (int y = 0; y < 9; ++y)
  {
    for (int x = 0; x < 9; ++x)
    {
      num = grid[y][x];
      printf("%d, ",num);
    }
    printf("\n");
  }
    printf("\n");
}









