#include "nn.hpp"

// Constructor initializes the neural network with settings
NeuralNetwork::NeuralNetwork(AISettings_t settings)
    : settings(settings) {}

NeuralNetwork::NeuralNetwork() {}

// Forward pass through the network
vector<double> NeuralNetwork::forward(vector<double> X)
{
    // Check that can forward
    if (layers.size() <= 1)
        throw NNerror("Only 1 layer found in Neural Network. Can't forward network");

    // Set input layer (first layer)
    layers[0] = vector<Neuron>(X.size());
    for (size_t i = 0; i < X.size(); i++)
    {
        layers[0][i].connections = {X[i]}; // Set input values as neuron inputs (not weights)
        // Set weights as needed, for now, assuming weights are set elsewhere
    }

    // Propagate through each layer
    for (size_t layer_idx = 1; layer_idx < layers.size(); layer_idx++)
    {
        for (size_t neuron_idx = 0; neuron_idx < layers[layer_idx].size(); neuron_idx++)
        {
            double net_input = 0.0;
            // Sum the weighted inputs from the previous layer's neurons
            for (size_t prev_neuron_idx = 0; prev_neuron_idx < layers[layer_idx - 1].size(); prev_neuron_idx++)
            {
                // Multiply the output from the previous layer neuron by the weight of the connection
                net_input += layers[layer_idx - 1][prev_neuron_idx].connections[0] * layers[layer_idx][neuron_idx].connections[prev_neuron_idx];
            }
            net_input += layers[layer_idx][neuron_idx].bias; // Add bias

            // Apply activation function
            layers[layer_idx][neuron_idx].connections[0] = activations[layer_idx](net_input);
        }
    }

    // Collect the output from the last layer
    vector<double> output;
    for (size_t i = 0; i < layers.back().size(); i++)
    {
        output.push_back(layers.back()[i].connections[0]);
    }

    return output;
}

// Backpropagation to update weights based on the error
double NeuralNetwork::backpropagate(vector<double> X, vector<double> y)
{
    // Check that can backpropagate
    if (layers.size() <= 1)
        throw NNerror("Only 1 layer found in Neural Network. Can't backpropagate network");

    double error = 0.0;

    // Perform forward pass to get predictions
    vector<double> output = forward(X);

    // Compute error using Mean Squared Error (MSE)
    for (size_t i = 0; i < output.size(); i++)
        error += (output[i] - y[i]) * (output[i] - y[i]);

    // Initialize deltas for each layer
    vector<vector<double>> deltas(layers.size());
    for (size_t layer = 0; layer < layers.size(); layer++)
        deltas[layer].resize(layers[layer].size());

    // Compute deltas for the output layer
    size_t last_layer = layers.size() - 1;
    for (size_t i = 0; i < layers[last_layer].size(); i++)
    {
        double output_error = output[i] - y[i];
        // Use derivative of activation function
        deltas[last_layer][i] = output_error * derivatives[last_layer](layers[last_layer][i].connections[0]);
    }

    // Compute deltas for hidden layers
    for (size_t layer = layers.size() - 2; layer >= 0; layer--)
    {
        for (size_t i = 0; i < layers[layer].size(); i++)
        {
            double hidden_error = 0.0;
            for (size_t j = 0; j < layers[layer + 1].size(); j++)
            {
                // Update hidden error using the weight (connection) instead of neuron output
                hidden_error += deltas[layer + 1][j] * layers[layer + 1][j].connections[i];
            }
            // Use derivative of activation function
            deltas[layer][i] = hidden_error * derivatives[layer](layers[layer][i].connections[0]);
        }

        if (layer == 0)
            break; // Stop at the input layer
    }

    // Update weights and biases using deltas (Gradient Descent)
    for (size_t layer = layers.size() - 1; layer > 0; layer--)
    {
        for (size_t i = 0; i < layers[layer].size(); i++)
        {
            // Update weights (connections) using the deltas for each neuron
            for (size_t j = 0; j < layers[layer - 1].size(); j++)
            {
                // Weight update rule: W = W - lr * delta * input
                layers[layer][i].connections[j] -= settings.lr * deltas[layer][i] * layers[layer - 1][j].connections[0];
            }
            // Update biases using delta
            layers[layer][i].bias -= settings.lr * deltas[layer][i];
        }
    }

    return error / output.size();  // Return the average error (MSE)
}

