/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* * Copyright (c) 2017
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
 * Modified by: Lucas Pacheco <lucassidpacheco@gmail.com>
 *
 */

/* This is a testing version.
  */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-address.h"
// NetAnim & Evalvid
#include "ns3/netanim-module.h"
#include "ns3/evalvid-client-server-helper.h"
// Pacotes LTE
#include "ns3/point-to-point-helper.h"
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/lte-module.h"
// Monitor de fluxo
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/gnuplot.h"

#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/boolean.h>
#include <ns3/enum.h>

#include "ns3/lte-hex-grid-enb-topology-helper.h"

#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <typeinfo>

#define SIMULATION_TIME_FORMAT(s) Seconds(s)

using namespace ns3;
using namespace std;

double TxRate = 0; // TAXA DE RECEBIMENTO DE PACOTES

const int node_ue = 50;
uint16_t n_cbr = 7;
uint16_t enb_HPN = 7; // 7;
uint16_t low_power = 56; // 56;
uint16_t hot_spot = 0; // 14;
int cell_ue[77][57]; // matriz de conexões
int txpower = 15; //  Lte Ue Tx Power

double simTime = 80.0; // TEMPO_SIMULAÇÃO
int transmissionStart = 23;

// número de handovers realizados
unsigned int handNumber = 0;

// variaveis do vídeo
const int numberOfFrames = 300;
const int numberOfPackets = 614;

int framePct[numberOfFrames + 1];
std::string frameTypeGlobal[numberOfFrames];
int LastReceivedFrame[node_ue];
bool receivedFrames[node_ue][numberOfPackets];
bool receivedPackets[node_ue][numberOfPackets];

NS_LOG_COMPONENT_DEFINE("v2x_3gpp");

/*------------------------- NOTIFICAÇÕES DE HANDOVER ----------------------*/
void NotifyConnectionEstablishedUe(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " UE IMSI " << imsi
        << ": connected to CellId " << cellid << " with RNTI " << rnti);

    //feed connection files
    std::stringstream strrnti;
    strrnti << rnti;
    std::ofstream ofs("rnti/" + strrnti.str() + ".txt"); //, ios::out);
    ofs << imsi << "\t" << cellid << "\n";
    ofs.close();

    //feed connection matrix
    cell_ue[cellid - 1][imsi - 1] = rnti;
}

void NotifyHandoverStartUe(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti,
    uint16_t targetCellId)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " UE IMSI " << imsi
        << ": previously connected to CellId " << cellid << " with RNTI "
        << rnti << ", doing handover to CellId " << targetCellId);

    std::stringstream strrnti;
    strrnti << rnti;
    std::ofstream ofs("rnti/" + strrnti.str() + ".txt", ios::out);
    ofs << imsi << "\t" << cellid << "\n";
    ofs.close();

    cell_ue[cellid - 1][imsi - 1] = 0;

    ++handNumber;
}

void NotifyHandoverEndOkUe(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " UE IMSI " << imsi
        << ": successful handover to CellId " << cellid << " with RNTI "
        << rnti);
    cell_ue[cellid - 1][imsi - 1] = rnti;
}

void NotifyConnectionEstablishedEnb(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " eNB CellId " << cellid
        << ": successful connection of UE with IMSI " << imsi << " RNTI "
        << rnti);
}

void NotifyHandoverStartEnb(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti,
    uint16_t targetCellId)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " eNB CellId " << cellid
        << ": start handover of UE with IMSI " << imsi << " RNTI "
        << rnti << " to CellId " << targetCellId);
}

void NotifyHandoverEndOkEnb(std::string context,
    uint64_t imsi,
    uint16_t cellid,
    uint16_t rnti)
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds()
        << " " << context << " eNB CellId " << cellid
        << ": completed handover of UE with IMSI " << imsi << " RNTI "
        << rnti);
}

