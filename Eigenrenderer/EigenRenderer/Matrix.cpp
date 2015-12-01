// This file contains the definition of the class Matrix

#include "Matrix.h"

// ----------------------------------------------------------------------- default constructor
// a default matrix is an identity matrix

Matrix::Matrix(void) {	
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}

Matrix::Matrix(const double _11, const double _12, const double _13, const double _14,
	const double _21, const double _22, const double _23, const double _24,
	const double _31, const double _32, const double _33, const double _34,
	const double _41, const double _42, const double _43, const double _44)
{
	m[1][1] = _11;
	m[1][2] = _12;
	m[1][3] = _13;
	m[1][4] = _14;

	m[2][1] = _21;
	m[2][2] = _22;
	m[2][3] = _23;
	m[2][4] = _24;

	m[3][1] = _31;
	m[3][2] = _32;
	m[3][3] = _33;
	m[3][4] = _34;

	m[4][1] = _41;
	m[4][2] = _42;
	m[4][3] = _43;
	m[4][4] = _44;

}

// ----------------------------------------------------------------------- copy constructor

Matrix::Matrix (const Matrix& mat) {
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = mat.m[x][y];	
}


// ----------------------------------------------------------------------- destructor

Matrix::~Matrix (void) {}   




// ----------------------------------------------------------------------- assignment operator

Matrix& 
Matrix::operator= (const Matrix& rhs) {
	if (this == &rhs)
		return (*this);

	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = rhs.m[x][y];	

	return (*this);
}


// ----------------------------------------------------------------------- operator*
// multiplication of two matrices

Matrix 
Matrix::operator* (const Matrix& mat) const {
	Matrix 	product;
	
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			double sum = 0.0;

			for (int j = 0; j < 4; j++)
				sum += m[x][j] * mat.m[j][y];
 
			product.m[x][y] = sum;			
		}
	
	return (product);
}


// ----------------------------------------------------------------------- operator/
// division by a scalar

Matrix 
Matrix::operator/ (const double d) const{
	Matrix result;
	for (auto x = 0; x < 4; x++)				
		for (auto y = 0; y < 4; y++)			
			result.m[x][y] = m[x][y] / d;	
	return (result);
}

Matrix Matrix::operator+(const Matrix& mat) const
{
	Matrix result;
	for (auto i = 0; i < 4; ++i) {
		for (auto j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] + mat.m[i][j];
		}
	}
	return result;
}
Matrix Matrix::operator-(const Matrix& mat) const
{
	Matrix result;
	for (auto i = 0; i < 4; ++i) {
		for (auto j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][j] - mat.m[i][j];
		}
	}
	return result;
}

Matrix Matrix::transpose() const
{
	Matrix result;
	for (auto row = 0; row < 4; ++row)
		for (auto column = 0; column < 4; ++column)
			result.m[column][row] = m[row][column];
	return result;
}

bool Matrix::isIdentity() const
{
	for (auto row = 0; row < 4; ++row) {
		for (auto column = 0; column < 4; ++column) {
			if (row == column && m[row][column] != 1.0) {
				return false;
			}
			if (row != column && m[row][column] != 0.0) {
				return false;
			}
		}
	}
	return true;
}



// ----------------------------------------------------------------------- set_identity
// set matrix to the identity matrix

void											
Matrix::set_identity(void) {
    for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}







