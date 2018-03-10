import numpy as np
import matplotlib as mpl
import glob
import re
import matplotlib.patches as patches
import matplotlib.path as path
mpl.use('agg')#this looks ugly, but you have to define the backend before instantiating pyplot
import matplotlib.pyplot as plt
import sys


chart_type = 'bar'
#Video = 'Container'
#mType = 'VQM'

#this is pretty much useless
#but kinda cool
xkcdEnabled = False
if (xkcdEnabled):
	plt.xkcd()

#-----------------LISTS WHERE THE VALUES TO BE PLOTTED WILL BE STORED-----------------#
nHo_a2a4 = []
nHo_a3 = []
nHo_ahp = []

nHo_a2a4_aux = []
nHo_a3_aux = []
nHo_ahp_aux = []

nHo_a2a4_std = []
nHo_a3_std = []
nHo_ahp_std = []

#------------------------------PATH WHERE THE RUSULT FILES ARE------------------------------#
ahp_path = f'/home/lucas/akiyo/lucasTestes/ahp/run*'
a2a4_path = f'/home/lucas/akiyo/lucasTestes/a2a4/run*'
a3_path = f'/home/lucas/akiyo/lucasTestes/a3/run*'


#---------------------FEED THE LISTS WITH INDIVIDUAL SSIM VAULES---------------------#
for filename in glob.iglob(ahp_path, recursive=True):
	print (filename)
	for i in [re.findall(r'Realizados (\d\d|\d)',line) for line in open(filename)]:
		if (i != []):
			nHo_ahp_aux.append(int(i[0]))
	nHo_ahp.append(max(nHo_ahp_aux))
	nHo_ahp_aux = []

for filename in glob.iglob(a2a4_path, recursive=True):
	print (filename)
	for i in [re.findall(r'Realizados (\d\d|\d)',line) for line in open(filename)]:
		if (i != []):
			nHo_a2a4_aux.append(int(i[0]))
	nHo_a2a4.append(max(nHo_a2a4_aux))
	nHo_a2a4_aux = []

for filename in glob.iglob(a3_path, recursive=True):
	print (filename)
	for i in [re.findall(r'Realizados (\d\d|\d)',line) for line in open(filename)]:
		if (i != []):
			nHo_a3_aux.append(int(i[0]))
	nHo_a3.append(max(nHo_a3_aux))
	nHo_a3_aux = []

#-----------------TAKE STANDARD DEVIATION FOR INDIVIDUAL SIMULATIONS-----------------#
if (np.mean(nHo_ahp_aux) == np.mean(nHo_ahp_aux)):
	nHo_ahp_std.append(np.mean(nHo_ahp_aux))
if (np.mean(nHo_a2a4_aux) == np.mean(nHo_a2a4_aux)):
	nHo_a2a4_std.append(np.mean(nHo_a2a4_aux))
if (np.mean(nHo_a3_aux) == np.mean(nHo_a3_aux)):
	nHo_a3_std.append(np.mean(nHo_a3_aux))
#-----------------------------RETURN THE AUX LISTS TO []-----------------------------#
nHo_ahp_aux = []
nHo_a2a4_aux = []
nHo_a3_aux = []

#--------------------------------PLOT FOR BAR-LIKE GRAPH---------------------------------#
if (chart_type == 'bar'):
	N = 3
	means = ( np.float(np.mean(nHo_ahp)),
				  np.float(np.mean(nHo_a2a4)),
				  np.float(np.mean(nHo_a3))
				  )

	std = (	np.float(np.std(nHo_ahp)),
				np.float(np.std(nHo_a2a4)),
				np.float(np.std(nHo_a3))
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.50       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, means, width, color=('blue', '#ed4728', '#8dc63f'), yerr=std, error_kw=dict(ecolor='black', lw=2, capsize=3, capthick=3))

	#-----------SETTING INDIVIDUAL COLORS AND PATTERS FOR EACH BAR-----------#
	labels =('AHP', 'A2A4', 'A3')
	patterns = ('-', '\\', 'o')
	#colors = ('#ff8d8d', '#ff4242', '#992727')
	for bar, pattern, label in zip(rects1, patterns, labels):
		#bar.set_color(color)
		bar.set_label(label)
		bar.set_hatch(pattern)
	ax.set_ylabel("Número de HO")
	ax.set_xticks(ind)
	ax.set_xticklabels(('AHP', 'A2A4', 'A3'))

	#ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
    #   ncol=3, mode="expand", borderaxespad=0.)
	#ax.legend(rects1[0], 'Men')


	def autolabel(rects):
		for rect in rects:
			height = rect.get_height()

	autolabel(rects1)

	plt.savefig(f'n_de_handover.pdf')

#-----------------------PLOT FOR BOXPLOT-LIKE GRAPH-----------------------#
elif (chart_type == 'boxplot'):
	data_to_plot = [nHo_ahp, nHo_a2a4, nHo_a3]

	fig = plt.figure(1, figsize=(9, 6))
	plt.title(f'SSIM - {Video}')
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
	fig.savefig(f'nHo_boxplot_{Video}.png', bbox_inches='tight')

elif (chart_type == 'histogram'):
	fig, ax = plt.subplots()

	plt.title("SSIM HISTOGRAM A2A4")
	n, bins = np.histogram(nHo_a2a4, 50)
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
	plt.savefig(f'nHo_histogram_a2a4_{Video}.png')


	fig, ax = plt.subplots()
	plt.title("SSIM HISTOGRAM AHP")
	n, bins = np.histogram(nHo_ahp, 50)
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
	plt.savefig(f'nHo_histogram_ahp_{Video}.png')

	fig, ax = plt.subplots()
	plt.title("SSIM HISTOGRAM A3")
	n, bins = np.histogram(nHo_a3, 50)
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
	plt.savefig(f'nHo_histogram_a3_{Video}.png')

else:
	raise Exception("Chart Type has to be either: bar, boxplot or histogram.")
