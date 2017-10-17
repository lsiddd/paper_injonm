/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Joahannes Costa <joahannes@gmail.com>
 * Modified by: Fábio Araújo <fabioaraujo289@gmail.com>
 * Modified by: Iago Medeiros <iagolmedeiros@gmail.com>
 */

#include "ns3/core-module.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/network-module.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/evalvid-client-server-helper.h"

//Monitor de fluxo
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/gnuplot.h"

#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/boolean.h>
#include <ns3/enum.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

//NS2 Mobility
#include <iostream>
#include <fstream>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"

// Bibliotecas do Random Forest
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#define SIMULATION_TIME_FORMAT(s) Seconds(s)

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("V2vExample");

void ThroughputMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset){

  double tempThroughput = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024);
      dataset.Add((double)Simulator::Now().GetSeconds(), (double)tempThroughput);
  }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&ThroughputMonitor, fmhelper, monitor, dataset);
}

void DelayMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset1){
  
  double delay = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      //Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      delay = stats->second.delaySum.GetSeconds ();
      dataset1.Add((double)Simulator::Now().GetSeconds(), (double)delay);
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&DelayMonitor, fmhelper, monitor, dataset1);
}

void LostPacketsMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset2){
  
  double packets = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      //Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      packets = stats->second.lostPackets;
      dataset2.Add((double)Simulator::Now().GetSeconds(), (double)packets);
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&LostPacketsMonitor, fmhelper, monitor, dataset2);
}

void JitterMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset3){
  
  double jitter = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      //Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      jitter = stats->second.jitterSum.GetSeconds ();
      dataset3.Add((double)Simulator::Now().GetSeconds(), (double)jitter);
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&LostPacketsMonitor, fmhelper, monitor, dataset3);
}

void ImprimeMetricas (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor){
  double tempThroughput = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      // A tuple: Source-ip, destination-ip, protocol, source-port, destination-port
      Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      
      std::cout<<"Flow ID: " << stats->first <<" ; "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
      std::cout<<"Tx Packets = " << stats->second.txPackets<<std::endl;
      std::cout<<"Rx Packets = " << stats->second.rxPackets<<std::endl;
      std::cout<<"Duration: " <<stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()<<std::endl;
      std::cout<<"Last Received Packet: "<< stats->second.timeLastRxPacket.GetSeconds()<<" Seconds"<<std::endl;
      tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024);
      std::cout<<"Throughput: "<< tempThroughput <<" Mbps"<<std::endl;
      std::cout<< "Delay: " << stats->second.delaySum.GetSeconds () << std::endl;
      std::cout<< "LostPackets: " << stats->second.lostPackets << std::endl;
      std::cout<< "Jitter: " << stats->second.jitterSum.GetSeconds () << std::endl;
      //std::cout<<"Last Received Packet: "<< stats->second.timeLastRxPacket.GetSeconds()<<" Seconds ---->" << "Throughput: " << tempThroughput << " Kbps" << std::endl;
      std::cout<<"------------------------------------------"<<std::endl;
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&ImprimeMetricas, fmhelper, monitor);
}

static void
CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition (); // Get position
  Vector vel = mobility->GetVelocity (); // Get velocity

  // Prints position and velocities
  *os << Simulator::Now () << " POS: x=" << pos.x << ", y=" << pos.y
      << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
      << ", z=" << vel.z << std::endl;
}

