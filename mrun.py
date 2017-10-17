from subprocess import call
import os

count = 0
var = 0

while (var < 30):
    os.makedirs('simul' + str(count))
    os.system('NS_GLOBAL_VALUE="RngRun=' + str(count) + '" ./waf --run scratch/v2x_3gpp > run$count 2>&1')
    os.system('mv rd* sd* V2X/*')

    if (os.path.exists("simul" + str(count) + "rd_a01_0")):
    	var += 1
    count += 1
