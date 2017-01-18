#include <fcntl.h>

typedef struct
{
    int from;
    int to;
} branch;

typedef struct
{
    pid_t id;
    int num;
    int size;
    branch *branches;
} work;

/*
 * return process graph as edge list 
 */
const branch *get_process_graph(void);

int graph_size(void);

work personal_work(int num, const branch *graph);
