########################################################
# run_tp.py: throughput prediction  
# Author: Jamie Zhu <jimzhu@GitHub>
# Created: 2014/2/6
# Last updated: 2016/04/26
# Implemented approach: PMF
########################################################

import __init__
import os, sys, time
import numpy as np
from commons.utils import logger 
from commons import utils
from commons import dataloader
from commons import myloader
import evaluator
 
# parameter config area
para = {
		'dataPath': '/data/wsds/',
		'dataName': 'dataset#1',
		'dataType': 'tp', # set the dataType as 'rt' or 'tp'
		'outPath': 'result/',
		'metrics': ['MAE', 'NMAE', 'RMSE', 'MRE', 'NPRE'], # delete where appropriate
		'density': [0.05,0.10,0.15,0.20],#np.arange(0.05, 0.31, 0.05), # matrix density
		'rounds': 3, # how many runs are performed at each matrix density
		# 'dimension': 10, # dimenisionality of the latent factors
		# 'etaInit': 0.001, # inital learning rate. We use line search
		# 				 # to find the best eta at each iteration
		# 'lambda': 800, # regularization parameter
		# 'maxIter': 300, # the max iterations
		'saveTimeInfo': False, # whether to keep track of the running time
		'saveLog': True, # whether to save log into file
		'debugMode': False, # whether to record the debug info
        'parallelMode': True, # whether to leverage multiprocessing for speedup

        'topK':3     ,# the knn choose top k 
	    'rw_alpha':0.70, # the random walk params 

	    'mf_dim':17,
	    'mf_lmda':30.0,
	    'mf_max_iter':32,
	    'mf_eta_init':0.3,

	    'ru_mf':0.7,    # rate of cf unioned mf 

}


startTime = time.time() # start timing
utils.setConfig(para) # set configuration
logger.info('==============================================')
logger.info('RWEMF: random walk enhanced by matrix factorization')

# load the dataset
dataMatrix = dataloader.load(para)
para["userinfo"]=myloader.getUserInfo(para)

# evaluate QoS prediction algorithm
evaluator.execute(dataMatrix, para)

logger.info('All done. Elaspsed time: ' + utils.formatElapsedTime(time.time() - startTime)) # end timing
logger.info('==============================================')

