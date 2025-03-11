#ifndef __NNVIS_HPP
#define __NNVIS_HPP

#include "nn.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class NNVisualizer : NeuralNetwork
{
    RenderWindow& _win;
public:
    // Input layer size, Number of hidden layers, size of output layer, size of each layer
    NNVisualizer(int input, int hidden, int output, int hiddens) : NeuralNetwork();

    double train(int epochs, NNSettings_t settings);

    void setWindow(RenderWindow& win);
    void draw();
};

#endif
