/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public 
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

//---------------------------------------------------------------------------

#ifndef LogicBaseH
#define LogicBaseH
//---------------------------------------------------------------------------

class CLogicBase
{  
private:
	UINT32		m_nTokRecvNum;
	UINT32		m_nTokRecvCnt;
    UINT32		m_nTokRecvID;
    UINT32		m_nTokRecvValue;
	UINT32		m_nIsStart;

public:
	UINT32		m_nDbgPos;
    INT32		m_nRecvNum;
    INT32		m_nRecvCnt;
    INT32		m_nRecvID;
    INT32		m_nRecvValue;
	UINT32		m_nRecvDT;
	UINT32		m_nHostID;
	UINT32		m_nPolicyID;
	UINT32		m_nValue;
    INT32		m_nPktRst;
	String		m_strName;
	String		m_strLogicName;

public:
	UINT16		m_nPktType;
	UINT16		m_nPktCode;
	INT32		m_nSessionID;
	PPKT_DATA	m_tPktData;

public:
	UINT32		m_nLastErrCode;
	UINT64		m_nPackageType;
	UINT64		m_nPolicyType;
	UINT64		m_nPolicyUnitType;
	UINT64		m_nControlType;
	UINT32		m_nEvtSubType;
	UINT32		m_nEvtTarType;
	UINT32		m_nEvtObjType;
	UINT32		m_nEvtObjCode;
	UINT32		m_nEvtOpType;
	String		m_strEvtDesc;

	UINT16		m_nConAPktType;
	UINT16		m_nConAPktCode;

	PDB_PO_HEADER	m_tDPH;
	DB_PO_HEADER	m_tBaseDPH;

	UINT32		m_nSSPoPkgMode;
	UINT32		m_nSSPoSeqIdx;
	UINT16		m_nAgtPktEditCode;

public:
	INT32		m_nIsMountedEncfs;
	UINT32		m_nLastTimerDT;
	UINT32		m_nForceApplyPolicy;

public:
    TListID		m_tIDList;
	TListID		m_tGIDList;
	TListID		m_tHIDList;
	TListID		m_tPIDList;

    TListID		m_tRecvIDList;
	TListID		m_tRecvGIDList;
	TListID		m_tRecvHIDList;
	TListID		m_tRecvPIDList;

    TListID		m_tSendIDList;
	TListID		m_tSendGIDList;
	TListID		m_tSendHIDList;
	TListID		m_tSendPIDList;
    
    TMapID		m_tIDMap;
	TMapID		m_tGIDMap;
	TMapID		m_tHIDMap;
	TMapID		m_tPIDMap;

    TMapID		m_tRecvIDMap;
	TMapID		m_tRecvGIDMap;
	TMapID		m_tRecvHIDMap;
	TMapID		m_tRecvPIDMap;

    TMapID		m_tSendIDMap;
	TMapID		m_tSendGIDMap;
	TMapID		m_tSendHIDMap;
	TMapID		m_tSendPIDMap;

	TMapIDStr	m_tSendIDStrMap;
    TMapIDStr	m_tRecvIDStrMap;

    TMapIDList	m_tRecvIDListMap;
	TMapIDList	m_tRecvGIDListMap;
	TMapIDList	m_tRecvHIDListMap;
	TMapIDList	m_tRecvPIDListMap;

    TMapIDList	m_tSendIDListMap;
	TMapIDList	m_tSendGIDListMap;
	TMapIDList	m_tSendHIDListMap;
	TMapIDList	m_tSendPIDListMap;
    
	TMapKey64IDList	m_tEvtGIDListMap;
	TMapKey64IDList	m_tEvtHIDListMap;
	TMapKey64IDList	m_tEvtPIDListMap;

	TMapID		m_tFailGIDMap;
	TMapID		m_tFailHIDMap;
	TMapID		m_tFailPIDMap;

public:
	MemToken	SendToken;
	MemToken	RecvToken;

public:
	void	InitBaseMember(PPKT_DATA pkt_data = NULL);

public:
	INT32	TokenRecvIDList(MemToken& RecvToken, TListID& tIDList);
    INT32	TokenRecvIDMap(MemToken& RecvToken, TMapID& tIDValueMap);
    INT32	TokenRecvIDMapList(MemToken& RecvToken, TMapIDList& tIDMapList);

    INT32	TokenSendIDList(TListID& tIDList, MemToken& SendToken);
    INT32	TokenSendIDMap(TMapID& tIDValueMap, MemToken& SendToken);
    INT32	TokenSendIDMapList(TMapIDList& tIDMapList, MemToken& SendToken);

	INT32	CheckPktResult(MemToken& RecvToken);

public:
	void	SendData_Link(UINT16 nType, UINT16 nCode, MemToken& SendToken);
	void	SendData_Link(UINT32 nID, UINT16 nType, UINT16 nCode, MemToken& SendToken);
	void	SendData_Link(TListID& tIDList, UINT16 nType, UINT16 nCode, MemToken& SendToken);
	void	SendData_Link(PPKT_DATA pkt_data, MemToken& SendToken);
	void	SendData_Link(PPKT_DATA pkt_data, MemToken& SendToken, TListID& tIDList);
	
