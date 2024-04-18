//
// Created by itani on 6/12/2023.
//
#include <cmath>
#include "Activation.h"
#include <iostream>

Matrix activation::relu(const Matrix& matrix){
    Matrix relu_mat = Matrix(matrix);
    for (int i = 0; i < relu_mat.get_cols()*relu_mat.get_rows(); i++){
        if (relu_mat[i] < 0 ){
            relu_mat[i] = 0;
        }
    }
    return relu_mat;
}

Matrix activation::softmax(const Matrix& matrix){
    Matrix softmax_mat = Matrix(matrix.get_rows(),
                                matrix.get_cols());
    float exp_sum = 0;
    for (int i = 0; i < softmax_mat.get_cols()*softmax_mat.get_rows(); i++){
        exp_sum += std::exp(matrix[i]);
    }
    if (exp_sum == 0) {
        throw std::runtime_error(ZERO_DIV);
    }
    for (int i = 0; i < softmax_mat.get_cols()*softmax_mat.get_rows(); i++){
        softmax_mat[i] = std::exp(matrix[i])/exp_sum;
    }
    return softmax_mat;
}