void printar(int row, int col, int row2, int col2){
	int tam = 4;
	int tam2 = 4;
	int um = 1;

// Definition of matrix to be used [and rows]
	float** matrix = new float*[row];
	float** mate = new float*[row];
	float** matrix2 = new float*[row2];
	float** mult = new float*[row];
	float** mult2 = new float*[row];
	float** mult3 = new float*[row];
	float** mult4 = new float*[row];
	float** a = new float*[row];
	float** b = new float*[row];
	float** tot = new float*[row];
	float** tot2 = new float*[row];
	float* cellS = new float[row];
	float* cellA = new float[row];
	float* cellB = new float[row];
	float* cellC = new float[row];
	//float** c0 = new float*[row];
	float** c1 = new float*[tam];
	float** c2 = new float*[tam];
	float** c3 = new float*[tam];
	//float** mc0 = new float*[row];
	//float** sc0 = new float*[row];
	//float** pc0 = new float*[row];
	float** mc1 = new float*[tam];
	float** sc1 = new float*[tam];
	float** pc1 = new float*[tam];
	float** mc2 = new float*[tam];
	float** sc2 = new float*[tam];
	float** pc2 = new float*[tam];
	float** mc3 = new float*[tam];
	float** sc3 = new float*[tam];
	float** pc3 = new float*[tam];
	float** alg = new float*[tam];
	float** res = new float*[tam];

// Definition of matrix to be used [and columns]
	for(int i = 0; i < row; ++i){
		matrix[i] = new float[col];
	    mate[i] = new float[col];
	    matrix2[i] = new float[col2];
	    mult[i] = new float[col2];
	    mult2[i] = new float[col2];
	    mult3[i] = new float[col2];
	    mult4[i] = new float[col2];
	    a[i] = new float[col2];
	    b[i] = new float[col2];

	    tot2[i] = new float[col2];
	    //c0[i] = new float[col2];
	}

	for(int i = 0; i < tam; ++i){
		c1[i] = new float[tam2];
	    c2[i] = new float[tam2];
		c3[i] = new float[tam2];
		mc1[i] = new float[tam2];
		sc1[i] = new float[tam2];
	    pc1[i] = new float[tam2];
	    mc2[i] = new float[tam2];
		sc2[i] = new float[tam2];
		pc2[i] = new float[tam2];
		mc3[i] = new float[tam2];
		sc3[i] = new float[tam2];
		pc3[i] = new float[tam2];
	}

	for(int i = 0; i < tam; ++i){
		alg[i] = new float[col2];
	}

	for(int i = 0; i < row; ++i){
		tot[i] = new float[um];
	}

	for(int i = 0; i < tam; ++i){
		res[i] = new float[um];
	}

// Judgements values and names
	float var01 = 4;
        float var02 = 2;
        float var12 = 0.5;
        string nvar1 = "sinr";
        string nvar2 = "qos";
        string nvar3 = "qoe";

//Judgement matrix
	mate[0][0]=1;
	mate[0][1]=var01;
	mate[0][2]=var02;
	mate[1][0]=1/var01;
	mate[1][1]=1;
	mate[1][2]=var12;
	mate[2][0]=1/var02;
	mate[2][1]=1/var12;
	mate[2][2]=1;

// Status of Serving Cell(sinr, qos, qoe)
	cellS[0]=12;
	cellS[1]=0.2;
	cellS[2]=3;

// Status of Cell A (sinr, qos, qoe)
	cellA[0]=24;
	cellA[1]=0.4;
	cellA[2]=2;

// Status of Cell B (sinr, qos, qoe)
	cellB[0]=6;
	cellB[1]=0.1;
	cellB[2]=4;

// Status of Cell C (sinr, qos, qoe)
	cellC[0]=18;
	cellC[1]=0.3;
	cellC[2]=3;

// Criteria based matrix of Priority SINR

	c1[0][0]=1;
	c1[0][1]=cellS[0]/cellA[0];
	c1[0][2]=cellS[0]/cellB[0];
	c1[0][3]=cellS[0]/cellC[0];
	c1[1][0]=cellA[0]/cellS[0];
	c1[1][1]=1;
	c1[1][2]=cellA[0]/cellB[0];
	c1[1][3]=cellA[0]/cellC[0];
	c1[2][0]=cellB[0]/cellS[0];
	c1[2][1]=cellB[0]/cellA[0];
	c1[2][2]=1;
	c1[2][3]=cellB[0]/cellC[0];
	c1[3][0]=cellC[0]/cellS[0];
	c1[3][1]=cellC[0]/cellA[0];
	c1[3][2]=cellC[0]/cellB[0];
	c1[3][3]=1;

// Criteria based matrix of Priority QoS
	c2[0][0]=1;
	c2[0][1]=cellS[1]/cellA[1];
	c2[0][2]=cellS[1]/cellB[1];
	c2[0][3]=cellS[1]/cellC[1];
	c2[1][0]=cellA[1]/cellS[1];
	c2[1][1]=1;
	c2[1][2]=cellA[1]/cellB[1];
	c2[1][3]=cellA[1]/cellC[1];
	c2[2][0]=cellB[1]/cellS[1];
	c2[2][1]=cellB[1]/cellA[1];
	c2[2][2]=1;
	c2[2][3]=cellB[1]/cellC[1];
	c2[3][0]=cellC[1]/cellS[1];
	c2[3][1]=cellC[1]/cellA[1];
	c2[3][2]=cellC[1]/cellB[1];
	c2[3][3]=1;

// Criteria based matrix of Priority QoE
	c3[0][0]=1;
	c3[0][1]=cellS[2]/cellA[2];
	c3[0][2]=cellS[2]/cellB[2];
	c3[0][3]=cellS[2]/cellC[2];
	c3[1][0]=cellA[2]/cellS[2];
	c3[1][1]=1;
	c3[1][2]=cellA[2]/cellB[2];
	c3[1][3]=cellA[2]/cellC[2];
	c3[2][0]=cellB[2]/cellS[2];
	c3[2][1]=cellB[2]/cellA[2];
	c3[2][2]=1;
	c3[2][3]=cellB[2]/cellC[2];
	c3[3][0]=cellC[2]/cellS[2];
	c3[3][1]=cellC[2]/cellA[2];
	c3[3][2]=cellC[2]/cellB[2];
	c3[3][3]=1;

// Initializing elements of matrix mult to 0.
	for(unsigned i = 0; i < row; i++){
	    for(unsigned j = 0; j < col2; j++)
	    {
	        mult[i][j]=0;
	    }
	}

// Showing Judgement Matrix
	cout << endl << "Matrix de judgements: " << endl;
	for(unsigned i=0;i<row2;i++) {
		    for(unsigned j=0;j<col2;j++) {
		        cout<<mate[i][j]<<"\t";
		    }
		    cout<<endl;
		}

//	Showing Criteria based matrix of Priority SINR
	cout << endl << "Matrix avaliando sinr: " << endl;
	for(unsigned i=0;i<tam;i++) {
		    for(unsigned j=0;j<tam2;j++) {
		        cout<<c1[i][j]<<"\t";
		    }
		    cout<<endl;
		}

//	Showing Criteria based matrix of Priority QoS
	cout << endl << "Matrix avaliando qos: " << endl;
	for(unsigned i=0;i<tam;i++) {
		    for(unsigned j=0;j<tam2;j++) {
		        cout<<c2[i][j]<<"\t";
		    }
		    cout<<endl;
		}

//	Showing Criteria based matrix of Priority QoE
	cout << endl << "Matrix avaliando qoe: " << endl;
	for(unsigned i=0;i<tam;i++) {
		    for(unsigned j=0;j<tam2;j++) {
		        cout<<c3[i][j]<<"\t";
		    }
		    cout<<endl;
		}

// Multiplying matrix 1 and 2 and storing in array mult.
	cout << endl << "Multiplicacao das Matrix: " << endl;
	for(unsigned i = 0; i < row; i++){
	    for(unsigned j = 0; j < col2; j++){
	        for(unsigned k = 0; k < col; k++){
	            mult[i][j] += mate[i][k] * mate[k][j];
	        }
	    }
	}

	for(unsigned i = 0; i < row; i++){
		//mult[i][0]+mult[i][1]+mult[i][2] = a[i][0];
		a[i][0] = mult[i][0]+mult[i][1]+mult[i][2];
		cout << " \nSOMA: " << a[i][0];
	}

	for(unsigned i = 0; i < row; i++){
		tot[i][0] = (a[i][0])/(a[0][0]+a[1][0]+a[2][0]);
		cout << " \nPorcentagem: " << (tot[i][0]);
	}

// Finding the Multiplication of priority 1, sum and percentage
	cout << endl << "Multiplicacao das prioridades 1 SINR da Matriz: " << endl;
	for(unsigned i = 0; i < tam; i++){
	    for(unsigned j = 0; j < tam2; j++){
	        for(unsigned k = 0; k < tam2; k++){
	            mc1[i][j] += c1[i][k] * c1[k][j];
	        }
	    }
	}

	for(unsigned i = 0; i < tam; i++){
		sc1[i][0] = mc1[i][0]+mc1[i][1]+mc1[i][2]+mc1[i][3];
		cout << " \nSOMA Prioridade 1: " << sc1[i][0];
	}

	for(unsigned i = 0; i < tam; i++){
		pc1[i][0] = (sc1[i][0])/(sc1[0][0]+sc1[1][0]+sc1[2][0]+sc1[3][0]);
		cout << " \nPorcentagem Prioridade 1: " << (pc1[i][0]);
	}

// Finding the Multiplication of priority 2, sum and percentage
	cout << endl << "Multiplicacao das prioridades 2 QoS da Matriz: " << endl;
	for(unsigned i = 0; i < tam; i++){
	    for(unsigned j = 0; j < tam2; j++){
	        for(unsigned k = 0; k < tam2; k++){
	            mc2[i][j] += c2[i][k] * c2[k][j];
	        }
	    }
	}

	for(unsigned i = 0; i < tam; i++){
		sc2[i][0] = mc2[i][0]+mc2[i][1]+mc2[i][2]+mc2[i][3];
		cout << " \nSOMA Prioridade 2: " << sc2[i][0];
	}

	for(unsigned i = 0; i < tam; i++){
		pc2[i][0] = (sc2[i][0])/(sc2[0][0]+sc2[1][0]+sc2[2][0]+sc2[3][0]);
		cout << " \nPorcentagem Prioridade 2: " << (pc2[i][0]);
	}

// Finding the Multiplication of priority 3, sum and percentage
	cout << endl << "Multiplicacao das prioridades 3 QoE da Matriz: " << endl;
	for(unsigned i = 0; i < tam; i++){
	    for(unsigned j = 0; j < tam2; j++){
	        for(unsigned k = 0; k < tam2; k++){
	            mc3[i][j] += c3[i][k] * c3[k][j];
	        }
	    }
	}

	for(unsigned i = 0; i < tam; i++){
		sc3[i][0] = mc3[i][0]+mc3[i][1]+mc3[i][2]+mc3[i][3];
		cout << " \nSOMA Prioridade 3: " << sc3[i][0];
	}

	for(unsigned i = 0; i < tam; i++){
		pc3[i][0] = (sc3[i][0])/(sc3[0][0]+sc3[1][0]+sc3[2][0]+sc3[3][0]);
		cout << " \nPorcentagem Prioridade 3: " << (pc3[i][0]);
	}

// Final results
	alg[0][0]=pc1[0][0];
	alg[1][0]=pc1[1][0];
	alg[2][0]=pc1[2][0];
	alg[3][0]=pc1[3][0];
	alg[0][1]=pc2[0][0];
	alg[1][1]=pc2[1][0];
	alg[2][1]=pc2[2][0];
	alg[3][1]=pc2[3][0];
	alg[0][2]=pc3[0][0];
	alg[1][2]=pc3[1][0];
	alg[2][2]=pc3[2][0];
	alg[3][2]=pc3[3][0];


// Final results calculation
	cout << endl << "Priorities x Criteria Ranking: " << endl;
		for(unsigned i = 0; i < tam; i++){
		    for(unsigned j = 0; j < um; j++){
		        for(unsigned k = 0; k < row; k++){
		            res[i][j] += alg[i][k] * tot[k][j];
		        }
		    }
		}

//	Showing Criteria based matrix of Priority QoE
	cout << endl << "Final Matrix: " << endl;
	for(unsigned i=0;i<tam;i++) {
	    for(unsigned j=0;j<um;j++) {
	        cout<<res[i][j]<<"\t";
	    }
	    cout<<endl;
	}
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}

