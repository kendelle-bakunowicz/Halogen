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
#include <cstring>
#include "EvalCache.h"
#include "BitBoardDefine.h"

constexpr size_t INPUT_NEURONS = 12 * 64;
constexpr size_t HIDDEN_NEURONS = 256;

constexpr int16_t MAX_VALUE = 128;
constexpr int16_t PRECISION = ((size_t)std::numeric_limits<int16_t>::max() + 1) / MAX_VALUE;
constexpr int32_t SQUARE_PRECISION = (int32_t)PRECISION * PRECISION;
constexpr int32_t HALF_SQUARE_PRECISION = SQUARE_PRECISION / 2;
constexpr int16_t HALF_PRECISION = PRECISION / 2;

struct deltaArray
{
    struct deltaPoint
    {
        int16_t index;
        int16_t delta;
    };

    int8_t size;
    deltaPoint deltas[4];
};

extern std::array<std::array<int16_t, HIDDEN_NEURONS>, INPUT_NEURONS>* hiddenWeights;
extern std::array<int16_t, HIDDEN_NEURONS>* hiddenBias;
extern std::array<int16_t, HIDDEN_NEURONS>* outputWeights;
extern int16_t* outputBias;

void RecalculateIncremental(std::array<int16_t, INPUT_NEURONS> inputs, std::array<deltaArray, MAX_DEPTH>& deltaStack, size_t& deltaIndex, std::array<std::array<int16_t, HIDDEN_NEURONS>, MAX_DEPTH>& zetaStack, size_t& zetaIndex);
void ApplyDelta(deltaArray& newDelta, std::array<deltaArray, MAX_DEPTH>& deltaStack, size_t& deltaIndex);     //incrementally update the connections between input layer and first hidden layer
void ApplyInverseDelta(size_t& deltaIndex, size_t& zetaIndex);                                                                                   //for un-make moves
int16_t QuickEval(std::array<deltaArray, MAX_DEPTH>& deltaStack, size_t& deltaIndex, std::array<std::array<int16_t, HIDDEN_NEURONS>, MAX_DEPTH>& zetaStack, size_t& zetaIndex);          //when used with above, this just calculates starting from the alpha of first hidden layer and skips input -> hidden

void NetworkInit();