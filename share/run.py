# run.py
#
# David Adams
# January 2014
#
# Example job options for running jet reconstruction.
#
# Run with
# > athena.py run.py
#

# Specify input file.
infile = "/home/mileswu/xaod_jets.root"
clname = "AntiKt4LCTopoJets"

#--------------------------------------------------------------
# Input stream
#--------------------------------------------------------------

from AthenaCommon.AppMgr import ServiceMgr
import AthenaPoolCnvSvc.ReadAthenaPool
ServiceMgr.EventSelector.InputCollections = [infile]

#--------------------------------------------------------------
# Event related parameters
#--------------------------------------------------------------

# Number of events to be processed. Default is end [-1].
theApp.EvtMax =  1

#--------------------------------------------------------------
# Configure algorithm.
#--------------------------------------------------------------

# Allow messge service more than default 500 lines.
ServiceMgr.MessageSvc.infoLimit = 10000

# Full job is a list of algorithms
from AthenaCommon.AlgSequence import AlgSequence
job = AlgSequence()

# Add the algorithm. It runs the demo tools.
from JetRec.JetRecConf import JetAlgorithm
job += JetAlgorithm("jetalg")
jetalg = job.jetalg
jetalg.OutputLevel = INFO

#--------------------------------------------------------------
# Configure tools.
#--------------------------------------------------------------

# Dump before reco.
if 1:
  from JetRec.JetRecConf import JetDumper
  ToolSvc += JetDumper("inpdmp")
  inpdmp = ToolSvc.inpdmp
  inpdmp.ContainerName = "JetClusters"
  inpdmp.Detail = 1
  inpdmp.MaxObject = 20
  inpdmp.OutputLevel = INFO
  jetalg.Tools += [inpdmp]

# JetRec tool for finding.
if 0:
  from JetRec.JetRecConf import JetRecTool
  ToolSvc += JetRecTool("jetrec")
  jetrec = ToolSvc.jetrec
  jetrec.OutputContainer = "MyNewJets"
  jetrec.OutputLevel = INFO
  jetalg.Tools += [jetrec]

# Find jet inputs.
if 0:
  from JetRec.JetRecConf import PseudoJetGetter
  ToolSvc += PseudoJetGetter("psjget")
  psjget = ToolSvc.psjget
  psjget.InputContainer = clname
  psjget.Label = "Cluster"
  psjget.OutputContainer = "PseudoJetClusters"
  psjget.OutputLevel = INFO
  psjget.SkipNegativeEnergy = True
  jetrec.PseudoJetGetters += [psjget]

# Find jets.
if 0:
  from JetRec.JetRecConf import JetFinder
  ToolSvc += JetFinder("jfind")
  jfind = ToolSvc.jfind
  jfind.JetAlgorithm = "akt"
  jfind.JetRadius = 0.8
  jfind.GhostArea = 0.01;
  jfind.PtMin = 20000.0
  jfind.OutputLevel = VERBOSE
  jetrec.JetFinder = jfind

# Dump pseudojets after reco.
if 0:
  from JetRec.JetRecConf import JetDumper
  ToolSvc += JetDumper("psjdmp")
  psjdmp = ToolSvc.psjdmp
  psjdmp.ContainerName = "PseudoJetClusters"
  psjdmp.Detail = 1
  psjdmp.MaxObject = 20
  psjdmp.OutputLevel = INFO
  jetalg.Tools += [psjdmp]

# Dump jets after reco.
if 0:
  from JetRec.JetRecConf import JetDumper
  ToolSvc += JetDumper("jdmp")
  jdmp = ToolSvc.jdmp
  jdmp.ContainerName = "MyNewJets"
  jdmp.Detail = 1
  jdmp.LineDetail = 3
  jdmp.MaxObject = 20
  jdmp.OutputLevel = INFO
  jetalg.Tools += [jdmp]

# Copy jets with a new JetRecTool.
if 1:
  from JetRec.JetRecConf import JetRecTool
  ToolSvc += JetRecTool("jetrec2")
  jetrec2 = ToolSvc.jetrec2
  jetrec2.OutputContainer = "MyCopiedJets"
  jetrec2.InputContainer = clname
  jetrec2.OutputLevel = INFO
  jetalg.Tools += [jetrec2]

# Add filter for copied jets.
if 1:
  from JetSubstructureVariables.JetSubstructureVariablesConf import Calculator
  ToolSvc += Calculator("calc")
  calc = ToolSvc.calc
  jetrec2.JetModifiers += [calc]

# Dump copied jets.
if 1:
  from JetRec.JetRecConf import JetDumper
  ToolSvc += JetDumper("jdmp2")
  jdmp2 = ToolSvc.jdmp2
  jdmp2.ContainerName = "MyCopiedJets"
  jdmp2.Detail = 2
  jdmp2.LineDetail = 3
  jdmp2.MaxObject = 20
  jdmp2.FloatMoments = ["Tau1", "Tau2", "Tau3"]
 # jdmp2.FourVectorMoments = ["ActiveArea4vec"]
  jdmp2.OutputLevel = INFO
  jetalg.Tools += [jdmp2]


# Create a POOL output file with the StoreGate contents:
from OutputStreamAthenaPool.MultipleStreamManager import MSMgr
xaodStream = MSMgr.NewPoolStream( "StreamXAOD", "xAOD.root" )

# compactify calo cell
from CaloTools.CaloToolsConf import CaloCompactCellTool
svcMgr.ToolSvc += CaloCompactCellTool()

# Set up its contents:
xaodStream.AddItem( "xAOD::JetContainer_v1#*" )
xaodStream.AddItem( "xAOD::JetAuxContainer_v1#*" )
xaodStream.AddMetaDataItem( "xAOD::EventFormat_v1#*" )
xaodStream.Print()

# Split all branches:
ServiceMgr.AthenaPoolCnvSvc.PoolAttributes += [
        "DEFAULT_SPLITLEVEL='99'" ]

# Force POOL to just simply use the StoreGate keys as branch names:
ServiceMgr.AthenaPoolCnvSvc.SubLevelBranchName = "<key>"
