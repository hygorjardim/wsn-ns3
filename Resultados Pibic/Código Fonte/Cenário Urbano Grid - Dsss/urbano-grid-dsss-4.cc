/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Autor: Hygor Jardim da Silva, Castanhal - Pará, Brasil. 
 * Contato: hygorjardim@gmail.com - https://hygorjardim.github.io/
 * Universidade Federal do Pará - Campus Universitário de Castanhal
 * Faculdade de Computação - FACOMP
 * Laboratório de Desenvolvimento de Sistemas - LADES
 */
#include <fstream>
#include <iostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/olsr-module.h"
#include "ns3/aodv-module.h"
#include "ns3/dsr-module.h"
#include "ns3/dsdv-module.h"

// Bibliotecas para o FlowMonitor
#include "ns3/gnuplot.h"
#include "ns3/flow-monitor-module.h"
#include <ns3/flow-monitor-helper.h>

using namespace ns3;

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet);
void JitterMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset2);
void DelayMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset3);

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  std::string phyMode ("DsssRate11Mbps"); // Modo de Dados - DsssRate or OfdmRate
  cmd.Parse (argc, argv);
  bool Random = false;
  bool Velocity = false;
  bool ifAodv = true;
  bool ifOlsr = false;
  bool ifStatic = false;
  bool ifDsr = false;
  bool ifDsdv = false;
  bool ifEco = true;
  bool ifOnoff = false;   
  double speed = -8;
  double distance = 20; // m 
  uint32_t numNodes = 25;
  uint32_t sinkNode = 12;
  uint32_t nSinks = 5;
   uint16_t port = 9;
  std::string rate = "1.512kbps";
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Mode", StringValue ("Time"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Time", StringValue ("2s"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Bounds", StringValue ("0|200|0|200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (phyMode));

  NodeContainer sensor;
  sensor.Create (numNodes);
  NodeContainer sink = NodeContainer (sensor.Get(12));
  NodeContainer source;
  source.Add(sensor.Get(0));
  source.Add(sensor.Get(1));
  source.Add(sensor.Get(2));
  source.Add(sensor.Get(3));
  source.Add(sensor.Get(4));
  source.Add(sensor.Get(5));
  source.Add(sensor.Get(6));
  source.Add(sensor.Get(7));
  source.Add(sensor.Get(8));
  source.Add(sensor.Get(9));
  source.Add(sensor.Get(10));
  source.Add(sensor.Get(11));
  source.Add(sensor.Get(13));
  source.Add(sensor.Get(14));
  source.Add(sensor.Get(15));
  source.Add(sensor.Get(16));
  source.Add(sensor.Get(17));
  source.Add(sensor.Get(18));
  source.Add(sensor.Get(19));
  source.Add(sensor.Get(20));
  source.Add(sensor.Get(21));
  source.Add(sensor.Get(22));
  source.Add(sensor.Get(23));
  source.Add(sensor.Get(24));

// ----------------------------------------------------

  WifiHelper wifi;

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (-10) );
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add an upper mac and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
								"ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devSensor = wifi.Install (wifiPhy, wifiMac, sensor);



  MobilityHelper mobility; 
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0), //onde inicia no eixo X
                                 "MinY", DoubleValue (0.0), //onde inicia no eixo Y
                                 "DeltaX", DoubleValue (distance), // Distância entre nós
                                 "DeltaY", DoubleValue (distance), // Distância entre nós
                                 "GridWidth", UintegerValue (5), // Quantidade de colunas em uma linha
                                 "LayoutType", StringValue ("RowFirst")); // Definindo posições em linha
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (sensor);

