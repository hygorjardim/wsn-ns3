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
#include "ns3/flow-monitor-helper.h"

// Biblioteca para o NetAnimator 
#include "ns3/netanim-module.h"

// Bibliotecas para gerar o Tracing
#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

// Test Channel
#include <iostream>
#include "ns3/spectrum-model-ism2400MHz-res1MHz.h"
#include "ns3/spectrum-model-300kHz-300GHz-log.h"
#include "ns3/wifi-spectrum-value-helper.h"
#include "ns3/multi-model-spectrum-channel.h"
#include "ns3/waveform-generator.h"
#include "ns3/spectrum-analyzer.h"
#include "ns3/spectrum-converter.h"
#include "ns3/log.h"
#include "string"
#include "ns3/friis-spectrum-propagation-loss.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/mobility-module.h"
#include "ns3/spectrum-helper.h"
#include "ns3/applications-module.h"
#include "ns3/adhoc-aloha-noack-ideal-phy-helper.h"
#include "ns3/waveform-generator-helper.h"
#include "ns3/spectrum-analyzer-helper.h"
#include "ns3/non-communicating-net-device.h"
#include "ns3/lr-wpan-spectrum-value-helper.h"

// OLSR 
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-helper.h"
#include "ns3/ipv6-list-routing.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/olsr-helper.h"
#include "ns3/olsr-header.h"

// AODV
#include "ns3/aodv-module.h"

using namespace ns3;

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet);
void JitterMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset2);
void DelayMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset3);

NS_LOG_COMPONENT_DEFINE ("WSN-802.15.04-FlowMonitor");

int main (int argc, char **argv)
{
  bool verbose = false;
  uint32_t packetSize = 64;
  int maxPackets = 1000;
  // double txPower = 0;
  // uint32_t channelNumber = 11;


  CommandLine cmd;
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable ("WSN-Ecoserver", LOG_LEVEL_INFO);
    }

  NS_LOG_INFO ("Create Nodes");
  NodeContainer nodes;
  nodes.Create (25); 

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
                                 "DeltaX", DoubleValue (25.0), // Distância entre nós
                                 "DeltaY", DoubleValue (25.0), // Distância entre nós
                                 "GridWidth", UintegerValue (5), // Quantidade de colunas em uma linha
                                 "LayoutType", StringValue ("RowFirst")); // Definindo posições em linha
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (50.0), //
                                 "MinY", DoubleValue (50.0), //
                                 "DeltaX", DoubleValue (0.0), // Distância entre nós
                                 "DeltaY", DoubleValue (0.0), // Distância entre nós
                                 "GridWidth", UintegerValue (1), // Quantidade de colunas em uma linha
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ap);

  NS_LOG_INFO ("Create Channels");

    SpectrumChannelHelper channelHelper = SpectrumChannelHelper::Default ();
    channelHelper.SetChannel ("ns3::MultiModelSpectrumChannel");
    Ptr<SpectrumChannel> channel = channelHelper.Create ();

    LrWpanHelper lrWpanHelper(channel);
    NetDeviceContainer devContainer = lrWpanHelper.Install(allnodes);
    lrWpanHelper.AssociateToPan (devContainer, 12);

  NS_LOG_INFO ("Install 6LoWPAN");

    SixLowPanHelper sixlowpan;
    NetDeviceContainer nodeNetDevice = sixlowpan.Install (devContainer);

  NS_LOG_INFO ("Seting Routing Protocol");

    OlsrHelper olsr;
    AodvHelper aodv;

    // Ipv6StaticRoutingHelper staticRouting;
    // Ipv6ListRoutingHelper list;
    
    // list.Add (staticRouting, 0);
    // list.Add (olsr, 100);

  NS_LOG_INFO ("Install Internet Stack");

    InternetStackHelper internetv6;
    internetv6.SetIpv4StackInstall (false);
    internetv6.SetRoutingHelper (aodv);
    internetv6.Install (allnodes);

    // InternetStackHelper internetv6Nodes;
    // internetv6Nodes.SetIpv4StackInstall (false);
    // internetv6Nodes.Install (nodes);

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
    ApplicationContainer serverApps = echoServer.Install (allnodes.Get (12));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (100.0));

    UdpEchoClientHelper echoClient (serverAddress, port);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (maxPackets));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
    ApplicationContainer clientApps = echoClient.Install (nodes);
    clientApps.Start (Seconds (1.1));
    clientApps.Stop (Seconds (100.0));


    // AsciiTraceHelper ascii;
    // lrWpanHelper.EnableAsciiAll (ascii.CreateFileStream ("tracing-lrWpanHelper.tr"));
    // lrWpanHelper.EnablePcapAll ("tracing-lrWpanHelper", true);

