import numpy as np
import matplotlib.pyplot as plt

def deal(data):
    # print data
    back=[]
    for i in range(4):
        a=data[i]
        b=a.split(' ')
        back.append(b[1])
    return back

def get(file):
    result=[]
    for i in range(1,8):
        tmpf="./topK_%d"%i
        tmpf+=file
        with open(tmpf,'r') as f:
            info=f.read()
            arr=info.split("[Average]\n")
            arrt=arr[1].split("\n")
            result.append(deal(arrt[0:4])) 
    return np.array(result)
    # for v in result:
    #     r=''
    #     for l in v:
    #         r+=l+","
    #     print r
file="dataset#1_rt_result.txt"
r=get(file)
row=range(1,8)
def draw(r):
    rows=["topK_"+str(x) for x in row]
    col=[0.05,0.10,0.15,0.20]
    cols=[str(x*100)+"%" for x in col]
    legend=[]
    count=0
    for v in r:
        count+=1
        if count==3:
            t,=plt.plot(range(4),v,'r-x')
        else:
            t,=plt.plot(range(4),v)
        legend.append(t)
    plt.title("the topK neighbors of dataset")
    plt.xlabel('the density of dataset')
    plt.ylabel(' MAE')
    plt.xticks([0,1,2,3], cols )
    plt.legend(handles = legend, labels = rows, loc = 'best')
    plt.show()
# draw(r)
# exit()

# print "====================="
file="dataset#1_tp_result.txt"
r=get(file)
draw(r)