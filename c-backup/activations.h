#ifndef __ACTIVATIONS_HPP
#define __ACTIVATIONS_HPP

#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Original functions
double nn_tanh(double x);
double nn_sigmoid(double x);
double nn_fsigmoid(double x);
double nn_relu(double x);

// Derivatives
double nn_dtanh(double x);
double nn_dsigmoid(double x);
double nn_dfsigmoid(double x);
double nn_drelu(double x);

#ifdef __cplusplus
}
#endif

#endif