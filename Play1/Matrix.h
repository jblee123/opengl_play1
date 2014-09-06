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

    void clear();
    void setIdentity();

    const T& operator()(int col, int row) const;
    T& operator()(int col, int row);

    const T* getBuf() const;

protected:
    T m_matrix[D][D];
};

template <class T, int D>
Matrix<T, D>::Matrix() {
    clear();
}

template <class T, int D>
Matrix<T, D>::~Matrix() {
}

template <class T, int D>
int Matrix<T, D>::size() const {
    return D;
}

template <class T, int D>
void Matrix<T, D>::clear() {
    memset(m_matrix, 0, sizeof(m_matrix));
}

template <class T, int D>
void Matrix<T, D>::setIdentity() {
    clear();
    for (int i = 0; i < D; i++) {
        m_matrix[i][i] = 1;
    }
}

template <class T, int D>
const T& Matrix<T, D>::operator()(int col, int row) const {
    return m_matrix[col][row];
}

template <class T, int D>
T& Matrix<T, D>::operator()(int col, int row) {
    return m_matrix[col][row];
}

template <class T, int D>
const T* Matrix<T, D>::getBuf() const {
    return (T*)m_matrix;
}

} // of namespace mat
