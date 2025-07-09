#ifndef __NN_HPP
#define __NN_HPP

// Math
#include <vector>
#include <cmath>

// Basic libs
#include <cstdio>
#include <cstdlib>

// Errors
#include <exception>

// Algorithms
#include <algorithm>
#include <random>

// Strings and output
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "activations.h"
#include "nnerrors.h"
#include "nnmemory.hpp"

typedef struct
{
    std::vector<std::vector<double>> X_train;
    std::vector<std::vector<double>> y_train;

    std::vector<std::vector<double>> X_test;
    std::vector<std::vector<double>> y_test;

    int train_size;
    int test_size;
} dataset_t;

typedef struct
{
    // Learning rate settings
    double lr = 0.03;
    double lr_decay = 0.001;
    double lr_stop = 0.005;

    // Etc
    int batchSize = 32;
} TrainSettings_t;

typedef struct
{
    std::vector<double> connections; // Representing weights
    double bias;                     // Bias term
} Neuron;

class NeuralNetwork
{
    std::vector<std::vector<Neuron>> layers; // Each layer contains a vector of Neurons
    std::vector<double(*)(double)> activations; // Stores activation functions
    std::vector<double(*)(double)> derivatives; // Stores derivatives of activation functions

    ErrorMeasure merr;

    MemoryPool NNPool; // Memory pool for efficient memory management

    bool bar;

public:
    NeuralNetwork(ErrorMeasure merr=ERROR_MEASURE_MSE, bool progressbar=true);
    ~NeuralNetwork();

    void addLayer(int size, const char* activation);
    void config(ErrorMeasure merr);

    double backpropagate(std::vector<double> X, std::vector<double> y, TrainSettings_t* tset);
    double train(int epochs, dataset_t* pDataset, TrainSettings_t* tset);
    std::vector<double> forward(std::vector<double> X);

    double calc_error(dataset_t* pDataset);
    double calc_error(std::vector<double> predict, std::vector<double> real);
};

class NNerror : std::exception
{
public:
    NNerror(const std::string &message) : message_(message)
    {
    }

    const char* what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};

dataset_t* createDataset(std::vector<std::vector<double>> X, std::vector<std::vector<double>> y, double test_percent, double train_percent);

#endif
