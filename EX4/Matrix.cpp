
//
// Created by itani on 6/10/2023.
//

#include <iostream>
#include "Matrix.h"
#include <cmath>


Matrix::Matrix(int rows, int cols) {
    if(rows<0 || cols<0){
        throw std::length_error(ILLEGAL_DIM);
    }
    this->rows_ = rows;
    this->cols_ = cols;
    matrix_ = new float[rows_*cols_];
    leader_index=NO_LEADER;
    make_mat_array();
}

Matrix::Matrix(const Matrix &other){
    this->rows_ = other.get_rows();
    this->cols_ = other.get_cols();
    this->leader_index = other.leader_index;
    matrix_ = new float[rows_*cols_];
    make_mat_array(other.matrix_, false);
}

Matrix::~Matrix() {
    delete[] matrix_;
}

void Matrix::make_mat_array() {
    for (int i = 0; i < rows_*cols_; ++i) {
        matrix_[i] = 0;
    }
}

void Matrix::make_mat_array(const float *other, bool trans) {
    matrix_ = new float[rows_ * cols_];
    if (trans) {
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < cols_; j++) {
                matrix_[i * cols_ + j] = other[j * rows_ + i];
            }
        }
    } else {
        for (int i = 0; i < cols_ * rows_; ++i) {
            matrix_[i] = other[i];
        }
    }
}

Matrix& Matrix::transpose() {
    float *old_mat = this->matrix_;
    int old_rows = this->rows_;
    int old_cols = this->cols_;

    this->rows_ = old_cols;
    this->cols_ = old_rows;
    make_mat_array(old_mat, true);
    delete (old_mat);
    return *this;
}

void Matrix::plain_print() const{
    for (int i = 0; i < rows_*cols_; ++i) {
        std::cout << matrix_[i] << " ";
        if ((i+1)%cols_ == 0){
            std::cout<<std::endl;
        }
    }
}

Matrix Matrix::dot(Matrix const &other) const{
    if (rows_ == other.rows_ && cols_ == other.cols_) {
        Matrix hardman_mat = Matrix(*this);
        for (int i = 0; i < rows_ * cols_; i++) {
            hardman_mat.matrix_[i] = this->matrix_[i] * other.matrix_[i];
        }
        return hardman_mat;
    }
    throw std::length_error(WORNG_DIM " 1");
}

float Matrix::norm() const{
    float squared_norm = 0;
    for (int i = 0 ; i < rows_*cols_; i++){
        squared_norm += matrix_[i]*matrix_[i];
    }
    return std::sqrt(squared_norm);
}

int Matrix::argmax() const{
    float max = matrix_[0];
    int index = 0;
    for (int i=1 ; i<rows_*cols_; i++){
        if (matrix_[i] > max){
            max = matrix_[i];
            index = i;
        }
    }
    return index;
}

Matrix Matrix::operator+ (const Matrix &other) const{
    if (rows_ == other.rows_ && cols_ == other.cols_) {
        Matrix sum_mat = Matrix(*this);
        for (int i = 0; i < rows_ * cols_; i++) {
            sum_mat.matrix_[i] = this->matrix_[i] + other.matrix_[i];
        }
        return sum_mat;
    }
    throw std::length_error(WORNG_DIM "in +");
}

Matrix Matrix::operator* (const Matrix &other)const{
    if (this->cols_ != other.rows_){
        throw std::length_error(WORNG_DIM "in *");
    }
    Matrix mult_mat = Matrix(rows_, other.cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < other.cols_; j++) {
            // Calculate the element at row i and column j
            for (int k = 0; k < cols_; k++) {
                mult_mat.matrix_[i * mult_mat.cols_ + j] +=
                        matrix_[i * cols_ + k]
                        * other.matrix_[k * other.cols_ + j];
            }
        }
    }

    return mult_mat;
}
Matrix Matrix::operator* (float c)const{
    Matrix mult_mat = Matrix(*this);
    for (int i =0; i < rows_*cols_; i++){
        mult_mat[i] = matrix_[i]*c;
    }
    return mult_mat;
}
Matrix& Matrix::operator= (const Matrix &other){
    if (&other == this){
        return *this;
    }
    rows_ = other.rows_;
    cols_ = other.cols_;
    leader_index= other.leader_index;
    delete(matrix_);
    make_mat_array(other.matrix_);
    return *this;}


