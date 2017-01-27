typedef struct
{
    int *data;
    int column;
    int row;
} matrix;

matrix create_matrix(int column, int row);

matrix matrix_sum(matrix a, matrix b);

void destroy(matrix a);
