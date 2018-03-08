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
mType = "SSIM"
#Video = 'Container'
#mType = 'VQM'

#this is pretty much useless
#but kinda cool
xkcdEnabled = False
if (xkcdEnabled):
	plt.xkcd()

#-----------------LISTS WHERE THE VALUES TO BE PLOTTED WILL BE STORED-----------------#
h_ssim_a2a4 = []
h_ssim_a3 = []
h_ssim_ahp = []

h_ssim_a2a4_aux = []
h_ssim_a3_aux = []
h_ssim_ahp_aux = []

h_ssim_a2a4_std = []
h_ssim_a3_std = []
h_ssim_ahp_std = []

c_ssim_a2a4 = []
c_ssim_a3 = []
c_ssim_ahp = []

c_ssim_a2a4_aux = []
c_ssim_a3_aux = []
c_ssim_ahp_aux = []

c_ssim_a2a4_std = []
c_ssim_a3_std = []
c_ssim_ahp_std = []

a_ssim_a2a4 = []
a_ssim_a3 = []
a_ssim_ahp = []

a_ssim_a2a4_aux = []
a_ssim_a3_aux = []
a_ssim_ahp_aux = []

a_ssim_a2a4_std = []
a_ssim_a3_std = []
a_ssim_ahp_std = []

