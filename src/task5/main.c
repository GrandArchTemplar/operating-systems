#include "graph.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int value;
} shell;

const branch *GRAPH;
int counter = 0;
pthread_mutex_t lock;
shell arg;
int ready = 1;
int used[10];
int values[100][100];

void *do_work(void* thread_num)
{
    pthread_mutex_lock(&lock);
    shell* u_t_id = (shell*) thread_num;
    int tid = u_t_id->value;
    if (used[tid]) {
        return NULL;
    }
    used[tid] = 1;
    ++counter;
    printf("tid: %d\n", tid);
    if (counter == graph_size() + 1)
    {
        printf("I'm last thread with number %d!\n",  tid);
        ready = 0;
        return NULL;
    }
    pthread_mutex_unlock(&lock);
    work thread_work = personal_work(tid, GRAPH);
    for (int i = 0; i < thread_work.size; ++i)
    {
        //pthread_mutex_lock(&lock);
        pthread_t thread_id;
        pthread_mutex_lock(&lock);
        arg.value = (thread_work.branches + i)->to;
        int status = pthread_create(&thread_id, NULL, do_work, (void*) &arg);
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
    usleep(30000);
}

void run(void)
{
    for (int i = 0; i < 10; ++i)
    {
        used[i] = 0;
    }
    shell* zero;
    zero->value = 0;
    do_work(zero);
}

int main()
{
    GRAPH = get_process_graph();
    run();
    usleep(400000);
}
