import numpy as np
import random
from matplotlib import pyplot as plt

data = np.array([11,1,1,1,1,1,1,1,1,1,2]) # list of values

# fixed bin size
bins = np.arange(-100, 100, 5) # fixed bin size

plt.xlim([min(data)-5, max(data)+5])

plt.hist(data, bins=bins, alpha=0.5)
plt.xlabel('Distribuição de SSIM'
plt.ylabel('Número de Ocorrências')

plt.show()
