/********************************************************
 * c_PMF.cpp
 * C++ implements on PMF
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/5/6
 * Last updated: 2014/5/12
 * used by RWEMF to target of matrix factorizatin
********************************************************/

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;
#include "common.h"
#include "c_MF.h"


/********************************************************
* Udata and Sdata are the output values
********************************************************/
void MF(double *removedData, int numUser, int numService, int dim, 
	double lmda, int maxIter, double etaInit, double *Udata, double *Sdata)
{	
	// --- transfer the 1D pointer to 2D array pointer
    double **removedMatrix = vector2Matrix(removedData, numUser, numService);
    double **U = vector2Matrix(Udata, numUser, dim);
    double **S = vector2Matrix(Sdata, numService, dim);
    
    // --- create a set of temporal matries
    double **gradU = createMatrix<double>(numUser, dim);
    double **gradS = createMatrix<double>(numService, dim);

	// --- inital loss value 
	double lossValue = loss(U, S, removedMatrix, lmda, numUser, numService, dim);

    // --- iterate by standard gradient descent algorithm
    int iter, i, j, k; 
    for (iter = 0; iter < maxIter; iter++) {
    	// update gradients
    	gradLoss(U, S, removedMatrix, gradU, gradS, lmda, numUser, numService, dim);

    	// line search to find the best learning rate eta
		double eta = linesearch(U, S, removedMatrix, lossValue, gradU, gradS, 
			etaInit, lmda, numUser, numService, dim);

		// gradient descent updates
		for (k = 0; k < dim; k++) {
			// update U
			for (i = 0; i < numUser; i++) {
				U[i][k] -= (eta * gradU[i][k]+U[i][k]*0.005);
			}
			// update S
			for (j = 0; j < numService; j++) {
				S[j][k] -= (eta * gradS[j][k]+S[j][k]*0.005);
			}
            // if(k==0){
            //     cout << S[0][k]<<" |= "<< gradS[0][k] << endl;
            // }
		}

		// update loss values
		lossValue = loss(U, S, removedMatrix, lmda, numUser, numService, dim);
		if(DEBUG) cout << eta<<" | "<<lossValue << endl;
    }

    deleteMatrix(gradU);
    deleteMatrix(gradS);
    delete ((char*) U);
    delete ((char*) S);
    delete ((char*) removedMatrix);
}


double loss(double **U, double **S, double **removedMatrix, double lmda, 
	int numUser, int numService, int dim)
{
	int i, j, k, g;
	double loss = 0;
	double **predMatrix = createMatrix<double>(numUser, numService);

	// update predMatrix
	U_dot_S(removedMatrix, U, S, numUser, numService, dim, predMatrix);

	// cost
	for (i = 0; i < numUser; i++) {
		for (j = 0; j < numService; j++) {
			if (removedMatrix[i][j] != 0) {
				loss += 0.5 * (removedMatrix[i][j] - predMatrix[i][j])
							* (removedMatrix[i][j] - predMatrix[i][j]);	
			}
		}
	}

	// L2 regularization
	for (k = 0; k < dim; k++) {
		for (i = 0; i < numUser; i++) {
			loss += 0.5 * lmda * U[i][k] * U[i][k];
		}
		for (j = 0; j < numService; j++) {
			loss += 0.5 * lmda * S[j][k] * S[j][k];
		}
	}

    deleteMatrix(predMatrix);
	return loss;
}


void gradLoss(double **U, double **S, double **removedMatrix, double **gradU, 
	double **gradS, double lmda, int numUser, int numService, int dim)
{
	int i, j, k, g;
	double grad = 0;

	// gradU
	for (i = 0; i < numUser; i++) {
		for (k = 0; k < dim; k++) {
			grad = 0;
			for (j = 0; j < numService; j++) {
				if (removedMatrix[i][j] != 0) {
					grad += (removedMatrix[i][j] - dotVector(U[i], S[j], dim)) 
						* (-S[j][k]);
				} 
			}
			grad += lmda * U[i][k];
			gradU[i][k] = grad;
		}
	}

	// gradS
	for (j = 0; j < numService; j++) {
		for (k = 0; k < dim; k++) {
			grad = 0;
			for (i = 0; i < numUser; i++) {
				if (removedMatrix[i][j] != 0) {
					grad += (removedMatrix[i][j] - dotVector(U[i], S[j], dim)) 
						* (-U[i][k]);
				} 
			}
			grad += lmda * S[j][k];
			gradS[j][k] = grad;			
		}
	}

}


double linesearch(double **U, double **S, double **removedMatrix, 
	double lastLossValue, double **gradU, double **gradS, double etaInit, 
	double lmda, int numUser, int numService, int dim)
{
	double eta = etaInit;
	double lossValue;
	double **U1 = createMatrix<double>(numUser, dim);
	double **S1 = createMatrix<double>(numService, dim);

	int iter, i, j, k;
	for (iter = 0; iter < 20; iter++) {
    	// update U1, S1
		for (k = 0; k < dim; k++) {
			// update U
			for (i = 0; i < numUser; i++) {
				U1[i][k] = U[i][k] - eta * gradU[i][k];
			}
			// update S
			for (j = 0; j < numService; j++) {
				S1[j][k] = S[j][k] - eta * gradS[j][k];
			}
		}    

        lossValue = loss(U1, S1, removedMatrix, lmda, numUser, numService, dim);

        if (lossValue <= lastLossValue)
            break;
        eta = eta / 2;
    }

    deleteMatrix(U1);
    deleteMatrix(S1);
    return eta;
}


void U_dot_S(double **removedMatrix, double **U, double **S, int numUser, 
		int numService, int dim, double **predMatrix)
{
	int i, j;
	for (i = 0; i < numUser; i++) {
		for (j = 0; j < numService; j++) {
			if (removedMatrix[i][j] != 0) {
				predMatrix[i][j] = dotVector(U[i], S[j], dim);  
			}
		}
	}
}