if (Random) {

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("2s"),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                             "Bounds", StringValue ("0|200|0|200"));

}
if (Velocity) {

  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (sensor);
  sensor.Get (12)->GetObject<MobilityModel> ()->SetPosition (Vector (-20, 100, 10));
  sensor.Get (12)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (0, speed, 0));

}


  InternetStackHelper stack;

  if (ifOlsr){

      OlsrHelper olsr;
      Ipv4StaticRoutingHelper staticRouting;
      Ipv4ListRoutingHelper list;
      list.Add (staticRouting, 0);
      list.Add (olsr, 1000);
      stack.SetRoutingHelper (list);
      stack.Install (sensor);

  }
  
  if (ifAodv){

      AodvHelper aodv;
      Ipv4ListRoutingHelper list;
      list.Add (aodv, 1000);

      stack.SetRoutingHelper (list);
      stack.Install (sensor);
  }

  if (ifDsr){

	  DsrMainHelper dsrMain;
	  DsrHelper dsr;
  	  stack.Install (sensor);
      dsrMain.Install (dsr, sensor);

  }

  if (ifDsdv){

  	  DsdvHelper dsdv;
  	  Ipv4ListRoutingHelper list;
      list.Add (dsdv, 1000);

      stack.SetRoutingHelper (list);
      stack.Install (sensor);
  }

  if (ifStatic){

      stack.Install (sensor);

  }


  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer InterSensor = address.Assign (devSensor);

  if (ifStatic){

      Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  }
  
  if (ifEco){

	  UdpEchoServerHelper echoServer (9);

		  ApplicationContainer serverApps = echoServer.Install (sensor.Get (sinkNode));
		  	serverApps.Start (Seconds (0));
		  	serverApps.Stop (Seconds (100.0));

		  UdpEchoClientHelper echoClient (InterSensor.GetAddress (12), 9);
			  echoClient.SetAttribute ("MaxPackets", UintegerValue (1200));
			  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.09)));
			  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

		  ApplicationContainer clientApps = echoClient.Install (source); 
		  	clientApps.Start (Seconds (0));
		  	clientApps.Stop (Seconds (100.0));

  }


  


  if (ifOnoff){

	  double randomStartTime = (1 / 1) / nSinks; 

	  for (uint32_t i = 0; i < nSinks; ++i)
	    {
	      PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
	      ApplicationContainer apps_sink = sink.Install (sensor.Get (i));
	      apps_sink.Start (Seconds (0.0));
	      apps_sink.Stop (Seconds (100));

	      OnOffHelper onoff1 ("ns3::UdpSocketFactory", Address (InetSocketAddress (InterSensor.GetAddress (i), port)));
	      onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
	      onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
	      onoff1.SetAttribute ("PacketSize", UintegerValue (1024));
	      onoff1.SetAttribute ("DataRate", DataRateValue (DataRate (rate)));

	      ApplicationContainer apps1 = onoff1.Install (sensor.Get (i + numNodes - nSinks));
	      apps1.Start (Seconds (0 + i * randomStartTime));
	      apps1.Stop (Seconds (100 + i * randomStartTime));
	    }

  }




//-----------------FlowMonitor-THROUGHPUT----------------

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

//-----------------FlowMonitor-JITTER--------------------

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

//-----------------FlowMonitor-DELAY---------------------

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


//-------------------------Metodo-Animation-------------------------

      AnimationInterface anim ("cenario1.xml"); // Mandatory
      anim.SetBackgroundImage ("/home/bmo/repos/ns-3-allinone/ns-3-dev/map.png", 20, 20, 0.5, 0.5, 0.5);
      for (uint32_t i = 0; i < sensor.GetN (); ++i)
      {
        anim.UpdateNodeDescription (sensor.Get (i), "NODE"); // Optional
        anim.UpdateNodeColor (sensor.Get (i), 255, 0, 0); // Coloração
      }
      for (uint32_t i = 0; i < sink.GetN (); ++i)
      {
        anim.UpdateNodeDescription (sink.Get (i), "SERVER"); // Optional
        anim.UpdateNodeColor (sink.Get (i), 0, 255, 0); // Coloração
      }
      anim.EnablePacketMetadata (); // Optional

  Simulator::Stop (Seconds (100.0));
  Simulator::Run ();
  
//Gnuplot ...continued
  gnuplot.AddDataset (dataset);
  // Open the plot file.
  std::ofstream plotFile (plotFileName.c_str());
  // Write the plot file.
  gnuplot.GenerateOutput (plotFile);
  // Close the plot file.
  plotFile.close ();

