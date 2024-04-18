//
// Created by itani on 6/12/2023.
//

#include "Dense.h"


Dense::Dense(const Matrix &weights, const Matrix &bias,
             activation::activation_func_ptr func) {
    if (bias.get_cols() != 1 || weights.get_rows()!=bias.get_rows()){
        throw std::length_error(BAD_ARGS);
    }
    weights_ = weights;
    bias_= bias;
    func_ =func;
}

Matrix Dense::operator()(const Matrix& input) const{
    return func_(weights_*input+bias_);}




