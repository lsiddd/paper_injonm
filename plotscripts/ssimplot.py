import sys
import matplotlib.pyplot as plt, numpy as np

if (len(sys.argv) != 2):
    print ("passar o arquivo como parâmetro")
    sys.exit(0)

value = []
with open (sys.argv[1]) as f:
    content = f.readlines()
for i in content:
    try:
        float(i)
        value.append(i)
        print (i)
    except ValueError:
        pass
value = np.array(value)
x = np.arange(5, 15, 10/len(value))
#y = np.arange(0, 1, 1/len(value))
plt.xlim(0.0, 35)
#plt.ylim(0.0, 1)
plt.yticks(np.arange(0, 1, 0.1))
#plt.xticks(np.arange(0, 10, 1/len(value)))
plt.plot(x, value)

plt.show()
