import numpy as np 
import matplotlib as mpl 

mpl.use('agg')

import matplotlib.pyplot as plt 

nERS = [1.00000,0.76511,1.00000,1.00000,1.00000,0.99654,1.00000,1.00000,1.00000,1.00000,1.00000,0.87681,1.00000,1.00000,0.80776,0.99719,1.00000,1.00000,1.00000]
nESR = [1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,0.78386,0.99401,1.00000,1.00000,1.00000,0.97271]
nRES = [1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,0.79615,0.99306,1.00000,1.00000,1.00000]
nRSE = [1.00000,1.00000,1.00000,1.00000,1.00000,0.99947,0.99993,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,0.99338,0.89796,1.00000,0.77323,1.00000,0.81067,0.98997,1.00000,1.00000,1.00000,0.92301]
nSER = [1.00000,0.99633,1.00000,1.00000,0.99947,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,0.78419,0.97604,1.00000,1.00000,1.00000]
nSRE = [1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,1.00000,0.79696,0.99617,1.00000,1.00000,1.00000,0.79150]

data_to_plot = [nERS, nESR, nRES, nRSE, nSER, nSRE]

fig = plt.figure(1, figsize=(9, 6))
plt.title("SSIM")

ax = fig.add_subplot(111)

bp = ax.boxplot(data_to_plot)

bp = ax.boxplot(data_to_plot, patch_artist=True)

for box in bp['boxes']:
    box.set( color='#7570b3', linewidth=2)
    box.set( facecolor = '#1b9e77' )

for whisker in bp['whiskers']:
    whisker.set(color='#7570b3', linewidth=2)

for cap in bp['caps']:
    cap.set(color='#7570b3', linewidth=2)

for median in bp['medians']:
    median.set(color='#b2df8a', linewidth=2)

for flier in bp['fliers']:
    flier.set(marker='o', color='#e7298a', alpha=0.5)

ax.set_xticklabels(['nERS', 'nESR', 'nRES', 'nRSE', 'nSER', 'nSRE'])

fig.savefig('fig1.png', bbox_inches='tight')