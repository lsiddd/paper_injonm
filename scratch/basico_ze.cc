/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Disciplina: Redes de Computadores
 * Grupo de Estudos em Redes de Computadores e Comunicação Multimídia - GERCOM
 *
 */

#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"

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

#include <time.h>

#include <math.h>
#include "ns3/ns2-mobility-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiScript_RedesII");

//Captura Localização
void CheckBound(Ptr<MobilityModel> mob){
    Vector position = mob->GetPosition();
    Vector speed = mob->GetVelocity();
    std::cout << "Tempo: " << Simulator::Now ().GetSeconds () << "\n";
    std::cout << ": x = " << position.x << ", y = " << position.y << "\n";
    std::cout << ": v = " << speed.x << "\n\n";
}

static Vector GetPosition (Ptr<Node> node){
	Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
	return mobility->GetPosition ();
}

static Vector GetVelocity (Ptr<Node> node){
	Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
	return mobility->GetVelocity ();
}

/*
static void SetPosition (Ptr<Node> node, double p){
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  Vector position = mobility->GetPosition();
  position.x = p;
  return mobility->SetPosition(position);
}
*/

void PrintLocations (Ptr<Node> node){
    Vector position = GetPosition (node);
    Vector speed = GetVelocity (node);
    std::cout << "Nó [" << node->GetId() << "]: " << "\n";
    std::cout << "Tempo: " << Simulator::Now ().GetSeconds () << "\n";
    std::cout << "Localização: x = " << position.x << ", y = " << position.y << "\n";
    std::cout << "Velocidade: " << speed.x << "\n\n";
}

static double GeraAcidenteX(double x){
    srand (time(NULL)); 
    double xx = ((double) rand()*(x-1)/(double)RAND_MAX-1);
    return xx;
}
static double GeraAcidenteY(double y){
    srand (time(NULL));
    double yy = ((double) rand()*(y-1)/(double)RAND_MAX-1);
    return yy;
}

//Calculo de Circunferência
static double CalculaZona (double x, double y, double a, double b){
    double x1 = (x - a);
    double y1 = (y - b);

    double d1 = pow(x1, 2.0) + pow(y1, 2.0); //Equações da circunferência
    double d2 = sqrt(d1);

    return d2;
}

//Verifica se o veículo ta na ZE
void ZonaEmergencia (NodeContainer node_container){
    int n = node_container.GetN();
    int cont = 0;
    int cont1 = 0;
    double x = 0.0;
    double y = 0.0;

    for (int i = 0; i < n; ++i) {
      Ptr<Node> node = node_container.Get(i);
      Vector position = GetPosition (node);

      double raio = 60.0;
      double a = position.x;
      double b = position.y;

      x = GeraAcidenteX(400.0);
      y = GeraAcidenteY(400.0);

      std::cout << "Acidente em: [" << x << ", " << y << "]" << "\n";

      double teste = CalculaZona(x, y, a, b);

      if(teste <= raio){
        std::cout << "------> Veiculo [" << node->GetId() << "] está na ZE! :)" << "\n";
        cont++;
      }else{
        std::cout << "------> Veiculo [" << node->GetId() << "] não está na ZE! :(" << "\n";
        cont1++;
      }
    }
    
    std::cout << "Veiculo na ZE: " << cont << "\n";
    std::cout << "Veiculo fora da ZE: " << cont1 << "\n";
}

int 
main (int argc, char *argv[])
{
  uint32_t nWifi = 99; //Quantidade de nós WiFi
  uint32_t apWifi = 1; //Quantidade de APs
  double simTime = 30.0; //Tempo de simulação
  bool tracing = true;

//Inserção de dados via linha de comando "--nomeDaVariavel"
  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("apWifi", "Number of wifi AP device", apWifi);
  cmd.AddValue ("simTime", "Tempo de simulacao", simTime);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

//Logs da aplicação para Terminal
  //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

//Cria apenas nós WiFi
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

//Cria o nó AP para WiFI
  NodeContainer wifiApNode;
  wifiApNode.Create (apWifi);

//Cria os nós WiFi e os interliga por meio de um canal
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

//Algoritmo de controle de taxa, que neste caso é o AARF
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

//SSID do AP
  Ssid ssid = Ssid ("GERCOM-WLAN");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));
//Nós Clientes
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

//Nó AP
  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  std::string traceFile = "scratch/ns2mobility.tcl";
  Ns2MobilityHelper ns2mobility = Ns2MobilityHelper(traceFile);
  ns2mobility.Install();

