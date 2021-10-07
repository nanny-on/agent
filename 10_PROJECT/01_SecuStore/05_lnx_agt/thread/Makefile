include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ThreadBase.cpp ThreadChkHkNoti.cpp ThreadDBMSQuery.cpp ThreadEvtMon.cpp ThreadExecute.cpp ThreadLog.cpp ThreadPoFaClear.cpp ThreadPoFaExPopup.cpp ThreadPoFePtnLo.cpp ThreadPoFeScan.cpp ThreadPoInAccFile.cpp ThreadPoInPtnFile.cpp ThreadPoInPtnScan.cpp ThreadPoInSpReq.cpp ThreadPoNcPtnScan.cpp ThreadPoNcPtnLo.cpp ThreadSocket.cpp ThreadSocket_CltLgn.cpp ThreadSocket_CltMgr.cpp ThreadSocket_CltUdt.cpp ThreadSocket_SvrLink.cpp ThreadSocket_Util.cpp ThreadTimer.cpp ThreadPoFaOp.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
