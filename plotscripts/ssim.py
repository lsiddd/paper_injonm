import numpy as np
import matplotlib as mpl
import glob
import re

mpl.use('agg')

chart_type = 'bar'

import matplotlib.pyplot as plt

ssim_a2a4 = []
ssim_a3 = []
ssim_ahp = []

ahp_path = '/home/lucas/evalvid-tools/final/lucasTestes/ahp/ahp/simul*/*ssim*'
a2a4_path = '/home/lucas/evalvid-tools/final/lucasTestes/a2a4/a2a4/simul*/*ssim*'
a3_path = '/home/lucas/evalvid-tools/final/lucasTestes/a3/a3/simul*/*ssim*'

for filename in glob.iglob(ahp_path, recursive=True):
	for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
		if (i != []):
			ssim_ahp.append(float(i[0][-7:]))
		else:
			pass

for filename in glob.iglob(a2a4_path, recursive=True):
	for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
		if (i != []):
			ssim_a2a4.append(float(i[0][-7:]))
		else:
			pass

for filename in glob.iglob(a3_path, recursive=True):
	for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
		if (i != []):
			ssim_a3.append(float(i[0][-7:]))
		else:
			pass

if (chart_type == 'bar'):
	N = 3

	men_means = (	np.float(np.mean(ssim_ahp)),
					np.float(np.mean(ssim_a2a4)),
					np.float(np.mean(ssim_a3))
					)

	men_std = (	np.float(np.std(ssim_ahp)),
				np.float(np.std(ssim_a2a4)),
				np.float(np.std(ssim_a3))
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, men_means, width, color='orange', yerr=men_std)


	# add some text for labels, title and axes ticks
	ax.set_ylabel('SSIM')
	ax.set_title('SSIM')
	ax.set_xticks(ind)
	ax.set_xticklabels(('AHP', 'A2A4', 'A3'))

	#ax.legend(rects1[0], 'Men')


	def autolabel(rects):
	    for rect in rects:
	        height = rect.get_height()
	        print (height)
	        '''
	        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
	                '%d' % int(height),
	                ha='center', va='bottom')
	        '''

	autolabel(rects1)

	plt.savefig("ssim_barchart.png")
elif (chart_type == 'boxplot'):
	data_to_plot = [ssim_ahp, ssim_a2a4, ssim_a3]

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

	ax.set_xticklabels(['AHP', 'A2-A4-RSRQ', 'A3-RSRP'])
	plt.show()
	fig.savefig('ssim_boxplot.png', bbox_inches='tight')
