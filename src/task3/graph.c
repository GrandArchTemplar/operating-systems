#include<stdlib.h>
#include"graph.h"

void set_edge(branch* list, const int number, const int from, const int to)
{
    (list + number)->from = from;
    (list + number)->to = to;
    return;
}

const branch* get_process_graph(void)
{
    branch* list = (branch*) malloc(9 * sizeof(branch));
    set_edge(list, 0, 0, 1);
    set_edge(list, 1, 1, 2);
    set_edge(list, 2, 1, 3);
    set_edge(list, 3, 2, 4);
    set_edge(list, 4, 3, 5);
    set_edge(list, 5, 4, 6);
    set_edge(list, 6, 6, 7);
    set_edge(list, 7, 7, 8);
    set_edge(list, 8, 8, 9);
    return list;
}

