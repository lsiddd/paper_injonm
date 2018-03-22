import sys
import matplotlib
import numpy as np
#matplotlib.use("agg")
#matplotlib.rcParams['axes.linewidth'] = 0 #set the value globally
import matplotlib.pyplot as plt

a2a4 = []
a3 = []
ahp = []

a2a4Path = "/home/lucas/AHPbk/a2a4_ssim.csv"
a3Path = "/home/lucas/AHPbk/a3_ssim.csv"
ahpPath = "/home/lucas/AHPbk/ahp_ssim.csv"

def extract(path):
	value = []
	with open (path) as f:
	    content = f.readlines()
	for i in content:
	    try:
	        value.append(float(i))
	    except ValueError:
	        pass
	return value

a2a4 = extract(a2a4Path)
a3 = extract(a3Path)
ahp = extract(ahpPath)

for i in range(260, 280):
	a2a4.append(a2a4[i])
	a3.append(a3[i])

plt.xlim(0, 300)
plt.ylim(0, 1)
plt.plot(ahp, label="SER", linestyle="-.", color='blue', linewidth=2)
plt.plot(a3, label="PBGT", linestyle="--", color='#8dc63f', linewidth=2)
plt.plot(a2a4, label='RSSI - Based', linestyle="-", color='#ed4728', linewidth=2)
#plt.title("SSIM Ao Longo da Transmissão")
plt.ylabel("SSIM", fontsize=15)
plt.xlabel("Frame", fontsize=15)
plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc='center',
       ncol=3, borderaxespad=0., fontsize=14)
plt.tick_params(axis='both', labelsize=15)
plt.show()
#plt.savefig("ssim_timewise.pdf")
