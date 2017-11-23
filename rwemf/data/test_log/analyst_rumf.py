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

def get(file,front='./std_rt_rumf_'):
    result=[]
    for i in [0.1,0.3,0.5,0.7,0.9]:
        tmpf=front+str(i)
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
# file="dataset#1_rt_result.txt"
# r=get(file,'./std_rt_rumf_')
# r_cf=get(file,'./cf_rt_rumf_')
# r_mf=get(file,'./mf_rt_rumf_')
# print r
# print r_cf
# print r_mf

def draw(r,r_cf,r_mf):
    col=[0.1,0.3,0.5,0.7,0.9]
    color=['b','g','y','r']
    legend=[]
    for i in [0,3]:
        t,=plt.plot(range(len(col)),r[:,i],color[i]+"-")
        legend.append(t)

    for i in [0,3]:
        t,=plt.plot(range(len(col)),r_cf[:,i],color[i]+"-.")
        legend.append(t)

    for i in [0,3]:
        t,=plt.plot(range(len(col)),r_mf[:,i],color[i]+"-o")
        legend.append(t)
    rows=['5% -rwemf','20%-rwemf','5% -rwe','20%-rwe','5% -mf','20%-mf']

    plt.title("the parameter ru_mf")
    plt.xlabel('the rate of ru_mf')
    plt.ylabel(' MAE ')
    plt.xticks([0,1,2,3,4], col )
    plt.legend(handles = legend, labels = rows, loc = 'center left')
    plt.show()

# exit()

# print "====================="

file="dataset#1_tp_result.txt"
r=get(file,'./std_tp_rumf_')
r_cf=get(file,'./cf_tp_rumf_')
r_mf=get(file,'./mf_tp_rumf_')
print r
print r_cf
print r_mf

draw(r,r_cf,r_mf)