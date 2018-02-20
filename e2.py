"""
========
Barchart
========

A bar plot with errorbars and height labels on individual bars
"""
import numpy as np
import matplotlib.pyplot as plt
import pylab as pyl

N = 3
ahp_means = (13.67, 10.0333, 13.0323)
ahp_std = (10.2254, 7.8717, 6.52778)

ind = np.arange(N)  # the x locations for the groups
width = 0.25       # the width of the bars
fig, ax = plt.subplots()
rects1 = ax.bar(ind, ahp_means, width, color='#6497b1', yerr=ahp_std, hatch="/")

a2a4_means = (166.7407, 301.0344, 680.4)
a2a4_std = (10.6828, 14.0902, 21.6024)
rects2 = ax.bar(ind + width, a2a4_means, width, color='#ff6347', yerr=a2a4_std)

a3_means = (11.7586, 18.633, 52.4666)
a3_std = (0.73945, 1.49673,  2.3596)
rects3 = ax.bar(ind + width + width, a3_means, width, color='#5fc35c', yerr=a3_std, hatch=".")

# add some text for labels, title and axes ticks
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
ax.set_ylabel('Quantidade de Handover', fontsize=15)
ax.set_xlabel('Quantidade de dispositivos (carros)', fontsize=15)
#ax.set_title('Algoritmos em diferentes cenarios')
ax.set_xticks(ind + width + width / 3)
ax.set_yticks(np.arange(0, 701, 50))
ax.set_xticklabels(('5 UE', '20 UE', '40 UE'))

ax.legend((rects1[0], rects2[1], rects3[2]), ('AHP', 'A2A4', 'A3'), bbox_to_anchor=(0.50,1.01), ncol=3, loc="upper center")


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
pyl.savefig('foo2.pdf', bbox_inches='tight')
plt.show()
