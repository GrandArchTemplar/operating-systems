#include "matrix.h"
#include <pthread.h>

typedef struct {
    matrix data;
    pthread_mutex_t lock;
} concurrent_matrix;

concurrent_matrix create_concurrent_matrix(int column, int row);

concurrent_matrix concurrent_matrix_sum(concurrent_matrix a, concurrent_matrix b);

void concurrent_destroy(concurrent_matrix a);