int main(int argc, char *argv[]) {

    /*--------------------- Logging System Configuration -------------------*/
    std::string a = exec("python2.7 ia.py 0.1 0.2 0.3 20");
    double b = stof(a);
    std::cout << b;

    printar(3, 3, 3, 3);

    NS_LOG_INFO("/------------------------------------------------\\");
    NS_LOG_INFO(" - V2vClusteringExample [Example] -> Cluster vehicles communication");
    NS_LOG_INFO("\\------------------------------------------------/");
    /*----------------------------------------------------------------------*/

    /*---------------------- Simulation Default Values ---------------------*/
    std::string phyMode ("OfdmRate6MbpsBW10MHz");

    uint16_t numberOfUes = 3; //VEÍCULOS

    uint16_t numberOfRsus = 2; //RSU

    double simTime = 90.0;
    /*----------------------------------------------------------------------*/


    /*-------------------- Set explicitly default values -------------------*/
    Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                            StringValue ("2200"));
    // turn off RTS/CTS for frames below 2200 bytes
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                            StringValue ("2200"));
    // Fix non-unicast data rate to be the same as that of unicast
    Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                            StringValue (phyMode));
    /*----------------------------------------------------------------------*/


    /*-------------------- Command Line Argument Values --------------------*/
    CommandLine cmd;
    cmd.AddValue("ueNumber", "Number of UE", numberOfUes);
    cmd.AddValue("simTime", "Simulation Time in Seconds", simTime);

    NS_LOG_INFO("");
    NS_LOG_INFO("|---"<< " SimTime -> " << simTime <<" ---|\n");
    NS_LOG_INFO("|---"<< " Number of UE -> " << numberOfUes <<" ---|\n");
    /*----------------------------------------------------------------------*/


    /*------------------------- Create UEs-RSUs ----------------------------*/
    NodeContainer ueNodes;
    ueNodes.Create(numberOfUes);


    //Instala pilha de Internet
    InternetStackHelper internet;
    internet.Install(ueNodes);
    /*----------------------------------------------------------------------*/


    /*-------------------- Instala Mobilidade nos Veículos --------------------*/
    /*MobilityHelper mobility;

    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (10.0),
                                 "MinY", DoubleValue (10.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (2.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

    mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
    mobility.Install (ueNodes);*/

    /*for (uint16_t i = 0; i < numberOfUes; i++)
    {
        if(i % 3 == 0){
            ueNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 0, 0));
        }
        else if(i % 3 == 1){
            ueNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 3, 0));
        }
        else{
            ueNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 6, 0));
        }

    }*/

      /*// setup a uniform random variable for the speed
      Ptr<UniformRandomVariable> rvar = CreateObject<UniformRandomVariable>();
      // for each node set up its speed according to the random variable
      for (NodeContainer::Iterator iter= ueNodes.Begin(); iter!=ueNodes.End(); ++iter){
          Ptr<Node> tmp_node = (*iter);
          // select the speed from (15,25) m/s
          double speed = rvar->GetValue(15, 30);
          tmp_node->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(speed, 0, 0));
      }*/


