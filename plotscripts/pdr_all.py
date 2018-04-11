import numpy as np
import matplotlib as mpl
import glob
import re
import matplotlib.patches as patches
import matplotlib.path as path
mpl.use('agg')#this looks ugly, but you have to define the backend before instantiating pyplot
import matplotlib.pyplot as plt
import sys

def file_len(fname):
	i = -1
	with open(fname) as f:
		for i, l in enumerate(f):
			pass
	return i + 1



chart_type = 'bar'
mType = "VQM"
#Video = 'Container'
#mType = 'VQM'

#this is pretty much useless
#but kinda cool
xkcdEnabled = False
if (xkcdEnabled):
	plt.xkcd()

#-----------------LISTS WHERE THE VALUES TO BE PLOTTED WILL BE STORED-----------------#
c_ssim_a2a4 = []
c_ssim_a3 = []
c_ssim_ahp = []

for j in range(1, 31):
	#------------------------------PATH WHERE THE RUSULT FILES ARE------------------------------#
	c_ahp_path = f'/home/lucas/AHPbk/evalvid/final/lucasTestes/ahp/ahp/simul*/rd*'
	c_a2a4_path = f'/home/lucas/AHPbk/evalvid/final/lucasTestes/a2a4/a2a4/simul*/rd*'
	c_a3_path = f'/home/lucas/AHPbk/evalvid/final/lucasTestes/a3/a3/simul*/rd*'


	h_ahp_path = f'/home/lucas/AHPbk/evalvid/h30_/lucasTestes/ahp/ahp/simul*/rd*'
	h_a2a4_path = f'/home/lucas/AHPbk/evalvid/h30_/lucasTestes/a2a4/a2a4/simul*/rd*'
	h_a3_path = f'/home/lucas/AHPbk/evalvid/h30_/lucasTestes/a3/a3/simul*/rd*'

	a_ahp_path = f'/home/lucas/AHPbk/akiyo (1)/lucasTestes/ahp/ahp/simul*/rd*'
	a_a2a4_path = f'/home/lucas/AHPbk/akiyo (1)/lucasTestes/a2a4/a2a4/simul*/rd*'
	a_a3_path = f'/home/lucas/AHPbk/akiyo (1)/lucasTestes/a3/a3/simul*/rd*'

	#---------------------FEED THE LISTS WITH INDIVIDUAL SSIM VAULES---------------------#
	for filename in glob.iglob(c_ahp_path, recursive=True):
		len = file_len(filename) / 614
		if (len > 0.2):
			c_ssim_ahp.append(len)

	for filename in glob.iglob(c_a2a4_path, recursive=True):
		len = file_len(filename) / 614
		if (len > 0.2):
			c_ssim_a2a4.append(len)

	for filename in glob.iglob(c_a3_path, recursive=True):
		len = file_len(filename) / 614
		if (len > 0.2):
			c_ssim_a3.append(len)

	for filename in glob.iglob(h_ahp_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_ahp.append(len)

	for filename in glob.iglob(h_a2a4_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_a2a4.append(len)

	for filename in glob.iglob(h_a3_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_a3.append(len)	

	for filename in glob.iglob(a_ahp_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_ahp.append(len)

	for filename in glob.iglob(a_a2a4_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_a2a4.append(len)

	for filename in glob.iglob(a_a3_path, recursive=True):
		len = file_len(filename) / 839
		if (len > 0.2):
			c_ssim_a3.append(len)

#--------------------------------PLOT FOR BAR-LIKE GRAPH---------------------------------#
if (chart_type == 'bar'):
	N = 3
	print (max(c_ssim_a2a4))
	print (max(c_ssim_a3))
	print (max(c_ssim_ahp))
	means = (	np.float(np.mean(c_ssim_ahp)) ,
				np.float(np.mean(c_ssim_a2a4)) ,
				np.float(np.mean(c_ssim_a3)) 
				)

	std = (		np.float(np.std(c_ssim_ahp)) ,
				np.float(np.std(c_ssim_ahp)) ,
				np.float(np.std(c_ssim_ahp)) 
				)

	ind = np.arange(N)  # the x locations for the groups
	width = 0.5       # the width of the bars

	fig, ax = plt.subplots()

	rects1 = ax.bar(ind, means, width, color=('blue', '#ed4728', '#8dc63f'), yerr=std, error_kw=dict(ecolor='black', lw=2, capsize=3, capthick=3))

	#-----------SETTING INDIVIDUAL COLORS AND PATTERS FOR EACH BAR-----------#
	labels = ('SER', 'RSSI - Based', 'PBGT')
	patterns = ('-', '\\','o')
	#colors = ('#ff8d8d', '#ff4242', '#992727')
	for bar, pattern, label in zip(rects1, patterns, labels):
		#bar.set_color(color)
		bar.set_label(label)
		bar.set_hatch(pattern)


	# add some text for labels, title and axes ticks
	ax.set_ylabel("PDR", fontsize=15)
	ax.set_ylim(0, 1)

	ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='center',
       ncol=3, borderaxespad=0., fontsize=14)

	#ax.set_xticks(range(N))
	#ax.set_xticklabels(('SER', 'RSSI - Based', 'PBGT'), fontsize=15)
	ax.tick_params(
	    axis='x',          # changes apply to the x-axis
	    which='both',      # both major and minor ticks are affected
	    bottom='off',      # ticks along the bottom edge are off
	    top='off',         # ticks along the top edge are off
	    labelbottom='off') # labels along the bottom edge are off
	ax.tick_params(axis='both', labelsize=15)


	def autolabel(rects):
	    for rect in rects:
	        height = rect.get_height()

	autolabel(rects1)
	plt.savefig('pdr_barchart.pdf')
