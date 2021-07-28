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
 */
#include "stdafx.h"
#include "com_struct.h"
#include "FDBMgrBase.h"

//---------------------------------------------------------------------------

CFDBMgrBase::CFDBMgrBase()
{
	m_nLoadMaxID 	= 0;
    m_nLoadNumber	= 0;
    m_nSyncDBNum	= 0;
    m_nNewID		= 0;
    m_nDBSVer		= 0;

    memset(m_tTCIdxBuff, 0, sizeof(INT32) * 200);

	SetDBFQUtil(t_ASIDBFQDLLUtil);

	//SetInitalize();
}
//---------------------------------------------------------------------------

CFDBMgrBase::~CFDBMgrBase()
{
}
//---------------------------------------------------------------------------

void		CFDBMgrBase::SetDBFQUtil(CASIDBFQDLLUtil* tASIDBFQDLLUtil)
{
	m_tASIDBFQDLLUtil = tASIDBFQDLLUtil;
	return;
}
//---------------------------------------------------------------------------

INT32		CFDBMgrBase::ExecuteQuery(CString strQuery)
{
    CString strCheck = strQuery;
	strCheck.MakeLower();
    if(strCheck.Find("select ") == 0)
    	return m_tASIDBFQDLLUtil->ASIDBFQ_QuerySelect(strQuery);

	return m_tASIDBFQDLLUtil->ASIDBFQ_QueryExecute(strQuery);
}
//---------------------------------------------------------------------------

UINT32  		CFDBMgrBase::GetLastID()
{
    UINT64 nLastID = 0;
    m_tASIDBFQDLLUtil->ASIDBFQ_GetLastID(&nLastID);

	return (UINT32)nLastID;
}
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::SetAutoInc(UINT32 nInitVal)
{
	CString strQuery;
	strQuery.Format("ALTER TABLE %s AUTO_INCREMENT=%u", (char*)(LPCSTR)m_strDBTName, nInitVal);

	return ExecuteQuery(strQuery);
}
//---------------------------------------------------------------------------

UINT32			CFDBMgrBase::GetTbLastID()
{
	UINT64 nLastID = 0;
	m_tASIDBFQDLLUtil->ASIDBFQ_GetTbLastID(m_strDBTName, &nLastID);

	return (UINT32)nLastID;
}
//---------------------------------------------------------------------------

UINT64  		CFDBMgrBase::GetLastFp()
{
    UINT64 nLastFp = 0;
    m_tASIDBFQDLLUtil->ASIDBFQ_GetLastFp(&nLastFp);

	return nLastFp;
}
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetTableName()
{
	return	m_strDBTName;
}
//---------------------------------------------------------------------------

INT32       	CFDBMgrBase::First()
{		   
	return m_tASIDBFQDLLUtil->ASIDBFQ_First();
}
//---------------------------------------------------------------------------

INT32       	CFDBMgrBase::Next()
{
	return m_tASIDBFQDLLUtil->ASIDBFQ_Next();
}
//---------------------------------------------------------------------------

