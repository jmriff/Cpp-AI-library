#include "nnerrors.h"

/*
 * calc_mae
 *
 * -------------------------
 * 
 * Calculate the Mean Absolute Error (MAE) between predicted and real values.
 * 
 * @param predict: Array of predicted values
 * @param real: Array of actual values
 * 
 * Return: The MAE loss
 */
double calc_mae(double* predict, double* real)
{
    double total_error = 0.0f;
    int samples        = sizeof(predict) / sizeof(double);

    // Check if samples are valid
    if (samples == 0) return -1;
    if (samples != sizeof(real) / sizeof(double)) return -1;

    for (int i = 0; i < samples; ++i)
    {
        total_error += fabs(predict[i] - real[i]);
    }

    return total_error / samples;
}

/*
 * calc_mse
 *
 * -------------------------
 * 
 * Calculate the Mean Squared Error (MSE) loss between predicted and real values.
 * 
 * @param predict: Array of predicted values
 * @param real: Array of actual values
 * 
 * Return: The MSE loss
 */
double calc_mse(double* predict, double* real)
{
    double total_error = 0.0f;
    int samples        = sizeof(predict) / sizeof(double);

    // Check if samples are valid
    if (samples == 0) return -1;
    if (samples != sizeof(real) / sizeof(double)) return -1;

    for (int i = 0; i < samples; ++i)
    {
        float error = predict[i] - real[i];
        total_error += error * error;
    }

    return total_error / samples;
}

/*
 * calc_bce
 *
 * -------------------------
 * 
 * Calculate the Binary Cross-Entropy (BCE) loss between predicted and real values.
 * 
 * @param predict: Array of predicted probabilities (between 0 and 1)
 * @param real: Array of actual binary values (0 or 1)
 * 
 * Return: The BCE loss
 */
double calc_bce(double* predict, int* real)
{
    double total_error = 0.0f;
    int samples        = sizeof(predict) / sizeof(double);

    // Check if samples are valid
    if (samples == 0) return -1;
    if (samples != sizeof(real) / sizeof(double)) return -1;

    for (int i = 0; i < samples; ++i)
    {
        double prediction = predict[i];
        int target = real[i];
        total_error += -(target * logf(prediction) + (1 - target) * logf(1 - prediction));
    }

    return total_error / samples;
}

/*
 * calc_cce
 * -------------------------
 * 
 * Calculate the Categorical Cross-Entropy (CCE) loss for multi-class classification.
 * 
 * @param predict: Array of predicted probabilities for each class (shape: [samples, num_classes])
 * @param real: Array of actual class labels (one-hot encoded)
 * @param num_classes: Number of classes in the classification problem
 * 
 * Return: The CCE loss
 */
double calc_cce(double* predict, int* real, int num_classes)
{
    double total_error = 0.0f;
    int samples        = sizeof(predict) / sizeof(double);

    // Check if samples and classes are valid
    if (samples == 0) return -1;
    if (num_classes <= 0) return -1;
    if (samples != sizeof(real) / sizeof(double)) return -1;

    for (int i = 0; i < samples; ++i)
    {
        for (int j = 0; j < num_classes; ++j)
        {
            double prediction = predict[i * num_classes + j];
            int target = (j == real[i]); // One-hot encoded target
            total_error += -(target * logf(prediction));
        }
    }

    return total_error / samples;
}