/*-------------------- Instala Mobilidade nos Veículos --------------------*/
/*  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
  	                             "MinX", DoubleValue (10.0),
                                 "MinY", DoubleValue (10.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (2.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (wifiStaNodes);

  for (uint16_t i = 0; i < wifiStaNodes.GetN(); i++){
     if(i % 3 == 0){
         wifiStaNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 0, 0));
     }
     else if(i % 3 == 1){
         wifiStaNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 3, 0));
     }
     else{
         wifiStaNodes.Get (i)->GetObject<MobilityModel> ()->SetPosition (Vector (i*5, 6, 0));
     }

  }

  // setup a uniform random variable for the speed
  Ptr<UniformRandomVariable> rvar = CreateObject<UniformRandomVariable>();
  // for each node set up its speed according to the random variable
  for (NodeContainer::Iterator iter= wifiStaNodes.Begin(); iter!=wifiStaNodes.End(); ++iter){
      Ptr<Node> tmp_node = (*iter);
      // select the speed from (15,25) m/s
      double speed = rvar->GetValue(15, 25);
      tmp_node->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(speed, 0, 0));
  }
  */

//Mobilidade para RSU
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 1; i <= wifiApNode.GetN(); i++){
    positionAlloc->Add (Vector(200 * i, 200, 0)); //DISTANCIA ENTRE RSUs [m] 
   }

  MobilityHelper mobilityRsu;
  mobilityRsu.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityRsu.SetPositionAllocator(positionAlloc);
  mobilityRsu.Install(wifiApNode);
/*----------------------------------------------------------------------*/

  //Pilha de Internet
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;
//Cria endereço para os Clientes
  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesAp = address.Assign (apDevices);
  Ipv4InterfaceContainer interfacesWifi = address.Assign (staDevices);

//srand(time(NULL));
// /uint32_t teste = rand() % nWifi + 1;

//Aplicação de Eco - Teste
for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++){
  uint16_t port = 1000 * i + 1000; //Para alcançar o nó ZERO quando i = 0

  //Servidor
  UdpEchoServerHelper echoServer (port);//is the server port number
  ApplicationContainer serverApps = echoServer.Install (wifiApNode.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (simTime));

  //Cliente
  UdpEchoClientHelper echoClient (interfacesAp.GetAddress(0), port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (wifiStaNodes.Get (i));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (simTime));
}

  //Localização
  //for (uint32_t n = 0; n < wifiStaNodes.GetN (); n++){
  //    Ptr<ConstantVelocityMobilityModel> cvMob = wifiStaNodes.Get(n)->GetObject<ConstantVelocityMobilityModel>();
  //    	Simulator::Schedule (Seconds (10.0), &CheckBound, cvMob);
  // }



  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //Monitor de fluxo
    Ptr<FlowMonitor> monitor;
    FlowMonitorHelper fmhelper;
    monitor = fmhelper.InstallAll();

  if (tracing == true){
      phy.EnablePcap ("V2X/wifi_redes", apDevices.Get (0));
    }

  AnimationInterface anim ("V2X/AnimationWifi.xml");
  //Cor e Descrição para AP
  for (uint32_t i = 0; i < wifiApNode.GetN (); ++i){
      anim.UpdateNodeDescription (wifiApNode.Get (i), "AP");
      anim.UpdateNodeColor (wifiApNode.Get (i), 0, 255, 0);
  }
  anim.EnablePacketMetadata ();

  Simulator::Stop (Seconds (simTime));

  for (uint32_t i = 0; i < wifiStaNodes.GetN (); ++i){
      Simulator::Schedule (Seconds (simTime - 10.0), &PrintLocations, wifiStaNodes.Get (i));
  }

  //for (uint32_t i = 0; i < wifiStaNodes.GetN (); ++i){
  //    Simulator::Schedule (Seconds (simTime - 10.0), &ZonaEmergencia, wifiStaNodes.Get (i));
  //}

  Simulator::Schedule (Seconds (simTime - 10.0), &ZonaEmergencia, wifiStaNodes);


  /*
  //Localização
  double j = Simulator::Now ().GetSeconds();
  while(j < simTime){
  	for(uint32_t i = 0; i < wifiStaNodes.GetN (); ++i){
  			Simulator::Schedule (Seconds (j), &PrintLocations, wifiStaNodes.Get (i));
  			
  			//Muda localização
  			Vector speed = GetVelocity (wifiStaNodes.Get (i));
    			if(speed.x > 15.0 && speed.x < 18.0){
    				SetPosition(wifiStaNodes.Get (i), 0.0);
    			}
  	}
  	j = j + 0.0001; //0.0000
  }
  */

  Simulator::Run ();

  //Imprime métricas no Terminal
  //ImprimeMetricas (&fmhelper, monitor);

  monitor->SerializeToXmlFile("V2X/fluxo.xml", true, true);

  Simulator::Destroy ();
  return 0;
}
