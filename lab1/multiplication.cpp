#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void PrintStars();
void MallocMatrix(int**& tab, int width, int height);
void FillArrayWithData(int rows, int cols, int**& tab, int fixedIncrementalValue);
void FillArrayWithZeroes(int rows, int cols, int** tab);
void PrintMatrix(int**& tab, int toWhichColumn, int toWhichRow);
void Multiply2dMatrixesParallel(int widthC, int heightC, int** tabA, int** tabB, int** tabC, int numThreads);
void ReleaseMatrix(int height, int** tab);

int main(int argc, char **argv){
    PrintStars();
    printf(">>Lab1 Matrix Multiplication with OpenMP \n");
    PrintStars();

    /** Declaration of size of arrays **/
    int widthA = 900, heightA = 900;
    int widthB = 900, heightB = 900;

    /** Printing matrices sizes **/
    printf("Matrix A: %d x %d Matrix B: %d x %d", widthA, heightA, widthB, heightB);

     /** Size check: (2x5 * 5x2 = 5x2) **/
    if (heightA == widthB)
    {
        const int widthC = widthB, heightC = heightA;
        printf("Matrix C: %d x %d \n", widthC, heightC);

        /** Declaration of arrays **/
        int** tabA;
        int** tabB;
        int** tabC;

        /**Allocation for tabA, tabB, tabC **/
        MallocMatrix(tabA, widthA, heightA);
        MallocMatrix(tabB, widthB, heightB);
        MallocMatrix(tabC, widthC, heightC);

        FillArrayWithZeroes(heightA, widthA, tabA);
        FillArrayWithZeroes(heightB, widthB, tabB);
        FillArrayWithZeroes(heightC, widthC, tabC);

        const int fixedIncrementalValueforA = 1;
        FillArrayWithData(heightA, widthA, tabA, fixedIncrementalValueforA);
        const int fixedIncrementalValueforB = 2;
        FillArrayWithData(heightB, widthB, tabB, fixedIncrementalValueforB);

    	PrintStars();
        printf("Matrix A: \n");
        PrintMatrix(tabA, 10, 3);
        PrintStars();
        printf("Matrix B: \n");
        PrintMatrix(tabB, 10, 3);
        PrintStars();

        PrintStars();
        int numThreads = 12;
        printf(">>Parallel Matrix Multiplication in progress...for %i thread(s)\n", numThreads);
        printf("Matrix A: %d x %d Matrix B: %d x %d\n", widthA, heightA, widthB, heightB);
        PrintStars();
        Multiply2dMatrixesParallel(widthC, heightC, tabA, tabB, tabC, numThreads);

        PrintStars();
        PrintMatrix(tabC, 10, 3);
        PrintStars();

        ReleaseMatrix(heightA, tabA);
        ReleaseMatrix(heightB, tabB);
        ReleaseMatrix(heightC, tabC);
    }
    else
    {
        printf("Matrices sizes are incorrect.\n");
    }

    printf("Please, press any character...\n");
    getchar();
    return 0;
}

void FillArrayWithZeroes(int rows, int cols, int** tab)
{
    //height
    for (int i = 0; i < rows; i++)
    {
        //width
        for (int j = 0; j < cols; j++)
        {
            tab[i][j] = 0;
        }
    }
}

void PrintMatrix(int**&tab, int toWhichColumn, int toWhichRow)
{
    //height
    for (int i = 0; i < toWhichRow; i++)
    {
        //width
        for (int j = 0; j < toWhichColumn; j++)
        {
            printf( "[%d , %d ] = %d  ",i , j, tab[i][j]);
        }
        printf("\n");
    }
}

void MallocMatrix(int**& tab, int width, int height)
{
    tab = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++)
    {
        tab[i] = (int*)malloc(width * sizeof(int));
    }
}

void FillArrayWithData(int rows, int cols, int**& tab, int fixedIncrementalValue)
{
    //height
    for (int i = 0; i < rows; i++)
    {
        //width
        for (int j = 0; j < cols; j++)
        {
            tab[i][j] = rows + i + j + fixedIncrementalValue;
        }
    }
}

void PrintStars()
{
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            printf("**");
        }
        printf("\n");
    }
}

void Multiply2dMatrixesParallel(int widthC, int heightC, int** tabA, int** tabB, int** tabC, int numThreads)
{
   int i,j,k,sum = 0;
   bool firstTimeEntry;
   #pragma omp parallel shared(tabA,tabB,tabC) private(i,j,k,sum,firstTimeEntry)  num_threads(numThreads)
   {    
        double start, end;
        start = omp_get_wtime();      
        firstTimeEntry = true;  
        if(firstTimeEntry)
        {
            printf("Thread<%d> starting work.\n", omp_get_thread_num());
            firstTimeEntry = false;
        }
        #pragma omp for schedule(static)
        for (int i = 0; i < widthC; i++)
        {
            for (int j = 0; j < heightC; j++)
            {
                for (int k = 0; k < widthC; k++)
                {
                    int tmp = tabA[i][k] * tabB[k][j];
                    sum += tmp;
                }
                tabC[i][j] = sum;
                sum = 0;
            }

            if( i == widthC - 1)
            {
                printf("Last Thread<%d> stopped work on index %d .\n", omp_get_thread_num(), i);
                end = omp_get_wtime();
                printf("The job has taken %f seconds\n", end - start);
            }
        }
   }
}

void ReleaseMatrix(int height, int** tab)
{
    for (int i = 0; i < height; i++)
    {
        free(tab[i]);
    }
}