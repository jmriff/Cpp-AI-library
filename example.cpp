#include <iostream> // Console IO
#include <chrono>   // For timing
#include <AI.hpp>   // NeuralNetwork class (main NeuralNetwork)

using namespace std;

AISettings_t AI_settings;
TrainSettings_t train_settings;

// Input
vector<vector<double>> X = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}};

// Output
vector<vector<double>> y = {
    {0},
    {1},
    {1},
    {0}};

int main()
{
    train_settings.max_cores        = 4;     // Edit as needed
    train_Settings.neuralnetworks   = 10000; // 10,000
    train_settings.rounds           = 100;   // 100
    train_settings.epochs_per_round = 10;    // 10
    train_settings.max_drop         = 0.01;

    try
    {
        train_settings.rounds = 10;

        // Create the network
        AI nn(AI_settings);
        nn.addLayer(2, "tanh");
        nn.addLayer(5, "tanh");
        nn.addLayer(1, "tanh");

        dataset_t dataset = createDataset(X, y, 0.5, 0.5);

        // Start timer before the training loop
        auto start = chrono::high_resolution_clock::now();

        nn.train(train_settings, dataset);  // Run the network training

        // End timer after the training loop
        auto end = chrono::high_resolution_clock::now();

        // Calculate elapsed time
        chrono::duration<double> duration = end - start;

        // Test the AI
        double score = nn.test(dataset);

        // Print the execution time and score
        cout << "\nExecution time: " << duration.count() << " seconds\n";
        cout << "Score: " << score << endl;
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
    }

    return 0;
}
