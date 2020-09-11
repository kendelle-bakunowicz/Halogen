#pragma once
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <random>
#include <numeric>
#include <algorithm>
#include <sstream>
#include "fixed.h"

#define INPUT_NEURONS 12 * 64 - 32 + 1 + 4

typedef BasicFixedReal<10> FixedReal;

struct trainingPoint
{
    trainingPoint(std::array<bool, INPUT_NEURONS> input, float gameResult);

    std::array<bool, INPUT_NEURONS> inputs;
    float result;
};

struct deltaPoint
{
    size_t index;
    int delta;
};

struct Neuron
{
    Neuron(const std::vector<float>& Weight, float Bias);
    float FeedForward(std::vector<float>& input) const;
    void Backpropogate(float delta_l, const std::vector<float>& prev_weights, float learnRate);
    void WriteToFile(std::ofstream& myfile);

    std::vector<float> weights;
    float bias;

    std::vector<float> grad;       //for adagrad
};

struct HiddenLayer
{
    HiddenLayer(std::vector<float> inputs, size_t NeuronCount);    // <for first neuron>: weight1, weight2, ..., weightN, bias, <next neuron etc...>
    std::vector<float> FeedForward(std::vector<float>& input);
    void Backpropogate(const std::vector<float>& delta_l, const std::vector<float>& prev_weights, float learnRate);
    void WriteToFile(std::ofstream& myfile);

    std::vector<Neuron> neurons;

    //cache for backprop after feedforward
    std::vector<float> zeta;    //weighted input     
    static std::vector<float> activationPrime(std::vector<float> x);

private:

    std::vector<float> weightTranspose; //first neuron first weight, second neuron first weight etc...
};

struct Network
{
    Network(std::vector<std::vector<float>> inputs, std::vector<size_t> NeuronCount);
    float FeedForward(std::vector<float> inputs);
    float Backpropagate(trainingPoint data, float learnRate);
    void WriteToFile();
    void Learn();

private:
    static std::vector<trainingPoint> quietlabeledDataset();
    static std::vector<trainingPoint> Stockfish3PerDataset();
    size_t inputNeurons;

    void AddExtraNullLayer(size_t neurons);   //given a network add another hidden layer at the end that wont change network output.

    //cache for backprop after feedforward (these are for the output neuron)
    float zeta;    //weighted input
    float alpha;   //result after activation function

    std::vector<HiddenLayer> hiddenLayers;
    Neuron outputNeuron;
};

class QuantizedNetwork
{
public:
    QuantizedNetwork();
    ~QuantizedNetwork();

    int QuickEval();
    int Eval(std::vector<int>& inputs);

    void ApplyDelta(std::vector<deltaPoint>& deltaVec);
    void ApplyInverseDelta(std::vector<deltaPoint>& deltaVec);

private:
    std::vector<FixedReal> zeta;                                //The incrementally updated output of the first hidden layer (without ReLU activation)                    
    std::vector<std::vector<std::vector<FixedReal>>> weights;   //by layer, by neuron, by weight

    void FeedForward(std::vector<std::vector<std::vector<FixedReal>>>::iterator& layer, std::vector<FixedReal>& next, std::vector<FixedReal>& zeta);


};

void Learn();

Network InitNetwork(std::string file);
Network CreateRandom(std::vector<size_t> NeuronCount);