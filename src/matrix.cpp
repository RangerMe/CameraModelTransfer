#include "matrix.h"
#include <iomanip> 

using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cerr;
using std::endl;

const Matrix operator+(const Matrix& lhs, const float rhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;
        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] += rhs;
            }
        }
        return ret;
    }
}

const Matrix operator+(const float rhs, const Matrix& lhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;
        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] += rhs;
            }
        }
        return ret;
    }
}

const Matrix operator-(const Matrix& lhs, const float rhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;
        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] -= rhs;
            }
        }
        return ret;
    }
}

const Matrix operator*(const Matrix& lhs, const float rhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;

        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] *= rhs;
            }
        }
        return ret;
    }
}

const Matrix operator*(const float rhs, const Matrix& lhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;

        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] *= rhs;
            }
        }
        return ret;
    }
}
const Matrix operator/(const Matrix& lhs, const float rhs)
{
    Matrix ret;
    if (lhs.empty())
    {
        return ret;
    }
    else
    {
        ret = lhs;
        int r = ret.rows();
        int c = ret.cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                ret[i][j] /= rhs;
            }
        }
        return ret;
    }
}


const Matrix& Matrix::operator=(int* a)
{
    if (empty())
    {
        return *this;
    }
    else
    {
        int r = rows();
        int c = cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                array[i][j] = a[i*c + j];
            }
        }
        return *this;
    }
}

const Matrix& Matrix::operator=(float* a)
{
    if (empty())
    {
        return *this;
    }
    else
    {
        int r = rows();
        int c = cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                array[i][j] = a[i*c + j];
            }
        }
        return *this;
    }
}

const Matrix& Matrix::operator=(double* a)
{
    if (empty())
    {
        return *this;
    }
    else
    {
        int r = rows();
        int c = cols();

        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                array[i][j] = a[i*c + j];
            }
        }
        return *this;
    }
}

const Matrix& Matrix::operator=(const Matrix& m)
{
    if (empty() == 0 && (rows() != m.rows() || cols() != m.cols()))
    {
        return *this;
    }

    int r = m.rows();
    int c = m.cols();

    if (empty()) resize(r, c);

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            array[i][j] = m[i][j];
        }
    }
    return *this;
}

const Matrix& Matrix::operator+=(const Matrix& m)
{
    if (rows() != m.rows() || cols() != m.cols())
    {
        return *this;
    }

    int r = rows();
    int c = cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            array[i][j] += m[i][j];
        }
    }

    return *this;
}


const Matrix& Matrix::operator-=(const Matrix& m)
{
    if (rows() != m.rows() || cols() != m.cols())
    {
        return *this;
    }

    int r = rows();
    int c = cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            array[i][j] -= m[i][j];
        }
    }

    return *this;
}

const Matrix& Matrix::operator*=(const Matrix& m)
{
    if (cols() != m.rows() || !m.square())
    {
        return *this;
    }

    Matrix ret(rows(), cols());

    int r = rows();
    int c = cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < c; ++k)
            {
                sum += array[i][k] * m[k][j];
            }
            ret[i][j] = sum;
        }
    }

    *this = ret;
    return *this;
}

const Matrix& Matrix::operator/=(const Matrix& m)
{
    Matrix tmp = inverse(m);
    return operator*=(tmp);
}


bool operator==(const Matrix& lhs, const Matrix& rhs)
{
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
    {
        return false;
    }

    for (int i = 0; i < lhs.rows(); ++i)
    {
        for (int j = 0; j < lhs.cols(); ++j)
        {
            if (rhs[i][j] != rhs[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs)
{
    return !(lhs == rhs);
}

const Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix m;
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
    {
        return m;
    }

    m = lhs;
    m += rhs;

    return m;
}

const Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
    Matrix m;
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
    {
        return m;
    }

    m = lhs;
    m -= rhs;

    return m;
}

const Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    Matrix m;
    if (lhs.cols() != rhs.rows())
    {
        return m;
    }

    m.resize(lhs.rows(), rhs.cols());

    int r = m.rows();
    int c = m.cols();
    int K = lhs.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < K; ++k)
            {
                sum += lhs[i][k] * rhs[k][j];
            }
            m[i][j] = sum;
        }
    }

    return m;
}

