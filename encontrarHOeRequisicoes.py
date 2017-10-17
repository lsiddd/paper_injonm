#!/usr/bin/python
#include <stdio.h>
import os

def contaTamanho(numPasta, dic):
    contador=0
    for k in dic.keys():
        n = k-2
        dado = 0
        try:
            dado = os.path.getsize("./simul%i/rd_a01_%i"%(numPasta, n))
            #with open("./simul%i/rd_a01_%i"%(numPasta, n), "r") as arquivo:
                #dado=arquivo.read()
            #print n, dado
            if dado!=0:
                dic[k] = 1
                contador = contador+1
            else:
                pass
            #arquivo.close()
        except:
            pass
    return contador

def contaCoisa(aux, dic):
    while(True):
        str2 = aux.find("Client at")
        if str2!=-1:
            aba=aux[str2:].split()[2]
            abax=aba.split('.')[3]
            if abax not in dic.keys():
                dic[int(abax)]=0
            aux = aux[str2+9:] #contar a ocorrencia
        else:
            break

for numLogs in range(0, 117):
    a, numHandover, contadorAntes, contadorDepois, pAntes, pDepois = (0,0,0,0,0,0)
    lista=[]
    nodeAntes={}
    nodeDepois={}
    with open("run%i"%(numLogs), "r") as arquivoDeLog:
        linha=arquivoDeLog.read()
        str1 = linha.find("realizados")
        if(str1!=-1):
            print "Run", numLogs,"-> HO:", linha[str1:].split()[1],
            numHandover = int(linha[str1:].split()[1])
            lista.append(numHandover)
        else:
            print numLogs,"-> HO:", numHandover,
        '''
        indice1 = linha.find("requesting a video streaming")
        indice2 = linha.find("Send packet at")
        if(indice1!=-1 and indice2!=-1): #se existir
            count = contaCoisa(linha[indice1:indice2]) #do indice 1 ao indice 2
            count2 = contaCoisa(linha[indice2:])
        print count, count2
        '''
        indice1 = linha.find("Client at")
        indice2 = linha.find("Send packet at")
        if(indice1!=-1 and indice2!=-1): #se existir
            contaCoisa(linha[indice1:indice2],nodeAntes) #do indice 1 ao indice 2
            contaCoisa(linha[indice2:],nodeDepois)
        #for k in nodeAntes.keys():
        #    print k, nodeAntes[k]
        contadorAntes=contaTamanho(numLogs,nodeAntes)
        contadorDepois=contaTamanho(numLogs,nodeDepois)
        if len(nodeAntes.keys())!=0:
            pAntes = (contadorAntes*100)/float(len(nodeAntes.keys()))
        if len(nodeDepois.keys())!=0:
            pDepois = (contadorDepois*100)/float(len(nodeDepois.keys()))
        #print "| Rec. a:",len(nodeAntes.keys()),"| Rec. d:",len(nodeDepois.keys()),
        print "| Rec. a:",len(nodeAntes.keys()),"| Tx. a:", contadorAntes,"(%.2f%%)"%(pAntes),
        print  "| Rec. d:",len(nodeDepois.keys()),"| Tx. d:", contadorDepois, "(%.2f%%)"%(pDepois)
        #print "| Tx. a:", contadorAntes,"(%.2f%%)"%(pAntes), "| Tx. d:", contadorDepois, "(%.2f%%)"%(pDepois)
    arquivoDeLog.close()
media = sum(lista)/float(len(lista))
print "Media:", media