//string logFile = "ns2-mobility-trace.log";

 // Enable logging from the ns2 helper
  LogComponentEnable ("Ns2MobilityHelper",LOG_LEVEL_DEBUG);

 // Create Ns2MobilityHelper with the specified trace log file as parameter
   Ns2MobilityHelper mobility = Ns2MobilityHelper ("scratch/ns2mobility.tcl");

   //open log file for output
   std::ofstream os;
   os.open ("ns2-mobility-trace.log");

   mobility.Install(); // configure movements for each node, while reading trace file

  // Configure callback for logging
  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeBoundCallback (&CourseChange, &os));

  os.close (); // close log file
  
    //-RSU
    NodeContainer rsuNodes;
    rsuNodes.Create(numberOfRsus);

    internet.Install(rsuNodes);

    //Mobilidade para RSU
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
    for (uint16_t i = 1; i <= rsuNodes.GetN(); i++){
     positionAlloc->Add (Vector(150 * i, 15, 0)); //DISTANCIA ENTRE RSUs [m] 
    }

    MobilityHelper mobilityRsu;
    mobilityRsu.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityRsu.SetPositionAllocator(positionAlloc);
    mobilityRsu.Install(rsuNodes);

    /*----------------------------------------------------------------------*/
    
    /*-------------------------- Setup Wifi nodes --------------------------*/
    // The below set of helpers will help us to put together the wifi NICs we want
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
    Ptr<YansWifiChannel> channel = wifiChannel.Create ();

    YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
    wifiPhy.SetChannel (channel);
    wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
    wifiPhy.Set ("TxPowerStart", DoubleValue(32));
    wifiPhy.Set ("TxPowerEnd", DoubleValue(32));
    wifiPhy.Set ("TxGain", DoubleValue(12));
    wifiPhy.Set ("RxGain", DoubleValue(12));
    wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue(-61.8));
    wifiPhy.Set ("CcaMode1Threshold", DoubleValue(-64.8));

    NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
    Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
    //wifi80211p.EnableLogComponents ();

    wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode));
    NetDeviceContainer wifiDevices1 = wifi80211p.Install (wifiPhy, wifi80211pMac, ueNodes);

    //RSU
    NetDeviceContainer wifiDevices2 = wifi80211p.Install (wifiPhy, wifi80211pMac, rsuNodes); //RSU

    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i1 = ipv4h.Assign (wifiDevices1);

    //Configuração de IP da RSU
    Ipv4InterfaceContainer i2 = ipv4h.Assign (wifiDevices2); //RSU

