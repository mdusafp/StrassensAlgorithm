#include <iostream>
#include <ctime>

using namespace std;

class matrix {
private:
    int **Array;
    int size;
    int presize;
public:
    matrix();
    matrix(int _size);
//    matrix(const matrix & matr);
//    ~matrix();
    void del();

    int ** getMatrix()const; // return pointer on matrix
    int   getSize()const { return this->size; }
    void setSize(int _size) { size = _size; }
    void setMatrix(); // input the matrix
    void showMatrix();

    friend const matrix operator+ (const matrix& left, const matrix& right);
    friend const matrix operator- (const matrix& left, const matrix& right);
    friend const matrix operator* (const matrix& left, const matrix& right);
    friend matrix& operator+= (matrix& left, const matrix& right);
    friend matrix& operator-= (matrix& left, const matrix& right);
    friend matrix& operator*= (matrix& left, const matrix& right);
    matrix& operator= (const matrix& right);
    friend bool operator!= (const matrix& left, const matrix& right);
    friend bool operator== (const matrix& left, const matrix& right);
};

inline int powerSizeToDegreeOfTwo(int a);
matrix& strassen(int size, matrix& a, matrix& b);
void separation(matrix& a, matrix& a11, matrix& a12, matrix& a21, matrix& a22);
matrix& collect(matrix& c11, matrix& c12, matrix& c21, matrix& c22);

int main(int argc, char *argv[]) {
    srand( time(NULL) );
    int size = 0;
    cout << "enter size of matrix A:\n";
    cin >> size;
    matrix A(size);

    cout << "enter size of matrix B:\n";
    cin >> size;
    matrix B(size);

    size = max(A.getSize(), B.getSize());
    matrix C(size);

    cout << "input the first matrix:\n";
    A.setMatrix();

    cout << "input the second matrix:\n";
    B.setMatrix();

    cout << "A:\n";
    A.showMatrix();
    cout << "B:\n";
    B.showMatrix();

    cout << "simple multiplication of matrix == A * B:\n";
    C = A * B;
    C.showMatrix();

    cout << "Strassen's multiplication of matrix == A * B:\n";
    C = strassen(size, A, B);
    C.showMatrix();

    return 0;
}

bool isPow2(int a) {
    return !(a & (a - 1));
}

inline int powerSizeToDegreeOfTwo(int a) {
    int res = 1;
    while (res < a) res *= 2;
    return res;
}

matrix::matrix() { Array = nullptr; size = 0; presize = 0; }

matrix::matrix(int _size) {
    presize = _size;
    size = isPow2(_size) ? _size : powerSizeToDegreeOfTwo(_size);
    Array = new int *[size];
    for (int i = 0; i < size; i++) {
        Array[i] = new int [size];
        for (int j = 0; j < size; j++) {
            Array[i][j] = 0;
        }
    }
}

//matrix::matrix(const matrix &matr) {
//    cout << "copy constructor was called;)\n";
//    cout << "size = " << matr.getSize() << '\n';
//    cout << "address = " << matr.getMatrix() << '\n';
//    size = matr.size;
//    Array = new int *[size];
//    for (int i = 0; i < size; i++) {
//        Array[i] = new int [size];
//    }
//    this->size = matr.size;
//    cout << this->size << ';' << matr.size << '\n';
//    this->Array = new int *[this->size];
//    for (int i = 0; i < this->size; i++) {
//        this->Array[i] = new int [this->size];
//        for (int j = 0; j < this->size; j++) {
//            try {
//                if (!(Array[i][j] = curr[i][j])) {
//                    throw 1;
//                }
//            } catch(int a) {
//                cout << "Error\n";
//            }
//        }
//    }
//}
//
//matrix::~matrix() {
//    for (int i = 0; i < this->getSize(); i++) {
//        delete this->Array[i];
//    }
//    delete Array;
//}
//
void matrix::del() {
    int** curr = this->getMatrix();
    for (int i = 0; i < this->getSize(); i++) {
        delete curr[i];
    }
    delete curr;

}

int ** matrix::getMatrix()const {
    return this->Array;
}

void matrix::setMatrix() {
    cout << "\nenter the matrix [" << size << " x " << size << "]:\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < presize && j < presize)
//            cin >> Array[i][j];
                Array[i][j] = rand() % 5 + i + j;
//                Array[i][j] = 10 + i + j;
            else
                Array[i][j] = 0;
        }
    }
}

void matrix::showMatrix() {
    cout << "\nfinal matrix of size [" << size << " x " << size << "]:\n\n";
    for (int i = 0; i < this->getSize(); i++) {
        for (int j = 0; j < this->getSize(); j++) {
            cout << this->Array[i][j] << ' ';
        }
        cout << '\n';
    }
}

