# dump.py
#
# David Adams
# September 2013
#
# Example job option for running JetDumper from Athena.

infile = "root://eosatlas//eos/atlas/user/m/mwu/KKGraviton_m1000_AOD/AOD.00893499._000001.pool.root.1"
container = "AntiKt4LCTopoJets"
if 0:
  container = "CaloCalTopoCluster"
if 0:
  infile = "xaod.root"
  #infile = "xaod1.root"
  #infile = "/afs/cern.ch/user/d/dadams/atlasdev/asgex_devval_2/PAT/AODUpgrade/xAODCreatorAlgs/run/xaod.root"
  if 1: infile = "xaod_clusters.root"
  container = "Clusters"
if 0:
  infile = "xaod.root"
  container = "AntiKt4LCTopoJets"

#--------------------------------------------------------------
# Input stream
#--------------------------------------------------------------

from AthenaCommon.AppMgr import ServiceMgr
import AthenaPoolCnvSvc.ReadAthenaPool
ServiceMgr.EventSelector.InputCollections = [infile]

#--------------------------------------------------------------
# Event related parameters
#--------------------------------------------------------------

# Number of events to be processed (default is until the end of
# input, or -1, however, since we have no input, a limit needs
# to be set explicitly, here, choose 10)
theApp.EvtMax = 10

#--------------------------------------------------------------
# Configure tools.
#--------------------------------------------------------------

# This uses the generic tool wrapper with the jet dumper, i.e.
# AthJaiTool<jai::JetDumper>.
from JetSubstructureVariables.JetSubstructureVariablesConf import Calculator
ToolSvc += Calculator("MyCalc")
mycalc = ToolSvc.MyCalc

#--------------------------------------------------------------
# Configure algorithm.
#--------------------------------------------------------------

# Allow messge service more than default 500 lines.
ServiceMgr.MessageSvc.infoLimit = 10000

# Full job is a list of algorithms
from AthenaCommon.AlgSequence import AlgSequence
job = AlgSequence()

# Add the JAI algorithm. It runs a list of tools.
from AsgExAlg.AsgExAlgConf import AsgAlgorithm
job += AsgAlgorithm("AsgAlg")
asgalg = job.AsgAlg
asgalg.OutputLevel = INFO
asgalg.Tools = [mycalc]
