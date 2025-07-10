#ifndef __ACTIVATIONS_HPP
#define __ACTIVATIONS_HPP

#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Original functions
extern double nn_tanh(double x);
extern double nn_sigmoid(double x);
extern double nn_fsigmoid(double x);
extern double nn_relu(double x);

// Derivatives
extern double nn_dtanh(double x);
extern double nn_dsigmoid(double x);
extern double nn_dfsigmoid(double x);
extern double nn_drelu(double x);

#ifdef __cplusplus
}
#endif

#endif