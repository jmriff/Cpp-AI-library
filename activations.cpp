#include "activations.hpp"
#include <cmath>

// Activation functions
double activation::tanh(double x)
{
    return std::tanh(x);
}

double activation::sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double activation::fsigmoid(double x)
{
    return x / (1 + abs(x));
}

double activation::relu(double x)
{
    return x * (x > 0);
}

// Derivative of activation functions
double activation::dtanh(double x)
{
    return pow(1 / cosh(x), 2);
}

double activation::dsigmoid(double x)
{
    double sig = sigmoid(x);  // Apply the sigmoid function
    return sig * (1 - sig);   // Derivative of sigmoid: sigma(x) * (1 - sigma(x))
}

double activation::dfsigmoid(double x)
{
    return 1 / pow((abs(x) + 1), 2);
}

double activation::drelu(double x)
{
    return (x > 0) ? 1.0 : 0.0;  // Derivative of ReLU: 1 if x > 0, 0 if x <= 0
}
