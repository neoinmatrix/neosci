########################################################
# Description: the main process to control the evaluations 
# Author: Jamie Zhu <jimzhu@GitHub>
# License: MIT
# Last updated: 2016/05/04
# Modified: neo <neolinjian@gmail.com>
# Last updated: 2017-11-16
# The single program tests the algorithm 
########################################################

from __init__ import *
import numpy as np
import time

from commons import evallib
from commons import dataloader
from commons import myloader
from XLib import RWEMF

def executeOneSetting(matrix, density, roundId, para):
    (trainMatrix, testMatrix) = evallib.removeEntries(matrix, density, roundId)
    print "begin====="
    predictedMatrix = RWEMF.predict(trainMatrix, para)
    evalResult = evallib.evaluate(testMatrix, predictedMatrix, para)
    print evalResult

if __name__=="__main__":
    # parameter config area
    para = {
        'dataPath': '/data/wsds/',
        'dataName': 'dataset#1',
        'dataType': 'rt', # set the dataType as 'rt' or 'tp'
        'outPath': 'result/',
        'metrics': ['MAE', 'NMAE', 'RMSE', 'MRE', 'NPRE'], # delete where appropriate
        # 'density': np.arange(0.05, 0.31, 0.05), # matrix density
        'density': [0.05],#np.arange(0.05, 0.15, 0.05), # matrix density
        'rounds': 3, # how many runs are performed at each matrix density

        'saveTimeInfo': False, # whether to keep track of the running time
        'saveLog': True, # whether to save log into file
        'debugMode': False, # whether to record the debug info
        'parallelMode': True ,# whether to leverage multiprocessing for speedup

        'topK':3     ,# the knn choose top k 
        'rw_alpha':0.70, # the random walk params 

        'mf_dim':17,
        'mf_lmda':30.0,
        'mf_max_iter':32,
        'mf_eta_init':0.3,

        'ru_mf':0.7,    # rate of cf unioned mf 
    }

    print "start======"
    startTime = time.time()
    dataMatrix = dataloader.load(para)
    para["userinfo"]=myloader.getUserInfo(para)
    # para["wsinfo"]=myloader.getWsInfo(para)
    executeOneSetting(dataMatrix,0.05,0, para)
    runningTime = float(time.time() - startTime)
    print "runningTime",runningTime
    print "over======"
