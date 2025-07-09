#ifndef __NNERRORS_H__
#define __NNERRORS_H__

#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum ErrorMeasure
{
    ERROR_MEASURE_MAE = 1,   // Mean Absolute Error
    ERROR_MEASURE_MSE = 0,  // Mean Square Error
    ERROR_MEASURE_BCE = 2,  // Binary Cross-Entropy
    ERROR_MEASURE_CCE = 3  // Categorical Cross-Entropy
} ErrorMeasure;

double calc_mae(double* predict, double* real);
double calc_mse(double* predict, double* real);
double calc_bce(double* predict, int* real);
double calc_cce(double* predict, int* real, int num_classes);

#ifdef __cplusplus
}
#endif

#endif // __NNERRORS_H__
