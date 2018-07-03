#include "Neuron.hpp"

Neuron::Neuron(int numOfInput)
{
    this->numOfInput = numOfInput + 1;
    
    const int minWeight = -1;
    const int maxWeight = 1;
    
    //Initialise random weights for each input
    for (int i = 0; i < this->numOfInput; i++)
    {
        float weight = minWeight + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(maxWeight-minWeight)));
        weights.push_back(weight);
    }
}
