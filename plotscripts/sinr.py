import matplotlib.pyplot as plt
import numpy as np
import sys

def main():
    if (len(sys.argv) == 2):
        imsi = set()
        serving = []
        y = []
        count = 0

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
                    serving[count].append(float(k.split('\t')[4]))
                    y.append(float(k.split('\t')[0]))
            plt.plot(y, serving[count], label='Rsrq (dbm) medido por Ue ' + str(count))
            plt.legend()
            count = count + 1
            if(count == 3):
                break
            y = []
        plt.show()

    else:
        print ("pass DlRsrqSinr log as parameter")
        sys.exit(0)

if __name__ == '__main__':
    main()