//-----------------------------FlowMonitor-Gnuplot-Throughput-------------------------

    std::string fileNameWithNoExtension = "Throughput";
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


   //-----------------FlowMonitor-JITTER----------------

    std::string fileNameWithNoExtension2 = "FlowVSJitter_";
    std::string graphicsFileName2      = fileNameWithNoExtension2 + ".png";
    std::string plotFileName2        = fileNameWithNoExtension2 + ".plt";
    std::string plotTitle2           = "Flow vs Jitter";
    std::string dataTitle2           = "Jitter";

    Gnuplot gnuplot2 (graphicsFileName2);
    gnuplot2.SetTitle(plotTitle2);

  gnuplot2.SetTerminal("png");

  gnuplot2.SetLegend("Flow", "Jitter");

  Gnuplot2dDataset dataset2;
  dataset2.SetTitle(dataTitle2);
  dataset2.SetStyle(Gnuplot2dDataset::LINES_POINTS);

  //FlowMonitorHelper fmHelper;
  //Ptr<FlowMonitor> allMon = fmHelper.InstallAll();

  JitterMonitor(&fmHelper, allMon, dataset2);

//--------------FlowMonitor-DELAY--------------------

  std::string fileNameWithNoExtension3 = "FlowVSDelay_";
  std::string graphicsFileName3      = fileNameWithNoExtension3 + ".png";
  std::string plotFileName3        = fileNameWithNoExtension3 + ".plt";
  std::string plotTitle3       = "Flow vs Delay";
  std::string dataTitle3       = "Delay";

      Gnuplot gnuplot3 (graphicsFileName3);
      gnuplot3.SetTitle(plotTitle3);

      gnuplot3.SetTerminal("png");

      gnuplot3.SetLegend("Flow", "Delay");

      Gnuplot2dDataset dataset3;
      dataset3.SetTitle(dataTitle3);
      dataset3.SetStyle(Gnuplot2dDataset::LINES_POINTS);

      DelayMonitor(&fmHelper, allMon, dataset3);

//---------------------------------------------------

  NS_LOG_INFO ("Run Simulation");
  Simulator::Stop (Seconds (120.1));

//-------------------------Metodo-Animation-------------------------

      AnimationInterface anim ("anim-wsn.xml"); // Mandatory
      for (uint32_t i = 0; i < nodes.GetN (); ++i)
      {
        anim.UpdateNodeDescription (nodes.Get (i), "STA"); // Optional
        anim.UpdateNodeColor (nodes.Get (i), 255, 0, 0); // Coloração
      }
      for (uint32_t i = 0; i < ap.GetN (); ++i)
      {
        anim.UpdateNodeDescription (ap.Get (i), "AP"); // Optional
        anim.UpdateNodeColor (ap.Get (i), 0, 255, 0); // Coloração
      }
      anim.EnablePacketMetadata (); // Optional

  Simulator::Run ();


  gnuplot.AddDataset (dataset);
  std::ofstream plotFile (plotFileName.c_str());
  gnuplot.GenerateOutput (plotFile);
  plotFile.close ();


  gnuplot2.AddDataset(dataset2);;
  std::ofstream plotFile2 (plotFileName2.c_str());
  gnuplot2.GenerateOutput(plotFile2);
  plotFile2.close();


  gnuplot3.AddDataset(dataset3);;
  std::ofstream plotFile3 (plotFileName3.c_str());
  gnuplot3.GenerateOutput(plotFile3);
  plotFile3.close();

  Simulator::Destroy ();
  NS_LOG_INFO ("Done");
  return 0;
} 

