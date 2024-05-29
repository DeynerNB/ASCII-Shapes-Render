#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix {
private:
    float ProjectionMatrix[3][3] = { {0.0} };

public:
    Matrix(float v11, float  v12, float  v13, float  v21, float  v22, float  v23, float  v31, float  v32, float  v33);
    ~Matrix();

    std::vector<int> Matrix_Multiplication(std::vector<int> pts);
    void updateValues(float v11, float  v12, float  v13, float  v21, float  v22, float  v23, float  v31, float  v32, float  v33);
};

Matrix::Matrix(float v11, float  v12, float  v13, float  v21, float  v22, float  v23, float  v31, float  v32, float  v33) {
    ProjectionMatrix[0][0] = v11;
    ProjectionMatrix[0][1] = v12;
    ProjectionMatrix[0][2] = v13;

    ProjectionMatrix[1][0] = v21;
    ProjectionMatrix[1][1] = v22;
    ProjectionMatrix[1][2] = v23;

    ProjectionMatrix[2][0] = v31;
    ProjectionMatrix[2][1] = v32;
    ProjectionMatrix[2][2] = v33;
}

Matrix::~Matrix() {}

std::vector<int> Matrix::Matrix_Multiplication(std::vector<int> pts) {

    float f_pts_0 = (float) pts[0];
    float f_pts_1 = (float) pts[1];
    float f_pts_2 = (float) pts[2];

    int prime_X = (f_pts_0 * this->ProjectionMatrix[0][0]) + (f_pts_1 * this->ProjectionMatrix[0][1]) + (f_pts_2 * this->ProjectionMatrix[0][2]);
    int prime_Y = (f_pts_0 * this->ProjectionMatrix[1][0]) + (f_pts_1 * this->ProjectionMatrix[1][1]) + (f_pts_2 * this->ProjectionMatrix[1][2]);
    int prime_Z = (f_pts_0 * this->ProjectionMatrix[2][0]) + (f_pts_1 * this->ProjectionMatrix[2][1]) + (f_pts_2 * this->ProjectionMatrix[2][2]);

    // std::cout << prime_X << std::endl << prime_Y << std::endl << std::endl;

    return {prime_X, prime_Y, prime_Z};
}

void Matrix::updateValues(float v11, float  v12, float  v13, float  v21, float  v22, float  v23, float  v31, float  v32, float  v33) {
    ProjectionMatrix[0][0] = v11;
    ProjectionMatrix[0][1] = v12;
    ProjectionMatrix[0][2] = v13;

    ProjectionMatrix[1][0] = v21;
    ProjectionMatrix[1][1] = v22;
    ProjectionMatrix[1][2] = v23;

    ProjectionMatrix[2][0] = v31;
    ProjectionMatrix[2][1] = v32;
    ProjectionMatrix[2][2] = v33;
}

#endif


