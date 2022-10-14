import numpy as np
import matplotlib.pyplot as plt



width = np.loadtxt("dados_width.txt")
N = len(width)
width.sort(kind='quicksort')
#probabilidade de cada indice
y = np.arange(N) / float(N) #cdf
y = 1- y #ccdf
# plotting
plt.xlabel('Width')
plt.ylabel('CCDF')
  
plt.title('CCDF of the width')
  
plt.plot(width, y, marker='o')
plt.show()

