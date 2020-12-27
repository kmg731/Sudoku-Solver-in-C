/* Sudoku Solver in C
 * 
 * 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DIM 9
#define SUBGRID_DIM  3
#define FILESIZE 81


typedef struct
puzzle_t
{
        int values[DIM][DIM];
        bool isFixed[DIM][DIM];
        bool subgridHasVal[SUBGRID_DIM][SUBGRID_DIM][DIM];
        bool colHasVal[DIM][DIM];
        bool rowHasVal[DIM][DIM];
        
}Puzzle;

Puzzle*
loadPuzzle(char* fileName)
{

        Puzzle* p = (Puzzle*)malloc(sizeof(Puzzle));

        memset(p->values, 0, (DIM * DIM));
        memset(p->isFixed, 0, (DIM * DIM));
        memset(p->colHasVal, 0, (DIM * DIM));
        memset(p->rowHasVal, 0, (DIM * DIM));
        memset(p->subgridHasVal, 0, (DIM * DIM));
        
        FILE *fp;
        fp = fopen(fileName, "r");

        int puzzleVal;
        int i = 0;
        
        while((puzzleVal = fgetc(fp)) != EOF){
                if (puzzleVal >= 48 && puzzleVal <=57){
                        int row = i / 9;
                        int col = i % 9;
                        int val = puzzleVal - 48;

                        p->values[row][col] = val;
                        
                        if( val > 0){
                                p->isFixed[row][col]   = true;
                                p->rowHasVal[row][val] = true;
                                p->colHasVal[col][val] = true;
                                p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = true;
                                }
 
                        ++i;
                } 
        }


        fclose(fp);
        return p;
}

int isSafe(Puzzle* p, int val, int row, int col)
{
        return (!(p->rowHasVal[row][val])
                && !(p->colHasVal[col][val])
                && !(p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val])
                && !(p->isFixed[row][col])                );
}

void
placeVal(Puzzle* p, int val, int row, int col)
{

        p->values[row][col] = val;
        p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = true;
        p->rowHasVal[row][val] = true;
        p->colHasVal[col][val] = true;
}

void
removeVal(Puzzle* p, int val, int row, int col)
{
        p->values[row][col] = 0;
        p->subgridHasVal[row / SUBGRID_DIM][col / SUBGRID_DIM][val] = false;
        p->rowHasVal[row][val] = false;
        p->colHasVal[col][val] = false;

}

// Something is wrong with the truth values of the numbers
int
solvePuzzle(Puzzle* p, int n){
        
        if (n == 81){
                return true;
        }

        int row = n / 9;
        int col = n % 9;

        if (p->isFixed[row][col]){
                if(solvePuzzle(p, n + 1)){
                        return true;
                }
        }

        for (int val = 1; val <= 9; ++val){
                if (isSafe(p, val, row, col)){
                        placeVal(p, val, row, col);

                        if (solvePuzzle(p, n + 1)){
                                return true;
                        }

                        removeVal(p, val, row, col);
                }
        }
        return false;
}
                

int
WRAPPER_Solve(Puzzle* p)
{
        int found = solvePuzzle(p, 0);
        return found;
}

void
printPuzzle(Puzzle* p)
{
        // Still need to fix, this is just to test if it's working
        for(int row = 0; row < 9; ++row){
                for (int col = 0; col < 9; ++col){
                        printf("%i ", p->values[row][col]);
                }
                putchar('\n');
        }
}

int
main(int argc, char** argv)
{
        
        Puzzle* p = loadPuzzle(argv[1]);
        printPuzzle(p);

        if(WRAPPER_Solve(p) == 1){
                printf("\nA solution has been found!\n");
                printPuzzle(p);
        } else {
                printf("Something has gone wrong, here is the output:\n");
                printPuzzle(p);
        }
        
        free(p);
        return 0;
}

