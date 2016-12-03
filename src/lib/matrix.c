#include "matrix.h"
#include <malloc.h>

matrix create_matrix(int column, int row)
{
    matrix result;
    result.data = (int *)malloc(sizeof(int) * column * row);
    result.column = column;
    result.row = row;
    return result;
}

matrix matrix_sum(matrix a, matrix b)
{
    matrix result = create_matrix(a.column, a.row);
    for (int i = 0; i < a.column * a.row; ++i)
    {
        *(result.data + i) = *(a.data + i) + *(b.data + i);
    }
}