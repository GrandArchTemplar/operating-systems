#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

const int READABLE = 0;
const int WRITEABLE = 1;
int gen_ready;
int sum_ready;
int print_ready;
int* gen_to_sum;
int* sum_to_print;
int* print_to_gen;
matrix a, b, result;
int* data;

void connector(int *pipes, pid_t child_id, int child_num, int* data)
{

    close(*(pipes + READABLE));
    pid_t *buffer_id = (pid_t *)malloc(sizeof(pid_t));
    *(buffer_id + 0) = child_id;
    int *buffer_num = (int *)malloc(sizeof(int) * 2);
    *(buffer_num + 0) = child_num;
    *(buffer_num + 1) = 4;
    int w1 = write(*(pipes + WRITEABLE), buffer_id, sizeof(pid_t));
    int w2 = write(*(pipes + WRITEABLE), buffer_num, sizeof(int) * 2);
    int w3 = write(*(pipes + WRITEABLE), data, sizeof(int) * 4);
    if (w1 + w2 + w3 != sizeof(pid_t) + sizeof(int) * 2 + sizeof(int) * 4)
    {
        perror("Error while writing information\n");
        exit(-1);
    }
    close(pipes[1]);
    free(buffer_id);
    buffer_id = NULL;
    free(buffer_num);
    buffer_num = NULL;
}

void *generator()
{
    while (1)
    {
        srand(time(NULL));
        while(!gen_ready)
        {
            usleep(1000);
        }
        int n = 3; //rand() % 10 + 10;
        int m = 2; //rand() % 10 + 10;
        a = create_matrix(m, n);
        b = create_matrix(m, n);
        for (int i = 0; i < n * m; ++i)
        {
            a.data[i] = rand() % 5 * 100;
            b.data[i] = rand() % 5 * 100;
        }
        gen_ready = 0;
        sum_ready = 1;
        printf("PID_GENERATOR:%d\n", getpid());
    }
}

void *summator()
{
    while (1)
    {
        while(!sum_ready)
        {
            usleep(1000);
        }
        result = create_matrix(a.column, a.row);
        for (int i = 0; i < a.row * a.column; ++i)
        {
            result.data[i] = a.data[i] + b.data[i];
        }
        result.column = a.column;
        result.row = a.row;
        sum_ready = 0;
        print_ready = 1;
        printf("PID_CALCULATOR:%d\n", getpid());
    }
}

void *printer()
{
    while(1)
    {
        while(!print_ready)
        {
            usleep(1000);
        }
        printf("input1:\n");
        for (int i = 0; i < result.row; ++i)
        {
            for(int j = 0; j < result.column; ++j)
            {
                printf("%d ", a.data[i * result.column + j]);
            }
            printf("\n");
        }
        printf("input2:\n");
        for (int i = 0; i < result.row; ++i)
        {
            for(int j = 0; j < result.column; ++j)
            {
                printf("%d ", b.data[i * result.column + j]);
            }
            printf("\n");
        }
        printf("output:\n");
        for (int i = 0; i < result.row; ++i)
        {
            for(int j = 0; j < result.column; ++j)
            {
                printf("%d ", result.data[i * result.column + j]);
            }
            printf("\n");
        }
        printf("PID_PRINTER:%d\n", getpid());
        print_ready = 0;
        gen_ready = 1;
        usleep(1000000);
        connector(gen_to_sum, getpid(), 4, data);
    }
}

void refresher(int *pipes)
{
    close(*(pipes + WRITEABLE));
    pid_t *buffer_id = (pid_t *)malloc(sizeof(pid_t));
    int *buffer_num = (int *)malloc(sizeof(int) * 2);
    int r1 = read(*(pipes + READABLE), buffer_id, sizeof(pid_t));
    int r2 = read(*(pipes + READABLE), buffer_num, sizeof(int) * 2);
    if (r1 + r2 != sizeof(pid_t) + sizeof(int) * 2)
    {
        perror("Error while reading information. Stage 1\n");
        exit(-1);
    }
    pid_t child_id = *(buffer_id + 0);
    int child_num = *(buffer_num + 0);
    int *data= (int *)malloc(sizeof(int) * 4);
    int r3 = read(*(pipes + READABLE), data, sizeof(int) * 4);
    if (r3 != sizeof(int) * 4)
    {
        perror("Error while reading information. Stage 2\n");
        exit(-1);
    }
    free(data);
    data = NULL;
    free(buffer_id);
    buffer_id = NULL;
    free(buffer_num);
    buffer_num = NULL;
    close(*(pipes + READABLE));
}


int main(void) {
    gen_to_sum = (int*) malloc(sizeof(int) * 2);
    return 0;
}
