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
  from JetSubstructureVariables.JetSubstructureVariablesConf import Calculator
  ToolSvc += Calculator("calc")
  inpdmp = ToolSvc.calc
  jetalg.Tools += [inpdmp]

