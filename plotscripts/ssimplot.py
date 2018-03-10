import sys
import matplotlib
import numpy as np
#matplotlib.use("agg")
import matplotlib.pyplot as plt

a2a4 = []
a3 = []
ahp = []

a2a4Path = "/home/lucas/a2a4_ssim.csv"
a3Path = "/home/lucas/a3_ssim.csv"
ahpPath = "/home/lucas/ahp_ssim.csv"

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

plt.xlim(0, 300)
plt.ylim(0, 1)
plt.plot(a2a4, label='A2A4', linestyle="-", color='#ed4728')
plt.plot(a3, label="A3", linestyle="--", color='#8dc63f')
plt.plot(ahp, label="AHP", linestyle="-.", color='blue')
#plt.title("SSIM Ao Longo da Transmissão")
plt.ylabel("SSIM")
plt.xlabel("Frame")
plt.legend()
#plt.show()
plt.savefig("ssim_timewise.pdf")
