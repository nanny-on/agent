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


#include "stdafx.h"
#include "com_struct.h"
#include "LogicHostVulnRst.h"

//---------------------------------------------------------------------------

CLogicHostVulnRst*		t_LogicHostVulnRst = NULL;

//---------------------------------------------------------------------------

CLogicHostVulnRst::CLogicHostVulnRst()
{	
	m_strLogicName		= "link host vuln rst";
		
	m_nEvtSubType		= EVENT_SUBJECT_TYPE_USER;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_USER;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_HOST_VULN_RST;
}
//---------------------------------------------------------------------------

CLogicHostVulnRst::~CLogicHostVulnRst()
{

}
//---------------------------------------------------------------------------

INT32	CLogicHostVulnRst::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		AnalyzePkt_FromLink_Ext_Sync();		break;
		default:						break;		
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicHostVulnRst::AnalyzePkt_FromLink_Ext_Sync()
{	
	m_nEvtOpType = EVENT_OPERATION_TYPE_SYNC;
	DB_HOST_VULN_RST data;
	PDB_HOST_VULN_RST pdata = NULL;
	TListDBHostVulnRstUnit tUnitList;

	if(t_ManageHostVulnRst->GetPkt(RecvToken, data) < 0)			return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_HOST_VULN_RST_UNIT data_unit;
		if(t_ManageHostVulnRstUnit->GetPkt_Link(RecvToken, data_unit) < 0)	return AZPKT_CB_RTN_PKT_INVALID;

		tUnitList.push_back(data_unit);
	}

	{
		if(data.nScanMon == 0)			data.nScanMon = GetCurScanMon();
		if(data.strVersion.empty())
		{
			PDB_PO_IN_VULN_OP pdata_op = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_OP);
			if(pdata_op)
				data.strVersion = SPrintf("%s : %s", t_EnvInfo->m_strHostBinVer.c_str(), pdata_op->tDPIVOPF.strPtnVersion.c_str());
			else
				data.strVersion = SPrintf("%s", t_EnvInfo->m_strHostBinVer.c_str());
		}


		data.nScanDate = GetCurrentDateTimeInt();

		pdata = t_ManageHostVulnRst->FindKeyItem(data.nScanMon);
		if(!pdata)
		{
			if(SetER(t_ManageHostVulnRst->AddHostVulnRst(data)))
			{
				WriteLogE("[%s] add vuln rst fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return AZPKT_CB_RTN_DBMS_FAIL;
			}
			pdata = t_ManageHostVulnRst->FindKeyItem(data.nScanMon);
		}
		else
		{
			data.nID = pdata->nID;
			t_ManageHostVulnRst->EditHostVulnRst(data);	
		}

		pdata->tRstUnitMap.clear();
	}


	{
		TMapID tOIDMap, tNIDMap;
		t_ManageHostVulnRstUnit->GetKeyTypeID(data.nScanMon, tOIDMap);

		{
			TListDBHostVulnRstUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				PDB_HOST_VULN_RST_UNIT pdata_unit = t_ManageHostVulnRstUnit->FindKeyTypeID_Item(pdata->nID, begin->tDHVRUS.nVulnID);
				if(pdata_unit)
				{
					begin->nID = pdata_unit->nID;
					pdata_unit->tDHVRUS = begin->tDHVRUS;
					pdata_unit->strRstInfo = begin->strRstInfo;

					if(SetER(t_ManageHostVulnRstUnit->EditHostVulnRstUnit(*begin)))
					{
						WriteLogE("[%s] edit vuln rst unit fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						return AZPKT_CB_RTN_DBMS_FAIL;
					}

					tNIDMap[begin->nID] = 0;
					pdata->tRstUnitMap[begin->tDHVRUS.nVulnID] = begin->tDHVRUS;
				}
				else
				{
					begin->nID = 0;
					begin->nVulnRstID = pdata->nID;
					begin->nRegDate	= GetCurrentDateTimeInt();

					if(SetER(t_ManageHostVulnRstUnit->AddHostVulnRstUnit(*begin)))
					{
						WriteLogE("[%s] add vuln rst unit fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						return AZPKT_CB_RTN_DBMS_FAIL;
					}

					tNIDMap[begin->nID] = 0;
					pdata->tRstUnitMap[begin->tDHVRUS.nVulnID] = begin->tDHVRUS;
				}
			}
		}

		{
			TMapIDItor begin, end;
			begin = tOIDMap.begin();	end = tOIDMap.end();
			for(begin; begin != end; begin++)
			{
				if(IsExistsIDFromMap(tNIDMap, begin->second))	continue;

				if(SetER(t_ManageHostVulnRstUnit->DelHostVulnRstUnit(begin->second)))
				{
					WriteLogE("[%s] del vuln rst unit fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					return AZPKT_CB_RTN_DBMS_FAIL;
				}
			}
		}
	}

	{
		t_ManageHostVulnRst->SetRstMapToStr(pdata);
		HISYNCSTEPUP(pdata->nSyncSvrStep);
		if(SetER(t_ManageHostVulnRst->EditHostVulnRst(*pdata)))
		{
			WriteLogE("[%s] edit vuln rst fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return AZPKT_CB_RTN_DBMS_FAIL;
		}
		t_LogicMgrHostVulnRst->SetHostVulnRst(*pdata);
	}
	
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicHostVulnRst::SendPkt_Del(UINT32 nID)
{
	SendToken.TokenAdd_32(nID);
	
	SendData_Link(G_TYPE_HOST_VULN_RST, G_CODE_COMMON_DEL, SendToken);
	//WriteLogN("[%s] send pkt scan : [id:%d]", m_strLogicName.c_str(), nID);
	SendToken.Clear();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT32		CLogicHostVulnRst::GetCurScanMon()
{
	UINT32 nScanMon = GetCurrentYearMon();
	PDB_PO_IN_VULN_SCAN pCurPolicy = (PDB_PO_IN_VULN_SCAN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SCAN);
	if(!pCurPolicy)		return nScanMon;
	if(pCurPolicy->tDPH.nUsedMode != STATUS_USED_MODE_ON)	return nScanMon;
	
	{
		U64_SCHEDULE tDT;
		tDT.all = pCurPolicy->nRecStdTime;

		switch(tDT.U8.type)
		{
			case SCHEDULE_PERIOD_TYPE_MON_WEEK_DOW:
			{
				UINT32 nCurTime = GetCurrentDateTimeInt();
				UINT32 nCurDoW = GetDayOfWeek(nCurTime, 1);
				UINT32 nCurDay = GetDayOfDay(nCurTime, 1);

				UINT32 nWeekCnt = GetWeekCnt(nCurDay, nCurDoW, tDT.U8.value);
				UINT32 nWeekCntDay = GetWeekCntDay(nCurDay, nCurDoW, tDT.U8.value, tDT.U8.hour, tDT.U8.min);

				if(nWeekCnt < tDT.U8.hour)
				{
					nScanMon = GetBeforScanMon(nScanMon);
					break;
				}
				else if(nWeekCnt == tDT.U8.hour && nCurDay < nWeekCntDay)
				{
					nScanMon = GetBeforScanMon(nScanMon);
					break;
				}				
				break;
			}
		}
	}
	return nScanMon;
}
//---------------------------------------------------------------------------

UINT32		CLogicHostVulnRst::GetBeforScanMon(UINT32 nScanMon)
{
	UINT32 nYear = nScanMon / 100;
	UINT32 nMon = nScanMon % 100;

	nMon -= 1;
	if(nMon == 0)	
	{
		nYear -= 1;
		nMon = 12;
	}

	return ((nYear * 100) + nMon);
}
//---------------------------------------------------------------------------