const Matrix operator/(const Matrix& lhs, const Matrix& rhs)
{
    Matrix tmp = inverse(rhs);
    Matrix m;

    if (tmp.empty())
    {
        return m;
    }

    return m = lhs * tmp;
}

inline static double LxAbs(double d)
{
    return (d >= 0) ? (d) : (-d);
}

inline
static bool isSignRev(const vector<double>& v)
{
    int p = 0;
    int sum = 0;
    int n = (int)v.size();

    for (int i = 0; i < n; ++i)
    {
        p = (int)v[i];
        if (p >= 0)
        {
            sum += p + i;
        }
    }

    if (sum % 2 == 0) // �����ż����˵�������
    {
        return false;
    }
    return true;
}

// ���㷽������ʽ
const double det(const Matrix& m)
{
    double ret = 0.0;

    if (m.empty() || !m.square()) return ret;

    Matrix N = LU(m);

    if (N.empty()) return ret;

    ret = 1.0;
    for (int i = 0; i < N.cols(); ++i)
    {
        ret *= N[i][i];
    }

    if (isSignRev(N[N.rows() - 1]))
    {
        return -ret;
    }

    return ret;
}

// �������ָ���ӷ��������ʽ 
const double det(const Matrix& m, int start, int end)
{
    return det(submatrix(m, start, end, start, end));
}


// �������ת��
const Matrix trans(const Matrix& m)
{
    Matrix ret;
    if (m.empty()) return ret;

    int r = m.cols();
    int c = m.rows();

    ret.resize(r, c);
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            ret[i][j] = m[j][i];
        }
    }

    return ret;
}

// ���������
const Matrix  inverse(const Matrix& m)
{
    Matrix ret;

    if (m.empty() || !m.square())
    {
        return ret;
    }

    int n = m.rows();

    ret.resize(n, n);
    Matrix A(m);

    for (int i = 0; i < n; ++i) ret[i][i] = 1.0;

    for (int j = 0; j < n; ++j)  //ÿһ��
    {
        int p = j;
        double maxV = LxAbs(A[j][j]);
        for (int i = j + 1; i < n; ++i)  // �ҵ���j����Ԫ�ؾ���ֵ�����
        {
            if (maxV < LxAbs(A[i][j]))
            {
                p = i;
                maxV = LxAbs(A[i][j]);
            }
        }

        if (maxV < 1e-20)
        {
            ret.resize(0, 0);
            return ret;
        }

        if (j != p)
        {
            A.swap_row(j, p);
            ret.swap_row(j, p);
        }

        double d = A[j][j];
        for (int i = j; i < n; ++i) A[j][i] /= d;
        for (int i = 0; i < n; ++i) ret[j][i] /= d;

        for (int i = 0; i < n; ++i)
        {
            if (i != j)
            {
                double q = A[i][j];
                for (int k = j; k < n; ++k)
                {
                    A[i][k] -= q * A[j][k];
                }
                for (int k = 0; k < n; ++k)
                {
                    ret[i][k] -= q * ret[j][k];
                }
            }
        }
    }

    return ret;
}

// �������ֵ
const Matrix abs(const Matrix& m)
{
    Matrix ret;

    if (m.empty())
    {
        return ret;
    }

    int r = m.rows();
    int c = m.cols();
    ret.resize(r, c);

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            double t = m[i][j];
            if (t < 0) ret[i][j] = -t;
            else ret[i][j] = t;
        }
    }

    return ret;
}

// ���ؾ�������Ԫ�ص����ֵ
const double max(const Matrix& m)
{
    if (m.empty()) return 0.;

    double ret = m[0][0];
    int r = m.rows();
    int c = m.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j] > ret) ret = m[i][j];
        }
    }
    return ret;
}

