#include "nn.hpp"

// Constructor initializes the neural network with settings
NeuralNetwork::NeuralNetwork(ErrorMeasure merr, bool progressbar)
{
    this->merr = merr;
    this->bar = progressbar;

    // Initialize the memory pool with a default size
    this->NNPool = new MemoryPool(1024 * 1024); // 1 MB pool size, can be adjusted as needed

    
    
    // Set the error measure
    this->config(merr);
}

NeuralNetwork::~NeuralNetwork()
{}

/*
 * NeuralNetwork::config
 *
 * -------------------------
 * 
 * Configure the Error measurment unit for the network, usually ERROR_MEASURE_MSE
 * 
 * @param merr: Error Measure unit
 * 
 * Return: The error of the model in the unit set by NeuralNetwork::config
 */
void NeuralNetwork::config(ErrorMeasure merr)
{
    this->merr = merr;
}

/*
 * NeuralNetwork::calc_error (1)
 *
 * -------------------------
 * 
 * Calculate the error with the data from the pDataset pointer
 * 
 * @param pDataset: Pointer to the dataset to calculate error off of
 * 
 * Return: The error of the model in the unit set by NeuralNetwork::config
 */
double NeuralNetwork::calc_error(dataset_t* pDataset)
{
    double error = 0.00;

    for (int i = 0; i < (int)pDataset->X_test.size(); i++)
    {
        std::vector<double> output = this->forward(pDataset->X_test[i]);

         if (merr == ERROR_MEASURE_MAE)
            error = (error + calc_mae(output.data(), pDataset->y_test[i].data())) / 2;
        else if (merr == ERROR_MEASURE_MSE)
            error = (error + calc_mse(output.data(), pDataset->y_test[i].data())) / 2;
        else if (merr == ERROR_MEASURE_BCE)
            error = (error + calc_bce(output.data(), (int *)pDataset->y_test[i].data())) / 2;
        else if (merr == ERROR_MEASURE_CCE)
            error = (error + calc_cce(output.data(), (int *)pDataset->y_test[i].data(), pDataset->y_test[i].size())) / 2;
        
        // Default to MSE
        else error = (error + calc_mse(output.data(), pDataset->y_test[i].data())) / 2;
    }

    return error;
}

/*
 * NeuralNetwork::calc_error (2)
 *
 * -------------------------
 * 
 * Does same thing as other NeuralNetwork::calc_error, except uses individual values to calculate error (primarily used in NeuralNetwork::backpropagate)
 * 
 * @param pDataset: Pointer to the dataset to calculate error off of
 * 
 * Return: The error of the model in the unit set by NeuralNetwork::config
 */
double NeuralNetwork::calc_error(std::vector<double> predict, std::vector<double> real)
{
    if (predict.size() != real.size())
        throw NNerror("Predict and real vectors must be of the same size");
    
    if (merr == ERROR_MEASURE_MAE)
        return calc_mae(predict.data(), real.data());
    else if (merr == ERROR_MEASURE_MSE)
        return calc_mse(predict.data(), real.data());
    else if (merr == ERROR_MEASURE_BCE)
        return calc_bce(predict.data(), (int *)real.data());
    else if (merr == ERROR_MEASURE_CCE)
        return calc_cce(predict.data(), (int *)real.data(), real.size());
        
    // Default to MSE
    else return calc_mse(predict.data(), real.data());
}

/*
 * NeuralNetwork::forward
 *
 * ----------------------------
 * 
 * Do a forward pass through the network
 * 
 * @param X: A vector of values to pass through the network
 * 
 * Return: A vector of values outputed from the network
 */
