#include "concurrent_matrix.h"

concurrent_matrix create_concurrent_matrix(int column, int row)
{
    concurrent_matrix result;
    result.data = create_matrix(column, row);
    pthread_mutex_init(&result.lock, NULL);
    return result;
}

concurrent_matrix concurrent_matrix_sum(concurrent_matrix a, concurrent_matrix b)
{
    concurrent_matrix result;
    pthread_mutex_init(&result.lock, NULL);
    pthread_mutex_lock(&result.lock);
    result.data = matrix_sum(a.data, b.data);
    pthread_mutex_destroy(&a.lock);
    pthread_mutex_destroy(&b.lock);
    concurrent_destroy(a);
    concurrent_destroy(b);
    pthread_mutex_unlock(&result.lock);
    return result;
}

void concurrent_destroy(concurrent_matrix a)
{
    pthread_mutex_lock(&a.lock);
    destroy(a.data);
    pthread_mutex_unlock(&a.lock);
    pthread_mutex_destroy(&a.lock);
}