for j in range(1, 31):
	#------------------------------PATH WHERE THE RUSULT FILES ARE------------------------------#
	c_ahp_path = f'/home/lucas/evalvid/final/lucasTestes/ahp/ahp/simul' + str(j) + f'/*{mType.lower()}*'
	c_a2a4_path = f'/home/lucas/evalvid/final/lucasTestes/a2a4/a2a4/simul' + str(j) + f'/*{mType.lower()}*'
	c_a3_path = f'/home/lucas/evalvid/final/lucasTestes/a3/a3/simul' + str(j) + f'/*{mType.lower()}*'

	h_ahp_path = f'/home/lucas/evalvid/h30_/lucasTestes/ahp/ahp/simul' + str(j) + f'/*{mType.lower()}*'
	h_a2a4_path = f'/home/lucas/evalvid/h30_/lucasTestes/a2a4/a2a4/simul' + str(j) + f'/*{mType.lower()}*'
	h_a3_path = f'/home/lucas/evalvid/h30_/lucasTestes/a3/a3/simul' + str(j) + f'/*{mType.lower()}*'

	a_ahp_path = f'/home/lucas/evalvid/akiyo/ahp/ahp/simul' + str(j) + f'/*{mType.lower()}*'
	a_a2a4_path = f'/home/lucas/evalvid/akiyo/a2a4/a2a4/simul' + str(j) + f'/*{mType.lower()}*'
	a_a3_path = f'/home/lucas/evalvid/akiyo/a3/a3/simul' + str(j) + f'/*{mType.lower()}*'


	#---------------------FEED THE LISTS WITH INDIVIDUAL SSIM VAULES---------------------#
	for filename in glob.iglob(c_ahp_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				c_ssim_ahp.append(float(i[0][-7:]))
				c_ssim_ahp_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(c_a2a4_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				c_ssim_a2a4.append(float(i[0][-7:]))
				c_ssim_a2a4_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(c_a3_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				c_ssim_a3.append(float(i[0][-7:]))
				c_ssim_a3_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(h_ahp_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				h_ssim_ahp.append(float(i[0][-7:]))
				h_ssim_ahp_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(h_a2a4_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				h_ssim_a2a4.append(float(i[0][-7:]))
				h_ssim_a2a4_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(h_a3_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				h_ssim_a3.append(float(i[0][-7:]))
				h_ssim_a3_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(a_ahp_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				a_ssim_ahp.append(float(i[0][-7:]))
				a_ssim_ahp_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(a_a2a4_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				a_ssim_a2a4.append(float(i[0][-7:]))
				a_ssim_a2a4_aux.append(float(i[0][-7:]))
			else:
				pass

	for filename in glob.iglob(a_a3_path, recursive=True):
		for i in [re.findall(r'AVG: \d.\d\d\d\d\d',line) for line in open(filename)]:
			if (i != []):
				a_ssim_a3.append(float(i[0][-7:]))
				a_ssim_a3_aux.append(float(i[0][-7:]))
			else:
				pass

	#-----------------TAKE STANDARD DEVIATION FOR INDIVIDUAL SIMULATIONS-----------------#
	if (np.mean(c_ssim_ahp_aux) == np.mean(c_ssim_ahp_aux)):
		c_ssim_ahp_std.append(np.mean(c_ssim_ahp_aux))
	if (np.mean(c_ssim_a2a4_aux) == np.mean(c_ssim_a2a4_aux)):
		c_ssim_a2a4_std.append(np.mean(c_ssim_a2a4_aux))
	if (np.mean(c_ssim_a3_aux) == np.mean(c_ssim_a3_aux)):
		c_ssim_a3_std.append(np.mean(c_ssim_a3_aux))
	#-----------------------------RETURN THE AUX LISTS TO []-----------------------------#
	c_ssim_ahp_aux = []
	c_ssim_a2a4_aux = []
	c_ssim_a3_aux = []

	if (np.mean(h_ssim_ahp_aux) == np.mean(h_ssim_ahp_aux)):
		h_ssim_ahp_std.append(np.mean(h_ssim_ahp_aux))
	if (np.mean(h_ssim_a2a4_aux) == np.mean(h_ssim_a2a4_aux)):
		h_ssim_a2a4_std.append(np.mean(h_ssim_a2a4_aux))
	if (np.mean(h_ssim_a3_aux) == np.mean(h_ssim_a3_aux)):
		h_ssim_a3_std.append(np.mean(h_ssim_a3_aux))
	#-----------------------------RETURN THE AUX LISTS TO []-----------------------------#
	h_ssim_ahp_aux = []
	h_ssim_a2a4_aux = []
	h_ssim_a3_aux = []

	if (np.mean(a_ssim_ahp_aux) == np.mean(a_ssim_ahp_aux)):
		a_ssim_ahp_std.append(np.mean(a_ssim_ahp_aux))
	if (np.mean(a_ssim_a2a4_aux) == np.mean(a_ssim_a2a4_aux)):
		a_ssim_a2a4_std.append(np.mean(a_ssim_a2a4_aux))
	if (np.mean(a_ssim_a3_aux) == np.mean(a_ssim_a3_aux)):
		a_ssim_a3_std.append(np.mean(a_ssim_a3_aux))
	#-----------------------------RETURN THE AUX LISTS TO []-----------------------------#
	a_ssim_ahp_aux = []
	a_ssim_a2a4_aux = []
	a_ssim_a3_aux = []

#--------------------------------PLOT FOR BAR-LIKE GRAPH---------------------------------#
if (chart_type == 'bar'):
	N = 11
	means = (	np.float(np.mean(c_ssim_ahp)),
				np.float(np.mean(h_ssim_ahp)),
				np.float(np.mean(a_ssim_ahp)),
				0,
				np.float(np.mean(c_ssim_a2a4)),
				np.float(np.mean(a_ssim_a2a4)),
				np.float(np.mean(h_ssim_a2a4)),
				0,
				np.float(np.mean(c_ssim_a3)),
				np.float(np.mean(h_ssim_a3)),
				np.float(np.mean(a_ssim_a3))
				)

	std = (		np.float(np.std(c_ssim_ahp_std)),
				np.float(np.std(h_ssim_ahp_std)),
				np.float(np.std(a_ssim_ahp_std)),
				0,
				np.float(np.std(c_ssim_a2a4_std)),
				np.float(np.std(a_ssim_a2a4_std)),
				np.float(np.std(h_ssim_a2a4_std)),
				0,
				np.float(np.std(c_ssim_a3_std)),
				np.float(np.std(h_ssim_a3_std)),
				np.float(np.std(a_ssim_a3_std)),
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 1.0       # the width of the bars

	fig, ax = plt.subplots()
	rects1 = ax.bar(ind, means, width, color=('blue', 'red', 'green', 'white','blue', 'red', 'green', 'white', 'blue', 'red', 'green'), yerr=std, error_kw=dict(ecolor='black', lw=2, capsize=3, capthick=3))

	#-----------SETTING INDIVIDUAL COLORS AND PATTERS FOR EACH BAR-----------#
	labels =('Container', 'HighWay', 'Akiyo', '', '', '', '', '', '', '', '')
	patterns = ('-', '\\','o', '-', '-', '\\', 'o', '-', '-', '\\', 'o')
	#colors = ('#ff8d8d', '#ff4242', '#992727')
	for bar, pattern, label in zip(rects1, patterns, labels):
		#bar.set_color(color)
		bar.set_label(label)
		bar.set_hatch(pattern)


	# add some text for labels, title and axes ticks
	ax.set_ylabel(mType)
	if (mType == "SSIM"):
		ax.set_ylim(0,1)
	else:
		ax.set_ylim(0, 2.5)
	#ax.set_title(f'{mType} - {Video}', y=1.09)
	ax.set_xticks([1, 5, 9])
	ax.set_xticklabels(('AHP', 'A2A4', 'A3'))

	ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='center',
       ncol=3, borderaxespad=0.)
	#ax.legend(rects1[0], 'Men')


	def autolabel(rects):
	    for rect in rects:
	        height = rect.get_height()

	autolabel(rects1)
	#plt.show()
	plt.savefig(f'{mType.lower()}_barchart.pdf')

#-----------------------PLOT FOR BOXPLOT-LIKE GRAPH-----------------------#
elif (chart_type == 'boxplot'):
	data_to_plot = [ssim_ahp, ssim_a2a4, ssim_a3]

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
	fig.savefig(f'ssim_boxplot_{Video}.png', bbox_inches='tight')

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
	plt.savefig(f'ssim_histogram_a2a4_{Video}.pdf')


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
	plt.savefig(f'ssim_histogram_ahp_{Video}.png')

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
	plt.savefig(f'ssim_histogram_a3_{Video}.png')

else:
	raise Exception("Chart Type has to be either: bar, boxplot or histogram.")