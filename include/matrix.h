/********************************************
常用矩阵计算库函数
不要修改
********************************************/
#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;

// 任意类型矩阵类
template <typename Object>
class MATRIX
{
public:
	explicit MATRIX() : array( 0 ) {}

	MATRIX( int rows, int cols):array( rows )
	{
		for( int i = 0; i < rows; ++i )
		{
			array[i].resize( cols );
		}
	}

	MATRIX( const MATRIX<Object>& m ){ *this = m;}

	void resize( int rows, int cols );           // 改变当前矩阵大小
	bool push_back( const vector<Object>& v );   // 在矩阵末尾添加一行数据
	void swap_row( int row1, int row2 );         // 将换两行的数据
	void clear();								 // 矩阵清零

    void zeros(int rows, int cols);				// 设置为全零矩阵

    int  rows() const{ return (int)array.size(); }
	int  cols() const { return rows() ? ((int)array[0].size()) : 0; }
	bool empty() const { return rows() == 0; }        // 是否为空
	bool square() const { return (!(empty()) && rows() == cols()); }  // 是否为方阵
	
	const vector<Object>& operator[](int row) const { return array[row]; } //[]操作符重载 
	vector<Object>& operator[](int row){ return array[row]; }

	int* toi(void);                             // matrix to int
	float* tof(void);                             // matrix to float
	double* tolf( void );                           // matrix to double
	
protected:
	vector< vector<Object> > array;
};

// 改变当前矩阵大小
template <typename Object>
void MATRIX<Object>::resize( int rows, int cols )
{
	int rs = this->rows();
	int cs = this->cols();

	if ( rows == rs && cols == cs )
	{
		return;
	}
	else if ( rows == rs && cols != cs )
	{
		for ( int i = 0; i < rows; ++i )
		{
			array[i].resize( cols );
		}
	}
	else if ( rows != rs && cols == cs )
	{
		array.resize( rows );
		for ( int i = rs; i < rows; ++i )
		{
			array[i].resize( cols );
		}
	}
	else
	{
		array.resize( rows );
		for ( int i = 0; i < rows; ++i )
		{
			array[i].resize( cols );
		}
	}
}

// 矩阵清零
template <typename Object>
void MATRIX<Object>::clear()
{
	int rs = this->rows();
	int cs = this->cols();

	for (int i = 0; i < rs; ++i)
	{
		for (int j = 0; j < cs; ++j)
		{
			array[i][j] = 0;
		}
	}
}

// 改变当前矩阵大小 + 矩阵清零
template <typename Object>
void MATRIX<Object>::zeros(int rows, int cols)
{
    this->resize(rows, cols);
    this->clear();
}

// 在矩阵末尾添加一行
template <typename Object>
bool MATRIX<Object>::push_back( const vector<Object>& v )
{
	if ( rows() == 0 || cols() == (int)v.size() )
	{
		array.push_back( v );
	}
	else
	{
		return false;
	}

	return true;
}

// 将换两行
template <typename Object>
void MATRIX<Object>::swap_row( int row1, int row2 )
{
	if ( row1 != row2 && row1 >=0 &&
		row1 < rows() && row2 >= 0 && row2 < rows() )
	{
		vector<Object>& v1 = array[row1];
		vector<Object>& v2 = array[row2];
		vector<Object> tmp = v1;
		v1 = v2;
		v2 = tmp;
	}
}

// 矩阵转置
template <typename Object>
const MATRIX<Object> trans( const MATRIX<Object>& m )
{
	MATRIX<Object> ret;
	if ( m.empty() ) return ret;

	int row = m.cols();
	int col = m.rows();
	ret.resize( row, col );

	for ( int i = 0; i < row; ++i )
	{
		for ( int j = 0; j < col; ++j )
		{
			ret[i][j] = m[j][i];
		}
	}

	return ret;
}

template <typename Object>
int* MATRIX<Object>::toi(void)
{
	int* result;

	int r = this->rows();
	int c = this->cols();

	result = new int[r * c];

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			result[i*c + j] = (int)array[i][j];
		}
	}
	return result;
}