std::vector<double> NeuralNetwork::forward(std::vector<double> X)
{
    // Check that can forward
    if (layers.size() <= 1)
        throw NNerror("Only 1 layer found in Neural Network. Can't forward network");

    // Set input layer (first layer)
    layers[0] = std::vector<Neuron>(X.size());
    for (int i = 0; i < (int)X.size(); i++)
    {
        layers[0][i].connections = {X[i]}; // Set input values as neuron inputs (not weights)
        // Set weights as needed, for now, assuming weights are set elsewhere
    }

    // Propagate through each layer
    for (int layer_idx = 1; layer_idx < (int)layers.size(); layer_idx++)
    {
        for (int neuron_idx = 0; neuron_idx < (int)layers[layer_idx].size(); neuron_idx++)
        {
            double net_input = 0.0;
            // Sum the weighted inputs from the previous layer's neurons
            for (int prev_neuron_idx = 0; prev_neuron_idx < (int)layers[layer_idx - 1].size(); prev_neuron_idx++)
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
    std::vector<double> output;
    for (int i = 0; i < (int)layers.back().size(); i++)
    {
        output.push_back(layers.back()[i].connections[0]);
    }

    return output;
}

/*
 * NeuralNetwork::backpropagate
 *
 * ----------------------------------
 * 
 * Backpropagate a network, training it for 1 epoch
 * 
 * @param X: Input vector for model to train on (Input for problem)
 * @param y: Output vector for model to train on (Encouraged output)
 * 
 * Return: Training accuracy, using MSE (Mean Square Error)
 */
double NeuralNetwork::backpropagate(std::vector<double> X, std::vector<double> y, TrainSettings_t* tset)
{
    // Check that can backpropagate
    if (layers.size() <= 1)
        throw NNerror("Only 1 layer found in Neural Network. Can't backpropagate network");

    double error = 0.0;

    // Perform forward pass to get predictions
    std::vector<double> output = this->forward(X);

    // Compute error using Mean Squared Error (MSE)
    error = this->calc_error(y, output) * y.size();

    // Initialize deltas for each layer
    std::vector<std::vector<double>> deltas(layers.size());
    for (int layer = 0; layer < (int)layers.size(); layer++)
        deltas[layer].resize(layers[layer].size());

    // Compute deltas for the output layer
    int last_layer = layers.size() - 1;
    for (int i = 0; i < (int)layers[last_layer].size(); i++)
    {
        double output_error = output[i] - y[i];
        // Use derivative of activation function
        deltas[last_layer][i] = output_error * derivatives[last_layer](layers[last_layer][i].connections[0]);
    }

    // Compute deltas for hidden layers
    for (int layer = (int)layers.size() - 2; layer >= 0; layer--)
    {
        for (int i = 0; i < (int)layers[layer].size(); i++)
        {
            double hidden_error = 0.0;
            for (int j = 0; j < (int)layers[layer + 1].size(); j++)
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
    for (int layer = layers.size() - 1; layer > 0; layer--)
    {
        for (int i = 0; i < (int)layers[layer].size(); i++)
        {
            // Update weights (connections) using the deltas for each neuron
            for (int j = 0; j < (int)layers[layer - 1].size(); j++)
            {
                // Weight update rule: W = W - lr * delta * input
                layers[layer][i].connections[j] -= tset->lr * deltas[layer][i] * layers[layer - 1][j].connections[0];
            }
            // Update biases using delta
            layers[layer][i].bias -= tset->lr * deltas[layer][i];
        }
    }

    return error / output.size();
}

double NeuralNetwork::train(int epochs, dataset_t* pDataset, TrainSettings_t* pTset)
{
    int barWidth = 100;
    int update_interval = std::max(1, epochs / 100);
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        for (int i = 0; i < (int)pDataset->X_train.size(); i += pTset->batchSize)
        {
            int batch_end = std::min(i + pTset->batchSize, (int)pDataset->X_train.size());
            for (int j = i; j < batch_end; j++)
                this->backpropagate(pDataset->X_train[j], pDataset->y_train[j], pTset);
        }
        if (bar && (epoch % update_interval == 0 || epoch == epochs - 1))
        {
            float progress = (float)(epoch + 1) / epochs;
            int pos = barWidth * progress;

            std::ostringstream oss;
            oss << "\r" << std::fixed << std::setprecision(2) << (progress * 100.0) << "% [";
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos) oss << "=";
                else if (i == pos) oss << ">";
                else oss << " ";
            }
            oss << "]";
            std::cout << oss.str();
            std::cout.flush();

            if (epoch == epochs - 1)
                std::cout << std::endl;
        }
    }
    return this->calc_error(pDataset);
}

/*
 * NeuralNetwork::addLayer
 *
 * -----------------------------
 * 
 * Add a layer to a neural network
 * 
 * @param size:       Number of neurons to put in that layer
 * @param activation: Activation for layer
 * 
 * Return: Nothing
 */
void NeuralNetwork::addLayer(int size, const char* activation)
{
    if (strcmp(activation, "tanh") == 0)
    {
        activations.push_back(nn_tanh);
        derivatives.push_back(nn_dtanh);
    }
    else if (strcmp(activation, "relu") == 0)
    {
        activations.push_back(nn_relu);
        derivatives.push_back(nn_drelu);
    }
    else if (strcmp(activation, "sigmoid") == 0)
    {
        activations.push_back(nn_sigmoid);
        derivatives.push_back(nn_dsigmoid);
    }
    else if (strcmp(activation, "fsigmoid") == 0)
    {
        activations.push_back(nn_fsigmoid);
        derivatives.push_back(nn_dfsigmoid);
    }
    else
        throw NNerror("Unsupported activation function: " + (std::string)activation);

    std::vector<Neuron> new_layer(size);

    // Random number generator for weights and biases
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // If this is not the first layer, initialize connections from the previous layer
    if (!layers.empty())
    {
        for (int i = 0; i < size; i++)
        {
            if (layers.back().empty())
                throw NNerror("Previous layer has no neurons to connect to.");
            new_layer[i].connections.resize(layers.back().size());
            for (auto& w : new_layer[i].connections)
                w = dis(gen); // Random weight
            new_layer[i].bias = dis(gen); // Random bias
        }
    }
    else
    {
        // For the first layer, just randomize bias
        for (int i = 0; i < size; i++)
            new_layer[i].bias = dis(gen);
    }

    layers.push_back(new_layer);
}

/*
 * createDataset
 *
 * -------------------------
 * 
 * Create a dataset_t structure for a NeuralNetwork class to train off of
 * 
 * @param X:             std::vector of std::vector<double>-type inputs, each element in the std::vector<std::vector> is a input dataset entry
 * @param y:             std::vector of std::vector<double>-type outputs, each element in the std::vector<std::vector> is a output dataset entry
 * @param test_percent:  % of data to be test data for the model
 * @param train_percent: % of data to be training data for the model
 * 
 * Return: The error of the model in the unit set by NeuralNetwork::config
 */
dataset_t* createDataset(std::vector<std::vector<double>> X, std::vector<std::vector<double>> y, double test_percent, double train_percent)
{
    int X_size = X.size();
    int Y_size = y.size();

    if (X_size != Y_size)
        throw NNerror("While creating dataset, X and y must have same length");
    if (test_percent + train_percent != 1)
        throw NNerror("While creating dataset, test_percent + train_percent  must be equal to 1");

    dataset_t* pDataset = new dataset_t();
    pDataset->X_train.reserve(X_size);
    pDataset->y_train.reserve(X_size);
    pDataset->X_test.reserve(X_size);
    pDataset->y_test.reserve(X_size);

    // Shuffle data points (to randomly split the data)
    std::vector<int> indices(X_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g); // Shuffle indices randomly

    // Calculate the number of test and train samples
    int test_size = static_cast<int>(X_size * test_percent);
    int train_size = X_size - test_size;

    // Populate the dataset with shuffled data
    for (int i = 0; i < train_size; i++)
    {
        pDataset->X_train.push_back(X[indices[i]]);
        pDataset->y_train.push_back(y[indices[i]]);
    }

    for (int i = train_size; i < X_size; i++)
    {
        pDataset->X_test.push_back(X[indices[i]]);
        pDataset->y_test.push_back(y[indices[i]]);
    }

    // Set the sizes for easy reference
    pDataset->train_size = train_size;
    pDataset->test_size = test_size;

    return pDataset;
}