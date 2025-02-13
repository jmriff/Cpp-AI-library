#ifndef __AI_HPP
#define __AI_HPP

#include <vector>
#include <iostream>
#include <iomanip>

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
    vector<thread> threads;
    AISettings_t settings;
    NeuralNetwork backup, nn, best_model;

    double last_error = 0.00;
    double best_error = 0.00;

public:
    AI(AISettings_t settings);

    void train(TrainSettings_t settings, dataset_t dataset);
    void core_round(dataset_t dataset, TrainSettings_t tsettings, size_t start_idx, size_t end_idx, size_t i, vector<double>& local_best_errors, vector<size_t>& local_indices);

    double test(dataset_t dataset);
    double test(NeuralNetwork nn, dataset_t dataset);
    void addLayer(size_t size, string activation);
    vector<double> forward(vector<double> input);
};

#endif
