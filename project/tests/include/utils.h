#ifndef PROJECT_TESTS_INCLUDE_UTILS_H_
#define PROJECT_TESTS_INCLUDE_UTILS_H_

#include <string.h>

#include "matrix.h"


char *path_join(const char *, ...);

Matrix *create_identity_matrix(int);
void print_matrix(const Matrix *);
void assert_matrix_equal(Matrix *, Matrix *);

int double_equals(double, double, double);


#endif  // PROJECT_TESTS_INCLUDE_UTILS_H_
