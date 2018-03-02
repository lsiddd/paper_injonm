import numpy as np
import matplotlib as mpl
import glob
import re
import matplotlib.patches as patches
import matplotlib.path as path


mpl.use('agg')

chart_type = 'bar'

import matplotlib.pyplot as plt

ssim_a2a4 = []
ssim_a3 = []
ssim_ahp = []

ssim_a2a4_aux = []
ssim_a3_aux = []
ssim_ahp_aux = []

ssim_a2a4_std = []
ssim_a3_std = []
ssim_ahp_std = []

for j in range(1, 31):
	#------------------------------PATH WHERE THE FILES ARE------------------------------#
	ahp_path = '/home/lucas/evalvid/final/lucasTestes_h600/ahp/ahp/simul' + str(j) + '/*ssim*'
	a2a4_path = '/home/lucas/evalvid/final/lucasTestes_h600/a2a4/a2a4/simul' + str(j) + '/*ssim*'
	a3_path = '/home/lucas/evalvid/final/lucasTestes_h600/a3/a3/simul' + str(j) + '/*ssim*'


	#---------------------FEED THE LISTS WITH INDIVIDUAL SSIM VAULES---------------------#
	for filename in glob.iglob(ahp_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				ssim_ahp.append(float(i[0][-7:]))
				ssim_ahp_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(a2a4_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				ssim_a2a4.append(float(i[0][-7:]))
				ssim_a2a4_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(a3_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				ssim_a3.append(float(i[0][-7:]))
				ssim_a3_aux.append(float(i[0][-7:]))
			else:
				pass

	#-----------------TAKE STANDARD DEVIATION FOR INDIVIDUAL SIMULATIONS-----------------#
	if (np.mean(ssim_ahp_aux) == np.mean(ssim_ahp_aux)):
		ssim_ahp_std.append(np.mean(ssim_ahp_aux))
	if (np.mean(ssim_a2a4_aux) == np.mean(ssim_a2a4_aux)):
		ssim_a2a4_std.append(np.mean(ssim_a2a4_aux))
	if (np.mean(ssim_a3_aux) == np.mean(ssim_a3_aux)):
		ssim_a3_std.append(np.mean(ssim_a3_aux))
	#-----------------------------RETURN THE AUX LISTS TO []-----------------------------#
	ssim_ahp_aux = []
	ssim_a2a4_aux = []
	ssim_a3_aux = []

#--------------------------------PLOT FOR BAR-LIKE GRAPH---------------------------------#
if (chart_type == 'bar'):
	N = 3

	men_means = (	np.float(np.mean(ssim_ahp)),
					np.float(np.mean(ssim_a2a4)),
					np.float(np.mean(ssim_a3))
					)

	men_std = (	np.float(np.std(ssim_ahp_std)),
				np.float(np.std(ssim_a2a4_std)),
				np.float(np.std(ssim_a3_std))
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.20       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, men_means, width, color='blue', yerr=men_std)


	# add some text for labels, title and axes ticks
	ax.set_ylabel('SSIM')
	ax.set_title('SSIM - Highway 600F')
	ax.set_xticks(ind)
	ax.set_xticklabels(('AHP', 'A2A4', 'A3'))

	#ax.legend(rects1[0], 'Men')


	def autolabel(rects):
	    for rect in rects:
	        height = rect.get_height()

	autolabel(rects1)

	plt.savefig("ssim_barchart_h600.png")

#-----------------------PLOT FOR BOXPLOT-LIKE GRAPH-----------------------#
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

elif (chart_type == 'histogram'):
	fig, ax = plt.subplots()

	plt.title("SSIM HISTOGRAM A2A4")
	n, bins = np.histogram(ssim_a2a4, 50)
	left = np.array(bins[:-1])
	right = np.array(bins[1:])
	bottom = np.zeros(len(left))
	top = bottom + n
	XY = np.array([[left, left, right, right], [bottom, top, top, bottom]]).T
	barpath = path.Path.make_compound_path_from_polys(XY)
	patch = patches.PathPatch(barpath)
	ax.add_patch(patch)
	ax.set_xlim(left[0], right[-1])
	ax.set_ylim(bottom.min(), top.max())
	plt.savefig('ssim_histogram_a2a4.png')


	fig, ax = plt.subplots()
	plt.title("SSIM HISTOGRAM AHP")
	n, bins = np.histogram(ssim_ahp, 50)
	left = np.array(bins[:-1])
	right = np.array(bins[1:])
	bottom = np.zeros(len(left))
	top = bottom + n
	XY = np.array([[left, left, right, right], [bottom, top, top, bottom]]).T
	barpath = path.Path.make_compound_path_from_polys(XY)
	patch = patches.PathPatch(barpath)
	ax.add_patch(patch)
	ax.set_xlim(left[0], right[-1])
	ax.set_ylim(bottom.min(), top.max())
	plt.savefig('ssim_histogram_ahp.png')

	fig, ax = plt.subplots()
	plt.title("SSIM HISTOGRAM A3")
	n, bins = np.histogram(ssim_a3, 50)
	left = np.array(bins[:-1])
	right = np.array(bins[1:])
	bottom = np.zeros(len(left))
	top = bottom + n
	XY = np.array([[left, left, right, right], [bottom, top, top, bottom]]).T
	barpath = path.Path.make_compound_path_from_polys(XY)
	patch = patches.PathPatch(barpath)
	ax.add_patch(patch)
	ax.set_xlim(left[0], right[-1])
	ax.set_ylim(bottom.min(), top.max())
	plt.savefig('ssim_histogram_a3.png')
