#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

struct args_strct{
    int rows_to_calc;
    int thread_number;
    int dim;
};

int** matrix1;
int** matrix2;
int** result;
pthread_t* threads;
struct args_strct* argsStrct;

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
    int i,j,k;
    int sum = 0;
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


void* thread_matrix_multiply(void* arguments){
    int i,j,k;
    struct args_strct* argsStrct = arguments;
    int start_row = (argsStrct->thread_number) * (argsStrct->rows_to_calc);
    for (i = start_row; i < start_row+argsStrct->rows_to_calc; i++)
        for (j = 0;j < argsStrct->dim;j++)
            for (k = 0; k < argsStrct->dim; k++)
                result[i][j] += (matrix1[i][k] * matrix2[k][j]);

    return NULL;
}

int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}



int main() {
    int i,j,n,t;
    char c;
    srand((unsigned int) time(NULL));
        // getting dimensions of matrices
    do
    {
        printf("\nEnter dimensions of matrices: ");

    } while ((scanf("%d%c", &n, &c)!=2 || c!='\n') && clean_stdin());
        // getting number of threads
    do
    {
        printf("\nEnter number of threads: ");

    } while ((scanf("%d%c", &t, &c)!=2 || c!='\n') && clean_stdin());


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

    // printing matrices
//    printf("\nmatrix1\n");
//    for(i = 0; i < n; i++) {
//        for (j = 0; j < n; j++)
//            printf("%d ", matrix1[i][j]);
//        printf("\n");
//    }
//    printf("\nmatrix2\n");
//    for(i = 0; i < n; i++) {
//        for (j = 0; j < n; j++)
//            printf("%d ", matrix2[i][j]);
//        printf("\n");
//    }
//    printf("\n");

    // creating threads
    gettimeofday(&tvBegin, NULL);
    threads = (pthread_t*)malloc(t * sizeof(pthread_t));
    argsStrct = (struct args_strct*)malloc(t * sizeof(struct args_strct));
    for(i = 0; i < t; i++){
        argsStrct[i].rows_to_calc = n/t;
        argsStrct[i].dim = n;
        argsStrct[i].thread_number = i;
        pthread_create(&threads[i],NULL, thread_matrix_multiply, (void*)&argsStrct[i]);
    }

    // wait for threads to finish their job
    for(i = 0; i < t; i++){
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&tvEnd, NULL);
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);


    // print result matrix
//    for(i = 0; i < n; i++) {
//        for (j = 0; j < n; j++)
//            printf("%d ", result[i][j]);
//        printf("\n");
//    }


    // deallocation of matrices and threads
    for(i = 0; i < n; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(threads);
    free(argsStrct);
    free(matrix1);
    free(matrix2);
    free(result);
    return 0;
}