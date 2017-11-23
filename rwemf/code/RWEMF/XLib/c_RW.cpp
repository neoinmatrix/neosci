/********************************************************
 * author: neo <neolinjian@gmail.com>
 * created: 2017-11-16
 * algorithm assembled random walk cf with mf 
********************************************************/

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "c_RW.h"
#include "c_MF.h"
#include "common.h"
using namespace std;

void c_predict(double *removedData, double *uMean,
               int numUser, int numService, int topK,  
               int *userinfo,
               double rw_alpha,
               int mf_dim,double mf_lmda,
               int mf_max_iter,double mf_eta_init,
               double ru_mf,
               double *predData)
{
    double **predMatrix = vector2Matrix(predData, numUser, numService);
    double **removedMatrix = vector2Matrix(removedData, numUser, numService);
    int **uinfoMat = vector2Matrix(userinfo, numUser, 3);

    double **pccMatrix = createMatrix<double>(numUser, numUser);
    double **paramsW = createMatrix<double>(numUser, numUser);
    double **transMP = createMatrix<double>(numUser, numUser);
    double **simM = createMatrix<double>(numUser, numUser);
    double **Udata = createMatrix<double>(numUser, mf_dim);
    double **Sdata = createMatrix<double>(numService, mf_dim);

    int i, j;
    for(i=1;i<numUser;i++){
        for(j=0;j<mf_dim;j++){
            Udata[i][j]=double(rand()%100)/double(100);
        }
    }
    for(i=1;i<numService;i++){
        for(j=0;j<mf_dim;j++){
            Sdata[i][j]=double(rand()%100)/double(100);
        }
    }
    if(DEBUG) cout<<"mf"<<endl;

    /*  
        void MF(double *removedData, int numUser, int numService, int mf_dim, 
            double lmda, int maxIter, double etaInit, double *Udata, double *Sdata)
    */
    MF(removedData, numUser,numService,mf_dim,mf_lmda,
        mf_max_iter,mf_eta_init,&Udata[0][0], &Sdata[0][0]);

    double pccValue = 0;
    double tmp=0;
    for (i = 0; i < numUser; i++)
    {
        for (j = 0; j < numUser; j++)
        {
            if(uMean[i] == 0 || uMean[j] == 0) continue;

            if (j > i)
            {
                pccValue=0;
                for(int k=0;k<mf_dim;k++){
                    tmp=Udata[i][k]-Udata[j][k];
                    pccValue+=tmp*tmp;
                }
                pccValue=pccValue/mf_dim;
                pccValue=1/(1+sqrt(pccValue));
                // pccValue = getSim(removedMatrix[i], removedMatrix[j], uMean[i], uMean[j], numService);
                pccMatrix[i][j] = pccValue;
                pccMatrix[j][i] = pccValue;
            }
        }
    }
    if(DEBUG) cout<<"p0"<<endl;
    getParamW(numUser, removedMatrix, pccMatrix, uinfoMat,paramsW);
    if(DEBUG) cout<<"p1"<<endl;
    getTransM(numUser, numService, pccMatrix, paramsW, transMP);
    if(DEBUG) cout<<"p2"<<endl;
    getResolve(numUser, rw_alpha , transMP);
    if(DEBUG) cout<<"p3"<<endl;
    getFinalSim(numUser, transMP, pccMatrix, simM);
    if(DEBUG) cout<<"p4"<<endl;

    double predValue = 0;
    int idx_as, idx_ct;
    double val_as, val_ct;
    for (i = 0; i < numUser; i++)
    {
        vector<int> simUser = getTop(simM[i], numUser, topK, uinfoMat, i);
        idx_as = uinfoMat[i][1];
        idx_ct = uinfoMat[i][2];
        for (j = 0; j < numService; j++)
        {
            if(removedMatrix[i][j] != 0) continue;
            predValue = predict_val(removedMatrix, simM, uMean, i, j, simUser, topK);
            if(predValue <= 0)  predValue = uMean[i] ;
            // predValue = predValue*ru_mf + dotVector(Udata[i],Sdata[j],mf_dim)*(1-ru_mf);
            // predValue =dotVector(Udata[i],Sdata[j],mf_dim);
            predMatrix[i][j] = predValue;
        }
    }

    delete ((char *) predMatrix);
    delete ((char *) removedMatrix);
    delete ((char *) uinfoMat);

    deleteMatrix(pccMatrix);
    deleteMatrix(paramsW);
    deleteMatrix(transMP);
    deleteMatrix(simM);
    deleteMatrix(Udata);
    deleteMatrix(Sdata);
}

void getFinalSim(int numUser, double **transMP, double **pccMatrix, double **simM)
{
    int i, j;
    int count;
    double sum;
    double phi[numUser] = {0};
    for(i = 0; i < numUser; i++)
    {
        count = 0;
        sum = 0;
        for(j = 0; j < numUser; j++)
        {
            if(transMP[i][j] == 0 || pccMatrix[i][j] == 0)continue;
            // if(transMP[i][j] == 0 )continue;
            count++;
            sum += pccMatrix[i][j] / transMP[i][j];
        }
        if(count == 0) continue;
        phi[i] = sum / double(count);
    }
    for(i = 0; i < numUser; i++)
    {
        for(j = i + 1; j < numUser; j++)
        {
            simM[i][j] = (phi[i] * transMP[i][j] + phi[j] * transMP[j][i]) / 2;
            simM[j][i] = simM[i][j];
        }
    }
}

