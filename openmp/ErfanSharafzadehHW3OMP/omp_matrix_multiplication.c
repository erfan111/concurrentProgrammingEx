#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


int** matrix1;
int** matrix2;
int** result;

struct timeval tvBegin;
struct timeval tvEnd;
struct timeval tvDiff;

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}


void matrix_multiply(int n, int** matrix1, int** matrix2, int** result){
    #pragma omp parallel
    {
        int i,j,k;
        int sum = 0;
        #pragma omp for schedule(guided) private(i,j,k,sum)
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    sum = sum + matrix1[i][k]*matrix2[k][j];
                }
                result[i][j] = sum;
                sum = 0;
            }
        }
    }

}


int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}



int main() {
    int i,j,n,t;
    //char c;
    t = 8;
    n = 2048;
    srand((unsigned int) time(NULL));
    // getting dimensions of matrices
//    do
//    {
//        printf("\nEnter dimensions of matrices: ");
//
//    } while ((scanf("%d%c", &n, &c)!=2 || c!='\n') && clean_stdin());
    // getting number of threads
//    do
//    {
//        printf("\nEnter number of threads: ");
//
//    } while ((scanf("%d%c", &t, &c)!=2 || c!='\n') && clean_stdin());

    omp_set_num_threads(t);
    // MATRIX 1 INIT

    matrix1 = (int**)malloc(n * sizeof(int *));
    for(i = 0; i < n; i++)
        matrix1[i] = (int*)malloc(n * sizeof(int));
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            matrix1[i][j] = (rand() % 100) + 1;

    // MATRIX 2 INIT

    matrix2 = (int**)malloc(n * sizeof(int *));
    for(i = 0; i < n; i++)
        matrix2[i] = (int*)malloc(n * sizeof(int));
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            matrix2[i][j] = (rand() % 100) + 1;

    // RESULT MALLOC

    result = (int**)malloc(n * sizeof(int *));
    for(i = 0; i < n; i++)
        result[i] = (int*)malloc(n * sizeof(int));

    gettimeofday(&tvBegin, NULL);

    matrix_multiply(n, matrix1, matrix2, result);


    gettimeofday(&tvEnd, NULL);
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);


    // deallocation of matrices
    for(i = 0; i < n; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);
    return 0;
}