// ����������ֵ�������ظ�Ԫ�ص�����
const double max(const Matrix& m, int& row, int& col)
{
    if (m.empty()) return 0.;

    double ret = m[0][0];
    row = 0;
    col = 0;

    int r = m.rows();
    int c = m.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j] > ret)
            {
                ret = m[i][j];
                row = i;
                col = j;
            }
        }
    }
    return ret;
}

// �����������Ԫ����Сֵ
const double min(const Matrix& m)
{
    if (m.empty()) return 0.;

    double ret = m[0][0];
    int r = m.rows();
    int c = m.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j] > ret) ret = m[i][j];
        }
    }

    return ret;
}

// ���������Сֵ�������ظ�Ԫ�ص�����
const double min(const Matrix& m, int& row, int& col)
{
    if (m.empty()) return 0.;

    double ret = m[0][0];
    row = 0;
    col = 0;
    int r = m.rows();
    int c = m.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j] > ret)
            {
                ret = m[i][j];
                row = i;
                col = j;
            }
        }
    }

    return ret;
}


// ���ؾ�������Ԫ��֮��
const double sum(const Matrix& m)
{
    if (m.empty()) return 0.;

    double ret = 0.f;
    int r = m.rows();
    int c = m.cols();

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            ret += m[i][j];
        }
    }
    return ret;
}

// ȡ������ָ��λ�õ��Ӿ��� 
const Matrix submatrix(const Matrix& m, int rb, int re, int cb, int ce)
{
    Matrix ret;
    if (m.empty()) return ret;

    if (rb < 0 || re >= m.rows() || rb > re) return ret;
    if (cb < 0 || ce >= m.cols() || cb > ce) return ret;

    ret.resize(re - rb + 1, ce - cb + 1);

    for (int i = rb; i <= re; ++i)
    {
        for (int j = cb; j <= ce; ++j)
        {
            ret[i - rb][j - cb] = m[i][j];
        }
    }

    return ret;
}


inline static
int max_idx(const Matrix& m, int k, int n)
{
    int p = k;
    for (int i = k + 1; i < n; ++i)
    {
        if (LxAbs(m[p][k]) < LxAbs(m[i][k]))
        {
            p = i;
        }
    }
    return p;
}

// ���㷽�� M �� LU �ֽ�
// ����LΪ�Խ���Ԫ��ȫΪ1����������UΪ�Խ�Ԫ������M����������
// ʹ�� M = LU
// ���ؾ��������ǲ��ִ洢L(�Խ�Ԫ�س���)�������ǲ��ִ洢U(�����Խ���Ԫ��)
const Matrix LU(const Matrix& m)
{
    Matrix ret;

    if (m.empty() || !m.square()) return ret;

    int n = m.rows();
    ret.resize(n + 1, n);

    for (int i = 0; i < n; ++i)
    {
        ret[n][i] = -1.0;
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            ret[i][j] = m[i][j];
        }
    }

    for (int k = 0; k < n - 1; ++k)
    {
        int p = max_idx(ret, k, n);
        if (p != k)              // �����н���
        {
            ret.swap_row(k, p);
            ret[n][k] = (double)p; // ��¼������Ϣ
        }

        if (ret[k][k] == 0.0)
        {
            cout << "ERROR: " << endl;
            ret.resize(0, 0);
            return ret;
        }

        for (int i = k + 1; i < n; ++i)
        {
            ret[i][k] /= ret[k][k];
            for (int j = k + 1; j < n; ++j)
            {
                ret[i][j] -= ret[i][k] * ret[k][j];
            }
        }
    }

    return ret;
}

