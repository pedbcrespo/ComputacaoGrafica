#ifndef MATRIX_H
#define MATRIX_H

#include <cstdio>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

template<int M, int N=M>
struct Mat{
    float v[M*N];

    float& operator()(int i, int j){
        return v[i*N + j];
    }
    
    float operator()(int i, int j) const{
        return v[i*N + j];
    }
    
    Mat operator+(const Mat& B) const{
        Mat R;
        for(int i = 0; i < M*N; i++)
            R.v[i] = v[i] + B.v[i];
        return R;
    }
    
    Mat operator-(const Mat& B) const{
        Mat R;
        for(int i = 0; i < M*N; i++)
            R.v[i] = v[i] - B.v[i];
        return R;
    }
    
    Mat operator-() const{
        Mat R;
        for(int i = 0; i < M*N; i++)
            R.v[i] = -v[i];
        return R;
    }

    template<int P>
    Mat<M, P> operator*(const Mat<N, P>& B) const{
        const Mat& A = *this;
        Mat<M, P> R;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < P; j++){
                R(i, j) = 0;
                for(int k = 0; k < N; k++)
                    R(i, j) += A(i, k)*B(k, j);
            }
        return R;
    }
    
    Mat<M,1> get_column(int j) const{
        const Mat& A = *this;
        Mat<M,1> C;
        for(int i = 0; i < M; i++)
            C(i, 0) = A(i, j);
        return C;
    }

    float& operator[](int j){
        return v[j];
    }
    
    float operator[](int j)const{
        return v[j];
    }

    void set_column(const Mat<M,1>& C, int j){
        Mat& A = *this;

        for(int i = 0; i < M; i++)
            A(i, j) = C(i, 0);
    }

    friend void print(const Mat& A){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++)
                printf("%10.4f ", A(i, j));
            printf("\n");
        }
    }
    friend void print(std::string name, const Mat& A){
        printf("%s = ", name.c_str());
        if(M > 1)
            puts("");
        print(A);
    }

    friend Mat<N, M> transpose(const Mat& A){
        Mat<N, M> r;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                r(j, i) = A(i, j);
        return r;
    }
    
    friend Mat operator*(float x, const Mat& A){
        Mat r = A;
        for(float& v: r.v)
            v *= x;
        return r;
    }
    
    friend float norm2(const Mat& A){
        float r = 0;
        for(float x: A.v)
            r += x*x;
        return r;
    }
    
    friend float norm(const Mat& A){
        return sqrt(norm2(A));
    }
};

using mat2 = Mat<2>;
using mat3 = Mat<3>;
using mat4 = Mat<4>;

using mat2x2 = Mat<2,2>;
using mat2x3 = Mat<2,3>;
using mat2x4 = Mat<2,4>;

using mat3x2 = Mat<3,2>;
using mat3x3 = Mat<3,3>;
using mat3x4 = Mat<3,4>;

using mat4x2 = Mat<4,2>;
using mat4x3 = Mat<4,3>;
using mat4x4 = Mat<4,4>;

template<int M>    
Mat<M> Id(){
    Mat<M> R;
    for(int i = 0; i < M; i++)
        for(int j = 0; j < M; j++)
            R(i, j) = (i==j);
    return R;
}

template<int M>
Mat<M-1> submatrix(const Mat<M>& A, int i0, int j0){
    Mat<M-1> R;
    int k = 0;
    for(int i = 0; i < M; i++){
        if(i == i0)
            continue;

        for(int j = 0; j < M; j++){
            if(j == j0)
                continue;

            R.v[k] = A(i, j);
            k++;
        }
    }
    
    return R;
}

template<int M>
float determinant(const Mat<M>& A);

template<int M>
float cofactor(const Mat<M>& A, int i, int j){
    float r = determinant(submatrix(A, i, j));
    if((i+j)%2!=0)
       r = -r;
    return r;
}

inline float determinant(const Mat<1>& A){
    return A(0, 0);
}

inline float determinant(const mat2& A){
    return A(0, 0)*A(1,1) - A(1, 0)*A(0, 1);
}

inline float determinant(const mat3& A){
    return A(0, 0)*(A(1,1)*A(2,2) - A(1, 2)*A(2,1))
          -A(1, 0)*(A(0,1)*A(2,2) - A(0, 2)*A(2, 1))
          +A(2, 0)*(A(0,1)*A(1,2) - A(0, 2)*A(1,1));
}

template<int M>
float determinant(const Mat<M>& A){
    float det = 0;
    for(int i = 0; i < M; i++)
        det += A(i, 0)*cofactor(A, i, 0);
    return det;
}

template<int M>
Mat<M> cofactor_matrix(const Mat<M>& A){
    Mat<M> R;
    for(int i = 0; i < M; i++)
        for(int j = 0; j < M; j++)
            R(i,j) = cofactor(A, i, j);
    return R;
}

template<int M>    
Mat<M> inverse(const Mat<M>& A){
    float det = determinant(A);
    if(fabs(det) < 1e-5)
        return {NAN};

    Mat<M> adjoint = transpose(cofactor_matrix(A));

    return 1/det*adjoint;
}

inline mat4 toMat4(mat3 A){
	return {
        A(0, 0), A(0, 1), A(0, 2), 0,
        A(1, 0), A(1, 1), A(1, 2), 0,
        A(2, 0), A(2, 1), A(2, 2), 0,
		      0,       0,       0, 1
	};
}

inline mat3 toMat3(mat4 A){
	return {
        A(0, 0), A(0, 1), A(0, 2), 
        A(1, 0), A(1, 1), A(1, 2), 
        A(2, 0), A(2, 1), A(2, 2)
	};
}

#endif