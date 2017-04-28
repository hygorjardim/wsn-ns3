// Autor: Hygor Jardim da Silva
// This example is based on the "wsn-ping6.cc" example.

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"

// Bibliotecas para o FlowMonitor
#include "ns3/gnuplot.h"
#include "ns3/flow-monitor-module.h"
#include <ns3/flow-monitor-helper.h>

using namespace ns3;

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet);


NS_LOG_COMPONENT_DEFINE ("WSN-802.15.04-FlowMonitor");

int main (int argc, char **argv)
{
  bool verbose = false;

  CommandLine cmd;
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable ("Ping6WsnExample", LOG_LEVEL_INFO);
    }

  NS_LOG_INFO ("Create Nodes");
  NodeContainer nodes;
  nodes.Create (100); 

  NodeContainer ap;
  ap.Create (1);

  NodeContainer allnodes;
  allnodes.Add (ap);
  allnodes.Add (nodes);

  NS_LOG_INFO ("Create Mobility");
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0), //onde inicia no eixo X
                                 "MinY", DoubleValue (0.0), //onde inicia no eixo Y
                                 "DeltaX", DoubleValue (20.0), // Distância entre nós
                                 "DeltaY", DoubleValue (20.0), // Distância entre nós
                                 "GridWidth", UintegerValue (5), // Quantidade de colunas em uma linha
                                 "LayoutType", StringValue ("RowFirst")); // Definindo posições em linha
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (10.0), //
                                 "MinY", DoubleValue (10.0), //
                                 "DeltaX", DoubleValue (0.0), // Distância entre nós
                                 "DeltaY", DoubleValue (0.0), // Distância entre nós
                                 "GridWidth", UintegerValue (1), // Quantidade de colunas em uma linha
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ap);

  NS_LOG_INFO ("Create Channels");
  LrWpanHelper lrWpanHelper;
  NetDeviceContainer devContainer = lrWpanHelper.Install(allnodes);
  lrWpanHelper.AssociateToPan (devContainer, 10);

  NS_LOG_INFO ("Install Internet Stack");
  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.Install (allnodes);
 
  NS_LOG_INFO ("Install 6LoWPAN");
  SixLowPanHelper sixlowpan;
  NetDeviceContainer nodeNetDevice = sixlowpan.Install (devContainer);

  NS_LOG_INFO ("Assign Addresses");

  Address serverAddress;

  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer nodeInterface = ipv6.Assign (nodeNetDevice);
  // nodeInterface.SetForwarding (1, true);
  serverAddress = Address(nodeInterface.GetAddress (1,1));


  NS_LOG_INFO ("Create Applications");

  uint16_t port = 9;
  UdpEchoServerHelper echoServer (port); // Porta que irá ouvir 
  ApplicationContainer serverApps = echoServer.Install (allnodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (100));

  UdpEchoClientHelper echoClient (serverAddress, port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1000));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", StringValue ("256Byte"));
  ApplicationContainer clientApps = echoClient.Install (nodes); // Instala nos 20 nós estáticos 
  clientApps.Start (Seconds (1.5));
  clientApps.Stop (Seconds (100.0));

//-----------------------------FlowMonitor-Gnuplot-Throughput-------------------------

    std::string fileNameWithNoExtension = "FlowVSThroughput_";
    std::string graphicsFileName        = fileNameWithNoExtension + ".png";
    std::string plotFileName            = fileNameWithNoExtension + ".plt";
    std::string plotTitle               = "Flow vs Throughput";
    std::string dataTitle               = "Throughput";

    // Instantiate the plot and set its title.
    Gnuplot gnuplot (graphicsFileName);
    gnuplot.SetTitle (plotTitle);

    // Make the graphics file, which the plot file will be when it
    // is used with Gnuplot, be a PNG file.
    gnuplot.SetTerminal ("png");

    // Set the labels for each axis.
    gnuplot.SetLegend ("Flow", "Throughput");

     
   Gnuplot2dDataset dataset;
   dataset.SetTitle (dataTitle);
   dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

    //flowMonitor declaration
    FlowMonitorHelper fmHelper;
    Ptr<FlowMonitor> allMon = fmHelper.InstallAll();
    // call the flow monitor function
    ThroughputMonitor(&fmHelper, allMon, dataset); 


  NS_LOG_INFO ("Run Simulation");
  Simulator::Stop (Seconds (101));
  Simulator::Run ();

  gnuplot.AddDataset (dataset);
  std::ofstream plotFile (plotFileName.c_str());
  gnuplot.GenerateOutput (plotFile);
  plotFile.close ();

  Simulator::Destroy ();
  NS_LOG_INFO ("Done");
  return 0;
} 

//-------------------------Metodo-VAZÃO---------------------------

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet) // OK
  {
          double localThrou=0; // OK
      std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats(); // OK
      Ptr<Ipv6FlowClassifier> classing = DynamicCast<Ipv6FlowClassifier> (fmhelper->GetClassifier6()); // OK
      for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats)
      {
          if(stats->first == 2){
          Ipv6FlowClassifier::FiveTuple fiveTuple = classing->FindFlow (stats->first);
          std::cout<<"Flow ID     : " << stats->first <<" ; "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
          std::cout<<"Tx Packets = " << stats->second.txPackets<<std::endl;
          std::cout<<"Rx Packets = " << stats->second.rxPackets<<std::endl;
          std::cout<<"Duration    : "<<(stats->second.timeLastRxPacket.GetSeconds()-stats->second.timeFirstTxPacket.GetSeconds())<<std::endl;
          std::cout<<"Last Received Packet  : "<< stats->second.timeLastRxPacket.GetSeconds()<<" Seconds"<<std::endl;
          std::cout<<"Throughput: " << stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds()-stats->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps"<<std::endl;
                localThrou=(stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds()-stats->second.timeFirstTxPacket.GetSeconds())/1024/1024);
          // updata gnuplot data
                DataSet.Add((double)Simulator::Now().GetSeconds(),(double) localThrou);
          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
        }
    }
      Simulator::Schedule(Seconds(1),&ThroughputMonitor, fmhelper, flowMon,DataSet);

      {
      flowMon->SerializeToXmlFile ("ThroughputMonitor.xml", true, true);
      }
  }