//---------------------------------------------------
//                      ��ȡ�ʹ�ӡ
//---------------------------------------------------
// ����������ȡ����
const Matrix readMatrix(istream& in)
{
    Matrix M;
    string str;
    double b;
    vector<double> v;

    while (getline(in, str))
    {
        for (string::size_type i = 0; i < str.size(); ++i)
        {
            if (str[i] == ',' || str[i] == ';')
            {
                str[i] = ' ';
            }
            else if (str[i] != '.' && (str[i] < '0' || str[i] > '9')
                && str[i] != ' ' && str[i] != '\t' && str[i] != '-')
            {
                M.resize(0, 0);
                return M;
            }
        }

        istringstream sstream(str);
        v.resize(0);

        while (sstream >> b)
        {
            v.push_back(b);
        }
        if (v.size() == 0)
        {
            continue;
        }
        if (!M.push_back(v))
        {
            M.resize(0, 0);
            return M;
        }
    }

    return M;
}

// ���ı��ļ��������
const Matrix readMatrix(string file)
{
    ifstream fin(file.c_str());
    Matrix M;

    if (!fin)
    {
        cerr << "Error: open file " << file << " failed." << endl;
        return M;
    }

    M = readMatrix(fin);
    fin.close();

    return M;
}

// �����������ָ�������
void printMatrix(const Matrix& m, ostream& out)
{
    if (m.empty())
    {
        return;
    }

    int r = m.rows();
    int c = m.cols();

    int n = 0;              // ����С����ǰ���λ��
    double maxV = max(abs(m));
    while (maxV >= 1.0)
    {
        maxV /= 10;
        ++n;
    }
    if (n == 0) n = 1;    // ������������ֵС��1����С����ǰλ��Ϊ1��Ϊ����0
    int pre = 4;            // С���������λ��
    int wid = n + pre + 3;  // �����ַ�����=n+pre+����λ+С����λ

    out << std::showpoint;
    out << std::setiosflags(std::ios::fixed);
    out << std::setprecision(pre);
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            out << std::setw(wid) << m[i][j];
        }
        out << endl;
    }

    out << std::setprecision(6);
    out << std::noshowpoint;
}

// �������ӡ��ָ���ļ� 
void printMatrix(const Matrix& m, string file)
{
    ofstream fout(file.c_str());
    if (!fout) return;

    printMatrix(m, fout);
    fout.close();
}

// ���������ݴ�Ϊ�������ļ� 
void saveMatrix(const Matrix& m, string file)
{
    if (m.empty()) return;

    ofstream fout(file.c_str(), std::ios_base::out | std::ios::binary);
    if (!fout) return;

    int r = m.rows();
    int c = m.cols();
    char Flag[12] = "MATRIX_DATA";
    fout.write((char*)&Flag, sizeof(Flag));
    fout.write((char*)&r, sizeof(r));
    fout.write((char*)&c, sizeof(c));

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            double t = m[i][j];
            fout.write((char*)&t, sizeof(t));
        }
    }

    fout.close();
}

// �Ӷ������ļ�load����
const Matrix loadMatrix(string file)
{
    Matrix m;

    ifstream fin(file.c_str(), std::ios_base::in | std::ios::binary);
    if (!fin) return m;

    char Flag[12];
    fin.read((char*)&Flag, sizeof(Flag));

    string str(Flag);
    if (str != "MATRIX_DATA")
    {
        return m;
    }

    int r, c;
    fin.read((char*)&r, sizeof(r));
    fin.read((char*)&c, sizeof(c));

    if (r <= 0 || c <= 0) return m;

    m.resize(r, c);
    double t;

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            fin.read((char*)&t, sizeof(t));
            m[i][j] = t;
        }
    }

    return m;
}

void dispMat(const Matrix& mat)
{
	if (mat.empty() == true)
	{
		printf("empty matrix err\n");
		return;
	}
    for (int i = 0; i < mat.rows(); i++)
    {
        for (int j = 0; j < mat.cols(); j++)
        {
            printf("%2.8f, ", mat[i][j]);
        }
        printf("\n");
    }
}

const Matrix zeros(int row, int col)
{
    Matrix ret;
    ret.resize(row, col);
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            ret[i][j] = 0.f;
        }
    }
    return ret;
}

const Matrix eye(int dim)
{
    Matrix ret;
    ret.zeros(dim, dim);
    for (int i = 0; i < dim; ++i)
    {
        ret[i][i] = 1.0f;
    }
    return ret;
}