template <typename Object>
float* MATRIX<Object>::tof(void)
{
	float* result;

	int r = this->rows();
	int c = this->cols();

	result = new float[r * c];

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			result[i*c + j] = (float)array[i][j];
		}
	}
	return result;
}

template <typename Object>
double* MATRIX<Object>::tolf(void)
{
	double* result;

	int r = this->rows();
	int c = this->cols();

	result = new double[r * c];

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			result[i*c + j] = array[i][j];
		}
	}
	return result;
}

//////////////////////////////////////////////////////////
// double类型矩阵类，用于科学计算
// 继承自MATRIX类
// 实现常用操作符重载，并实现计算矩阵的行列式、逆以及LU分解
class Matrix:public MATRIX<double>
{
public:
	Matrix():MATRIX<double>(){}
	Matrix( int c, int r ):MATRIX<double>(c,r){}
	Matrix( const Matrix& m){ *this  = m; }

	const Matrix& operator =( int* a );
	const Matrix& operator =( float* a );
	const Matrix& operator =( double* a );
	const Matrix& operator =( const Matrix& m );
	const Matrix& operator+=( const Matrix& m );
	const Matrix& operator-=( const Matrix& m );
	const Matrix& operator*=( const Matrix& m );
	const Matrix& operator/=( const Matrix& m );
};

bool  operator==(const Matrix& lhs, const Matrix& rhs);        // 重载操作符==
bool  operator!=(const Matrix& lhs, const Matrix& rhs);        // 重载操作符!=

const Matrix operator+(const Matrix& lhs, const float rhs);  // 重载操作符+
const Matrix operator+(const float rhs, const Matrix& lhs);  // 重载操作符+
const Matrix operator-(const Matrix& lhs, const float rhs);  // 重载操作符-
const Matrix operator*(const Matrix& lhs, const float rhs);  // 重载操作符*
const Matrix operator*(const float rhs, const Matrix& lhs);  // 重载操作符*
const Matrix operator/(const Matrix& lhs, const float rhs);  // 重载操作符/

const Matrix operator+(const Matrix& lhs, const Matrix& rhs);  // 重载操作符+
const Matrix operator-(const Matrix& lhs, const Matrix& rhs);  // 重载操作符-
const Matrix operator*(const Matrix& lhs, const Matrix& rhs);  // 重载操作符*
const Matrix operator/(const Matrix& lhs, const Matrix& rhs);  // 重载操作符/
const double det(const Matrix& m);                             // 计算行列式
const double det(const Matrix& m, int start, int end);         // 计算子矩阵行列式
const Matrix abs(const Matrix& m);                             // 计算所有元素的绝对值
const double max(const Matrix& m);                             // 所有元素的最大值
const double max(const Matrix& m, int& row, int& col);          // 所有元素中的最大值及其下标
const double min(const Matrix& m);                             // 所有元素的最小值
const double min(const Matrix& m, int& row, int& col);          // 所有元素的最小值及其下标
const double sum(const Matrix& m);                               // 计算所有元素之和
const Matrix trans(const Matrix& m);                           // 返回转置矩阵
const Matrix submatrix(const Matrix& m, int rb, int re, int cb, int ce);  // 返回子矩阵
const Matrix inverse(const Matrix& m);                         // 计算逆矩阵
const Matrix LU(const Matrix& m);                              // 计算方阵的LU分解
const Matrix readMatrix(istream& in = std::cin);               // 从指定输入流读入矩阵
const Matrix readMatrix(string file);                          // 从文本文件读入矩阵
const Matrix loadMatrix(string file);                          // 从二进制文件读取矩阵
void  printMatrix(const Matrix& m, ostream& out = std::cout);  // 从指定输出流打印矩阵
void  printMatrix(const Matrix& m, string file);                // 将矩阵输出到文本文件
void  saveMatrix(const Matrix& m, string file);                 // 将矩阵保存为二进制文件

void dispMat(const Matrix& mat);


const Matrix zeros(int row, int col);
const Matrix eye(int dim);

#endif
