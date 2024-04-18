//
// Created by itani on 6/12/2023.
//

#include "MlpNetwork.h"

MlpNetwork::MlpNetwork(const Matrix* weights, const Matrix* biases):
    layer1_(weights[ZERO], biases[ZERO], activation::relu),
    layer2_(weights[ONE], biases[ONE], activation::relu),
    layer3_(weights[TWO], biases[TWO], activation::relu),
    layer4_(weights[THREE], biases[THREE], activation::softmax)
{};

digit MlpNetwork::operator()(const Matrix& matrix){

    Matrix r0 = Matrix(matrix).vectorize();
    Matrix r1 = layer1_(r0);
    Matrix r2 = layer2_(r1);
    Matrix r3 = layer3_(r2);
    Matrix r4 = layer4_(r3);


    unsigned int max_index = r4.argmax();
    return digit{max_index, r4[r4.argmax()]};


}
#include "MlpNetwork.h"
