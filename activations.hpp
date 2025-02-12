#ifndef __ACTIVATIONS_HPP
#define __ACTIVATIONS_HPP

#include <cmath>

using namespace std;

namespace activation
{
    // Original functions
    double tanh(double x);
    double sigmoid(double x);
    double fsigmoid(double x);
    double relu(double x);

    // Derivatives
    double dtanh(double x);
    double dsigmoid(double x);
    double dfsigmoid(double x);
    double drelu(double x);
};

#endif