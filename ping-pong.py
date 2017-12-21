import re, os, sys

#([0-9]|[0-9][0-9])\.([0-9])\w+ simtime
#IMSI ([0-9]|[0-9][0-9])   --->> ([0-9]|[0-9][0-9])
#CellId ([0-9]|[0-9][0-9]) --->> ([0-9]|[0-9][0-9])
handover = []
pp = 0
verbose = False
checked = []
if (len(sys.argv) > 1):
	filename = sys.argv[1]
	#--------------CONTRUÇÃO DA MATRIZ COM INFOS DE HANDOVER--------------
	if (os.path.isfile(filename)): #LEITURA DO ARQUIVO DE RUN
		with open(filename, 'r') as f:
			data = f.readlines()
			for i in data: #REGEX PARA EXTRAIR INFOS
				if (re.search("doing handover", i)):
					
					simtime = re.search("([0-9]|[0-9][0-9])\.([0-9])\w+", i)

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

					handover.append([simtime.group(0), imsi.group(0), originCell.group(0), targetCell.group(0)])
	else:
		print ("isto non exsiste")
	#------------------------------------------------------------------------

	#-------------------------DETECÇÃO DE PING-PONG--------------------------
	for i in handover:
		origin = i[2]
		for u in handover:
			if (i[1] == u[1]):
				if (float(u[0]) - float(i[0]) < 6 and
					float(u[0]) - float(i[0]) > 0 and
					origin == u[3]):
					if i[1] not in checked:
						pp = pp + 1
				origin = u[2]
		checked.append(i[1])

	#------------------------------------------------------------------------
else:
	print ("passar o nome do arquivo como parâmetro")
print("realizados " + str(pp) + " handover ping pong")
