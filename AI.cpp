#include "AI.hpp"

mutex mtx;

AI::AI(AISettings_t settings) : settings(settings) {};

void AI::train(TrainSettings_t tsettings, dataset_t dataset)
{
    // Initialize the networks
    nns.resize(tsettings.neuralnetworks);
    for (size_t i = 0; i < tsettings.neuralnetworks; i++)
    {
        nns[i] = nn; // Clone the base network
        nns[i].config(settings); // Configure network with settings
    }
    backup = nn;
    nn.config(settings);  // Configure base network
    
    // Generation learning
    double best_error;
    size_t index = 0;

    // Function to train a portion of the neural networks in parallel
    auto train_part = [&](size_t start_idx, size_t end_idx) {
        double local_best_error = best_error;
        size_t local_index = index;

        // Iterate over the subset of networks assigned to this thread
        for (size_t i = start_idx; i < end_idx; i++)
        {
            double error = nns[i].run_round(tsettings.epochs_per_round, dataset);  // Train network for a round
            if (i == start_idx || error < local_best_error)
            {
                local_best_error = error;
                local_index = i;
            }
        }

        // Use mutex to update shared resources like best_error and index
        std::lock_guard<std::mutex> lock(mtx);
        if (local_best_error < best_error)  // Check if this thread's result is better
        {
            best_error = local_best_error;
            index = local_index;
        }
    };

    // Iterate over each round of training
    for (size_t round = 0; round < tsettings.rounds; round++)
    {
        // Create threads to train the networks in parallel
        size_t chunk_size = tsettings.neuralnetworks / tsettings.max_cores;
        vector<std::thread> threads;

        for (size_t i = 0; i < tsettings.max_cores; i++)
        {
            size_t start_idx = i * chunk_size;
            size_t end_idx = (i == tsettings.max_cores - 1) ? tsettings.neuralnetworks : (i + 1) * chunk_size;
            threads.push_back(std::thread(train_part, start_idx, end_idx));
        }

        // Wait for all threads to finish
        for (auto& th : threads)
        {
            th.join();
        }

        // Check if the error has dropped significantly and update the network accordingly
        if (last_error - best_error > tsettings.max_drop)
        {
            nn = backup;  // Revert to the backup network if error drop is too large
        }
        else
        {
            nn = nns[index];  // Copy the best-performing network to the main one
            backup = nn;      // Backup the best network
        }

        // Update last_error for comparison in the next round
        last_error = best_error;  // Store the best error after this round

        // Gradual reduction of population (eliminating poor networks)
        size_t surviving_networks = static_cast<size_t>(tsettings.neuralnetworks * (1.0 - round / static_cast<double>(tsettings.rounds)));
        if (surviving_networks < 1) surviving_networks = 1;  // Ensure at least 1 network survives

        // Select the best networks to carry over
        vector<NeuralNetwork> new_generation;
        new_generation.push_back(nns[index]);  // Always keep the best one
        for (size_t i = 1; i < surviving_networks; i++)
        {
            // Clone and mutate a few of the best networks randomly
            NeuralNetwork new_nn = nns[i % tsettings.neuralnetworks];
            new_nn.edit(rand(), settings.intensity);  // Mutate slightly
            new_generation.push_back(new_nn);
        }

        // Replace the old generation with the new one
        nns = new_generation;

        // Output current training stats
        cout << "Round #" << round << " Completed | Best error: " << best_error << " | Population: " << nns.size() << endl;
    }
}

// Test method to evaluate network performance
double AI::test(dataset_t dataset)
{
    double error = 0.00;
    for (size_t i = 0; i < dataset.test_size; i++)
    {
        vector<double> output = nn.forward(dataset.X_test[i]);
        for (size_t j = 0; j < dataset.X_test.size(); j++)
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