UINT64			CFDBMgrBase::GetDBField_Fp()
{
	return m_tASIDBFQDLLUtil->GetFieldFP();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT64      	CFDBMgrBase::GetDBField_UInt64(UINT32 nIndex)
{
	return (UINT64)StrToInt64(GetDBField_String(nIndex));
}
//---------------------------------------------------------------------------

UINT32      	CFDBMgrBase::GetDBField_UInt(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->GetField(nIndex);

	if(!pDIR->pRtnUINT32)	return 0;

	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

INT32       	CFDBMgrBase::GetDBField_Int(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->GetField(nIndex);

	if(!pDIR->pRtnUINT32)	return 0;

	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

CString      	CFDBMgrBase::GetDBField_String(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->GetField(nIndex);

	if(!pDIR->szRtnString)	return "";

	return CString(pDIR->szRtnString, pDIR->nRtnLen);
}
//---------------------------------------------------------------------------

CStringW		CFDBMgrBase::GetDBField_StringW(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->GetField(nIndex);

	if(pDIR->lpRtnObject == NULL)	return L"";
	return CStringW(PWCHAR(pDIR->lpRtnObject), pDIR->nRtnLen / 2);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT64      	CFDBMgrBase::GetDBFieldLow_UInt64(UINT32 nIndex)
{
	return (UINT64)StrToInt64(GetDBFieldLow_String(nIndex));
}
//---------------------------------------------------------------------------

UINT32      	CFDBMgrBase::GetDBFieldLow_UInt(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(nIndex);

	if(!pDIR->pRtnUINT32)	return 0;

	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

INT32       	CFDBMgrBase::GetDBFieldLow_Int(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(nIndex);

	if(!pDIR->pRtnUINT32)	return 0;

	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

CString      	CFDBMgrBase::GetDBFieldLow_String(UINT32 nIndex)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(nIndex);

	if(!pDIR->szRtnString)	return "";

	return CString(pDIR->szRtnString, pDIR->nRtnLen);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT64      	CFDBMgrBase::GetDBFieldLow_UInt64(CString strField)
{
	return (UINT64)StrToInt64(GetDBFieldLow_String(strField));
}
//---------------------------------------------------------------------------

UINT32      	CFDBMgrBase::GetDBFieldLow_UInt(CString strField)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(strField);
	if(!pDIR->pRtnUINT32)	return 0;
	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

INT32       	CFDBMgrBase::GetDBFieldLow_Int(CString strField)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(strField);
	if(!pDIR->pRtnUINT32)	return 0;
	return *(pDIR->pRtnUINT32);
}
//---------------------------------------------------------------------------

CString      	CFDBMgrBase::GetDBFieldLow_String(CString strField)
{
	PDBF_ITEM_RTN pDIR = m_tASIDBFQDLLUtil->ASIDBF_GetField(strField);
	if(pDIR->szRtnString == NULL)	return "";

	return CString(pDIR->szRtnString, pDIR->nRtnLen);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::GetDBFieldLow_ColIdx(CString strCName)
{
    INT32 nRtn = m_tASIDBFQDLLUtil->ASIDBF_GetFieldCIdx(strCName);

    if(nRtn < 0)
    {
    	WriteLogE("not find dbf table column : [%s][%s]", (char*)(LPCSTR)m_strDBTName, (char*)(LPCSTR)strCName);
    }
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::DeleteExecute(UINT32 nID)
{
	GetMDMM().strQuery.Format(DBMS_DEFAULT_QUERY_TYPE_DELETE, (char*)(LPCSTR)m_strDBTName, nID);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_DELETE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

DB_POLICY		CFDBMgrBase::GetDBField_Po(INT32& nIndex)
{
	DB_POLICY tDP;

	tDP.nPUpdateID			= GetDBField_UInt(nIndex++);  
	tDP.nPSvrInfoLgnID		= GetDBField_UInt(nIndex++);  
	tDP.nPHostRmInfoID		= GetDBField_UInt(nIndex++); 
	tDP.nPHostRunID			= GetDBField_UInt(nIndex++);
	tDP.nPHostClearID		= GetDBField_UInt(nIndex++);
	tDP.nPPowerID			= GetDBField_UInt(nIndex++); 
	tDP.nPCtlPanelID		= GetDBField_UInt(nIndex++); 
	tDP.nPCtlProcID			= GetDBField_UInt(nIndex++); 
	tDP.nPSelfProtectID		= GetDBField_UInt(nIndex++);
	tDP.nPDeployFileID		= GetDBField_UInt(nIndex++);
	tDP.nPFaOpID			= GetDBField_UInt(nIndex++);
	tDP.nPFaClearID			= GetDBField_UInt(nIndex++);
	tDP.nPFaProcID			= GetDBField_UInt(nIndex++);
	tDP.nPFaEnvID			= GetDBField_UInt(nIndex++);
	tDP.nPFaBkID			= GetDBField_UInt(nIndex++);
	tDP.nPWinRunID			= GetDBField_UInt(nIndex++);
	tDP.nPSvrInfoUdtID		= GetDBField_UInt(nIndex++); 
	tDP.nPSvrInfoMgrID		= GetDBField_UInt(nIndex++); 

	tDP.nPFePtnOpID			= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnBLID			= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnWLID			= GetDBField_UInt(nIndex++); 
	tDP.nPFeExcludeID		= GetDBField_UInt(nIndex++); 
	tDP.nPFeNotifyID		= GetDBField_UInt(nIndex++); 
	tDP.nPFeSinglePtnID		= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnLoID			= GetDBField_UInt(nIndex++); 

	tDP.nPFaNotifyID		= GetDBField_UInt(nIndex++); 
	tDP.nPSchID				= GetDBField_UInt(nIndex++);

	tDP.nPInPtnOpID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnBLID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnWLID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnExID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnSPID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnNoID			= GetDBField_UInt(nIndex++);

	tDP.nReservedA			= GetDBField_UInt(nIndex++);
	tDP.nReservedB			= GetDBField_UInt(nIndex++);
	tDP.nReservedC			= GetDBField_UInt(nIndex++);	

	tDP.nPInVulnOpID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnScanID		= GetDBField_UInt(nIndex++);

	tDP.nPHostNotifyID		= GetDBField_UInt(nIndex++);

	tDP.nPInDevOOpID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOBLID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOWLID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOExID		= GetDBField_UInt(nIndex++);

	tDP.nPPmOpID			= GetDBField_UInt(nIndex++);
	tDP.nPPmDmID			= GetDBField_UInt(nIndex++);
	tDP.nPPmScanID			= GetDBField_UInt(nIndex++);
	tDP.nPPmExID			= GetDBField_UInt(nIndex++);
	tDP.nPPmNoID			= GetDBField_UInt(nIndex++);

	tDP.nPInVulnAxID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnEditAppID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnQnaID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnSecuUsbID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnSwID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnPatchExceptionID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevONotifyID		= GetDBField_UInt(nIndex++);

	tDP.nPInRsOpID			= GetDBField_UInt(nIndex++);
	tDP.nPInRsNoID			= GetDBField_UInt(nIndex++);
	tDP.nPInRsBkID			= GetDBField_UInt(nIndex++);

	tDP.nPNcPtnOpID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnLoID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnBLID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnWLID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnExID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnSPID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnNoID			= GetDBField_UInt(nIndex++);
	
	tDP.nPHostWebSockID		= GetDBField_UInt(nIndex++);

	tDP.nPSvrInfoFBID		= GetDBField_UInt(nIndex++);

	tDP.nPCtlShareFileID	= GetDBField_UInt(nIndex++);


	return tDP;
}
//---------------------------------------------------------------------------

DB_PO_HEADER	CFDBMgrBase::GetDBField_PoHDR(INT32& nIndex)
{
	DB_PO_HEADER tDPH;

	tDPH.nID					= GetDBField_UInt(nIndex++);
	tDPH.nRegDate				= GetDBField_UInt(nIndex++);
	tDPH.nSeqNo					= GetDBField_UInt(nIndex++);
	tDPH.strName				= GetDBField_String(nIndex++);
	tDPH.strDescr				= GetDBField_String(nIndex++);
	tDPH.nAdminID				= GetDBField_UInt(nIndex++);
	tDPH.nSubAdminLock			= GetDBField_UInt(nIndex++);
	tDPH.nTargetLock			= GetDBField_UInt(nIndex++);
	tDPH.nTargetOS				= GetDBField_UInt(nIndex++);
	tDPH.nExtOption				= GetDBField_UInt(nIndex++);
	tDPH.nUsedMode				= GetDBField_UInt(nIndex++);

	return tDPH;
}
//---------------------------------------------------------------------------

DB_PO_HEADER	CFDBMgrBase::GetDBField_PoPkgHDR(INT32& nIndex)
{
	DB_PO_HEADER tDPH;

	tDPH.nID					= GetDBField_UInt(nIndex++);
	tDPH.nRegDate				= GetDBField_UInt(nIndex++);
	tDPH.strName				= GetDBField_String(nIndex++);
	tDPH.strDescr				= GetDBField_String(nIndex++);
	tDPH.nAdminID				= GetDBField_UInt(nIndex++);
	tDPH.nSubAdminLock			= GetDBField_UInt(nIndex++);
	tDPH.nPolicyID				= GetDBField_UInt(nIndex++);
	tDPH.nUnitID				= GetDBField_UInt(nIndex++);
	tDPH.nTargetOS				= GetDBField_UInt(nIndex++);
	tDPH.nExtOption				= GetDBField_UInt(nIndex++);
	tDPH.nUsedMode				= GetDBField_UInt(nIndex++);

	return tDPH;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetPoQuery_InsertValue(DB_POLICY& tDP)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_INSERT_VALUE, 
		tDP.nPUpdateID, tDP.nPSvrInfoLgnID, tDP.nPHostRmInfoID, tDP.nPHostRunID, tDP.nPHostClearID, 
		tDP.nPPowerID, tDP.nPCtlPanelID, tDP.nPCtlProcID, tDP.nPSelfProtectID, tDP.nPDeployFileID,
		tDP.nPFaOpID, tDP.nPFaClearID, tDP.nPFaProcID, tDP.nPFaEnvID, tDP.nPFaBkID,
		tDP.nPWinRunID, tDP.nPSvrInfoUdtID, tDP.nPSvrInfoMgrID,
		tDP.nPFePtnOpID, tDP.nPFePtnBLID, tDP.nPFePtnWLID, tDP.nPFeExcludeID, tDP.nPFeNotifyID, tDP.nPFeSinglePtnID, tDP.nPFePtnLoID,
		tDP.nPFaNotifyID, tDP.nPSchID,
		tDP.nPInPtnOpID, tDP.nPInPtnBLID, tDP.nPInPtnWLID, tDP.nPInPtnExID, tDP.nPInPtnSPID, tDP.nPInPtnNoID,
		tDP.nReservedA, tDP.nReservedB, tDP.nReservedC,
		tDP.nPInVulnOpID, tDP.nPInVulnScanID, tDP.nPHostNotifyID,
		tDP.nPInDevOOpID, tDP.nPInDevOBLID, tDP.nPInDevOWLID, tDP.nPInDevOExID,
		tDP.nPPmOpID, tDP.nPPmDmID, tDP.nPPmScanID, tDP.nPPmExID, tDP.nPPmNoID,
		tDP.nPInVulnAxID, tDP.nPInVulnEditAppID, tDP.nPInVulnQnaID, tDP.nPInVulnSecuUsbID, tDP.nPInVulnSwID, tDP.nPInVulnPatchExceptionID,
		tDP.nPInDevONotifyID,
		tDP.nPInRsOpID, tDP.nPInRsNoID, tDP.nPInRsBkID,
		tDP.nPNcPtnOpID, tDP.nPNcPtnLoID, tDP.nPNcPtnBLID, tDP.nPNcPtnWLID, tDP.nPNcPtnExID, tDP.nPNcPtnSPID, tDP.nPNcPtnNoID,
		tDP.nPHostWebSockID,
		tDP.nPSvrInfoFBID, tDP.nPCtlShareFileID
		);

	return strRtn;
}
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetPoQuery_Update(DB_POLICY& tDP)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_UPDATE, 
		tDP.nPUpdateID, tDP.nPSvrInfoLgnID, tDP.nPHostRmInfoID, tDP.nPHostRunID, tDP.nPHostClearID, 
		tDP.nPPowerID, tDP.nPCtlPanelID, tDP.nPCtlProcID, tDP.nPSelfProtectID, tDP.nPDeployFileID,
		tDP.nPFaOpID, tDP.nPFaClearID, tDP.nPFaProcID, tDP.nPFaEnvID, tDP.nPFaBkID,
		tDP.nPWinRunID, tDP.nPSvrInfoUdtID, tDP.nPSvrInfoMgrID,
		tDP.nPFePtnOpID, tDP.nPFePtnBLID, tDP.nPFePtnWLID, tDP.nPFeExcludeID, tDP.nPFeNotifyID, tDP.nPFeSinglePtnID, tDP.nPFePtnLoID,
		tDP.nPFaNotifyID, tDP.nPSchID,
		tDP.nPInPtnOpID, tDP.nPInPtnBLID, tDP.nPInPtnWLID, tDP.nPInPtnExID, tDP.nPInPtnSPID, tDP.nPInPtnNoID,
		tDP.nReservedA, tDP.nReservedB, tDP.nReservedC,
		tDP.nPInVulnOpID, tDP.nPInVulnScanID, tDP.nPHostNotifyID, 
		tDP.nPInDevOOpID, tDP.nPInDevOBLID, tDP.nPInDevOWLID, tDP.nPInDevOExID,
		tDP.nPPmOpID, tDP.nPPmDmID, tDP.nPPmScanID, tDP.nPPmExID, tDP.nPPmNoID,
		tDP.nPInVulnAxID, tDP.nPInVulnEditAppID, tDP.nPInVulnQnaID, tDP.nPInVulnSecuUsbID, tDP.nPInVulnSwID, tDP.nPInVulnPatchExceptionID,
		tDP.nPInDevONotifyID,
		tDP.nPInRsOpID, tDP.nPInRsNoID, tDP.nPInRsBkID,
		tDP.nPNcPtnOpID, tDP.nPNcPtnLoID, tDP.nPNcPtnBLID, tDP.nPNcPtnWLID, tDP.nPNcPtnExID, tDP.nPNcPtnSPID, tDP.nPNcPtnNoID,
		tDP.nPHostWebSockID,
		tDP.nPSvrInfoFBID, tDP.nPCtlShareFileID
		);

	return strRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetPoHDRQuery_InsertValue(DB_PO_HEADER& tDPH)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_HDR_INSERT_VALUE, 
#ifdef _DB_DEMAND_GEN_INDEX
					tDPH.nID, tDPH.nUsedFlag, tDPH.nRegDate, tDPH.nSeqNo, 
#else
					tDPH.nUsedFlag, tDPH.nRegDate, tDPH.nSeqNo, 
#endif
					tDPH.strName.c_str(), tDPH.strDescr.c_str(),
					tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nTargetLock, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//---------------------------------------------------------------------------
CString			CFDBMgrBase::GetPoHDRQuery_Update(DB_PO_HEADER& tDPH)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_HDR_UPDATE, 
					tDPH.nRegDate, tDPH.nSeqNo, 
					tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
					tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nTargetLock, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetPoPkgHDRQuery_InsertValue(DB_PO_HEADER& tDPH)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_PKG_HDR_INSERT_VALUE, 
#ifdef _DB_DEMAND_GEN_INDEX
		tDPH.nID, tDPH.nUsedFlag, tDPH.nRegDate,
#else
		tDPH.nUsedFlag, tDPH.nRegDate,  
#endif
		tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
		tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nPolicyID, tDPH.nUnitID, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//---------------------------------------------------------------------------

CString			CFDBMgrBase::GetPoPkgHDRQuery_Update(DB_PO_HEADER& tDPH)
{
	CString strRtn;
	strRtn.Format(DBMS_POLICY_QUERY_PKG_HDR_UPDATE, 
		tDPH.nUsedFlag, tDPH.nRegDate,
		tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
		tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nPolicyID, tDPH.nUnitID, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void			CFDBMgrBase::SetCurTableName(LPCTSTR lpTableName)
{
	m_tASIDBFQDLLUtil->SetCurTableName(lpTableName);
	return;
}
//---------------------------------------------------------------------------

void			CFDBMgrBase::SetCurTableName(CString lpTableName)
{
	m_tASIDBFQDLLUtil->SetCurTableName(lpTableName);
	return;
}
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::PreSetValue16(LPCTSTR lpColName, UINT16 nValue)
{
	return m_tASIDBFQDLLUtil->ASIDBF_PreSetValue16(lpColName, nValue);
}
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::PreSetValue32(LPCTSTR lpColName, UINT32 nValue)
{
	return m_tASIDBFQDLLUtil->ASIDBF_PreSetValue32(lpColName, nValue);
}
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::PreSetValue64(LPCTSTR lpColName, UINT64 nValue)
{
	return m_tASIDBFQDLLUtil->ASIDBF_PreSetValue64(lpColName, nValue);
}
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::PreSetValue(LPCTSTR lpColName, CString& strValue)
{
	return m_tASIDBFQDLLUtil->ASIDBF_PreSetValue(lpColName, strValue);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CFDBMgrBase::IsExistTable()
{
	return m_tASIDBFQDLLUtil->ASIDBFQ_IsExistTable(m_strDBTName);
}
//---------------------------------------------------------------------------

INT32		CFDBMgrBase::DBOP_Check(INT32 nRtn)
{
	if(nRtn)
    {
    	if(nRtn == DBMS_LOAD_EMPTY)
        {
        	CString strChkValue = GetMDMM().strQuery;
            strChkValue.MakeLower();
        	CTokenCString Token(strChkValue, strChkValue.GetLength(), ' ');
            while(Token.IsNextToken())
            {
            	if(Token.NextToken() != "from")		continue;

                WriteLogN("dbms_operation empty : [%d][%s]", nRtn, (char*)(LPCSTR)Token.NextToken());
                break;
            }
        }
        else
	    	WriteLogE("dbms_operation error : [%d][%s]", nRtn, (char*)(LPCSTR)GetMDMM().strQuery);
    }
    return nRtn;
}
//---------------------------------------------------------------------------

CString		CFDBMgrBase::ConvertListToCString(TListID& tIDList)
{
	CString strIDs;
	CString strTemp;

    TListIDItor begin, end;
    begin = tIDList.begin();	end = tIDList.end();
    for(begin; begin != end; begin++)
    {
    	if(strIDs.IsEmpty())
		{
			strTemp.Format("%u", *begin);	
			strIDs = strTemp;
		}
        else
		{
			strTemp.Format(", %u", *begin);	
			strIDs += strTemp;
		}
    }
    return strIDs;
}
//---------------------------------------------------------------------------

CString      CFDBMgrBase::ChangeFilePathToDBPath(CString strFilePath)
{
    CString strResult;
    for(int i = 1 ; i < strFilePath.GetLength() + 1; i++)
    {
        if(strFilePath[i] == STR_TOKEN_FILE_PATH)
        {
            strResult += STR_TOKEN_DB_PATH;
        }
        else
            strResult += strFilePath[i];
    }
    return strResult;
}
//------------------------------------------------------------------------------

CString      CFDBMgrBase::ChangeDBPathToFilePath(CString strDBPath)
{
    CString strResult;
    for(int i = 1 ; i < strDBPath.GetLength() + 1; i++)
    {
        if(strDBPath[i] == STR_TOKEN_DB_PATH)
        {
            strResult += STR_TOKEN_FILE_PATH;
        }
        else
            strResult += strDBPath[i];
    }
    return strResult;
}

//------------------------------------------------------------------------------

INT32		CFDBMgrBase::DeleteExecuteFp(UINT64 nDBFp)
{
	GetMDMM().strQuery.Format(DBMS_DEFAULT_QUERY_TYPE_DELETE_FP, (char*)(LPCSTR)m_strDBTName, ASI_DBF_WHERE_FP_KEY_NAME, nDBFp);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_DELETE_FAIL;

	return 0;
}

//------------------------------------------------------------------------------

INT32		CFDBMgrBase::DeleteExecuteFps(TListID64& tListFp)
{
	TListID64Itor begin, end;
	begin = tListFp.begin();	end = tListFp.end();
	for(begin; begin != end; begin++)
	{
		DeleteExecuteFp(*begin);
	}

	return 0;
}
//------------------------------------------------------------------------------

INT32		CFDBMgrBase::LoadDB()
{
	{
		CRegUtil	tRegUtil;
		tRegUtil.SetRootPath(STR_REG_DEFAULT_SVC_DB_PATH);
//		tRegUtil.SetWowDefaultKey();

		tRegUtil.RegWriteString(HKEY_LOCAL_MACHINE, "", "dbf_load", (char*)(LPCSTR)m_strDBTName);
	}

	return 0;
}
//------------------------------------------------------------------------------
INT32		CFDBMgrBase::ResetTable()
{
	GetMDMM().strQuery.Format(DBMS_DEFAULT_QUERY_DELETE_TABLE, (char*)(LPCSTR)m_strDBTName);
	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_DELETE_FAIL;

	GetMDMM().strQuery.Format(DBMS_DEFAULT_QUERY_AUTO_INC_RESET, (char*)(LPCSTR)m_strDBTName);
	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_DELETE_FAIL;

	return 0;
}
//------------------------------------------------------------------------------