//TESTE DE TRANSMISSÃO DE VÍDEO

//-------------Rodar aplicação EvalVid
  for (uint32_t i = 0; i < ueNodes.GetN(); i++){
    //Gera SD e RD para cada Veículo
    
    string video_trans = "st_highway_cif.st";
    string caminho = "./";

    std::stringstream sdTrace;
    std::stringstream rdTrace;
    sdTrace << caminho + "sd_a01_" << (int)i;
    rdTrace << caminho + "rd_a01_" << (int)i;
 
    double start = 10.0;
    double stop = simTime; 
    
    uint16_t port = 2000;
    uint16_t m_port = port * i + 2000; //Para alcançar o nó ZERO quando i = 0

    EvalvidServerHelper server (m_port);
     server.SetAttribute ("SenderTraceFilename", StringValue(video_trans));
     server.SetAttribute ("SenderDumpFilename", StringValue(sdTrace.str()));
     server.SetAttribute ("PacketPayload",UintegerValue(1014));
     ApplicationContainer apps = server.Install(rsuNodes.Get(0));
     apps.Start (Seconds (start));
     apps.Stop (Seconds (stop));
  
    EvalvidClientHelper client (i2.GetAddress (0),m_port);
     client.SetAttribute ("ReceiverDumpFilename", StringValue(rdTrace.str()));
     apps = client.Install (ueNodes.Get(i));
     apps.Start (Seconds (start));
     apps.Stop (Seconds (stop));
   }

