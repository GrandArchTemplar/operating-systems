#include"graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>

const int BRANCH_COUNT = 9;
const int PROC_COUNT = 10;
const int READABLE = 0;
const int WRITEABLE = 1;
typedef struct
{
    pid_t id;
    int num;
    int size;
    branch* branches;
} work;

work personal_work(int num, const branch* graph)
{
    work result;
    int count = 0;
    for (int i = 0; i < BRANCH_COUNT; ++i)
    {
        if ((graph + i)->from == num)
        {
            ++count;
        }
    }
    result.branches = (branch*) malloc(count * sizeof(branch));
    result.size = count;
    result.num = num;
    return result;
}

void do_parent(int* pipes, pid_t child_id, int child_num, const branch* graph)
{
    //close read pipe
    close(*(pipes + READABLE));
    pid_t* buffer_id = (pid_t*) malloc(sizeof(pid_t));
    *(buffer_id) = child_id;
    int* buffer_num = (int*) malloc(sizeof(int));
    *(buffer_num) = child_num;
    *(buffer_num + 1) = BRANCH_COUNT;
    if (write(*(pipes + WRITEABLE), buffer_id, sizeof(pid_t)) + 
        write(*(pipes + WRITEABLE), buffer_num, sizeof(int) * 2) +
        write(*(pipes + WRITEABLE), graph, sizeof(branch) * BRANCH_COUNT) != 
        sizeof(pid_t) + sizeof(int) * 2 + sizeof(branch) * BRANCH_COUNT) 
    {
        perror("Error while writing information");
    }
    //close write pipe
    close(*(pipes + WRITEABLE));
    free(buffer_id);
    free(buffer_num);
}

void do_child(int* pipes) 
{
    //close write pipe
    close(*(pipes + WRITEABLE));
    pid_t* buffer_id = (pid_t*) malloc(sizeof(pid_t));
    int* buffer_num = (int*) malloc(sizeof(int));
    if (read(*(pipes + READABLE), buffer_id, sizeof(pid_t)) + 
        read(*(pipes + READABLE), buffer_num, sizeof(int)) != 
        sizeof(pid_t) + sizeof(int)) 
    {
        perror("Error while reading information");
    }
    pid_t child_id = *(buffer_id + 0);
    int child_num = *(buffer_num + 0);
    int branch_count = *(buffer_num + 1);
    branch* buffer_branch = (branch*) malloc(sizeof(branch) * branch_count);
    if (read(*(pipes + READABLE), buffer_branch, sizeof(branch) * branch_count) != 
        branch_count * sizeof(branch_count)) 
    {
        perror("Error while reading information");
    }
    free(buffer_id);
    free(buffer_num);
    work child_work = personal_work(child_num, buffer_branch);
    child_work.id = child_id;
}
/*
 *@executor must have all initialized fields
 */
void unlimited_fork_works(work executor)
{   
    pid_t child_id;
    int* pipes = (int*) malloc(2 * sizeof(int));
    for (int i = 0; i < executor.size; ++i)
    {
        if (getpid() == executor.id)
        {
            if (pipe(pipes) == -1)
            {
                perror("Error in creating pipes");
            }
            
            child_id = fork();
            switch (child_id)
            {
                case -1: perror("Error in forking");
                case 0: do_child(pipes);
            default:break;
                do_parent(pipes, child_id, (executor.branches + i)->to, executor.branches);
            }
        }
    }
    free(pipes);
}

void run(void) 
{
    const branch* graph = get_process_graph();
    
}

int main(void) 
{
    run();
    return 0;
}