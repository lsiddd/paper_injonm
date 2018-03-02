import numpy as np
import matplotlib as mpl
import glob
import re
import matplotlib.patches as patches
import matplotlib.path as path

mpl.use('agg')

chart_type = 'bar'

import matplotlib.pyplot as plt

VQM_a2a4 = []
VQM_a3 = []
VQM_ahp = []

VQM_a2a4_aux = []
VQM_a3_aux = []
VQM_ahp_aux = []

VQM_a2a4_std = []
VQM_a3_std = []
VQM_ahp_std = []

for j in range(1, 31):
	#------------------------------PATH WHERE THE FILES ARE------------------------------#
	ahp_path = '/home/lucas/evalvid/final/lucasTestes_h600/ahp/ahp/simul' + str(j) + '/*vqm*'
	a2a4_path = '/home/lucas/evalvid/final/lucasTestes_h600/a2a4/a2a4/simul' + str(j) + '/*vqm*'
	a3_path = '/home/lucas/evalvid/final/lucasTestes_h600/a3/a3/simul' + str(j) + '/*vqm*'


	#---------------------FEED THE LISTS WITH INDIVIDUAL SSIM VAULES---------------------#
	for filename in glob.iglob(ahp_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				VQM_ahp.append(float(i[0][-7:]) / 4)
				VQM_ahp_aux.append(float(i[0][-7:]) / 4)
			else:
				pass

	for filename in glob.iglob(a2a4_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				VQM_a2a4.append(float(i[0][-7:]) / 4)
				VQM_a2a4_aux.append(float(i[0][-7:]) / 4)
			else:
				pass

	for filename in glob.iglob(a3_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				VQM_a3.append(float(i[0][-7:]) / 4)
				VQM_a3_aux.append(float(i[0][-7:]) / 4)
			else:
				pass

	#-----------------TAKE STANDARD DEVIATION FOR INDIVIDUAL SIMULATIONS-----------------#
	if (np.mean(VQM_ahp_aux) == np.mean(VQM_ahp_aux)):
		VQM_ahp_std.append(np.mean(VQM_ahp_aux))
	VQM_ahp_aux = []
	if (np.mean(VQM_a2a4_aux) == np.mean(VQM_a2a4_aux)):
		VQM_a2a4_std.append(np.mean(VQM_a2a4_aux))
	VQM_a2a4_aux = []
	if (np.mean(VQM_a3_aux) == np.mean(VQM_a3_aux)):
		VQM_a3_std.append(np.mean(VQM_a3_aux))
	VQM_a3_aux = []

#--------------------------------PLOT FOR BAR-LIKE GRAP--------------------------------H#
if (chart_type == 'bar'):
	N = 3

	men_means = (	np.float(np.mean(VQM_ahp)),
					np.float(np.mean(VQM_a2a4)),
					np.float(np.mean(VQM_a3))
					)

	men_std = (	np.float(np.std(VQM_ahp_std)),
				np.float(np.std(VQM_a2a4_std)),
				np.float(np.std(VQM_a3_std))
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.35       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, men_means, width, color='orange', yerr=men_std)


	# add some text for labels, title and axes ticks
	ax.set_ylabel('VQM')
	ax.set_title('VQM - Highway 600F')
	ax.set_xticks(ind)
	ax.set_xticklabels(('AHP', 'A2A4', 'A3'))

	#ax.legend(rects1[0], 'Men')


	def autolabel(rects):
	    for rect in rects:
	        height = rect.get_height()

	autolabel(rects1)

	plt.savefig("VQM_barchart_h600.png")

#-----------------------PLOT FOR BOXPLOT-LIKE GRAPH-----------------------#
elif (chart_type == 'boxplot'):
	data_to_plot = [VQM_ahp, VQM_a2a4, VQM_a3]

	fig = plt.figure(1, figsize=(9, 6))
	plt.title("VQM")
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
	fig.savefig('VQM_boxplot.png', bbox_inches='tight')

elif (chart_type == 'histogram'):
	fig, ax = plt.subplots()

	plt.title("VQM HISTOGRAM A2A4")
	n, bins = np.histogram(VQM_a2a4, 50)
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
	plt.savefig('VQM_histogram_a2a4.png')


	fig, ax = plt.subplots()
	plt.title("VQM HISTOGRAM AHP")
	n, bins = np.histogram(VQM_ahp, 50)
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
	plt.savefig('VQM_histogram_ahp.png')

	fig, ax = plt.subplots()
	plt.title("VQM HISTOGRAM A3")
	n, bins = np.histogram(VQM_a3, 50)
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
	plt.savefig('VQM_histogram_a3.png')
