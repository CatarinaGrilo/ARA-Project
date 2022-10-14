import numpy as np
import matplotlib.pyplot as plt



time = np.loadtxt("dados_time.txt")
N = len(time)
time.sort(kind='quicksort')
#probabilidade de cada indice
y = np.arange(N) / float(N) #cdf
y = 1- y #ccdf
# plotting
plt.xlabel('time')
plt.ylabel('CCDF')
  
plt.title('CCDF of the time')
  
plt.plot(time, y)
plt.show()

