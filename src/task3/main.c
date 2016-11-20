#include"graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>

pid_t* pids;
int runned_process;

void advanced_fork(int from, int to)
{
    pid_t instance_id = *(pids + from);
    pid_t unknown_id = fork();
    if (unknown_id != instance_id)
    {
        *(pids + to) = getpid();
    } 
}

void init(void)
{
    const size_t length = 10;
    pids = (pid_t*) malloc(10 * sizeof(pid_t));
    *(pids + 0) = getpid();
    for (size_t i = 1; i < length; i++)
    {
        *(pids + i) = 0;
        ++runned_process;
    }
}

void simple_mode_run(void)
{
    init();
    const run_process* runs = get_process_graph();
    const size_t length = 9;
    for (size_t i = 0; i < length; i++)
    {
        advanced_fork((runs + i)->from, (runs + i)->to);
    }
    srand(time(NULL));
    int output_number = rand() % 10;
    if (getpid() == *(pids + output_number))
    {
        printf("Number of runned processes is %d", runned_process);
    }
}

int main(void) 
{
    simple_mode_run();
    return 0;
}