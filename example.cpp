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
    try
    {
        train_settings.rounds = 10;

        // Create the network
        NeuralNetwork nn(AI_settings);
        nn.addLayer(2, "tanh");
        nn.addLayer(10, "tanh");
        nn.addLayer(10, "tanh");
        nn.addLayer(1, "tanh");

        dataset_t dataset = createDataset(X, y, 0.5, 0.5);

        // Start timer before the training loop
        auto start = chrono::high_resolution_clock::now();

        nn.run_round(100000, dataset);  // Run the network training

        // End timer after the training loop
        auto end = chrono::high_resolution_clock::now();

        // Calculate elapsed time
        chrono::duration<double> duration = end - start;

        // Test the AI
        double score = 0.00;
        for (size_t i = 0; i < dataset.test_size; i++)
            score = (score + nn.calc_abs_error(nn.forward(X[0]), y[0])) / 2;

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