void GetInverse(gsl_matrix *A, gsl_matrix *inverse)
{
    int n = A->size1;
    // cout<< "sizeA"<<A->size1<<endl;
    gsl_matrix *tmpA = gsl_matrix_alloc(n, n);
    gsl_matrix_memcpy(tmpA, A);
    gsl_permutation *p = gsl_permutation_alloc(n);
    int sign = 0;
    gsl_linalg_LU_decomp(tmpA, p, &sign);
    // inverse = gsl_matrix_alloc(n, n);
    gsl_linalg_LU_invert(tmpA, p, inverse);
    gsl_permutation_free(p);
    gsl_matrix_free(tmpA);
}

void  getResolve(int numUser, double rw_alpha , double **transMP)
{
    int i;
    gsl_matrix *m = gsl_matrix_calloc(numUser, numUser);
    m->data = &transMP[0][0];

    gsl_matrix_transpose(m);
    gsl_matrix_scale(m, -1.0 * rw_alpha);
    double tmp;
    for(i = 0; i < numUser; i++)
    {
        tmp = gsl_matrix_get(m, i, i);
        tmp += 1.0;
        gsl_matrix_set (m, i, i, tmp);
    }
    GetInverse(m, m);
    gsl_matrix_scale(m, (1.0 - rw_alpha));
}

void getTransM(int numUser, int numService, double **pccMatrix,
               double **paramsW, double **transM)
{
    int i, j;
    double up, down;
    for(i = 0; i < numUser; i++)
    {
        down = 0;
        for(j = 0; j < numUser; j++)
        {
            if(pccMatrix[i][j] == 0)continue;
            down += paramsW[i][j] * pccMatrix[i][j];
            // down += pccMatrix[i][j];
        }
        if(down == 0)continue;
        for(j = 0; j < numUser; j++)
        {
            up = paramsW[i][j] * pccMatrix[i][j];
            // up = pccMatrix[i][j];
            transM[i][j] = up / down;
        }
    }
}

void getParamW(int numUser, double **removedMatrix,
               double **pccMatrix,int **uinfo, double **paramsW)
{
    int i, j;
    int uas, vas, type;
    double avg;
    for(i = 0; i < numUser; i++)
    {
        for(j = 0; j < numUser; j++)
        {
            if(i >= j)continue;
            type = 1;
            uas = uinfo[i][type];
            vas = uinfo[j][type];
            if(type == 1 && uas != vas)
            {
                type = 2;
            }
            uas = uinfo[i][type];
            vas = uinfo[j][type];
            if(type == 2 && uas != vas){
                type = 3;
            }
            avg = pccMatrix[i][j]*(4-type);
            paramsW[i][j] = avg;
            paramsW[j][i] = avg;
        }
    }
}

vector<int> getTop(double *simarr, int numUser, int topK, int **uinfoMat, int uidx)
{
    vector<pair<int, double> > sim;
    vector<int> users;
    int i = 0;
    for(i = 0; i < numUser; i++)
    {
        if(simarr[i] > 0)
        {
            sim.push_back(pair<int, double>(i, simarr[i]));
        }
    }
 
    for(i = 0; i < numUser; i++)
    {
        if(sim.empty())break;
        vector<pair<int, double> >::iterator it = sim.begin();
        vector<pair<int, double> >::iterator get = sim.begin();
        for(; it != sim.end(); it++)
        {
            if(it->second > get->second)
            {
                get = it;
            }
        }
        users.push_back(get->first);
        sim.erase(get);
    }
    return users;
}

double getSim(double *uA, double *uB, double meanA, double meanB, int numService)
{
    vector<int> commonIndex;
    int i;
    for (i = 0; i < numService; i++)
    {
        if(uA[i] > 0 && uB[i] > 0)
        {
            commonIndex.push_back(i);
        }
    }
    int common_size = commonIndex.size();
    // no common rate items.
    if(common_size < 2) return 0;


    double downall = 0;
    for (i = 0; i < common_size ; i++)
    {
        int key = commonIndex[i];
        double valueA = uA[key];
        double valueB = uB[key];

        // double tempA = valueA - meanA;
        // double tempB = valueB - meanB;

        double tempA = valueA ;
        double tempB = valueB ;

        downall += (tempB - tempA) * (tempB - tempA);
    }
    double downValue = 0;
    downValue = 1.0 + sqrt(downall)/double(common_size);

    if(downValue == 0) return 0;
    return 1.0 / downValue;
}

double predict_val(double **removedMatrix, double **sim, double *u,
                   int uidx, int sidx, vector<int> simUser, int topK)
{
    double sum = 0;
    double down = 0;
    double simvalue = 0;
    int k = 0;
    int suidx = 0;
    double theta = 0.26;
    double avg = 0;
    unsigned int i ;
    for(i = 0; i < simUser.size(); i++)
    {
        suidx = simUser[i];
        if (removedMatrix[suidx][sidx] == 0 || uidx == suidx) continue;
        simvalue = sim[uidx][suidx];
        avg = u[suidx] * theta + u[uidx] * (1 - theta);
        sum += simvalue * (removedMatrix[suidx][sidx] - avg);
        down += simvalue;
        k++;
        if(k >= topK)
        {
            break;
        }
    }
    double tmp=0;
    if(down > 0)
    {
        sum = sum / down + u[uidx];
    }
    return sum;
}