std::ostream &operator<<(std::ostream &o_stream, const Matrix& mat) {
    for (int i = 0; i<mat.get_rows(); i++) {
        for (int j = 0; j < mat.get_cols(); j++) {
            if (mat[i * mat.get_cols()+ j] > POINT_ONE) {
                o_stream << "**";
            } else {
                o_stream << "  ";
            }
        }
        o_stream << "\n";
    }
    return o_stream;
}


std::istream& operator >> (std::istream& i_stream,Matrix& matrix){

    int counter = 0;
    while (i_stream && counter < matrix.get_rows()*matrix.get_cols()){
        i_stream.read((char *) &matrix[counter++], sizeof (float));
    }
    if (counter<matrix.get_rows()*matrix.get_cols()){
        throw std::runtime_error(NO_DATA);
    }
    if(i_stream.fail()){
        throw std::runtime_error(NO_FILE);
    }
    return i_stream;
}

Matrix Matrix::rref() const{
    Matrix* rows_array = this->make_row_array();
    for (int row = 0; row < rows_; row++){
        rows_array[row].find_leader();
    }
    bubble_sort(rows_array);
    for (int i = 0; i < rows_; i++){
        if (rows_array[i].leader_index != NO_LEADER) {
            for (int j = 0; j < rows_; j++) {
                if ((i != j) && (rows_array[j][rows_array[i].leader_index] !=
                0) && (rows_array[j][rows_array[i].leader_index] != -0)) {
                    rows_array[j] -= rows_array[i] *
                                     rows_array[j][rows_array[i].leader_index];
                }
            }
            for (int row = i+1; row < rows_; row++) {
                rows_array[row].find_leader();
            }
        }
    }
    bubble_sort(rows_array);
    Matrix g_mat = Matrix(rows_,cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            g_mat[i*cols_ + j] =rows_array[i][j];
        }
    }
    delete [] rows_array;
    return g_mat;
}

void Matrix::find_leader(){
    float leading= 0;
    leader_index = NO_LEADER;
    for (int col = 0; col < cols_; col++){
        if ((matrix_[col] != 0) && (matrix_[col] != -0)){
            leader_index = col;
            leading = matrix_[col];
            *this = *this * (1 / leading);
            break;
        }
    }
}

void Matrix::bubble_sort(Matrix *rows_array) const {
    for (int i = 0; i < rows_; i++){
        for (int j =0; j < rows_ - 1; j++){
            if (rows_array[j].leader_index > rows_array[j+1].leader_index){
                Matrix tmp = rows_array[j];
                rows_array[j] = rows_array[j+1];
                rows_array[j+1] = tmp;
            }
        }
    }
}

Matrix* Matrix::make_row_array() const{
    Matrix* rows_array = new Matrix[rows_];
    for (int row = 0; row < rows_; row++) {
        Matrix row_mat = Matrix(1, cols_);
        for (int col = 0; col < cols_; col++) {
            row_mat[col] = matrix_[row*cols_ + col];
        }
        rows_array[row] = row_mat;
    }
    return rows_array;
}

void Matrix::operator-= (const Matrix &other){
    if (rows_ == other.rows_ && cols_ == other.cols_) {
        for (int i = 0; i < rows_ * cols_; i++) {
            matrix_[i] -= other.matrix_[i];
        }
        return;
    }
    throw std::length_error(WORNG_DIM "in -=");
}

void Matrix::operator+= (const Matrix &other){
    if (rows_ == other.rows_ && cols_ == other.cols_) {
        for (int i = 0; i < rows_ * cols_; i++) {
            matrix_[i] += other.matrix_[i];
        }
        return;
    }
    throw std::length_error(WORNG_DIM "in +=");
}

float Matrix::sum() const{
    float sum = 0;
    for (int i =0; i<rows_*cols_; i++){
        sum +=matrix_[i];
    }
    return sum;
}

float& Matrix::operator() (int row,int col) const{
    if (row>=rows_ || row<0 || col>=cols_ || col < 0){
        throw std::out_of_range(OUT_OF_RANGE);
    }
    return matrix_[row*cols_ + col];
}

float& Matrix::operator() (int row,int col){
    if (row>=rows_ || row<0 || col>=cols_ || col < 0){
        throw std::out_of_range(OUT_OF_RANGE);
    }
    return matrix_[row*cols_ + col];
}

float& Matrix::operator[] (int index) const{
    if (index >= rows_*cols_){
        throw std::out_of_range(OUT_OF_RANGE);
    }
    return matrix_[index];};

float& Matrix::operator[] (int index) {
    if (index >= rows_*cols_){
        throw std::out_of_range(OUT_OF_RANGE);
    }
    return matrix_[index];};


