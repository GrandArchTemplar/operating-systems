#include "concurrent_matrix.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int gen_ready;
int sum_ready;
int print_ready;
int gen_id;
int print_id;
int sum_id;
pthread_mutex_t lock;
concurrent_matrix a, b, result;
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
        a = create_concurrent_matrix(m, n);
        b = create_concurrent_matrix(m, n);
        pthread_mutex_lock(&lock);
        for (int i = 0; i < n * m; ++i)
        {
            a.data.data[i] = rand() % 5 * 100;
            b.data.data[i] = rand() % 5 * 100;
        }
        gen_ready = 0;
        sum_ready = 1;
        printf("PID_GENERATOR:%d\n", gen_id);
        pthread_mutex_unlock(&lock);
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
        pthread_mutex_lock(&lock);
        result = create_concurrent_matrix(a.data.column, a.data.row);
        for (int i = 0; i < a.data.row * a.data.column; ++i)
        {
            result.data.data[i] = a.data.data[i] + b.data.data[i];
        }
        result.data.column = a.data.column;
        result.data.row = a.data.row;
        sum_ready = 0;
        print_ready = 1;
        printf("PID_CALCULATOR:%d\n", sum_id);
        pthread_mutex_unlock(&lock);
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
        pthread_mutex_lock(&lock);
        printf("input1:\n");
        for (int i = 0; i < result.data.row; ++i)
        {
            for(int j = 0; j < result.data.column; ++j)
            {
                printf("%d ", a.data.data[i * result.data.column + j]);
            }
            printf("\n");
        }
        printf("input2:\n");
        for (int i = 0; i < result.data.row; ++i)
        {
            for(int j = 0; j < result.data.column; ++j)
            {
                printf("%d ", b.data.data[i * result.data.column + j]);
            }
            printf("\n");
        }
        printf("output:\n");
        for (int i = 0; i < result.data.row; ++i)
        {
            for(int j = 0; j < result.data.column; ++j)
            {
                printf("%d ", result.data.data[i * result.data.column + j]);
            }
            printf("\n");
        }
        printf("PID_PRINTER:%d\n", print_id);
        print_ready = 0;
        gen_ready = 1;
        usleep(1000000);
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    srand(time(NULL));
    gen_id = rand() % 250 + 9800;
    for (int i = 0; i < rand() % 5; ++i)
        gen_id++;
    sum_id = rand() % 250 + 9700;
    print_id = rand() % 450 + 9800;
    for (int i = 0; i < rand() % 5; ++i)
        print_id++;
    for (int i = 0; i < rand() % 5; ++i)
        sum_id++;
    gen_ready = 1;
    sum_ready = 0;
    print_ready = 0;
    pthread_t gen;
    pthread_t sum;
    pthread_t print;
    char c;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&gen, NULL, generator, NULL);
    pthread_create(&sum, NULL, summator, NULL);
    pthread_create(&print, NULL, printer, NULL);
    //usleep(40000);
    while (c != 's')
    {
        scanf("%c", &c);
    }

}