	INT32	ExistsID_Link(UINT32 nID);
	INT32	ExistsID_Link();
	void	EditSID_Link(INT32 nOldID, INT32 nNewID);
	void	EditSKey_Link(INT32 nID, UINT32 nSessionKey);
	void	Disconnected_Link(INT32 nID, INT32 nRemainPkt = 1);
	void	Disconnected_Link(TListID& tHIDList);
	String	GetHostAddr_Link(INT32 nID);
	UINT32	GetHostIP_Link(INT32 nID);

public:
	void	SendData_Lgn(PPKT_DATA pkt_data, MemToken& SendToken);
	void	SendData_Lgn(UINT16 nType, UINT16 nCode, MemToken& SendToken);
	void	EditSKey_Lgn(UINT32 nSessionKey);
	void	Disconnected_Lgn();
	String	GetConnectAddr_Lgn();
	UINT32	GetConnectIP_Lgn();

public:
	INT32	SendData_Mgr(PPKT_DATA pkt_data, MemToken& SendToken);
	INT32	SendData_Mgr(UINT16 nType, UINT16 nCode, MemToken& SendToken);
	INT32	IsInitialize_Mgr();
	void	EditSKey_Mgr(UINT32 nSessionKey);
	void	Disconnected_Mgr();
	String	GetConnectAddr_Mgr();
	UINT32	GetConnectIP_Mgr();

public:
	void	SendData_Udt(PPKT_DATA pkt_data, MemToken& SendToken);
	void	SendData_Udt(UINT16 nType, UINT16 nCode, MemToken& SendToken);
	void	EditSKey_Udt(UINT32 nSessionKey);
	void	Disconnected_Udt();
	String	GetConnectAddr_Udt();
	UINT32	GetConnectIP_Udt();

public:
	virtual void	AnalyzePkt_FromMgr(PPKT_DATA pkt_data);
	virtual void	AnalyzePkt_FromMgr_Add();
	virtual void	AnalyzePkt_FromMgr_Edit();
	virtual void	AnalyzePkt_FromMgr_Del();

	virtual INT32	AnalyzePkt_FromMgr_Ext()				{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromMgr_Add_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromMgr_Edit_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromMgr_Del_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};

public:
	virtual void	AnalyzePkt_FromLink(PPKT_DATA pkt_data);	
	virtual void	AnalyzePkt_FromLink_Add();
	virtual void	AnalyzePkt_FromLink_Edit();
	virtual void	AnalyzePkt_FromLink_Del();

	virtual INT32	AnalyzePkt_FromLink_Ext()				{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromLink_Add_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromLink_Edit_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};
	virtual INT32	AnalyzePkt_FromLink_Del_Ext()			{	return AZPKT_CB_RTN_SUCCESS;	};

public:
	virtual	INT32	ApplyPolicy()							{	return AZPKT_CB_RTN_SUCCESS;	};

public:
	virtual INT32	OnTimer();
	virtual INT32	OnTimer_Logic()							{	return 0;	};
	INT32			IsValidSchedule(UINT64 nSchInfo, UINT32& nLastChkTime, PCHAR szLog = NULL, UINT32 nLogLen = 0);
	INT32			IsValidSchedule(UINT32 nType, UINT64 nStart, UINT64 nEnd);

public:
	INT32	SendPktToLink();

public:
	INT32	SendPktToLink_Ctrl_Etc();

	INT32	SendPktToLink_Host();
	INT32	SendPktToLink_PoEtc();
	INT32	SendPktToLink_PoFA();
	INT32	SendPktToLink_PoFE();
	INT32	SendPktToLink_PoOnPtn();
	INT32	SendPktToLink_PoOnVuln();
	INT32	SendPktToLink_PoOnRs();
	INT32	SendPktToLink_PoPm();
	INT32	SendPktToLink_PoNc();

	INT32	SendPktToLink_Log();


public:
	INT32	SetHdrAndRtn(INT32 nRtnValue);

protected:
	INT32	ChkAdminModRight();	

protected:
	TListDBLogEvent			m_tDLEList;
	DB_LOG_EVENT			m_tDLE;
	
	
public:	
	void	InitDLEH(UINT32 nOpType, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, String strDesc = "", UINT32 nSyncMode = SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
	void	InitDLEH_PE(UINT32 nOID, String strOInfo, String strDesc = "");
	void	InitDLEH_Sync(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc = "");
	void	InitDLEH_SyncAgt(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc = "");
	void	InitDLEH_SyncSvr(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc = "");

public:
	void	InitDLEU(UINT32 nOpType, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, String strDesc = "");

public:
	void	InitDLEH_EC();
	void	SetDLEH_EC(UINT32 nErrCode, INT32 nApplyPkt = 1);

	void	InitDLE_OBJ(UINT32 nOID, String strOInfo = "", UINT32 nSyncMode = SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
	void	InitDLE(UINT32 nNType, UINT32 nNID, UINT32 nSType, UINT32 nSID, UINT32 nTType, UINT32 nTID, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc = "", String strSInfo = "", String strTInfo = "", UINT32 nSyncMode = SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
	void	AppendDLEDesc(String strValue);
	void	AppendDLEDesc(UINT32 nValue);
	void	AppendDLEDesc(UINT32 nKey, String strValue);
	void	AppendDLEDesc(UINT32 nKey, UINT32 nValue);
	void	AppendDLEDescHex(UINT32 nKey, UINT32 nValue);


public:
	CLogicBase();
    ~CLogicBase();
};

#endif
