import matplotlib.pyplot as plt
import numpy as np
import sys
import re

def main():
    if (len(sys.argv) == 2):
        imsi = set()
        serving = []
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
                        serving[count].append(k.split('\t')[1])
            plt.plot(serving[count], label='user ' + str(count + 1))
            plt.legend()

            count = count + 1
        plt.show()
    else:
        print ("pass DlRsrqSinr log as parameter")
        sys.exit(0)


if __name__ == '__main__':
    main()