//---ContJITTER---

  gnuplot2.AddDataset(dataset2);;
  std::ofstream plotFile2 (plotFileName2.c_str());
  gnuplot2.GenerateOutput(plotFile2);
  plotFile2.close();

  //---ContDelay---

  gnuplot3.AddDataset(dataset3);;
  std::ofstream plotFile3 (plotFileName3.c_str());
  gnuplot3.GenerateOutput(plotFile3);
  plotFile3.close();


  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  Simulator::Destroy ();
  return 0;
}
//-------------------------Metodo-VAZÃO---------------------------

  void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon,Gnuplot2dDataset DataSet)
    {
          double localThrou=0;
      std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();
      Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());
      for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats)
      {
                        if(stats->first == 20){//IFFFFFFFFFFFFFFFFFFFFFFF
        Ipv4FlowClassifier::FiveTuple fiveTuple = classing->FindFlow (stats->first);
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
    }//IFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF  
  }
        Simulator::Schedule(Seconds(1),&ThroughputMonitor, fmhelper, flowMon,DataSet);
     //if(flowToXml)
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
         Ptr<Ipv4FlowClassifier> classing2 = DynamicCast<Ipv4FlowClassifier> (fmHelper->GetClassifier());
         for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats2 = flowStats2.begin(); stats2 != flowStats2.end(); ++stats2)
         {
                 if(stats2->first == 20){//IFFFFFFFFFFF
              Ipv4FlowClassifier::FiveTuple fiveTuple2 = classing2->FindFlow (stats2->first);
      std::cout<<"Flow ID : "<< stats2->first <<";"<< fiveTuple2.sourceAddress <<"------>" <<fiveTuple2.destinationAddress<<std::endl;
      std::cout<<"Tx Packets = " << stats2->second.txPackets<<std::endl;
      std::cout<<"Rx Packets = " << stats2->second.rxPackets<<std::endl;
      std::cout<<"Duration  : "<<(stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeFirstTxPacket.GetSeconds())<<std::endl;
      std::cout<<"Atraso: "<<stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeLastTxPacket.GetSeconds() <<"s"<<std::endl;
  atraso2 = stats2->second.timeLastRxPacket.GetSeconds()-stats2->second.timeLastTxPacket.GetSeconds();
  atraso1 = stats2->second.timeFirstRxPacket.GetSeconds()-stats2->second.timeFirstTxPacket.GetSeconds();
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

//-------------------------Metodo-DELAY---------------------------

  void  DelayMonitor(FlowMonitorHelper *fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset3)
  {
    double localDelay=0;

    std::map<FlowId, FlowMonitor::FlowStats> flowStats3 = flowMon->GetFlowStats();
    Ptr<Ipv4FlowClassifier> classing3 = DynamicCast<Ipv4FlowClassifier> (fmHelper->GetClassifier());
    for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats3 = flowStats3.begin(); stats3 != flowStats3.end(); ++stats3)
    {
                 if(stats3->first == 20){//IFFFFFFFFFFF
            Ipv4FlowClassifier::FiveTuple fiveTuple3 = classing3->FindFlow (stats3->first);
            std::cout<<"Flow ID : "<< stats3->first <<";"<< fiveTuple3.sourceAddress <<"------>" <<fiveTuple3.destinationAddress<<std::endl;
            localDelay = stats3->second.timeLastTxPacket.GetSeconds()-stats3->second.timeLastRxPacket.GetSeconds();
          Dataset3.Add((double)Simulator::Now().GetSeconds(), (double) localDelay);
          std::cout<<"---------------------------------------------------------------------------"<<std::endl;
      }//IFFFFFFFFF
    }
    Simulator::Schedule(Seconds(1),&DelayMonitor, fmHelper, flowMon, Dataset3);
    {
       flowMon->SerializeToXmlFile("DelayMonitor.xml", true, true);
    }
  }

// void DelayMonitor (FlowMonitorHelper* fmHelper, Ptr<FlowMonitor> flowMon, Gnuplot2dDataset Dataset3){
  
//   double delay = 0.0;
//   flowMon->CheckForLostPackets(); 
//   std::map<FlowId, FlowMonitor::FlowStats> flowStats3 = flowMon->GetFlowStats();
//   Ptr<Ipv4FlowClassifier> classing3 = DynamicCast<Ipv4FlowClassifier> (fmHelper->GetClassifier());

//   for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats3 = flowStats3.begin (); stats3 != flowStats3.end (); ++stats3){ 
//       //Ipv4FlowClassifier::FiveTuple fiveTuple = classing3->FindFlow (stats3->first);
//       delay = stats3->second.delaySum.GetSeconds ();
      
//       //if(stats3->first == 20) {
//       Dataset3.Add((double)Simulator::Now().GetSeconds(), (double)delay);
//       //}    
//     }
  
//   //Tempo que será iniciado
//   Simulator::Schedule(Seconds(1),&DelayMonitor, fmHelper, flowMon, Dataset3);
// }