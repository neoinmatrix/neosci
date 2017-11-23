import numpy as np
import matplotlib.pyplot as plt

def deal(data):
    # print data
    mae=[]
    nmae=[]
    for i in range(4):
        a=data[i]
        b=a.split(' ')
        mae.append(b[1])
        nmae.append(b[3])
    return mae,nmae

def get(file):
    result=[]
    # for i in range(1,8):
        # tmpf="./topK_%d"%i
    tmpf=file
    with open(tmpf,'r') as f:
        info=f.read()
        arr=info.split("[Average]\n")
        arrt=arr[1].split("\n")
        return deal(arrt[0:4])

def printx(r):
    t=""
    for v in r[0]:
        t+=" & %s"%v 
    print t
    t=""
    for v in r[1]:
        t+=" & %s"%v 
    print t

file="dataset#1_rt_result.txt"
r=get(file)
printx(r)

file="dataset#1_tp_result.txt"
r=get(file)
printx(r)
