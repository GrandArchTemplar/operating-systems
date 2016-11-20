typedef struct 
{
    int from;
    int to;
} run_process;

/*
 * return process graph as edge list 
 */
const run_process* get_process_graph(void);