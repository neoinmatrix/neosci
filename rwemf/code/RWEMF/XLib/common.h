/**
 * Author: neo <neolinjian@gmail.com>
 * Created: 2017-11-16
 * Target: make the matrix operate easily
**/
#define DEBUG false

#include<iostream>
#include<vector>
#include<cstring>
using namespace std;

template<typename T>
T **vector2Matrix(T *vector, int row, int col){
    T **matrix = new T *[row];
    if (!matrix) {
        cout << "Memory allocation failed in vector2Matrix." << endl;
        return NULL;
    }
    int i;
    for (i = 0; i < row; i++) {
        matrix[i] = vector + i * col;  
    }
    return matrix;
}

template<typename T>
void printMatrix(T **mat,int row,int col){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
}

template<typename T>
void printVector(T *vec,int len){
    for(int i=0;i<len;i++){
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

template<typename T>
T **createMatrix(int row, int col){
    T **matrix = new T *[row];
    matrix[0] = new T[row * col];
    memset(matrix[0], 0, row * col * sizeof(T));
    int i;
    for (i = 1; i < row; i++) {
        matrix[i] = matrix[i - 1] + col;
    }
    return matrix;
}

template<typename T>
void deleteMatrix(T **ptr) {
    delete ptr[0];
    delete ptr;
}

template<typename T>
void copyMatrix(T **dst,T **src,int row,int col=0){
    col=col>0?col:row;
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
            dst[i][j]=src[i][j];
}

template<typename T>
T dotVector(T *va, T *vb, int len){
    T sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

template<typename T>
void dotMatrix(T **mo,  T **mr, int num){
    int i, j;
    for (i = 0; i < num; i++) {
        T *tmp = new T[num];
        for (j = 0; j < num; j++) {
            tmp[j]=mr[j][i];
        }
        for (j = 0; j < num; j++) {
            mr[j][i] = dotVector(mo[j], tmp, num);  
        }
        delete tmp;
    }
}
