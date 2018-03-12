#include <stdio.h>
#include <stdlib.h>


#include "matrix.h"

int main(void) {
    Matrix *mymatrix_first = create_matrix_from_file("data.txt");
    Matrix *mymatrix_second = create_matrix(4, 4);

    size_t rows, cols;
    double val = 0;
    det(mymatrix_first, &val);
    printf("%lf",val);

    get_rows(mymatrix_first, &rows);
    get_cols(mymatrix_first, &cols);
    set_elem(mymatrix_first, 2, 2, 45);
    get_elem(mymatrix_first, 2, 2, &val);
    Matrix *mymatrix_1 = mul_scalar(mymatrix_first, 5);
    Matrix *mymatrix_2 = mul(mymatrix_first, mymatrix_first);
    Matrix *mymatrix_3 = inv(mymatrix_first);
    Matrix *mymatrix_4 = sum(mymatrix_first, mymatrix_second);
    Matrix *mymatrix_5 = sub(mymatrix_first, mymatrix_second);
    

    free_matrix(mymatrix_first);
    free_matrix(mymatrix_1);
    free_matrix(mymatrix_second);
    free_matrix(mymatrix_2);
    free_matrix(mymatrix_3);
    free_matrix(mymatrix_4);
    free_matrix(mymatrix_5);
    return 0;
}

