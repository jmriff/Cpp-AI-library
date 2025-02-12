#ifndef __AI_HPP
#define __AI_HPP

#include <vector>
#include <iostream>

#include <thread>
#include <mutex>

#include "nn.hpp"

using namespace std;

extern mutex mtx;

typedef struct
{
    size_t neuralnetworks = 10000;

    size_t rounds = 100;
    size_t epochs_per_round = 10;
    
    size_t max_cores = 4;

    double max_drop = 0.02;
} TrainSettings_t;

class AI
{
    vector<NeuralNetwork> nns;

    AISettings_t settings;

    NeuralNetwork backup, nn;

    double last_error = 0.00;
public:
    AI(AISettings_t settings);

    void train(TrainSettings_t settings, dataset_t dataset);

    double test(dataset_t dataset);

    void addLayer(size_t size, string activation);

    vector<double> forward(vector<double> input);
};

#endif