bool operator== (const matrix& left, const matrix& right) {
    int **_left = left.Array, **_right = right.Array;
    if (left.size != right.size) {
        return false;
    }
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            if (_left[i][j] != _right[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool operator!= (const matrix& left, const matrix& right) {
    int **_left = left.Array, **_right = right.Array;
    if (left.size == right.size) {
        return false;
    }
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            if (_left[i][j] == _right[i][j]) {
                return false;
            }
        }
    }
    return true;
}

const matrix operator+ (const matrix& left, const matrix& right) {
    matrix res(left.size);
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            res.Array[i][j] = left.Array[i][j] + right.Array[i][j];
        }
    }
    return res;
}

matrix& operator+= (matrix& left, const matrix& right) {
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            left.Array[i][j] += right.Array[i][j];
        }
    }
    return left;
}

const matrix operator- (const matrix& left, const matrix& right) {
    matrix res(left.size);
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            res.Array[i][j] = left.Array[i][j] - right.Array[i][j];
        }
    }
    return res;
}

matrix& operator-= (matrix& left, const matrix& right) {
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            left.Array[i][j] -= right.Array[i][j];
        }
    }
    return left;
}

const matrix operator* (const matrix& left, const matrix& right) {
    matrix res(left.size);
    int sum;
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            sum = 0;
            for (int t = 0; t < left.size; t++) {
                sum += left.Array[i][t] * right.Array[t][j];
            }
            res.Array[i][j] = sum;
        }
    }
    return res;
}

matrix& operator*= (matrix& left, const matrix& right) {
    matrix res(left.size);
    int sum;
    for (int i = 0; i < left.size; i++) {
        for (int j = 0; j < left.size; j++) {
            sum = 0;
            for (int t = 0; t < left.size; t++) {
                sum += left.Array[i][t] * right.Array[t][i];
            }
            res.Array[i][j] = sum;
        }
    }
    left = res;
    return left;
}

matrix& matrix::operator= (const matrix& right) {
    if (this == &right) {
        return *this;
    }
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->Array[i][j] = right.Array[i][j];
        }
    }
    return *this;
}

matrix& strassen(int size, matrix& a, matrix& b) {
    matrix res(size), left(size / 2), right(size / 2);
    if (size < 2) {
        res = a * b;
        return res;
    }

    matrix a11(size / 2), a12(size / 2), a21(size / 2), a22(size / 2);
    matrix b11(size / 2), b12(size / 2), b21(size / 2), b22(size / 2);
    matrix c1(size / 2), c2(size / 2), c3(size / 2), c4(size / 2);
    separation(a, a11, a12, a21, a22);
    separation(b, b11, b12, b21, b22);

    left = a11 + a22;
    right = b11 + b22;
    matrix m1 = strassen(size / 2, left, right);

    left = a21 + a22;
    right = b11;
    matrix m2 = strassen(size / 2, left, right);

    left = a11;
    right = b12 - b22;
    matrix m3 = strassen(size / 2, left, right);

    left = a22;
    right = b21 - b11;
    matrix m4 = strassen(size / 2, left, right);

    left = a11 + a12;
    right = b22;
    matrix m5 = strassen(size / 2, left, right);

    left = a21 - a11;
    right = b11 + b12;
    matrix m6 = strassen(size / 2, left, right);

    left = a12 - a22;
    right = b21 + b22;
    matrix m7 = strassen(size / 2, left, right);
    c1 = m1 + m4 - m5 + m7;
    c2 = m3 + m5;
    c3 = m2 + m4;
    c4 = m1 - m2 + m3 + m6;

    res.del(); left.del(); right.del();
    a11.del(); a12.del(); a21.del(); a22.del();
    b11.del(); b12.del(); b21.del(); b22.del();
    m1.del(); m2.del(); m3.del(); m4.del(); m5.del(); m6.del(); m7.del();

    return collect(c1, c2, c3, c4);
}

void separation(matrix& a, matrix& a11, matrix& a12, matrix& a21, matrix& a22) {
    int** A   =   a.getMatrix();
    int** A11 = a11.getMatrix();
    int** A12 = a12.getMatrix();
    int** A21 = a21.getMatrix();
    int** A22 = a22.getMatrix();

    for (int i = 0; i < a.getSize() / 2; i++) {
        for (int j = 0; j < a.getSize() / 2; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + a.getSize() / 2];
            A21[i][j] = A[i + a.getSize() / 2][j];
            A22[i][j] = A[i + a.getSize() / 2][j + a.getSize() / 2];
        }
    }
}

matrix& collect(matrix& c11, matrix& c12, matrix& c21, matrix& c22) {
    matrix c(c11.getSize() * 2);
    int** C   =   c.getMatrix();
    int** C11 = c11.getMatrix();
    int** C12 = c12.getMatrix();
    int** C21 = c21.getMatrix();
    int** C22 = c22.getMatrix();

    for (int i = 0; i < c11.getSize(); i++) {
        for (int j = 0; j < c11.getSize(); j++) {
            C[i][j]                                   = C11[i][j];
            C[i][j + c12.getSize()]                   = C12[i][j];
            C[i + c21.getSize()][j]                   = C21[i][j];
            C[i + c22.getSize()][j + c22.getSize()]   = C22[i][j];
        }
    }

    c11.del(); c12.del(); c21.del(); c22.del();

    return c;
}