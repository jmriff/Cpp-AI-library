#include "nnerrors.h"

double calc_mse(double* predict, double* real)
{
    double error = 0.00;

    for (int i = 0; i < (int)(sizeof(predict) / sizeof(double)); i++)
    {
        double diff = predict[i] - real[i];
        error = (i == 0) ? diff * diff : (error + diff * diff) / 2;
    }

    return error;
}

double calc_mae(double* predict, double* real)
{
    double error = 0.00;

    for (int i = 0; i < (int)(sizeof(predict) / sizeof(double)); i++)
    {
        error = (i == 0) ? fabs(predict[i] - real[i]) : (error + fabs(predict[i] - real[i])) / 2;
    }

    return error;
}

/*
double calc_huber(double* predict, double* real, double delta)
{
    double error = predict - real;
    double terror = 0.00; // Total Error
    
    for (int i = 1; i < sizeof(predict) / sizeof(double); i++)
    {
        double abs_error = abs(error);

        if (abs_error <= delta)
        {
            error = (error + error * error / 2) / 2;
        }
        else
        {
            error = (error + delta * abs_error - delta * delta / 2) / 2;
        }
    }
}*/