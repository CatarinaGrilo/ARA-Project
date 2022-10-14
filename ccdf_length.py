import numpy as np
import matplotlib.pyplot as plt



length = np.loadtxt("dados_length.txt")
N = len(length)
length.sort(kind='quicksort')
#probabilidade de cada indice
y = np.arange(N) / float(N) #cdf
y = 1- y #ccdf
# plotting
plt.xlabel('length')
plt.ylabel('CCDF')
  
plt.title('CCDF of the length')
  
plt.plot(length, y, marker='o')
plt.show()

