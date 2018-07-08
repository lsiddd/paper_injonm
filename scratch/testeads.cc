#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/internet-module.h>
#include <ns3/lte-module.h>
#include <ns3/config-store-module.h>
#include <ns3/buildings-module.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/applications-module.h>
#include <ns3/log.h>
#include <iomanip>
#include <ios>
#include <string>
#include <vector>
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/config-store.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/csma-helper.h"
#include <cmath>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "ns3/mac-stats-calculator.h"
#include "ns3/lte-stats-calculator.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include <fstream>
#include <inttypes.h>
#include "ns3/netanim-module.h"


using namespace ns3;

int main (int argc, char *argv[])
{
int distance = 1000;
/*
CommandLine cmd;
cmd.AddValue("numberOfNodes", "Numero de UEs", numberOfNodes);
cmd.AddValue("simTime", "Tempo da simulao [s])", simTime);
cmd.AddValue("distance", "Radio da eNBs [m]", distance);
cmd.AddValue("interPacketInterval", "Intervalo entre pacotes[ms])", interPacketInterval);
cmd.Parse(argc, argv);
*/
ConfigStore inputConfig;
inputConfig.ConfigureDefaults ();
//cmd.Parse (argc, argv);
Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
//lteHelper->SetAttribute ("PathlossModel", StringValue //("ns3::FriisSpectrumPropagationLossModel"));
lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (100));
lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (18100));
lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (25));
lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (25));

Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (43));
NodeContainer enbNodes;
NodeContainer ueNodes;
enbNodes.Create (1);
ueNodes.Create (30);
MobilityHelper mobility;
mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install (enbNodes);
MobilityHelper uemobility;
uemobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator",
                                "X", DoubleValue (0.0),
                                "Y", DoubleValue (0.0),
                                "rho", DoubleValue (distance)); 
  
uemobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
			     "Bounds", RectangleValue (Rectangle (-500, 500, -500, 			     500)));
uemobility.Install (ueNodes);

NetDeviceContainer enbDevs;
NetDeviceContainer ueDevs;
enbDevs = lteHelper->InstallEnbDevice (enbNodes);
ueDevs = lteHelper->InstallUeDevice (ueNodes);
lteHelper->Attach (ueDevs, enbDevs.Get (0));
enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
EpsBearer bearer (q);
lteHelper->ActivateDataRadioBearer (ueDevs, bearer);
Simulator::Stop (Seconds (0.5));
lteHelper->EnablePhyTraces ();
lteHelper->EnableMacTraces ();
lteHelper->EnableRlcTraces ();
double distance_temp [] = { 1000,1000,1000};
std::vector<double> userDistance;
userDistance.assign (distance_temp, distance_temp + 3);

AnimationInterface anim ("testeads.xml"); 

Simulator::Run ();

Simulator::Destroy ();
return 0;
}
