#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stddef.h>
#include <stdio.h>


typedef struct Matrix {
    double **matrix;
    size_t n_rows;
    size_t n_cols;
} Matrix;

Matrix* create_matrix_from_file(const char* path_file);
Matrix* create_matrix(size_t rows, size_t cols);
int free_matrix(Matrix* matrix);


double *read_row_from_file(FILE *f, size_t n_cols);
Matrix* fork_matrix(const Matrix* matrix, double t_row, double t_col);

//  Basic operations
int get_rows(const Matrix* matrix, size_t* rows);
int get_cols(const Matrix* matrix, size_t* cols);
int get_elem(const Matrix* matrix, int row, int col, double* val);
int set_elem(Matrix* matrix, int row, int col, double val);

//  Math operations
Matrix* mul_scalar(const Matrix* matrix, double val);
Matrix* transp(const Matrix* matrix);
Matrix* sum(const Matrix* l, const Matrix* r);
Matrix* sub(const Matrix* l, const Matrix* r);
Matrix* mul(const Matrix* l, const Matrix* r);

//  Extra operations
int det(const Matrix* matrix, double* val);
Matrix* adj(const Matrix* matrix);
Matrix* inv(const Matrix* matrix);

#endif  // PROJECT_INCLUDE_MATRIX_H_
