import numpy as np 

def draw():
    from matplotlib import pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
   
    X = np.arange(0, 50, 2)
    Y = np.arange(0, 50, 2)
    X, Y = np.meshgrid(X, Y)
    # Zstd=np.loadtxt('./std_tp.txt')
    Zstd=np.loadtxt('./std.txt')
    Zstd=Zstd[range(25),:][:,range(25)]
    Zstd[np.where(Zstd>50)]=0
    Zstd[np.where(Zstd<-50)]=0
    # Zmf=np.loadtxt('./mf_tp.txt')
    Zmf=np.loadtxt('./mf.txt')
    Zmf=Zmf[range(25),:][:,range(25)]
    Zmf[np.where(Zmf>50)]=0
    Zmf[np.where(Zmf<-50)]=0
    Zcf=np.loadtxt('./cf_tp.txt')
    Zcf=np.loadtxt('./cf.txt')
    Zcf=Zcf[range(25),:][:,range(25)]
    Zcf[np.where(Zcf>50)]=0
    Zcf[np.where(Zcf<-50)]=0
    
    fig = plt.figure(1)

    ax = Axes3D(fig)

    # ax.plot_surface(X, Y, Zstd, rstride=1, cstride=1, cmap='rainbow')
    legend=[]
    t=ax.scatter3D(X, Y, Zstd)
    legend.append(t)
    t=ax.scatter3D(X, Y, Zcf,c='r')
    legend.append(t)
    t=ax.scatter3D(X, Y, Zmf,c='g')
    legend.append(t)

    ax.set_zlabel('Absolute Error')
    ax.set_ylabel('services')
    ax.set_xlabel('users')

    ax.set_title('the Absolute Error of points')
    rows=['rwemf-AE','rwe-AE','mf-AE']
    plt.legend(handles = legend, labels = rows, loc = 'upper left')
    plt.show()

if __name__=="__main__":
    draw()
    print "hello world!"