void NeuralNetwork::addLayer(size_t size, string activation)
{
    if (activation == "tanh")
    {
        activations.push_back(activation::tanh);
        derivatives.push_back(activation::dtanh);
    }
    else if (activation == "relu")
    {
        activations.push_back(activation::relu);
        derivatives.push_back(activation::drelu);
    }
    else if (activation == "sigmoid")
    {
        activations.push_back(activation::sigmoid);
        derivatives.push_back(activation::dsigmoid);
    }
    else if (activation == "fsigmoid")
    {
        activations.push_back(activation::fsigmoid);
        derivatives.push_back(activation::dfsigmoid);
    }
    else
        throw NNerror("Unsupported activation function: " + activation);

    vector<Neuron> new_layer(size);

    // If this is not the first layer, initialize connections from the previous layer
    if (!layers.empty())
    {
        for (size_t i = 0; i < size; i++)
        {
            // Check to prevent out-of-bounds access
            if (layers.back().empty())
                throw NNerror("Previous layer has no neurons to connect to.");
            new_layer[i].connections.resize(layers.back().size()); // Initialize connections
        }
    }

    // Add the new layer to the layers vector
    layers.push_back(new_layer);
}

double NeuralNetwork::run_round(size_t epochs, dataset_t dataset)
{
    // Train the network for each epoch
    for (size_t epoch = 0; epoch < epochs; epoch++)
    {
        // Shuffle dataset before each epoch
        std::random_device rd;
        std::mt19937 g(rd());
        std::vector<size_t> indices(dataset.X_train.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), g);

        // Process the data in mini-batches
        for (size_t i = 0; i < dataset.X_train.size(); i += settings.batchs)
        {
            // Prepare the mini-batch
            size_t batch_end = min(i + settings.batchs, dataset.X_train.size());
            for (size_t j = i; j < batch_end; j++)
                backpropagate(dataset.X_train[indices[j]], dataset.y_train[indices[j]]);
        }
    }

    // Calculate average absolute error for testing
    double total_error = 0.0;
    for (size_t i = 0; i < dataset.X_test.size(); i++)
    {
        vector<double> output = forward(dataset.X_test[i]);
        total_error += calc_abs_error(output, dataset.y_test[i]);
    }

    return total_error / dataset.X_test.size(); // Return average absolute error
}

double NeuralNetwork::calc_abs_error(vector<double> X, vector<double> y)
{
    double error = 0.00;
    for (size_t i = 0; i < X.size(); i++)
        error += abs(X[i] - y[i]);
    return error;
}

void NeuralNetwork::edit(size_t seed, double intensity)
{
    srand(seed); // Initialize random seed

    // Generate random number between (-intensity / 2) and (intensity / 2)
    double random = (static_cast<double>(rand() - (int)(RAND_MAX / 2)) / RAND_MAX) * intensity;

    // Edit connections
    for (size_t i = 0; i < layers.size(); i++)
    {
        for (size_t j = 0; j < layers[i].size(); j++)
        {
            // Check to ensure that connections are valid
            if (layers[i][j].connections.size() > 0)
            {
                size_t rand_index = rand() % layers[i][j].connections.size(); // Pick a random connection
                layers[i][j].connections[rand_index] *= random;               // Apply mutation
            }
        }
    }

    // Edit biases
    for (size_t i = 0; i < layers.size(); i++)
    {
        for (size_t j = 0; j < layers[i].size(); j++)
        {
            layers[i][j].bias *= -random; // Mutate bias
        }
    }
}

dataset_t createDataset(vector<vector<double>> X, vector<vector<double>> y, double test_percent, double train_percent)
{
    size_t X_size = X.size();
    size_t Y_size = y.size();

    if (X_size != Y_size)
        throw NNerror("While creating dataset, X and y must have same length");
    if (test_percent + train_percent != 1)
        throw NNerror("While creating dataset, test_percent + train_percent  must be equal to 1");

    dataset_t dataset;

    // Shuffle data points (to randomly split the data)
    std::vector<size_t> indices(X_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g); // Shuffle indices randomly

    // Calculate the number of test and train samples
    size_t test_size = static_cast<size_t>(X_size * test_percent);
    size_t train_size = X_size - test_size;

    // Populate the dataset with shuffled data
    for (size_t i = 0; i < train_size; i++)
    {
        dataset.X_train.push_back(X[indices[i]]);
        dataset.y_train.push_back(y[indices[i]]);
    }

    for (size_t i = train_size; i < X_size; i++)
    {
        dataset.X_test.push_back(X[indices[i]]);
        dataset.y_test.push_back(y[indices[i]]);
    }

    // Set the sizes for easy reference
    dataset.train_size = train_size;
    dataset.test_size = test_size;

    return dataset;
}

void NeuralNetwork::config(AISettings_t settings)
{
    this->settings = settings;
}