void ArrayPositionAllocator(Ptr<ListPositionAllocator> HpnPosition)
{
    if (enb_HPN == 7) {
        HpnPosition->Add(Vector(2000, 2000, 25));
        HpnPosition->Add(Vector(1000, 2000, 25));
        HpnPosition->Add(Vector(3000, 2000, 25));
        HpnPosition->Add(Vector(2500, 2866, 25));
        HpnPosition->Add(Vector(1500, 2866, 25));
        HpnPosition->Add(Vector(2500, 1135, 25));
        HpnPosition->Add(Vector(1500, 1135, 25));

        HpnPosition->Add(Vector(2000 - 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(1000 - 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(3000 - 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(2500 - 105 - 10, 2866, 10));
        HpnPosition->Add(Vector(1500 - 105 - 10, 2866, 10));
        HpnPosition->Add(Vector(2500 - 105 - 10, 1135, 10));
        HpnPosition->Add(Vector(1500 - 105 - 10, 1135, 10));

        HpnPosition->Add(Vector(2000 + 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(1000 + 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(3000 + 105 - 10, 2000, 10));
        HpnPosition->Add(Vector(2500 + 105 - 10, 2866, 10));
        HpnPosition->Add(Vector(1500 + 105 - 10, 2866, 10));
        HpnPosition->Add(Vector(2500 + 105 - 10, 1135, 10));
        HpnPosition->Add(Vector(1500 + 105 - 10, 1135, 10));

        HpnPosition->Add(Vector(2000 - 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(1000 - 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(3000 - 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(2500 - 105 + 10, 2866, 10));
        HpnPosition->Add(Vector(1500 - 105 + 10, 2866, 10));
        HpnPosition->Add(Vector(2500 - 105 + 10, 1135, 10));
        HpnPosition->Add(Vector(1500 - 105 + 10, 1135, 10));

        HpnPosition->Add(Vector(2000 + 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(1000 + 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(3000 + 105 + 10, 2000, 10));
        HpnPosition->Add(Vector(2500 + 105 + 10, 2866, 10));
        HpnPosition->Add(Vector(1500 + 105 + 10, 2866, 10));
        HpnPosition->Add(Vector(2500 + 105 + 10, 1135, 10));
        HpnPosition->Add(Vector(1500 + 105 + 10, 1135, 10));

        HpnPosition->Add(Vector(2000 - 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(1000 - 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(3000 - 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(2500 - 105, 2866 - 10, 10));
        HpnPosition->Add(Vector(1500 - 105, 2866 - 10, 10));
        HpnPosition->Add(Vector(2500 - 105, 1135 - 10, 10));
        HpnPosition->Add(Vector(1500 - 105, 1135 - 10, 10));

        HpnPosition->Add(Vector(2000 + 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(1000 + 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(3000 + 105, 2000 - 10, 10));
        HpnPosition->Add(Vector(2500 + 105, 2866 - 10, 10));
        HpnPosition->Add(Vector(1500 + 105, 2866 - 10, 10));
        HpnPosition->Add(Vector(2500 + 105, 1135 - 10, 10));
        HpnPosition->Add(Vector(1500 + 105, 1135 - 10, 10));

        HpnPosition->Add(Vector(2000 - 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(1000 - 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(3000 - 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(2500 - 105, 2866 + 10, 10));
        HpnPosition->Add(Vector(1500 - 105, 2866 + 10, 10));
        HpnPosition->Add(Vector(2500 - 105, 1135 + 10, 10));
        HpnPosition->Add(Vector(1500 - 105, 1135 + 10, 10));

        HpnPosition->Add(Vector(2000 + 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(1000 + 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(3000 + 105, 2000 + 10, 10));
        HpnPosition->Add(Vector(2500 + 105, 2866 + 10, 10));
        HpnPosition->Add(Vector(1500 + 105, 2866 + 10, 10));
        HpnPosition->Add(Vector(2500 + 105, 1135 + 10, 10));
        HpnPosition->Add(Vector(1500 + 105, 1135 + 10, 10));

        HpnPosition->Add(Vector(2000 - 105, 2000, 10));
        HpnPosition->Add(Vector(1000 - 105, 2000, 10));
        HpnPosition->Add(Vector(3000 - 105, 2000, 10));
        HpnPosition->Add(Vector(2500 - 105, 2866, 10));
        HpnPosition->Add(Vector(1500 - 105, 2866, 10));
        HpnPosition->Add(Vector(2500 - 105, 1135, 10));
        HpnPosition->Add(Vector(1500 - 105, 1135, 10));

        HpnPosition->Add(Vector(2000 + 105, 2000, 10));
        HpnPosition->Add(Vector(1000 + 105, 2000, 10));
        HpnPosition->Add(Vector(3000 + 105, 2000, 10));
        HpnPosition->Add(Vector(2500 + 105, 2866, 10));
        HpnPosition->Add(Vector(1500 + 105, 2866, 10));
        HpnPosition->Add(Vector(2500 + 105, 1135, 10));
        HpnPosition->Add(Vector(1500 + 105, 1135, 10));
    }
    else {
        for (uint16_t i = 0; i <= 3; i++) {
            for (uint16_t j = 0; j <= 3; j++) {
                HpnPosition->Add(Vector(500 + 1000 * j, 1000 * i,
                    0)); // DISTANCIA ENTRE RSUs [m]
            }
        }
    }
}

/*------------------------- CÁLCULO DE MÉTRICAS ----------------------*/

void ThroughputMonitor(FlowMonitorHelper* fmhelper,
    Ptr<FlowMonitor> monitor,
    Gnuplot2dDataset dataset)
{
    double tempThroughput = 0.0;
    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin();
         stats != flowStats.end(); ++stats) {
        tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()) / 1024);
        dataset.Add((double)Simulator::Now().GetSeconds(), (double)tempThroughput);
    }

    // Tempo que será iniciado
    Simulator::Schedule(Seconds(1), &ThroughputMonitor, fmhelper, monitor,
        dataset);
}

void DelayMonitor(FlowMonitorHelper* fmhelper,
    Ptr<FlowMonitor> monitor,
    Gnuplot2dDataset dataset1)
{
    double delay = 0.0;
    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin();
         stats != flowStats.end(); ++stats) {
        // Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow
        // (stats->first);
        delay = stats->second.delaySum.GetSeconds();
        dataset1.Add((double)Simulator::Now().GetSeconds(), (double)delay);
    }

    // Tempo que será iniciado
    Simulator::Schedule(Seconds(1), &DelayMonitor, fmhelper, monitor, dataset1);
}

void LostPacketsMonitor(FlowMonitorHelper* fmhelper,
    Ptr<FlowMonitor> monitor,
    Gnuplot2dDataset dataset2)
{
    double packets = 0.0;
    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin();
         stats != flowStats.end(); ++stats) {
        // Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow
        // (stats->first);
        // packets = stats->second.lostPackets;
        packets = stats->second.txPackets - stats->second.rxPackets;
        dataset2.Add((double)Simulator::Now().GetSeconds(), (double)packets);
    }

    // Tempo que será iniciado
    Simulator::Schedule(Seconds(1), &LostPacketsMonitor, fmhelper, monitor,
        dataset2);
}

void JitterMonitor(FlowMonitorHelper* fmhelper,
    Ptr<FlowMonitor> monitor,
    Gnuplot2dDataset dataset3)
{
    double jitter = 0.0;
    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin();
         stats != flowStats.end(); ++stats) {
        // Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow
        // (stats->first);
        jitter = stats->second.jitterSum.GetSeconds();
        dataset3.Add((double)Simulator::Now().GetSeconds(), (double)jitter);
    }

    // Tempo que será iniciado
    Simulator::Schedule(Seconds(1), &LostPacketsMonitor, fmhelper, monitor,
        dataset3);
}

/*------------------------- STATUS DAS TRANSMISSÕES E CÁLCULO DE LOST
 * PACKETS--*/

void ImprimeMetricas(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor)
{
    double tempThroughput = 0.0;
    monitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(fmhelper->GetClassifier());

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin();
         stats != flowStats.end(); ++stats) {
        // A tuple: Source-ip, destination-ip, protocol, source-port,
        // destination-port
        Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow(stats->first);

        ns3::Ipv4Address a = fiveTuple.destinationAddress;

        NS_LOG_INFO("Flow ID: " << stats->first << " ; " << fiveTuple.sourceAddress
                                << " -----> " << fiveTuple.destinationAddress
                                << std::endl);
        NS_LOG_INFO("Tx Packets = " << stats->second.txPackets << std::endl);
        NS_LOG_INFO("Rx Packets = " << stats->second.rxPackets << std::endl);
        NS_LOG_INFO("Duration: " << stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()
                                 << std::endl);
        NS_LOG_INFO("Last Received Packet: "
            << stats->second.timeLastRxPacket.GetSeconds() << " Seconds"
            << std::endl);
        tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()) / 1024);
        NS_LOG_INFO("Throughput: " << tempThroughput << " Mbps" << std::endl);
        NS_LOG_INFO("Delay: " << stats->second.delaySum.GetSeconds() << std::endl);
        TxRate = (double)stats->second.rxPackets / (double)stats->second.txPackets;
        NS_LOG_INFO("LostPackets: "
            << stats->second.txPackets - stats->second.rxPackets
            << std::endl);
        NS_LOG_INFO("taxa de entrega: " << TxRate << std::endl);
        NS_LOG_INFO("Jitter: " << stats->second.jitterSum.GetSeconds()
                               << std::endl);
        NS_LOG_INFO("------------------------------------------" << std::endl);
    }
    // Tempo que será iniciado
    Simulator::Schedule(Seconds(1), &ImprimeMetricas, fmhelper, monitor);
}

static Vector GetPosition(Ptr<Node> node)
{
    Ptr<MobilityModel> mobility = node->GetObject<MobilityModel>();
    return mobility->GetPosition();
}

static Vector GetVelocity(Ptr<Node> node)
{
    Ptr<MobilityModel> mobility = node->GetObject<MobilityModel>();
    NS_LOG_DEBUG("NODE " << node->GetId() << " AT SPEED " << mobility->GetVelocity());
    return mobility->GetVelocity();
}

void VideoTraceParse(std::string m_videoTraceFileName)
{
    std::ifstream VideoTraceFile(m_videoTraceFileName.c_str(), ios::in);
    if (VideoTraceFile.fail()) {
        NS_FATAL_ERROR(">> EvalvidServer: Error while opening video trace file: "
            << m_videoTraceFileName.c_str());
        return;
    }

    uint32_t frameId;
    string frameType;
    uint32_t frameSize;
    uint16_t numOfUdpPackets;
    double sendTime;
    uint32_t packet = 0;

    while (VideoTraceFile >> frameId >> frameType >> frameSize >> numOfUdpPackets >> sendTime) {
        framePct[frameId - 1] = packet + 1;
        frameTypeGlobal[frameId - 1] = frameType;
        packet += numOfUdpPackets;
    }
}

std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

/*------------------------- CRIAÇÃO ARQUIVO COM QOS'S ----------------------*/
void WriteMetrics()
{
    NS_LOG_DEBUG(Simulator::Now().GetSeconds() << " Segundos...");
    for (int i = 0; i < 77; ++i)
        for (int u = 0; u < node_ue; ++u)
            if (cell_ue[i][u]) {
                std::stringstream rdTrace;
                rdTrace << "rd_a01_" << u;
                std::ifstream rdFile(rdTrace.str());

                double rdTime;
                std::string id;
                int rdPacketId;
                string a;
                int b;

                int lastPacket = 0;
                int counter;
                int npackets;

                while (rdFile >> rdTime >> id >> rdPacketId >> a >> b) {
                    receivedPackets[u][rdPacketId - 1] = true;
                    lastPacket = rdPacketId;
                }

                /*----------------QOS METRIC CALCULATION----------------*/
                int nReceived = 0;
                for (int i = lastPacket - 60; i <= lastPacket; ++i) {
                    if (receivedPackets[u][i])
                        ++nReceived;
                }
                if (lastPacket >= 60) {
                    NS_LOG_INFO("Taxa de recebimento, node " << u << " :"
                                                             << ((float)nReceived - 1) / 60);
                    //NS_LOG_INFO ("Recebidos " << nReceived << " pacotes.");
                    stringstream qosFilename;
                    double valorAtualQos = 0;
                    qosFilename << "qosTorre" << i + 1;
                    ifstream qosInFile(qosFilename.str());
                    while (qosInFile >> valorAtualQos) {
                    }
                    ofstream qosOutFile(qosFilename.str(),
                        std::ofstream::out | std::ofstream::trunc);
                    qosOutFile << (((float)nReceived - 1) / 60 + valorAtualQos) / 2;
                }

                /*--------------------------------------------------------*/

                /*-----------------QOE METRIC CALCULATION-----------------*/

                framePct[numberOfFrames] = numberOfPackets + 1;
                for (int j = 0; j < numberOfFrames; ++j) {
                    npackets = framePct[j + 1] - framePct[j];
                    counter = 0;
                    if (framePct[j] > lastPacket) {
                        continue;
                    }

                    for (int k = framePct[j]; k < framePct[j + 1]; ++k)
                        if (receivedPackets[u][k - 1]) {
                            ++counter;
                        }

                    if (npackets == counter) {
                        receivedFrames[u][j] = true;
                        LastReceivedFrame[u] = j + 1;
                    }
                }

                int lastGop = 0;
                int IReceived = 0;
                int ITotal = 0;
                double ILoss = 0;

                int PReceived = 0;
                int PTotal = 0;
                double PLoss = 0;

                int BReceived = 0;
                int BTotal = 0;
                double BLoss = 0;

                for (int j = 0; j < LastReceivedFrame[u] / 20; ++j)
                    ++lastGop;

                if (lastGop != 0) {
                    for (int j = 20 * (lastGop - 1); j < lastGop * 20 - 1; ++j) {
                        if (frameTypeGlobal[j].find("I") != std::string::npos || frameTypeGlobal[j].find("H") != std::string::npos)
                            ++ITotal;
                        else if (frameTypeGlobal[j].find("P") != std::string::npos)
                            ++PTotal;
                        else if (frameTypeGlobal[j].find("B") != std::string::npos)
                            ++BTotal;

                        if (receivedFrames[u][j]) {
                            if (frameTypeGlobal[j].find("I") != std::string::npos || frameTypeGlobal[j].find("H") != std::string::npos)
                                ++IReceived;
                            else if (frameTypeGlobal[j].find("P") != std::string::npos)
                                ++PReceived;
                            else if (frameTypeGlobal[j].find("B") != std::string::npos)
                                ++BReceived;
                        }
                    }
                    ILoss = ITotal != 0 ? ((double)ITotal - (double)IReceived) * 100 / (double)ITotal : 0;
                    PLoss = PTotal != 0 ? ((double)PTotal - (double)PReceived) * 100 / (double)PTotal : 0;
                    BLoss = BTotal != 0 ? ((double)BTotal - (double)BReceived) * 100 / (double)BTotal : 0;

                    std::stringstream cmd;
                    std::stringstream qoeFileName;
                    std::string qoeResult;

                    qoeFileName << "qoeTorre" << i + 1;
                    double valorAtualQoe = 0;
                    ifstream qoeInFile(qoeFileName.str());
                    while (qoeInFile >> valorAtualQoe) {
                    }

                    ofstream qoeOutFile(qoeFileName.str(),
                        std::ofstream::out | std::ofstream::trunc);

                    cmd << "python2.7 ia.py " << ILoss << " " << PLoss << " " << BLoss
                        << " 20";

                    qoeOutFile << (stod(exec(cmd.str().c_str())) + valorAtualQoe) / 2;

                    NS_LOG_DEBUG(cmd.str());

                    NS_LOG_INFO("NODE " << u << " QOE ESTIMADO " << (stod(exec(cmd.str().c_str())) + valorAtualQoe) / 2);

                    /*std::ifstream qoeFile(qoeFileName.str());
                    while (qoeFile >> qoeResult)
                        NS_LOG_INFO("NODE " << u << " QOE ESTIMADO " << qoeResult);*/
                }
            }
    NS_LOG_INFO("\n\n\n\n");
    return;
}

void requestStream(Ptr<Node> remoteHost, NodeContainer ueNodes, Ipv4Address remoteHostAddr, double simTime, double start)
{
    for (uint32_t i = 0; i < ueNodes.GetN(); ++i) {
        string video_trans = "st_container_cif_h264_300_20.st";

        std::stringstream sdTrace;
        std::stringstream rdTrace;
        std::stringstream rdWindow;
        sdTrace << "sd_a01_" << (int)i;
        rdTrace << "rd_a01_" << (int)i;

        double stop = simTime;
        uint16_t port = 2000;
        uint16_t m_port = 2000 * i + 2000; // Para alcançar o nó ZERO quando i = 0

        // Servidor de vídeo
        EvalvidServerHelper server(m_port);
        server.SetAttribute("SenderTraceFilename", StringValue(video_trans));
        server.SetAttribute("SenderDumpFilename", StringValue(sdTrace.str()));
        server.SetAttribute("PacketPayload", UintegerValue(512));
        ApplicationContainer apps = server.Install(remoteHost);
        apps.Start(Seconds(start));
        apps.Stop(Seconds(stop));

        // Clientes do vídeo
        EvalvidClientHelper client(remoteHostAddr, m_port);
        client.SetAttribute("ReceiverDumpFilename", StringValue(rdTrace.str()));
        apps = client.Install(ueNodes.Get(i));
        apps.Start(Seconds(start + 1));
        apps.Stop(Seconds(stop));

        Ptr<Ipv4> ipv4 = ueNodes.Get(i)->GetObject<Ipv4>();
        Ipv4InterfaceAddress iaddr = ipv4->GetAddress(1, 0);
        Ipv4Address addri = iaddr.GetLocal();
    }
}

void resend(Ptr<Node> remoteHost, NodeContainer ueNodes, Ipv4Address remoteHostAddr, double simTime, double start)
{
    for (uint32_t i = 0; i < ueNodes.GetN(); ++i) {
        std::stringstream filename;
        filename << "sd_a01_" << (int)i;

        ifstream in(filename.str());
        if (in.is_open()) {
            in.seekg(0, ios::end);
            size_t size = in.tellg();
            if (size == 0) {
                string video_trans = "st_container_cif_h264_300_20.st";

                std::stringstream sdTrace;
                std::stringstream rdTrace;
                std::stringstream rdWindow;
                sdTrace << "sd_a01_" << (int)i;
                rdTrace << "rd_a01_" << (int)i;

                double stop = simTime;
                uint16_t m_port = 2000 * i + 2000; // Para alcançar o nó ZERO quando i = 0

                // Servidor de vídeo
                EvalvidServerHelper server(m_port);
                server.SetAttribute("SenderTraceFilename", StringValue(video_trans));
                server.SetAttribute("SenderDumpFilename", StringValue(sdTrace.str()));
                server.SetAttribute("PacketPayload", UintegerValue(512));
                ApplicationContainer apps = server.Install(remoteHost);
                apps.Start(Seconds(start));
                apps.Stop(Seconds(stop));

                // Clientes do vídeo
                EvalvidClientHelper client(remoteHostAddr, m_port);
                client.SetAttribute("ReceiverDumpFilename", StringValue(rdTrace.str()));
                apps = client.Install(ueNodes.Get(i));
                apps.Start(Seconds(start + 2));
                apps.Stop(Seconds(stop));
            }
        }
    }
}
uint16_t cnt = 0;
void ReceivePacket(std::string context, Ptr<const Packet> packet)
{
    cnt++;
    std::ostringstream stream;
    NS_LOG_INFO("CG Rx from Enb ok " << cnt << " times, and packet size is: " << packet->GetSize() << std::endl);
}
/*--------------------------MAIN FUNCTION-------------------------*/
int main(int argc, char* argv[])
{
    for (int u = 0; u < node_ue; ++u) {
        for (int i = 0; i < numberOfPackets; ++i)
            receivedPackets[u][i] = false;

        for (int i = 0; i < numberOfFrames; ++i)
            receivedFrames[u][i] = false;
    }
    /*---------------------CRIAÇÃO DE OBJETOS ÚTEIS-----------------*/
    double interPacketInterval = 0.1;

    std::string handoverAlg = "ahp";

    VideoTraceParse("st_container_cif_h264_300_20.st");

    // void WriteMetrics();

    /*--------------------- COMMAND LINE PARSING -------------------*/
    //std::string entradaSumo = "mobil/reta2km.tcl"; // Mobilidade usada
    std::string entradaSumo = "mobil/3gppMobility.tcl"; // Mobilidade usada

    CommandLine cmm;
    cmm.AddValue("entradaSumo", "arquivo de entrada de mobilidade", entradaSumo);
    cmm.AddValue("node_cbr", "nós de cbr", n_cbr);
    cmm.AddValue("node_hpn", "torrer high power", enb_HPN);
    cmm.AddValue("node_low_power", "torres low power", low_power);
    cmm.AddValue("node_hot_spot", "hot spots", hot_spot);
    cmm.AddValue("txpower", "txpower", txpower);
    cmm.AddValue("handoverAlg", "Handover algorith in use", handoverAlg);
    cmm.Parse(argc, argv);

    // asssertions
    NS_ASSERT_MSG(node_ue < 51, "exceeded number of nodes.");
    NS_ASSERT_MSG(n_cbr <= 7, "exceeded number of cbr nodes.");
    NS_ASSERT_MSG(enb_HPN + low_power + hot_spot <= 77, "Too many towers.");

    // Logs

    /*LogComponentEnable("v2x_3gpp", LOG_LEVEL_DEBUG);
    LogComponentEnable("v2x_3gpp", LOG_LEVEL_INFO);
    LogComponentEnable("AhpHandoverAlgorithm", LOG_LEVEL_INFO);
    LogComponentEnable("AhpHandoverAlgorithm", LOG_LEVEL_DEBUG);
    LogComponentEnable("EvalvidClient", LOG_LEVEL_INFO);
    //LogComponentEnable("LteAnr", LOG_LEVEL_FUNCTION);
    /*LogComponentEnable("LteEnbNetDevice", LOG_LEVEL_FUNCTION);
    LogComponentEnable("EvalvidServer", LOG_LEVEL_INFO);
    LogComponentEnable("A2A4RsrqHandoverAlgorithm", LOG_LEVEL_LOGIC);*/

    //-------------Parâmetros da simulação
    uint16_t node_remote = 1; // HOST_REMOTO
    for (double t = 0; t < simTime; t += 1)
        Simulator::Schedule(Seconds(t), &WriteMetrics);
    /*----------------------------------------------------------------------*/

    //*********** CONFIGURAÇÃO LTE ***************//
    // Configuração padrão de Downlink e Uplink
    Config::SetDefault("ns3::LteEnbNetDevice::DlBandwidth", UintegerValue(25));
    Config::SetDefault("ns3::LteEnbNetDevice::UlBandwidth", UintegerValue(25));

    // Modo de transmissão (SISO [0], MIMO [1])
    Config::SetDefault("ns3::LteEnbRrc::DefaultTransmissionMode",
        UintegerValue(1));

    /*------------------------- MÓDULOS LTE ----------------------*/
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    Ptr<PhyStatsCalculator> m_phyStats = CreateObject<PhyStatsCalculator>();

    lteHelper->SetEnbDeviceAttribute("DlEarfcn", UintegerValue(100));
    lteHelper->SetEnbDeviceAttribute("UlEarfcn", UintegerValue(18100));
    lteHelper->SetSchedulerType("ns3::PssFfMacScheduler");
    lteHelper->SetSchedulerAttribute(
        "nMux",
        UintegerValue(1)); // the maximum number of UE selected by TD scheduler
    lteHelper->SetSchedulerAttribute(
        "PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS

    // Ptr<EpcHelper> epcHelper = CreateObject<EpcHelper> ();
    lteHelper->SetEpcHelper(epcHelper);
    // lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");
    lteHelper->SetAttribute("PathlossModel",
        StringValue("ns3::NakagamiPropagationLossModel"));

    /**----------------ALGORITMO DE
*HANDOVER---------------------------------------*/
    if (handoverAlg == "ahp")
        lteHelper->SetHandoverAlgorithmType("ns3::AhpHandoverAlgorithm");

    if (handoverAlg == "noop")
        lteHelper->SetHandoverAlgorithmType("ns3::NoOpHandoverAlgorithm");

    if (handoverAlg == "a2a4") {
        lteHelper->SetHandoverAlgorithmType("ns3::A3RsrpHandoverAlgorithm");
        lteHelper->SetHandoverAlgorithmAttribute("Hysteresis", DoubleValue(3.0));
        lteHelper->SetHandoverAlgorithmAttribute("TimeToTrigger",
            TimeValue(MilliSeconds(256)));
    }

    if (handoverAlg == "a3") {
        lteHelper->SetHandoverAlgorithmType("ns3::A2A4RsrqHandoverAlgorithm");
        lteHelper->SetHandoverAlgorithmAttribute("ServingCellThreshold",
            UintegerValue(30));
        lteHelper->SetHandoverAlgorithmAttribute("NeighbourCellOffset",
            UintegerValue(1));
    }

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();

    Config::SetDefault("ns3::LteHelper::UseIdealRrc", BooleanValue(true));

    //-------------Parâmetros da Antena
    lteHelper->SetEnbAntennaModelType("ns3::CosineAntennaModel");
    lteHelper->SetEnbAntennaModelAttribute("Orientation", DoubleValue(0));
    lteHelper->SetEnbAntennaModelAttribute("Beamwidth", DoubleValue(60));
    lteHelper->SetEnbAntennaModelAttribute("MaxGain", DoubleValue(0.0));

    //-------------Criação do RemoteHost
    // Cria um simples RemoteHost
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(node_remote);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);

    // Pilha de Internet
    InternetStackHelper internet;
    internet.Install(remoteHost);

    // Cria link Internet
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.010)));
    p2ph.EnablePcapAll("ahp-handover");
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);

    // Determina endereço ip para o Link
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("10.1.0.0", "255.255.0.0");
    Ipv4InterfaceContainer internetIpIfaces;
    internetIpIfaces = ipv4h.Assign(internetDevices);

    // interface 0 é localhost e interface 1 é dispositivo p2p
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"),
        Ipv4Mask("255.0.0.0"), 1);
    /*----------------------------------------------------------------------*/

    /*------------------- Criacao de UEs-Enb--------------------------*/
    // UE - Veículos
    NodeContainer ueNodes;
    ueNodes.Create(node_ue);

    NodeContainer cbr_nodes;
    cbr_nodes.Create(n_cbr);

    // eNODEb
    NodeContainer enbNodes;
    enbNodes.Create(enb_HPN + low_power + hot_spot);

    // Instala pilha de Internet em UE e EnodeB
    internet.Install(ueNodes);
    internet.Install(cbr_nodes);

    /*-----------------POSIÇÃO DAS TORRES----------------------------------*/
    Ptr<ListPositionAllocator> HpnPosition = CreateObject<ListPositionAllocator>();
    ArrayPositionAllocator(HpnPosition);

    MobilityHelper remoteHostMobility;
    remoteHostMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    remoteHostMobility.Install(remoteHost);
    remoteHostMobility.Install(pgw);

    /*-----------------MONILIDADE DAS TORRES (PARADA)--------------*/

    MobilityHelper mobilityEnb;
    mobilityEnb.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityEnb.SetPositionAllocator(HpnPosition);
    mobilityEnb.Install(enbNodes);

    MobilityHelper mobilityCbr;
    mobilityEnb.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityEnb.SetPositionAllocator(HpnPosition);
    mobilityEnb.Install(cbr_nodes);

    // LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

    /*---------------MONILIDADE DOS CARROS------------------------------*/
    Ns2MobilityHelper mobility = Ns2MobilityHelper(entradaSumo);

    // open log file for output
    std::ofstream os;
    os.open("gri50ns2-mobility-trace.log");

    mobility.Install(ueNodes.Begin(), ueNodes.End()); // configure movements for
    // each node, while reading
    // trace file

    os.close(); // close log file

    /*----------------------------------------------------------------------*/

    //-------------Instala LTE Devices para cada grupo de nós
    NetDeviceContainer enbLteDevs;
    enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs;
    ueLteDevs = lteHelper->InstallUeDevice(ueNodes);
    NetDeviceContainer cbrLteDevs;
    cbrLteDevs = lteHelper->InstallUeDevice(cbr_nodes);

    /*----------------------------------------------------------------------*/

    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));
    Ipv4InterfaceContainer cbrIpFace;
    cbrIpFace = epcHelper->AssignUeIpv4Address(NetDeviceContainer(cbrLteDevs));

    //-------------Definir endereços IPs e instala aplicação
    for (uint32_t u = 0; u < ueNodes.GetN(); ++u) {
        Ptr<Node> ueNode = ueNodes.Get(u);
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(),
            1);
    }

    /*-------------------------CONFIGURAÇÃO DE CBR-------------------------*/
    uint16_t otherPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

    //--------------------DEFINIR GATEWAY---------------------
    for (uint32_t u = 0; u < cbr_nodes.GetN(); ++u) {
        Ptr<Node> ueNode = cbr_nodes.Get(u);
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(),
            1);

        Ptr<Ipv4> ipv4 = cbr_nodes.Get(u)->GetObject<Ipv4>();
        Ipv4InterfaceAddress iaddr = ipv4->GetAddress(1, 0);
        Ipv4Address addri = iaddr.GetLocal();
        PacketSinkHelper packetSinkHelper(
            "ns3::UdpSocketFactory",
            InetSocketAddress(Ipv4Address::GetAny(), otherPort));
        serverApps.Add(packetSinkHelper.Install(cbr_nodes.Get(u)));
        serverApps.Start(Seconds(15));

        UdpClientHelper client(addri, otherPort);
        client.SetAttribute("Interval",
            TimeValue(MilliSeconds(interPacketInterval)));
        client.SetAttribute("MaxPackets", UintegerValue(1000000));

        clientApps.Add(client.Install(remoteHost));

        clientApps.Start(Seconds(15));
    }

    /*-----------------POTENCIA DE TRASMISSAO-----------------*/
    Ptr<LteEnbPhy> enb0Phy;

    for (int i = 0; i < enbLteDevs.GetN(); i++) {
        enb0Phy = enbLteDevs.Get(i)->GetObject<LteEnbNetDevice>()->GetPhy();
        if (i < enb_HPN) {
            enb0Phy->SetTxPower(46);
        }
        else if (i < low_power) {
            enb0Phy->SetTxPower(30);
        }
        else {
            enb0Phy->SetTxPower(15);
        }
    }

    //LogComponentEnable("LteUePhy", LOG_LEVEL_INFO);
    /*
    Ptr<LteUePhy> ue0Phy;
    for (int i = 0; i < ueLteDevs.GetN(); i++) {
        ue0Phy = ueLteDevs.Get(i)->GetObject<LteUeNetDevice>()->GetPhy();
        enb0Phy->SetTxPower(txpower);
        NS_LOG_INFO("Node " << i << " transmitting at " << enb0Phy->GetTxPower() << " dbm");
    }
    */

    //-------------Anexa as UEs na eNodeB
    lteHelper->Attach(ueLteDevs);
    lteHelper->AttachToClosestEnb(cbrLteDevs, enbLteDevs);
    lteHelper->AddX2Interface(enbNodes);

    NS_LOG_INFO("Create Applications.");

    // Início Transmissão de Vídeo
    //Rodar aplicação EvalVid
    requestStream(remoteHost, ueNodes, remoteHostAddr, simTime, transmissionStart);
    for (int i = 1; i < simTime - 10; i += 1)
        resend(remoteHost, ueNodes, remoteHostAddr, simTime, i);

    /*----------------NETANIM-------------------------------*/
    AnimationInterface anim("LTEnormal_v2x.xml");
    // Cor e Descrição para eNb
    for (uint32_t i = 0; i < enbNodes.GetN(); ++i) {
        anim.UpdateNodeDescription(enbNodes.Get(i), "eNb");
        anim.UpdateNodeColor(enbNodes.Get(i), 0, 255, 0);
    }

    /*---------------------- Simulation Stopping Time ----------------------*/
    Simulator::Stop(SIMULATION_TIME_FORMAT(simTime));

    /*---------------------GERAÇÃO DE PLOTS---------------------------------*/

    string tipo = "graficos/LTE_";

    Ptr<FlowMonitor> monitor;
    FlowMonitorHelper fmhelper;
    monitor = fmhelper.InstallAll();

    /*--------------NOTIFICAÇÕES DE HANDOVER E SINAL-------------------------*/
    // Config::Connect("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished",
    //                MakeCallback(&NotifyConnectionEstablishedEnb));
    Config::Connect("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished",
        MakeCallback(&NotifyConnectionEstablishedUe));
    // Config::Connect("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
    //                MakeCallback(&NotifyHandoverStartEnb));
    Config::Connect("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
        MakeCallback(&NotifyHandoverStartUe));
    // Config::Connect("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
    //                MakeCallback(&NotifyHandoverEndOkEnb));
    Config::Connect("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
        MakeCallback(&NotifyHandoverEndOkUe));

    Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/$ns3::LteEnbNetDevice/LteEnbPhy/UlSpectrumPhy/RxEndOk", MakeCallback(&ReceivePacket));
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/$ns3::LteEnbNetDevice/LteEnbPhy/DlSpectrumPhy/RxEndOk", MakeCallback(&ReceivePacket));
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/$ns3::LteEnbNetDevice/ComponentCarrierMap/*/LteEnbPhy/UlSpectrumPhy/RxEndOk", MakeCallback(&ReceivePacket));
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/$ns3::LteEnbNetDevice/ComponentCarrierMap/*/LteEnbPhy/UlSpectrumPhy/TxEndOk", MakeCallback(&ReceivePacket));

    /*----------------PHY TRACES ------------------------------------*/
    lteHelper->EnablePhyTraces();
    lteHelper->EnableUlPhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();
    lteHelper->EnablePdcpTraces();

    // gera o arquivo de métrica
    /*--------------------------- Simulation Run ---------------------------*/
    Simulator::Run(); // Executa

    monitor->SerializeToXmlFile("V2X/LTE01_flow.xml", true, true);

    Simulator::Destroy();

    //ImprimeMetricas(&fmhelper, monitor);

    NS_LOG_INFO("realizados " << handNumber << " handovers");

    Simulator::Run();
    return EXIT_SUCCESS;
}
