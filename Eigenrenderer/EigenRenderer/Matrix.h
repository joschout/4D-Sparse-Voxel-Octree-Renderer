#ifndef __MATRIX__
#define __MATRIX__

class Matrix
{
public:

	double m[4][4]; // elements

	
	Matrix(void); // default constructor

	Matrix(const double _11, const double _12, const double _13, const double _14,
		const double _21, const double _22, const double _23, const double _24,
		const double _31, const double _32, const double _33, const double _34,
		const double _41, const double _42, const double _43, const double _44);

	Matrix(const Matrix& mat); // copy constructor

	~Matrix(void); // destructor

	Matrix& // assignment operator
	operator =(const Matrix& rhs);

	Matrix // multiplication of two matrices
	operator *(const Matrix& mat) const;

	Matrix // divsion by a double
	operator /(const double d) const;

	Matrix
	operator +(const Matrix& mat) const;

	Matrix operator-(const Matrix& mat) const;

	Matrix transpose() const;



	void // set to the identity matrix
	set_identity(void);

	bool isIdentity() const;


};




#endif


