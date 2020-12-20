#include "Network.h"
#include "halogen-256-32-555E5D4B.nn"

std::array<std::array<int16_t, HIDDEN_NEURONS_1>, INPUT_NEURONS>* hiddenWeights1;
std::array<int16_t, HIDDEN_NEURONS_1>* hiddenBias1;
std::array<std::array<int16_t, HIDDEN_NEURONS_2>, HIDDEN_NEURONS_1>* hiddenWeights2;
std::array<int16_t, HIDDEN_NEURONS_2>* hiddenBias2;
std::array<int16_t, HIDDEN_NEURONS_2>* outputWeights;
int16_t* outputBias;

void NetworkInit()
{
    hiddenWeights1 = new std::array<std::array<int16_t, HIDDEN_NEURONS_1>, INPUT_NEURONS>;
    hiddenBias1    = new std::array<int16_t, HIDDEN_NEURONS_1>;
    hiddenWeights2 = new std::array<std::array<int16_t, HIDDEN_NEURONS_2>, HIDDEN_NEURONS_1>;
    hiddenBias2    = new std::array<int16_t, HIDDEN_NEURONS_2>;
    outputWeights  = new std::array<int16_t, HIDDEN_NEURONS_2>;
    outputBias     = new int16_t;

    auto* HiddenWeights1 = new float[INPUT_NEURONS * HIDDEN_NEURONS_1];
    auto* HiddenBias1    = new float[HIDDEN_NEURONS_1];
    auto* HiddenWeights2 = new float[HIDDEN_NEURONS_1 * HIDDEN_NEURONS_2];
    auto* HiddenBias2    = new float[HIDDEN_NEURONS_2];
    auto* OutputWeights  = new float[HIDDEN_NEURONS_2];
    auto* OutputBias     = new float[1];

    size_t index = 0;

    memcpy(HiddenBias1,     &label[index], sizeof(float) * HIDDEN_NEURONS_1);
    index += sizeof(float) * HIDDEN_NEURONS_1;
    memcpy(HiddenWeights1,  &label[index], sizeof(float) * INPUT_NEURONS * HIDDEN_NEURONS_1);
    index += sizeof(float) * INPUT_NEURONS * HIDDEN_NEURONS_1;

    memcpy(HiddenBias2,     &label[index], sizeof(float) * HIDDEN_NEURONS_2);
    index += sizeof(float) * HIDDEN_NEURONS_2;
    memcpy(HiddenWeights2,  &label[index], sizeof(float) * HIDDEN_NEURONS_1 * HIDDEN_NEURONS_2);
    index += sizeof(float) * HIDDEN_NEURONS_1 * HIDDEN_NEURONS_2;

    memcpy(OutputBias,      &label[index], sizeof(float) * 1);
    index += sizeof(float) * 1;
    memcpy(OutputWeights,   &label[index], sizeof(float) * HIDDEN_NEURONS_2);
    index += sizeof(float) * HIDDEN_NEURONS_2;

    assert(index == sizeof(label));

    for (size_t i = 0; i < INPUT_NEURONS; i++)
        for (size_t j = 0; j < HIDDEN_NEURONS_1; j++)
            (*hiddenWeights1)[i][j] = (int16_t)round(HiddenWeights1[i * HIDDEN_NEURONS_1 + j] * PRECISION);

    for (size_t i = 0; i < HIDDEN_NEURONS_1; i++)
        (*hiddenBias1)[i] = (int16_t)round(HiddenBias1[i] * PRECISION);

    for (size_t i = 0; i < HIDDEN_NEURONS_1; i++)
        for (size_t j = 0; j < HIDDEN_NEURONS_2; j++)
            (*hiddenWeights2)[i][j] = (int16_t)round(HiddenWeights2[i * HIDDEN_NEURONS_2 + j] * PRECISION);

    for (size_t i = 0; i < HIDDEN_NEURONS_2; i++)
        (*hiddenBias2)[i] = (int16_t)round(HiddenBias2[i] * PRECISION);

    for (size_t i = 0; i < HIDDEN_NEURONS_2; i++)
        (*outputWeights)[i] = (int16_t)round(OutputWeights[i] * PRECISION);

    (*outputBias) = (int16_t)round(OutputBias[0] * PRECISION);

    delete[] HiddenWeights1;
    delete[] HiddenBias1;
    delete[] HiddenWeights2;
    delete[] HiddenBias2;
    delete[] OutputWeights;
    delete[] OutputBias;
}

void RecalculateIncremental(std::array<int16_t, INPUT_NEURONS> inputs, std::array<std::array<int16_t, HIDDEN_NEURONS_1>, MAX_DEPTH + 1>& Zeta, size_t& incrementalDepth)
{
    incrementalDepth = 0;

    for (size_t i = 0; i <= MAX_DEPTH; i++)
        Zeta[i] = {};

    for (size_t i = 0; i < HIDDEN_NEURONS_1; i++)
        Zeta[0][i] = (*hiddenBias1)[i];

    for (size_t i = 0; i < HIDDEN_NEURONS_1; i++)
        for (size_t j = 0; j < INPUT_NEURONS; j++)
            Zeta[0][i] += inputs[j] * (*hiddenWeights1)[j][i];
}

void ApplyDelta(deltaArray& update, std::array<std::array<int16_t, HIDDEN_NEURONS_1>, MAX_DEPTH + 1>& Zeta, size_t& incrementalDepth)
{
    incrementalDepth++;
    Zeta[incrementalDepth] = Zeta[incrementalDepth - 1];

    for (size_t i = 0; i < update.size; i++)
    {
        if (update.deltas[i].delta == 1)
            for (size_t j = 0; j < HIDDEN_NEURONS_1; j++)
                Zeta[incrementalDepth][j] += (*hiddenWeights1)[update.deltas[i].index][j];
        else
            for (size_t j = 0; j < HIDDEN_NEURONS_1; j++)
                Zeta[incrementalDepth][j] -= (*hiddenWeights1)[update.deltas[i].index][j];
    }
}

void ApplyInverseDelta(size_t& incrementalDepth)
{
    --incrementalDepth;
}

int16_t QuickEval(const std::array<std::array<int16_t, HIDDEN_NEURONS_1>, MAX_DEPTH + 1>& Zeta, const size_t& incrementalDepth)
{
    //Calculate hidden layer 2 weighted inputs
    std::array<int32_t, HIDDEN_NEURONS_2> Zeta2;

    for (size_t i = 0; i < HIDDEN_NEURONS_2; i++)
        Zeta2[i] = (*hiddenBias2)[i];

    for (size_t i = 0; i < HIDDEN_NEURONS_2; i++)
        for (size_t j = 0; j < HIDDEN_NEURONS_1; j++)
            Zeta2[i] += std::max(int16_t(0), Zeta[incrementalDepth][j]) * (*hiddenWeights2)[j][i] / PRECISION;  //ReLU done here

    //Calculate the output weighted input
    int32_t output = (*outputBias) * PRECISION;

    for (size_t i = 0; i < HIDDEN_NEURONS_2; i++)
        output += std::max(int32_t(0), Zeta2[i]) * (*outputWeights)[i];                             //ReLU done here

    return output / SQUARE_PRECISION;
}