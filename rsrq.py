import matplotlib.pyplot as plt
import numpy as np
import sys

def main():
    imsi = set()
    serving = []
    count = 0

    if (len(sys.argv) == 2):
        with open(sys.argv[1]) as f:
            lines = f.readlines()
        for i in lines:
            try:
                imsi.add(int(i.split('\t')[2]))
            except ValueError:
                pass
        for i in imsi:
            serving.append([])
            for k in lines:
                if(k.split('\t')[2] == str(i)):
                    serving[count].append(10 * np.log10(float(k.split('\t')[5])))
            plt.plot(serving[count], label='Rsrq (dbm) medido por Ue ' + str(count))
            plt.legend()
            count = count + 1
        plt.show()
    else:
        print ("pass DlRsrqSinr log as parameter")
        sys.exit(0)

if __name__ == '__main__':
    main()
