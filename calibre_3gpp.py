#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import re
import sys


def run():
    for i in range(16, 31):
        os.system('rm qosTorre.txt qoeTorre.txt')
        cmd = 'NS_GLOBAL_VALUE="RngRun='+ str(i) \
            + '" ./waf --run scratch/v2x_3gpp'
        os.system(cmd)
        # rgParse()
        os.system('mkdir simul' + str(i) + '; mv wd* qo* Dl* Ul* ueR* rd* sd* simul' +str(i))

#+ ' --entradaSumo=sumoTest/mobilityGrid50UesV' + str(i) \

def maiorQoe():
    for i in range(0, 16):
        fname = 'qoeTorre' + str(i) + '.txt'
        with open(fname) as f:
            content = f.readlines()
            content = [x.strip() for x in content]
            floatList = [float(i) for i in content]
            listBigEelements = max(floatList)

            # average = sum(listBigEelements)/float(len(listBigEelements))

            print(listBigEelements)


def mediaQoe():
    for i in range(0, 16):
        fname = 'qoeTorre' + str(i) + '.txt'
        with open(fname) as f:
            content = f.readlines()
            content = [x.strip() for x in content]
            floatList = [float(i) for i in content]
            average = sum(floatList) / float(len(floatList))
            print(average)


def rgParse():
    filename = 'DlRsrpSinrStats.txt'
    pattern = "\t\d{1,2}\t\d{1,2}\t"
    counter = 0
    dic = set()

    with open(filename, 'r') as f:
        lines = f.readlines()
    for line in lines:
        match = re.search(pattern, line)
        if match:
            new_line = match.group() + '\n'
            tempList = [int(s) for s in new_line.split() if s.isdigit()]
            for i in range(0, 51):
                dic.add(tuple (tempList))

    towers = sorted(list (dic), key=lambda tup: tup[1])
    print (towers)

    # with open('qoeTorre.txt', 'r') as f:
    #     lines = f.readlines()
    # for i in range (0, 50):
    #     os.system('echo ' + lines[i][:9] + ' >> qoeTorre' + str (towers[i][0] -1) + '.txt')

    with open('qosTorre.txt', 'r') as f:
        lines = f.readlines()
    qosList = [line.replace('\n', '').replace('\t', ' ')[:9] for line in lines if line.replace('\n', '').replace('\t', ' ')[15:][0] != '.' ]
    print (qosList)
    for i in [int (line.replace('\n', '').replace('\t', ' ')[15:]) - 1 for line in lines if line.replace('\n', '').replace('\t', ' ')[15:][0] != '.' ]:
        os.system('echo ' + qosList[counter][:9] + ' >> qosTorre' + str (towers[i-1][0] -1) + '.txt')
        counter = counter + 1


nT = 77
nV = 2

if (len (sys.argv)) > 1:
    if (sys.argv[1] == '--parse'):
        rgParse()
    elif (sys.argv[1] == '--run'):
        os.system('rm -rf simul*')
        run()
    else:
        print ('Opcao invalida')
else:
    print ("Requer argumento")
