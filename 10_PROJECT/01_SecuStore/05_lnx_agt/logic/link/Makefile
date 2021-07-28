include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicAuth.cpp LogicCtrlVuln.cpp LogicCtrlVuln_EditProg.cpp LogicCtrlVuln_Hwp.cpp LogicCtrlVuln_NonConfirmProg.cpp LogicCtrlVuln_PDF.cpp LogicCtrlVuln_Patch.cpp LogicCtrlVuln_SecuUSBProg.cpp LogicEnvNotifyInfo.cpp LogicHostVulnRst.cpp LogicInfoProcReputation.cpp LogicInitLink.cpp LogicInitLink_Policy.cpp LogicLogDeployFile.cpp LogicLogDevice.cpp LogicLogDoc.cpp LogicLogDocDScan.cpp LogicLogEvent.cpp LogicLogPatch.cpp LogicLogRs.cpp LogicLogRsBk.cpp LogicLogSecu.cpp LogicLogStatus.cpp LogicPoFaClear.cpp LogicPoInPtnExTrust.cpp LogicPoInPtnSPRule.cpp LogicPoNcPtnExTrust.cpp LogicPoNcPtnLoPtn.cpp  LogicPoNcPtnSPRule.cpp LogicPtnPatch.cpp LogicPtnVuln.cpp LogicSiteFile.cpp LogicSiteVuln.cpp LogicSiteVulnScan.cpp LogicUser.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
