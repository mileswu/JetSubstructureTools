# Specify input file.
infile = "xAOD.root"
clname = "MyCopiedJets"

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
theApp.EvtMax =  -1

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
# Configure output.
#--------------------------------------------------------------
from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output = ["JetSubstructureMoments DATAFILE='validation.root' OPT='RECREATE'"]

#--------------------------------------------------------------
# Configure tools.
#--------------------------------------------------------------

from JetSubstructureTools.JetSubstructureToolsConf import Validator
ToolSvc += Validator("validator")
validator = ToolSvc.validator
validator.InputContainer = clname
jetalg.Tools += [validator]

