#include <stdio.h>
#include <time.h>
#include <mpi.h >


// Declatations
// 
//Declare Matrix
float inputted_matrix[3][4] = {
{1,1,1,1},
{2,2,2,2},
{3,3,3,3}
};

float F[3][4] = {
{1,1,-1,-2},
{2,-1,1,5},
{-1,2,2,1}
};

float M[3][4] = {
{2,1,1,10}, 
{3,2,3,18},
{1,4,9,16}
};

float G[3][4] = {
{1, 1, -1, -3},
{ 6, 2, 2, 2 },
{ -3, 4, 1, 1 }
};

//Get Matrix size (Amount of Rows)   
int matrix_size = (sizeof(inputted_matrix) / sizeof(inputted_matrix[0]));

int main() {

    //MPI Parrallel initialise
    MPI_Init(NULL, NULL);
    //MPI Get Rank of Thread and num of Threads
    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int numproc;
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);
    int partition = (matrix_size / numproc);


    //Start Timer
    clock_t t;
    t = clock();
    printf("Timer starts\n");

    int k;
    int i;
    int j;

    // Forward Elimination:
    for (k = (myRank * partition); k < (myRank * partition + partition); k++) {

        for (i = k + 1; i < myRank * partition + partition; i++) {

            float multiple = M[i][k] / M[k][k];


            for (j = k; j < myRank * partition + partition; j++) {
                M[i][j] = M[i][j] - multiple * M[k][j];
            }
            M[i][matrix_size] = M[i][matrix_size] - multiple * M[k][matrix_size];
        }
    }

    //Calculating Values
    float outputs[matrix_size];
    for (i = 0; i < matrix_size; i++) {
        outputs[i] = 0;
    }
    outputs[matrix_size - 1] = M[matrix_size - 1][matrix_size] / M[matrix_size - 1][matrix_size - 1];

    //Backwards Subsitution
    for (i = matrix_size - 1; i >= 0; i--) {

        float sum = M[i][matrix_size];
        for (j = i + 1; j < matrix_size; j++) {
            sum = sum - ((M[i][j] * outputs[j]));

        }
        outputs[i] = sum / (M[i][i]);
    }

    // Outputs
    for (i = 0; i < matrix_size; i++)
    {
        printf("X%d Value: %.2f \n", i + 1, outputs[i]);
    }

    printf("\n");
    printf("\n");

    // calculate the elapsed time
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("The program took %f seconds to execute", time_taken);

    //MPI Finalize
    MPI_Finalize();

    return 0;
}