/*Teste para Outra RSU*/

for (uint32_t i = 0; i < ueNodes.GetN(); i++){
    //Gera SD e RD para cada Veículo

    string video_trans = "st_highway_cif.st";
    string caminho = "./";

    std::stringstream sdTrace;
    std::stringstream rdTrace;
    sdTrace << caminho + "sd_a02_" << (int)i;
    rdTrace << caminho + "rd_a02_" << (int)i;

    double start = 10.0;
    double stop = simTime;

    uint16_t port = 2000;
    uint16_t m_port = port * i + 2000; //Para alcançar o nó ZERO quando i = 0

    EvalvidServerHelper server (m_port);
     server.SetAttribute ("SenderTraceFilename", StringValue(video_trans));
     server.SetAttribute ("SenderDumpFilename", StringValue(sdTrace.str()));
     server.SetAttribute ("PacketPayload",UintegerValue(1014));
     ApplicationContainer apps = server.Install(rsuNodes.Get(1));
     apps.Start (Seconds (start));
     apps.Stop (Seconds (stop));

    EvalvidClientHelper client (i2.GetAddress (1),m_port);
     client.SetAttribute ("ReceiverDumpFilename", StringValue(rdTrace.str()));
     apps = client.Install (ueNodes.Get(i));
     apps.Start (Seconds (start));
     apps.Stop (Seconds (stop));
   }


