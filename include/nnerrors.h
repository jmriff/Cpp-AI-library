#ifndef __NNERRORS_H__
#define __NNERRORS_H__

#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

// MSE (Mean Square Error)
// predit: Model predition for output
// real:   Actual anwser to problem
double calc_mse(double* predict, double* real);

// MAE (Mean Absolute Error)
// predit: Model predition for output
// real:   Actual anwser to problem
double calc_mae(double* predict, double* real);

#ifdef __cplusplus
}
#endif

#endif // __NNERRORS_H__
