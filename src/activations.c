#include "activations.h"

// Activation functions
double nn_tanh(double x)
{
    return tanh(x);
}

double nn_sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double nn_fsigmoid(double x)
{
    return x / (1 + fabs(x));
}

double nn_relu(double x)
{
    return x * (x > 0);
}

// Derivative of activation functions
double nn_dtanh(double x)
{
    return pow(1 / cosh(x), 2);
}

double nn_dsigmoid(double x)
{
    double sig = nn_sigmoid(x);
    return sig * (1 - sig);
}

double nn_dfsigmoid(double x)
{
    return 1 / pow((fabs(x) + 1), 2);
}

double nn_drelu(double x)
{
    return (x > 0) ? 1.0 : 0.0;
}
