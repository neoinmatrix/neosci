#########################################################
# UIPCC.pyx: a python wrapper
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2016/4/27
# Modified by neo: 2017-11-16
#########################################################

import numpy as np
cimport numpy as np # import C-API


#########################################################
# Make declarations on functions from cpp file
#
cdef extern from "c_RW.h":
    void c_predict(double *removedData, double *uMean, 
        int numUser, int numService, int topK, 
        int *userinfo,
        double alpha,
        int mf_dim,double mf_lmda,
        int mf_max_iter,double mf_eta_init,
        double ru_mf,
        double *predData)
    
#########################################################
# Function to perform RWEMF
# return the predicted matrix
def predict(removedMatrix, para):
    cdef int numUser = removedMatrix.shape[0]
    cdef int numService = removedMatrix.shape[1]
    cdef int topK = para['topK']
    cdef np.ndarray[double, ndim=2, mode='c'] predMatrix = \
        np.zeros((numUser, numService), dtype=np.float64)

    userinfo=np.array(para["userinfo"],dtype=np.int32) # must keep, due to int32 to int64 
    uMean = np.sum(removedMatrix, axis=1) / (np.sum(removedMatrix > 0, axis=1) + np.spacing(1)) # avoid divide-by-zero

    c_predict(
        <double *> (<np.ndarray[double, ndim=2, mode='c']> removedMatrix).data, 
    	<double *> (<np.ndarray[double, ndim=1, mode='c']> uMean).data, 
    	numUser,numService,topK, 
        <int *> (<np.ndarray[int, ndim=2, mode='c']> userinfo).data, 
        <double> para['rw_alpha'],
        <int> para['mf_dim'],<double> para['mf_lmda'],
        <int> para['mf_max_iter'],<double> para['mf_eta_init'],
        <double > para['ru_mf'],
    	<double *> predMatrix.data
    )
    return predMatrix
