#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE *f = fopen(path_file, "r");
    Matrix *struct_ptr = malloc(sizeof(Matrix));
    if (!f) {
        printf("Can't open file %s! Error - %s"
                , path_file, strerror(errno));
        free_matrix(struct_ptr);
        return NULL;
    }

    int fields_read = fscanf(f, "%zu %zu",
                             &struct_ptr->n_rows, &struct_ptr->n_cols);
    if (fields_read != 2) {
        fprintf(stderr, "Can't read "
                "n_rows/n_cols\n");
        fclose(f);
        free_matrix(struct_ptr);
        return NULL;
    }

    struct_ptr->matrix = (double**)malloc(sizeof(double*) *
                                               struct_ptr->n_rows);
    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        fclose(f);
        return NULL;
    }

    for (size_t i = 0; i < struct_ptr->n_rows; i++) {
        struct_ptr->matrix[i] = read_row_from_file(f, struct_ptr->n_cols);
        if (!struct_ptr->matrix[i]) {
            free_matrix(struct_ptr);
            return NULL;
        }
    }
    fclose(f);
    return struct_ptr;
}


double* read_row_from_file(FILE *f, size_t n_cols) {
    double *ret = (double *)malloc(n_cols * sizeof(double));
    if (!ret) {
        free(ret);
        return NULL;
    }
    for (size_t i = 0; i < n_cols; i++) {
        double val = 0;
        double read_objects =
                fscanf(f, "%lf", &val);
        if (read_objects != 1) {
            free(ret);
            fclose(f);
            return NULL;
        }
        ret[i] = val;
    }
    return ret;
}

int free_matrix(Matrix* str_matrix) {
    for (size_t i = 0; i < str_matrix->n_rows; ++i) {
        free(str_matrix->matrix[i]);
    }
    free(str_matrix->matrix);
    free(str_matrix);
    return 0;
}

Matrix* create_matrix(size_t n_rows, size_t n_cols) {
    Matrix *struct_ptr = malloc(sizeof(Matrix));
    if (!struct_ptr) {
        free(struct_ptr);
        return NULL;
    }
    struct_ptr->n_cols = n_cols;
    struct_ptr->n_rows = n_rows;

    struct_ptr->matrix = (double**)malloc(struct_ptr->n_rows
                                       * sizeof(double*));

    for (size_t i = 0; i < struct_ptr->n_rows; i++) {
        struct_ptr->matrix[i] = (double*)calloc(n_cols,
                                              sizeof(double));
        if (!struct_ptr->matrix) {
            free_matrix(struct_ptr);
            return NULL;
        }
    }
    return struct_ptr;
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (!matrix->n_rows) {
        return -1;
    }
    *rows = matrix->n_rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (!matrix->n_cols) {
        return -1;
    }
    *cols = matrix->n_cols;
    return 0;
}

int get_elem(const Matrix* matrix, int row, int col, double* val) {
    if ((int)matrix->n_rows <= row ||
            (int)matrix->n_cols <= col) {
        return -1;
    }
    *val = matrix->matrix[row][col];
    return 0;
}

int set_elem(Matrix* matrix, int row, int col, double val) {
    if ((int)matrix->n_rows <= row ||
        (int)matrix->n_cols <= col) {
        return -1;
    }
    matrix->matrix[row][col] = val;
    return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    Matrix *struct_ptr = create_matrix(matrix->n_rows,
                                       matrix->n_cols);
    if (!struct_ptr) {
        free(struct_ptr);
        return NULL;
    }
    for (size_t row = 0; row < struct_ptr->n_rows; row++) {
        for (size_t cols = 0; cols < struct_ptr->n_cols; cols++) {
            struct_ptr->matrix[row][cols] =
                    matrix->matrix[row][cols] * val;
        }
    }
    return struct_ptr;
}

Matrix* transp(const Matrix* matrix) {
    Matrix *struct_ptr = create_matrix(matrix->n_cols,
                                       matrix->n_rows);

    for (size_t row = 0; row < struct_ptr->n_rows; row++) {
        for (size_t col = 0; col < struct_ptr->n_cols; col++) {
            struct_ptr->matrix[row][col] =
                    matrix->matrix[col][row];
        }
    }
    return struct_ptr;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    Matrix *struct_ptr = create_matrix(l->n_rows,
                                       l->n_cols);
    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        return NULL;
    }
    for (size_t row = 0; row < l->n_rows; row++) {
        for (size_t col = 0; col < l->n_cols; col++) {
            struct_ptr->matrix[row][col] =
                    l->matrix[row][col] + r->matrix[row][col];
        }
    }
    return struct_ptr;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    Matrix *struct_ptr = create_matrix(r->n_rows,
                                       r->n_cols);
    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        return NULL;
    }

    for (size_t row = 0; row < r->n_rows; row++) {
        for (size_t col = 0; col < r->n_cols; col++) {
            struct_ptr->matrix[row][col] =
                    l->matrix[row][col] - r->matrix[row][col];
        }
    }
    return struct_ptr;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (l == NULL || r == NULL) {
        return NULL;
    }
    double temp;
    Matrix *struct_ptr = create_matrix(l->n_rows,
                                       r->n_cols);

    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        return NULL;
    }

    for (size_t row = 0; row < l->n_rows; ++row) {
        for (size_t col = 0; col < r->n_cols; ++col) {
            temp = 0;
            for (size_t arr = 0; arr < l->n_cols; ++arr) {
                temp += l->matrix[row][arr] * r->matrix[arr][col];
            }
            struct_ptr->matrix[row][col] = temp;
        }
    }

    return struct_ptr;
}

