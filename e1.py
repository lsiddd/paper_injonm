"""
========
Barchart
========

A bar plot with errorbars and height labels on individual bars
"""
import numpy as np
import matplotlib.pyplot as plt
import pylab as pyl
#import pickle as p

N = 3
ahp_means = (0.933045, 0.871983, 0.857968)
ahp_std = (0.0337, 0.0214971, 0.0245396)

ind = np.arange(N)  # the x locations for the groups
width = 0.25       # the width of the bars
fig, ax = plt.subplots()
rects1 = ax.bar(ind, ahp_means, width, color='#6497b1', yerr=ahp_std, hatch="/")

a2a4_means = (0.9300, 0.8620, 0.8244)
a2a4_std = (0.0284, 0.0359, 0.02298)
rects2 = ax.bar(ind + width, a2a4_means, width, color='#ff6347', yerr=a2a4_std)

a3_means = (0.9158, 0.8526, 0.8297)
a3_std = (0.03373, 0.03289, 0.02838)
rects3 = ax.bar(ind + width + width, a3_means, width, color='#5fc35c', yerr=a3_std, hatch=".")

# add some text for labels, title and axes ticks
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
ax.set_ylabel('SSIM', fontsize=15)
ax.set_xlabel('Quantidade de dispositivos (carros)', fontsize=15)
# ax.set_title('Algoritmos em diferentes cenarios')
ax.set_xticks(ind + width + width / 3)
ax.set_xticklabels(('5 UE', '20 UE', '40 UE'))
ax.set_yticks(np.arange(0, 1.099, 0.10))
#ax.legend((rects1[0], rects2[1], rects3[2]), ('AHP', 'A2A4', 'A3'))
ax.legend((rects1[0], rects2[1], rects3[2]), ('AHP', 'A2A4', 'A3'), bbox_to_anchor=(0.70,1.01), ncol=3, loc="upper center")



def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)

fig.tight_layout()
pyl.savefig('foo.pdf', bbox_inches='tight')
plt.show()
