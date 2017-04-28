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


using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("WSN");

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
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", StringValue ("500Kb"));
  ApplicationContainer clientApps = echoClient.Install (nodes); // Instala nos 20 nós estáticos 
  clientApps.Start (Seconds (1.5));
  clientApps.Stop (Seconds (100.0));


  NS_LOG_INFO ("Run Simulation");
  Simulator::Stop (Seconds (101));
  Simulator::Run ();


  Simulator::Destroy ();
  NS_LOG_INFO ("Done");
  return 0;
} 