//FIM TESTE DE TRANSMISSÃO DE VÍDEO

    //AsciiTraceHelper ascii;
    //wifiPhy.EnableAsciiAll(ascii.CreateFileStream ("resultados/socket-options-ipv4.txt"));
    //wifiPhy.EnablePcapAll ("resultados/socket.pcap", false);

    /*----------------------------------------------------------------------*/
       
    AnimationInterface anim ("V2X/normal_v2x_netanim.xml");
    //Cor e Descrição para RSU
    for (uint32_t i = 0; i < rsuNodes.GetN (); ++i){
        anim.UpdateNodeDescription (rsuNodes.Get (i), "RSU");
        anim.UpdateNodeColor (rsuNodes.Get (i), 0, 255, 0);
    }

        //Monitor de fluxo
    Ptr<FlowMonitor> monitor;
    FlowMonitorHelper fmhelper;
    monitor = fmhelper.InstallAll(); 

    /*---------------------- Simulation Stopping Time ----------------------*/
    Simulator::Stop(SIMULATION_TIME_FORMAT(simTime));
    /*----------------------------------------------------------------------*/

    string tipo = "V2X/graficos/Normal_";
    //string tipo = "graficos/normal/Normal_";

    //Throughput
    string vazao = tipo + "FlowVSThroughput";
    string graphicsFileName        = vazao + ".png";
    string plotFileName            = vazao + ".plt";
    string plotTitle               = "Flow vs Throughput";
    string dataTitle               = "Throughput";

    Gnuplot gnuplot (graphicsFileName);
    gnuplot.SetTitle (plotTitle);
    gnuplot.SetTerminal ("png");
    gnuplot.SetLegend ("Flow", "Throughput (Kbps)");

    Gnuplot2dDataset dataset;
    dataset.SetTitle (dataTitle);
    dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

    //Delay
    string delay = tipo + "FlowVSDelay";
    string graphicsFileName1        = delay + ".png";
    string plotFileName1            = delay + ".plt";
    string plotTitle1               = "Flow vs Delay";
    string dataTitle1               = "Delay";

    Gnuplot gnuplot1 (graphicsFileName1);
    gnuplot1.SetTitle (plotTitle1);
    gnuplot1.SetTerminal ("png");
    gnuplot1.SetLegend ("Flow", "Delay");

    Gnuplot2dDataset dataset1;
    dataset1.SetTitle (dataTitle1);
    dataset1.SetStyle (Gnuplot2dDataset::LINES_POINTS);

    //LostPackets
    string lost = tipo + "FlowVSLostPackets";
    string graphicsFileName2        = lost + ".png";
    string plotFileName2            = lost + ".plt";
    string plotTitle2               = "Flow vs LostPackets";
    string dataTitle2               = "LostPackets";

    Gnuplot gnuplot2 (graphicsFileName2);
    gnuplot2.SetTitle (plotTitle2);
    gnuplot2.SetTerminal ("png");
    gnuplot2.SetLegend ("Flow", "LostPackets");

    Gnuplot2dDataset dataset2;
    dataset2.SetTitle (dataTitle2);
    dataset2.SetStyle (Gnuplot2dDataset::LINES_POINTS);

    //Jitter
    string jitter = tipo + "FlowVSJitter";
    string graphicsFileName3        = jitter + ".png";
    string plotFileName3            = jitter + ".plt";
    string plotTitle3               = "Flow vs Jitter";
    string dataTitle3               = "Jitter";

    Gnuplot gnuplot3 (graphicsFileName3);
    gnuplot3.SetTitle (plotTitle3);
    gnuplot3.SetTerminal ("png");
    gnuplot3.SetLegend ("Flow", "Jitter");

    Gnuplot2dDataset dataset3;
    dataset3.SetTitle (dataTitle3);
    dataset3.SetStyle (Gnuplot2dDataset::LINES_POINTS);

    //Chama classe de captura do fluxo
    ThroughputMonitor (&fmhelper, monitor, dataset);
    DelayMonitor (&fmhelper, monitor, dataset1);
    LostPacketsMonitor (&fmhelper, monitor, dataset2);
    JitterMonitor (&fmhelper, monitor, dataset3);

    /*--------------------------- Simulation Run ---------------------------*/
    Simulator::Run();

    ImprimeMetricas (&fmhelper, monitor);

    //Throughput
    gnuplot.AddDataset (dataset);
    std::ofstream plotFile (plotFileName.c_str()); // Abre o arquivo.
    gnuplot.GenerateOutput (plotFile);    //Escreve no arquivo.
    plotFile.close ();        // fecha o arquivo.
    //Delay
    gnuplot1.AddDataset (dataset1);
    std::ofstream plotFile1 (plotFileName1.c_str()); // Abre o arquivo.
    gnuplot1.GenerateOutput (plotFile1);    //Escreve no arquivo.
    plotFile1.close ();        // fecha o arquivo.
    //LostPackets
    gnuplot2.AddDataset (dataset2);
    std::ofstream plotFile2 (plotFileName2.c_str()); // Abre o arquivo.
    gnuplot2.GenerateOutput (plotFile2);    //Escreve no arquivo.
    plotFile2.close ();        // fecha o arquivo.
    //Jitter
    gnuplot3.AddDataset (dataset3);
    std::ofstream plotFile3 (plotFileName3.c_str()); // Abre o arquivo.
    gnuplot3.GenerateOutput (plotFile3);    //Escreve no arquivo.
    plotFile3.close ();        // fecha o arquivo.

    monitor->SerializeToXmlFile("V2X/flow_normal.xml", true, true);
    //monitor->SerializeToXmlFile("resultados/normal/flow_normal.xml", true, true);

    Simulator::Destroy();
    /*----------------------------------------------------------------------*/

    return EXIT_SUCCESS;
}
