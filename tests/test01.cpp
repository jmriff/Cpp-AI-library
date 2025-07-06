#include <iostream>
#include <iomanip>
#include <vector>

#include <AI/nn.hpp>

using namespace std;

std::vector<std::vector<double>> X =
{
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
};

std::vector<std::vector<double>> y =
{
    {0},
    {1},
    {1},
    {0}
};

TrainSettings_t tset;

int main(int argc, char** argv)
{
    try
    {
        NeuralNetwork* pNN = new NeuralNetwork(ERROR_MEASURE_MSE, true);

        dataset_t* pDataset = createDataset(X, y, 0, 1);

        tset.lr       = 0.05;
        tset.lr_decay = 0.0003;
        tset.lr_stop  = 0.01;
        tset.batchSize = 2;

        pNN->addLayer(2, "tanh");
        pNN->addLayer(10, "tanh");
        pNN->addLayer(10, "tanh");
        pNN->addLayer(10, "tanh");
        pNN->addLayer(1, "tanh");

        pNN->train(300000, pDataset, &tset);

        std::vector<double> errors = {};
        std::vector<std::vector<double>> out;
        for (int i = 0; i < X.size(); i++) 
        {
            out.push_back(pNN->forward(X[i]));
            errors.push_back(pNN->calc_error(out[i], y[i]));
        }

        double maxerr = *std::max_element(errors.begin(), errors.end());

        for (int i = 0; i < X.size(); i++)
        {
            std::cout << "{";
            for (int j = 0; j < X[i].size(); j++)
            {
                std::cout << std::fixed << std::setprecision(2) << X[i][j];
                if (j != X[i].size() - 1)
                    std::cout << ", ";
            }
            std::cout << "} -> ";

            std::vector<double> out = pNN->forward(X[i]);

            std::cout << "{";
            for (int j = 0; j < out.size(); j++)
            {
                std::cout << std::fixed << std::setprecision(5) << out[j];
                if (j != out.size() - 1)
                    std::cout << ", ";
            }
            std::cout << "} | Error: ";
            std::cout << std::fixed << std::setprecision(2) << errors[i];
            std::cout << " (" << errors[i] * 100 << "%" << ")" << std::endl;
        }

        delete pNN;
        delete pDataset;
        return 0;
    }
    catch (const NNerror& e)
    {
        std::cerr << "NNerror: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "std::exception: " << e.what() << std::endl;
        return 1;
    }
}