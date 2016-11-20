#include<stdlib.h>
#include"graph.h"

void set_edge(run_process* list, const int number, const int from, const int to)
{
    (list + number)->from = from;
    (list + number)->to = to;
}

const run_process* get_process_graph(void)
{
    run_process* list = (run_process*) malloc(9 * sizeof(run_process));
    set_edge(list, 0, 1, 2);
    set_edge(list, 1, 2, 3);
    set_edge(list, 2, 2, 4);
    set_edge(list, 3, 3, 5);
    set_edge(list, 4, 4, 6);
    set_edge(list, 5, 4, 7);
    set_edge(list, 6, 7, 8);
    set_edge(list, 7, 8, 9);
    set_edge(list, 8, 9, 10);
    return list;
}

