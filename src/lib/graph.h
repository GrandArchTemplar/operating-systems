typedef struct
{
    int from;
    int to;
} branch;

/*
 * return process graph as edge list 
 */
const branch *get_process_graph(void);