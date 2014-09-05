#pragma once

#include <cstring>

namespace mat {

template <class T, int D>
class Matrix
{
public:
    Matrix();
    virtual ~Matrix();

    int size() const;

    const T& operator()(int col, int row) const;
    T& operator()(int col, int row);

protected:
    T m_matrix[D][D];
};

template <class T, int D>
Matrix<T, D>::Matrix() {
    memset(m_matrix, 0, sizeof(m_matrix));
}

template <class T, int D>
Matrix<T, D>::~Matrix() {
}

template <class T, int D>
int Matrix<T, D>::size() const {
    return D;
}

template <class T, int D>
const T& Matrix<T, D>::operator()(int col, int row) const {
    return m_matrix[col][row];
}

template <class T, int D>
T& Matrix<T, D>::operator()(int col, int row) {
    return m_matrix[col][row];
}

} // of namespace mat
