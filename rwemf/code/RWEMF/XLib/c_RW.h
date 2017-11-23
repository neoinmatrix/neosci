/********************************************************
 * UIPCC.h: header file of UIPCC.cpp
 * Author: Jamie Zhu <jimzhu@GitHub>
 * Created: 2014/4/29
 * Last updated: 2014/4/29
 * Modified: neo <neolinjian@gmail.com>
********************************************************/

#include<gsl/gsl_matrix.h>
#include<gsl/gsl_linalg.h>

#include <utility>
#include <vector>
#include <map>
using namespace std;

void c_predict(double *removedData, double *uMean, 
               int numUser, int numService,int topK,  
               int *userinfo,
               double rw_alpha,
               int mf_dim,double mf_lmda,
               int mf_max_iter,double mf_eta_init,
               double ru_mf,
               double *predData);

double getSim(double *uA, double *uB, double meanA, double meanB, int numUser);

vector<int> getTop(double *simarr, int numUser, int topK, int **uinfoMat, int uidx);


double predict_val(double **removedMatrix, double **sim, double *u,
                   int uidx, int sidx, vector<int> simUser, int topK);

void getParamW(int numUser, double **removedMatrix,
               double **pccMatrix,int **uinfo, double **paramsW);

void getTransM(int numUser, int numService, double **pccMatrix,
               double **paramsW, double **transM);

void GetInverse(gsl_matrix *A, gsl_matrix *inverse);

void getResolve(int numUser, double rw_alpha , double **transMP);

void getFinalSim(int numUser, double **transMP, double **pccMatrix, double **simM);