//-----------------------------Metodo-Vazão-------------------------------------------

  void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet) // OK
    {
            double localThrou=0; // OK
        std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats(); // OK
        Ptr<Ipv6FlowClassifier> classing = DynamicCast<Ipv6FlowClassifier> (fmhelper->GetClassifier6()); // OK
        for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats)
        {
            //if(stats->first == 2){
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
          //}
      }
        Simulator::Schedule(Seconds(1),&ThroughputMonitor, fmhelper, flowMon,DataSet);

        {
        flowMon->SerializeToXmlFile ("ThroughputMonitor.xml", true, true);
        }
    }
//-------------------------Metodo-JINTTER-------------------------

double atraso1=0;
void JitterMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset2)
{
       double localJitter=0;
       double atraso2 =0;

       std::map<FlowId, FlowMonitor::FlowStats> flowStats2 = flowMon->GetFlowStats();
       Ptr<Ipv6FlowClassifier> classing2 = DynamicCast<Ipv6FlowClassifier> (fmHelper->GetClassifier6());
       for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats2 = flowStats2.begin(); stats2 != flowStats2.end(); ++stats2)
       {
               if(stats2->first == 2){//IFFFFFFFFFFF
            Ipv6FlowClassifier::FiveTuple fiveTuple2 = classing2->FindFlow (stats2->first);
    std::cout<<"Flow ID : "<< stats2->first <<";"<< fiveTuple2.sourceAddress <<"------>" <<fiveTuple2.destinationAddress<<std::endl;
    std::cout<<"Tx Packets = " << stats2->second.txPackets<<std::endl;
    std::cout<<"Rx Packets = " << stats2->second.rxPackets<<std::endl;
    std::cout<<"Duration  : "<<(stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeFirstTxPacket.GetSeconds())<<std::endl;
    std::cout<<"Atraso: "<<stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeLastTxPacket.GetSeconds() <<"s"<<std::endl;
atraso2 = stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeLastTxPacket.GetSeconds();
// atraso1 = stats2->second.timeFirstRxPacket.GetSeconds()-stats2->second.timeFirstTxPacket.GetSeconds();
    std::cout<<"Jitter: "<< atraso2-atraso1 <<std::endl;
    localJitter= atraso2-atraso1;//Jitter
    Dataset2.Add((double)Simulator::Now().GetSeconds(), (double) localJitter);
    std::cout<<"---------------------------------------------------------------------------"<<std::endl;
               }//IFFFFFFFFFF

       atraso1 = atraso2;
       }

       Simulator::Schedule(Seconds(1),&JitterMonitor, fmHelper, flowMon, Dataset2);
       {
         flowMon->SerializeToXmlFile("JitterMonitor.xml", true, true);
       }
}
//-----------------------------Metodo-Delay-------------------------------------------

  void  DelayMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset3)
  {
    double localDelay=0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats3 = flowMon->GetFlowStats();
    Ptr<Ipv6FlowClassifier> classing3 = DynamicCast<Ipv6FlowClassifier> (fmHelper->GetClassifier6());
    for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats3 = flowStats3.begin(); stats3 != flowStats3.end(); ++stats3)
    {
                 if(stats3->first == 2){//IFFFFFFFFFFF
            Ipv6FlowClassifier::FiveTuple fiveTuple3 = classing3->FindFlow (stats3->first);
            std::cout<<"Flow ID : "<< stats3->first <<";"<< fiveTuple3.sourceAddress <<"------>" <<fiveTuple3.destinationAddress<<std::endl;
            localDelay = stats3->second.timeLastRxPacket.GetSeconds()-stats3->second.timeLastTxPacket.GetSeconds();
          Dataset3.Add((double)Simulator::Now().GetSeconds(), (double) localDelay);
          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
      }//IFFFFFFFFF
    }
    Simulator::Schedule(Seconds(1),&DelayMonitor, fmHelper, flowMon, Dataset3);
    {
       flowMon->SerializeToXmlFile("DelayMonitor.xml", true, true);
    }
  }
