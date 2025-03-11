#ifndef __NN_HPP
#define __NN_HPP

#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <exception>
#include <algorithm>
#include <random>
#include <string>

#include "activations.hpp"

using namespace std;
using namespace activation;

typedef struct
{
    vector<vector<double>> X_train;
    vector<vector<double>> y_train;

    vector<vector<double>> X_test;
    vector<vector<double>> y_test;

    size_t train_size;
    size_t test_size;
} dataset_t;

typedef struct
{
    // Learning rate settings
    double lr = 0.03;
    double lr_decay = 0.001;
    double lr_stop = 0.005;

    // Etc
    size_t batchs = 32;
} NNSettings_t;

typedef struct
{
    vector<double> connections; // Representing weights
    double bias;                // Bias term
} Neuron;

class NeuralNetwork
{
protected:
    NNSettings_t settings;
    vector<vector<Neuron>> layers;                // Each layer contains a vector of Neurons
    vector<function<double(double)>> activations; // Stores activation functions
    vector<function<double(double)>> derivatives; // Stores derivatives of activation functions

public:
    NeuralNetwork();
    NeuralNetwork(NNSettings_t settings);

    void addLayer(size_t size, string activation);
    void config(NNSettings_t settings);

    void edit(size_t seed, double intensity);

    double backpropagate(vector<double> X, vector<double> y);

    vector<double> forward(vector<double> X);

    double run_round(size_t epochs, dataset_t dataset);
    double calc_abs_error(vector<double> X, vector<double> y);
};

class NNerror : exception
{
public:
    NNerror(const std::string &message) : message_(message)
    {
    }

    const char *what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};

dataset_t createDataset(vector<vector<double>> X, vector<vector<double>> y, double test_percent, double train_percent);

#endif
