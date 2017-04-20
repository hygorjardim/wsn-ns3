// Autor: Hygor Jardim da Silva
// This example is based on the "wsn-ping6.cc" example.

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/sixlowpan-module.h"
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
  nodes.Create (2);

  NodeContainer ap;
  ap.Create (1);

  NodeContainer allnodes;
  allnodes.Add (nodes);
  allnodes.Add (ap);

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
  lrWpanHelper.AssociateToPan (lrwpanDevices, 10);

  NS_LOG_INFO ("Install Internet Stack");
  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.Install (allnodes);
}