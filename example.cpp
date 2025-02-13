#include <iostream> // Console IO
#include <chrono>   // For timing
#include <AI.hpp>   // AI class

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
    train_settings.max_cores = 12;
    train_settings.rounds = 100;

    try
    {
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
