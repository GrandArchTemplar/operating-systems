#include <stdlib.h>
#include "graph.h"

void set_edge(branch *list, const int number, const int from, const int to)
{
    (list + number)->from = from;
    (list + number)->to = to;
    return;
}

const branch *get_process_graph(void)
{
    branch *list = (branch *)malloc(9 * sizeof(branch));
    set_edge(list, 0, 0, 1);
    set_edge(list, 1, 1, 3);
    set_edge(list, 2, 1, 2);
    set_edge(list, 3, 1, 4);
    set_edge(list, 4, 4, 5);
    set_edge(list, 5, 4, 6);
    set_edge(list, 6, 5, 7);
    set_edge(list, 7, 6, 8);
    set_edge(list, 8, 1, 9);
    return list;
}

int graph_size(void)
{
    return 9;
}

work personal_work(int num, const branch *graph)
{
    work result;
    int BRANCH_COUNT = graph_size();
    int count = 0;
    for (int i = 0; i < BRANCH_COUNT; ++i)
    {
        count += ((graph + i)->from == num) ? 1 : 0;
    }
    result.branches = (branch *)malloc(count * sizeof(branch));
    int j = 0;
    const branch *t = graph;
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