int det(const Matrix* matrix, double* val) {
    if (matrix->n_cols != matrix->n_rows) {
        return -1;
    }
    double temp;
    if (matrix->n_cols == 2 && 
    matrix->n_rows == 2) {
        *val = matrix->matrix[0][0] * matrix->matrix[1][1]
               - matrix->matrix[0][1] * matrix->matrix[1][0];
        return 0;
    }
    for (size_t i = 0; i < matrix->n_rows; i++) {
        Matrix* t = fork_matrix(matrix, i, 0);
        det(t, &temp);
        double temp_second = matrix->matrix[i][0] * temp;
        (i%2 == 0) ? (*val = *val + temp_second) :
        (*val = *val - temp_second);
        free_matrix(t);
    }
    return 0;
}

Matrix* fork_matrix(const Matrix* matrix, double t_row, double t_col ) {
    Matrix *struct_ptr = create_matrix(matrix->n_rows-1,
                                       matrix->n_cols-1);
    if (!struct_ptr) {
        free(struct_ptr);
        return NULL;
    }
    size_t i = 0;
    double *arr = malloc(sizeof(double)*
    struct_ptr->n_rows*struct_ptr->n_cols);

    if (!arr) {
        free_matrix(struct_ptr);
        free(arr);
        return NULL;
    }

    for (size_t row = 0; row < matrix->n_rows; row++) {
        for (size_t col = 0; col < matrix->n_cols; col++) {
            if (row != t_row && col != t_col) {
                arr[i] = matrix->matrix[row][col];
                i++;
            }
        }
    }
    i = 0;
    for (size_t row = 0; row < struct_ptr->n_rows; row++) {
        for (size_t col = 0; col < struct_ptr->n_cols; col++) {
            struct_ptr->matrix[row][col] = arr[i];
            i++;
        }
    }
    free(arr);
    return struct_ptr;
}

Matrix* inv(const Matrix* matrix) {
    double temp_det;
    double temp;

    det(matrix, &temp_det);
    if (temp_det == 0 ||
            matrix->n_cols != matrix->n_rows) {
        return NULL;
    }

    Matrix *struct_ptr = create_matrix(matrix->n_rows,
                                       matrix->n_cols);
    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        return NULL;
    }
    for (size_t row = 0; row < matrix->n_rows; row++) {
        for (size_t col = 0; col < matrix->n_cols; col++) {
            Matrix* t = fork_matrix(matrix, row, col);
            det(t, &temp);
            struct_ptr->matrix[row][col] = temp;
            free_matrix(t);
        }
    }
    Matrix* struct_ptr_one = adj(struct_ptr);
    Matrix* struct_ptr_two = transp(struct_ptr_one);
    for (size_t row = 0; row < matrix->n_rows; row++) {
        for (size_t col = 0; col < matrix->n_cols; col++) {
            struct_ptr_two->matrix[row][col] =
                    struct_ptr_two->matrix[row][col]/temp_det;
        }
    }
    free_matrix(struct_ptr);
    free_matrix(struct_ptr_one);
    return struct_ptr_two;
}

Matrix* adj(const Matrix* matrix) {
    if (matrix->n_cols != matrix->n_rows) {
        return NULL;
    }
    Matrix *struct_ptr = create_matrix(matrix->n_rows,
                                       matrix->n_cols);
    if (!struct_ptr->matrix) {
        free_matrix(struct_ptr);
        return NULL;
    }
    for (size_t row = 0; row < struct_ptr->n_rows; row++) {
        for (size_t col = 0; col < struct_ptr->n_cols; col++) {
            if ((row+col+2) % 2 != 0) {
                struct_ptr->matrix[row][col] = -matrix->matrix[row][col];
            } else {
                struct_ptr->matrix[row][col] = matrix->matrix[row][col];
            }
        }
    }
    return struct_ptr;
}




















