#include "RusRails.h"
#include <iostream>
#include <vector>
using namespace std;

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    data.resize(rows, vector<int>(cols, 0));
}

int Matrix::getRows() const {
    return rows;
}

int Matrix::getCols() const {
    return cols;
}

vector<int>& Matrix::operator[](int index) {
    if (index < 0 || index >= rows) {
        throw out_of_range("Индекс строки вне диапазона");
    }
    return data[index];
}

const vector<int>& Matrix::operator[](int index) const {
    if (index < 0 || index >= rows) {
        throw out_of_range("Индекс строки вне диапазона");
    }
    return data[index];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw invalid_argument("Размеры матриц не совпадают");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = data[i][j] + other[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw invalid_argument("Невозможно умножить: число столбцов первой матрицы не равно числу строк второй");
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            for (int k = 0; k < cols; k++) {
                result[i][j] += data[i][k] * other[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::sumOfPowers(int n) {
    if (n < 1) {
        throw invalid_argument("Показатель степени должен быть >= 1");
    }

    Matrix result = *this;
    Matrix temp = *this;
    for (int i = 2; i <= n; i++) {
        temp = temp * (*this);
        result = result + temp;
    }
    return result;
}
