#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

const int BRANCH_COUNT = 9;
const int PROC_COUNT = 10;
const int READABLE = 0;
const int WRITEABLE = 1;

int d1, d2 = 0;

typedef struct
{
    pid_t id;
    int num;
    int size;
    branch *branches;
} work;

work personal_work(int num, const branch *graph)
{
    work result;
    int count = 0;
    for (int i = 0; i < BRANCH_COUNT; ++i)
    {
        count += ((graph + i)->from == num) ? 1 : 0;
    }
    result.branches = (branch *)malloc(count * sizeof(branch));
    int j = 0;
    const branch* t = graph;
    for (int i = 0; i < BRANCH_COUNT; ++i)
    {
        if (t->from == num)
        {
            *(result.branches + j++) = *t;
        }
        t++;
    }
    result.size = count;
    result.num = num;
    return result;
}

void do_parent(int *pipes, pid_t child_id, int child_num, const branch *graph)
{

    close(pipes[0]);
    pid_t buffer_id[1];
    buffer_id[0] = child_id;
    int buffer_num[2];
    buffer_num[0] = child_num;
    buffer_num[1] = BRANCH_COUNT;
    int w1 = write(pipes[1], buffer_id, sizeof(pid_t));
    int w2 = write(pipes[1], buffer_num, sizeof(int) * 2);
    int w3 = write(pipes[1], graph, sizeof(branch) * BRANCH_COUNT);
    if (w1 + w2 + w3 != sizeof(pid_t) + sizeof(int) * 2 + sizeof(branch) * BRANCH_COUNT)
    {
        perror("Error while writing information\n");
        exit(-1);
    }
    close(pipes[1]);
}

void unlimited_fork_works(work executor);

void do_child(int *pipes)
{
    close(pipes[1]);
    pid_t buffer_id[1];
    int buffer_num[2];
    int r1 = read(pipes[0], buffer_id, sizeof(pid_t));
    int r2 = read(pipes[0], buffer_num, sizeof(int) * 2);
    if (r1 + r2 != sizeof(pid_t) + sizeof(int) * 2)
    {
        perror("Error while reading information\n");
        printf("%d %d\n", r1, r2);
        exit(-1);
    }
    pid_t child_id = buffer_id[0];
    int child_num = buffer_num[0];
    int branch_count = buffer_num[1];
    branch *buffer_branch = (branch *)malloc(sizeof(branch) * branch_count);
    int r3 = read(pipes[0], buffer_branch, sizeof(branch) * branch_count);
    if (r3 != sizeof(branch) * branch_count)
    {
        perror("Error while reading information\n");
        exit(-1);
    }
    work child_work = personal_work(child_num, buffer_branch);
    child_work.id = child_id;
    free(buffer_branch);
    close(pipes[0]);
    printf("I'm god %d %d\n", child_id, child_num);
    unlimited_fork_works(child_work);
}
/*
 *@executor must have all initialized fields
 */
void unlimited_fork_works(work executor)
{
    pid_t child_id;
    int pipes[2];
    for (int i = 0; i < executor.size; ++i)
    {
        if (getpid() == executor.id)
        {
            if (pipe(pipes) == -1)
            {
                perror("Error in creating pipes");
                exit(-1);
            }

            child_id = fork();
            switch (child_id)
            {
            case -1:
                perror("Error in forking");
                exit(-1);
                break;
            case 0:
                do_child(pipes);
                break;
            default:
                do_parent(pipes, child_id, (executor.branches + i)->to, get_process_graph());
                break;
            }
        }
    }
}

void run(void)
{
    const branch *graph = get_process_graph();
    work root_work = personal_work(0, graph);
    root_work.id = getpid();
    unlimited_fork_works(root_work);
    if (getpid() == root_work.id)
    {
        printf("%s", "IZI\n");
    }
}

int main(void)
{
    run();
    return 0;
}