#include "AI.hpp"

mutex mtx;

AI::AI(AISettings_t settings) : settings(settings), best_error(std::numeric_limits<double>::max()), last_error(std::numeric_limits<double>::max()) {}

void AI::train(TrainSettings_t tsettings, dataset_t dataset)
{
    // Initialize the networks
    nns.resize(tsettings.neuralnetworks);
    for (size_t i = 0; i < tsettings.neuralnetworks; i++)
    {
        nns[i] = nn;             // Clone the base network
        nns[i].config(settings); // Configure network with settings
    }
    backup = nn;
    nn.config(settings); // Configure base network

    // Initialize best_error
    cout << "Initial best_error: " << best_error << endl;

    size_t index = 0;

    // Iterate over each round of training
    for (size_t round = 0; round < tsettings.rounds; round++)
    {
        size_t chunk_size = tsettings.neuralnetworks / tsettings.max_cores;

        // Clear previous threads
        threads.clear();

        // Initialize thread-local best_error
        vector<double> local_best_errors(tsettings.max_cores, std::numeric_limits<double>::max());
        vector<size_t> local_indices(tsettings.max_cores, 0);

        // Create threads with lambda functions that capture 'this' and call 'core_round'
        for (size_t i = 0; i < tsettings.max_cores; i++)
        {
            size_t start_idx = i * chunk_size;
            size_t end_idx = (i == tsettings.max_cores - 1) ? tsettings.neuralnetworks : (i + 1) * chunk_size;

            threads.push_back(thread([this, dataset, tsettings, start_idx, end_idx, i, &local_best_errors, &local_indices]()
                                     { this->core_round(dataset, tsettings, start_idx, end_idx, i, local_best_errors, local_indices); }));
        }

        for (auto &th : threads)
            th.join();

        // Find the overall best error after all threads have completed
        best_error = *min_element(local_best_errors.begin(), local_best_errors.end());
        index = local_indices[distance(local_best_errors.begin(), min_element(local_best_errors.begin(), local_best_errors.end()))];

        // Debugging print for best_error after all threads complete
        cout << "Round #" << round + 1 << " Completed | Best error: " << fixed << setprecision(8) << best_error << " | Population: " << nns.size() << endl;

        // Check if we should revert to backup model based on error drop
        if (last_error - best_error > tsettings.max_drop)
            nn = backup;
        else
        {
            nn = nns[index];
            backup = nn;
        }

        // Update last_error for comparison in the next round
        last_error = best_error;
    }
}

void AI::core_round(dataset_t dataset, TrainSettings_t tsettings, size_t start_idx, size_t end_idx, size_t i, vector<double>& local_best_errors, vector<size_t>& local_indices)
{
    double local_best_error = std::numeric_limits<double>::max();
    size_t local_index = start_idx;

    // Iterate over the subset of networks assigned to this thread
    for (size_t i = start_idx; i < end_idx; i++)
    {
        double error = nns[i].run_round(tsettings.epochs_per_round, dataset); // Train network for a round

        // Update local_best_error and local_index if current network error is better
        if (error < local_best_error)
        {
            local_best_error = error;
            local_index = i;
        }
    }

    // Update thread-local best_error and index
    local_best_errors[i] = local_best_error;
    local_indices[i] = local_index;
}

// Test method to evaluate network performance
double AI::test(dataset_t dataset)
{
    double error = 0.00;
    for (size_t i = 0; i < dataset.test_size; i++)
    {
        vector<double> output = nn.forward(dataset.X_test[i]);
        for (size_t j = 0; j < dataset.X_test[0].size(); j++)  // Fix indexing of dataset.X_test
            error = (error + abs(output[j] - dataset.y_test[i][j])) / 2;
    }
    return error;
}

vector<double> AI::forward(vector<double> input)
{
    return nn.forward(input);
}

void AI::addLayer(size_t size, string activation)
{
    nn.addLayer(size, activation);
}
