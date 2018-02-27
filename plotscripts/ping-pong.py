#!/bin/python

import re, os, sys

verbose = False
plot = False
interval = 6

def parseRun(filename):
		global verbose
		handover = []
		#--------------CONTRUÇÃO DA MATRIZ COM INFOS DE HANDOVER--------------
		if (os.path.isfile(filename)): #LEITURA DO ARQUIVO DE RUN
			with open(filename, 'r') as f:
				data = f.readlines()
				for i in data: #REGEX PARA EXTRAIR INFOS
					if (re.search("doing handover", i)):

						simtime = re.search("^[-+]?([0-9]*\.[0-9]+|[0-9]+)", i)

						imsi = re.search("IMSI ([0-9][0-9]|[0-9])", i)
						imsi = re.search("([0-9][0-9]|[0-9])", imsi.group(0))

						originCell = re.search("CellId ([0-9][0-9]|[0-9])", i)
						originCell = re.search("([0-9][0-9]|[0-9])", originCell.group(0))

						targetCell = re.search("([0-9][0-9]|[0-9])$", i)

						if (verbose):
							print (i)
							print ("tempo do handover:\t" + str(simtime.group(0)))
							print("imsi do nó:\t" + str(imsi.group(0)))
							print("célula origem:\t" + str(originCell.group(0)))
							print("célula alvo:\t" + str(targetCell.group(0)) + "\n\n\n")

						handover.append([float(simtime.group(0)), 
										int(imsi.group(0)), 
										int(originCell.group(0)), 
										int(targetCell.group(0))
										])
		else:
			print ("isto non exsiste")

		return handover

def findPP(handover):
	checked = []
	global interval
	pp = 0
	for i in handover:
		if i[1] not in checked:
			cellList = []
			cellList.append([i[2], 0])
			for k in handover:
				if (k[1] == i[1]):
					cellList.append([k[3], k[0]])
			checked.append(i[1])

			for i in range(len(cellList) - 2):
				if (cellList[i+2][0] == cellList[i][0] and cellList[i+2][1] - cellList[i][1] < interval):
					pp = pp +1
	print (pp)

	return pp

def test():
	#if the calculation is correct, findPP should return 2 for this matrix
	testMatrix=[[18.0 , 6, 11,  1], 
				[18.32, 3, 1 , 11],
				[18.35, 5, 2 ,  3], 
				[18.4 , 6, 1 , 11], 
				[19.32, 3, 11,  1],
				[19.35, 5, 3 ,  2], 
				[21.0 , 6, 11,  1], 
				[21.08, 3, 1 , 11],
				[30.0 , 5, 2 ,  3]]

	if (findPP(testMatrix) != 2):
		raise ValueError("k' something's vry wrong here")

def main():
	global testEnabled

	if (len(sys.argv) == 1):
		raise Exception("Passar arquivo de run como parâmetro")
	
	if(sys.argv[1] == "--test"):
		test()

	else:
		findPP(parseRun(sys.argv[1]))

if (__name__ == "__main__"):
	main()