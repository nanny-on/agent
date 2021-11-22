/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version, with the additional
 * special exception to link portions of this program with the OpenSSL
 * library. See LICENSE for more details. 
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
#include "LogicMgrPtnPatch.h"

//---------------------------------------------------------------------------

CLogicMgrPtnPatch*		t_LogicMgrPtnPatch = NULL;

//---------------------------------------------------------------------------

CLogicMgrPtnPatch::CLogicMgrPtnPatch()
{
//	t_MMPPPatch = new MEM_MGR_PTN_PKG;
//	t_MMPPPatch->_init();

	t_LogicPtnPatch			= new CLogicPtnPatch();
	
	m_strLogicName		= "mgr agt ptn patch";
	
	m_nPackageType		= SS_PACKAGE_TYPE_NPMS;
	m_nPolicyType		= 0;
	m_nSSPoSeqIdx		= 0;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;

//	m_tASIDBFQDLLUtil	= NULL;
	m_nInitLogic		= 0;
	//m_nInitPatchUtil	= SS_PTN_PATCH_UTIL_STATUS_NONE;
	m_nInitPatchWsus	= SS_PTN_PATCH_WSUS_STATUS_NONE;
	m_nInitPtnDeploy	= SS_PTN_DEPLOY_STATUS_NONE;

//	SetInterLockExchange(&m_nIsRunRBMode, 0);
}
//---------------------------------------------------------------------------

CLogicMgrPtnPatch::~CLogicMgrPtnPatch()
{
//	t_MMPPPatch->_free();
//	SAFE_DELETE(t_MMPPPatch);

	SAFE_DELETE(t_LogicPtnPatch);

//	SAFE_DELETE(m_tASIDBFQDLLUtil);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::InitPtnPatch()
{	
/*
	if(OpenDBFQ())	return -1;

	{
		t_MMPPPatch->_set_dbfq(m_tASIDBFQDLLUtil);
	}

	{
		INT32 nLoadEnd = 0;
		do 
		{
			if(t_MMPPPatch->t_ManagePtnPatch->LoadDBMS())				break;
			if(t_MMPPPatch->t_ManagePtnPatchFile->LoadDBMS())			break;
			if(t_MMPPPatch->t_ManagePtnPatchWsus->LoadDBMS())			break;
			if(t_MMPPPatch->t_ManagePtnPatchScan->LoadDBMS())			break;
			if(t_MMPPPatch->t_ManagePtnPatchScanEnv->LoadDBMS())		break;
			if(t_MMPPPatch->t_ManagePtnPatchScanEnvUnit->LoadDBMS())	break;
			if(t_MMPPPatch->t_ManagePtnDeploy->LoadDBMS())				break;

			nLoadEnd = 1;
		} while (FALSE);

		if(!nLoadEnd)		return -2;
	}

	{
		MemToken SendToken(1024);
		{
			t_MMPPPatch->t_ManagePtnPatch->SetPkt(SendToken);
			if(SetER(t_ASIPMSPatchUtil->ASIPMS_SetData(ASI_PMS_DATA_TYPE_PTN_PATCH, 0, SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set data fail : [%d]-[%d]", m_strLogicName.c_str(), ASI_PMS_DATA_TYPE_PTN_PATCH, g_nErrRtn);
				return g_nErrRtn;
			}
			SendToken.Clear();
		}
		{
			t_MMPPPatch->t_ManagePtnPatchScan->SetPkt(SendToken);
			if(SetER(t_ASIPMSPatchUtil->ASIPMS_SetData(ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN, 0, SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set data fail : [%d]-[%d]", m_strLogicName.c_str(), ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN, g_nErrRtn);
				return g_nErrRtn;
			}
			SendToken.Clear();
		}
		{
			t_MMPPPatch->t_ManagePtnPatchScanEnv->SetPkt(SendToken);
			if(SetER(t_ASIPMSPatchUtil->ASIPMS_SetData(ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV, 0, SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set data fail : [%d]-[%d]", m_strLogicName.c_str(), ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV, g_nErrRtn);
				return g_nErrRtn;
			}
			SendToken.Clear();
		}
		{
			t_MMPPPatch->t_ManagePtnPatchScanEnvUnit->SetPkt(SendToken);
			if(SetER(t_ASIPMSPatchUtil->ASIPMS_SetData(ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV_UNIT, 0, SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set data fail : [%d]-[%d]", m_strLogicName.c_str(), ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV, g_nErrRtn);
				return g_nErrRtn;
			}
			SendToken.Clear();
		}
	}

	{
		if(t_ManageLinkLicense->GetSIDCount())
		{
// 			SendToken.Clear();
// 			t_ManagePtnMimsPatch->SetPkt(SendToken);
// 			SendData_Link(G_TYPE_PTN_MIMS_PATCH, G_CODE_COMMON_SYNC, SendToken);
// 			SendToken.Clear();
		}
	}
	
	CloseDBFQ();
*/
	m_nInitLogic = 1;
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::FreePtnPatch()
{
	m_nInitLogic = 0;
	m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_NONE;
	m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_NONE;
	m_strWScanFileName = "";

	{
//		t_MMPPPatch->_clear();
	}

	{
//		CloseDBFQ();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ReloadPtnPatch()
{
	FreePtnPatch();
	return InitPtnPatch();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::OpenDBFQ()
{
/*
	if(m_tASIDBFQDLLUtil)	return 0;
	PDB_PO_PM_OP pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_OP);
	if(!pdata)				return -1;

	m_tASIDBFQDLLUtil = new CASIDBFQDLLUtil();	
	CHAR szDLLName[TCHAR_MAX_SIZE] = {0, };
	if(m_tASIDBFQDLLUtil->LoadLibraryExt(FormatString(szDLLName, "%s\\asi_dbfq.so", t_EnvInfo->m_strDLLPath)))
	{
		WriteLogE("[%s] load library fail : [%s][%d]", m_strLogicName.c_str(), szDLLName, GetLastError());
		return -51;
	}

	ASIDBFQ_INIT_INFO tSII;
	tSII.nRemainLog		= 1;
	tSII.nOpenWaitCnt	= 20;
	tSII.nRemainQOpTime = t_EnvInfo->m_nRemainDBFQOpTime;
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szLogFileName, "\\sa_ptn_patch_dbfq_sys_log_");
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szLogFilePath, "%s\\log", t_EnvInfo->m_strRootPath);
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szASIDBFDLLPath, "%s", t_EnvInfo->m_strDLLPath);
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szASIDBFDataPath, "%s\\%s", t_EnvInfo->m_strRootPath, STR_PTN_PATCH_FILE);
	
	{
		CTokenString TokenDBF(pdata->strPtnFileName, pdata->strPtnFileName.GetLength(), '.');
		m_strDBFName = TokenDBF.NextToken();
	}


	if(m_tASIDBFQDLLUtil->ASIDBFQ_Create())
	{
		WriteLogE("[%s] create asi_dbfq object fail", m_strLogicName);
		return -1;
	}
	else if(m_tASIDBFQDLLUtil->ASIDBFQ_Init(&tSII))
	{
		WriteLogE("[%s] init asi_dbfq fail", m_strLogicName.c_str());
		return -1;
	}
	else if(m_tASIDBFQDLLUtil->ASIDBFQ_OpenDatabase(m_strDBFName, "root", "tmzosjfnxm"))
	{
		WriteLogE("[%s] open file database fail : [%s]", m_strLogicName.c_str(), m_strDBFName.c_str());
		return -1;
	}
	else
	{
		WriteLogN("[%s] open file database success : [%s]", m_strLogicName.c_str(), m_strDBFName.c_str());
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::CloseDBFQ()
{
/*
	if(!m_tASIDBFQDLLUtil)		return 0;
	if(m_strDBFName.empty())	return 0;

	m_tASIDBFQDLLUtil->ASIDBFQ_CloseDatabase(m_strDBFName);
	m_tASIDBFQDLLUtil->FreeLibraryExt();
	SAFE_DELETE(m_tASIDBFQDLLUtil);
	m_strDBFName.empty();
*/
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::IsEnableRunMode(UINT32 nType)
{
	INT32 nRtn = 0;
/*
	switch(nType)
	{
		case G_TYPE_PO_PM_EX:	nRtn = IsSameInterLockExchange(&m_nIsRunRBMode, 1);		break;
		break;
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::SetEnableRunMode(UINT32 nType, UINT32 nMode)
{
/*
	switch(nType)
	{
		case G_TYPE_PO_PM_EX:	SetInterLockExchange(&m_nIsRunRBMode, nMode);		break;
		break;
	}
*/
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ChkPtnPatchWsus()
{
	INT32 nRtn = 0;
/*
	if(m_nInitPatchWsus != SS_PTN_PATCH_WSUS_STATUS_NONE)		return -1;
	
	
	{
		PDB_PTN_PATCH_WSUS pdata = t_MMPPPatch->t_ManagePtnPatchWsus->GetPtnPatchWsus(m_tSysInfo.GetSystemID(), m_tSysInfo.GetSPID());
		if(!pdata)		
		{
			m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_FAIL;
			return -1;
		}

		if(ChkPtnPatchWsusUnit(pdata))
		{
			nRtn = -1;
		}
		else
		{
			m_strWScanFileName.Format("%s\\%s\\%s", t_EnvInfo->m_strRootPath, STR_PTN_PATCH_FILE_PATCH, pdata->strName);
		}		
	}

	if(m_nInitPatchWsus == SS_PTN_PATCH_WSUS_STATUS_NONE)	
	{
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_CONFIRM;
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ChkPtnPatchWsusUnit(PDB_PTN_PATCH_WSUS pdata, UINT32 nChkOnly)
{
/*
	if(t_MMPPPatch->t_ManagePtnPatchWsus->IsValidPtnFile(pdata, SS_DN_FILE_TYPE_PPTN_WSUS))
	{
		return 0;
	}

	if(nChkOnly)	return -1;

	AddDpDownInfoWsus(pdata);
*/
	return -1;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ApplyPtnPatchWsus(UINT32 nID)
{
/*
	PDB_PTN_PATCH_WSUS pdata = t_MMPPPatch->t_ManagePtnPatchWsus->FindItem(nID);
	if(!pdata)		return 0;

	if(ChkPtnPatchWsusUnit(pdata, 1))
	{
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_WAIT;
		return 0;
	}

	m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_NONE;
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPtnPatch::AddDpDownInfoWsus(PDB_PTN_PATCH_WSUS pdata)
{
/*
	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName);
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_WAIT;
		return -1;
	}
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName);
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_WAIT;
		return -2;
	}

	String strDnFileName, strDnFileHash;
	strDnFileName = pdata->strName;
	strDnFileHash = pdata->strHash;

	if(strDnFileName.empty() || strDnFileHash.empty())
	{
		WriteLogE("[%s] invalid ptn patch wsus file info [%s][%s]", m_strLogicName.c_str(), strDnFileName.c_str(), strDnFileHash.c_str());
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_FAIL;
		return -4;
	}

	{
		m_nInitPatchWsus = SS_PTN_PATCH_WSUS_STATUS_CHANGE;

		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = SS_DN_FILE_TYPE_PPTN_WSUS;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdata->nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strcpy_ext(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_PPTN_PATCH, MAX_PATH);

		{	
			strcpy_ext(tAFI.szFileName, strDnFileName, MAX_PATH);
			strcpy_ext(tAFI.szFileHash, strDnFileHash, MAX_PATH);
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists ptn patch wsus download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
			return 0;
		}

		WriteLogN("start ptn patch wsus download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
		t_ManageFileDown->AddItem(tAFI);
	}
*/
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::InitPtnDeploy()
{
/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_READY_IN_DEPLOY);
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ChkPtnDeploy()
{
	INT32 nRtn = 0;
/*
	if(m_nInitPtnDeploy != SS_PTN_DEPLOY_STATUS_NONE)	return -1;

	TListID tIDList;
	t_MMPPPatch->t_ManagePtnDeploy->GetItemIDList(tIDList);
	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PTN_DEPLOY pdata = t_MMPPPatch->t_ManagePtnDeploy->FindItem(*begin);
			if(!pdata)		continue;

			if(pdata->nSysType && (pdata->nSysType & m_tSysInfo.GetSystemID()) == 0)						continue;
			if(pdata->nSysArchType != 99 && pdata->nSysArchType != m_tSysInfo.GetSysProcArchitecture())		continue;
			if(pdata->nSysSPType != -1 && pdata->nSysSPType != m_tSysInfo.GetSPID())						continue;

			if(GET_RIGHT_POS(SS_PACKAGE_TYPE_NPMS) != GET_RIGHT_POS(pdata->nMatchPackage))					continue;
			if((GET_RIGHT_VALUE(SS_PACKAGE_TYPE_NPMS) & GET_RIGHT_VALUE(pdata->nMatchPackage)) == 0)		continue;

			if(ChkPtnDeployUnit(pdata))
			{
				nRtn = -1;
				break;
			}
		}
	}

	if(m_nInitPtnDeploy == SS_PTN_DEPLOY_STATUS_NONE)	m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_CONFIRM;
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ChkPtnDeployUnit(PDB_PTN_DEPLOY pdata, UINT32 nChkOnly)
{
/*
	CFileUtil tFileUtil;
	String strTarFile = tFileUtil.GetEnvPathToLocalPathSingle(pdata->strSvFileName, 0);

	if(pdata->nUsedMode == 0)
	{
		if(tFileUtil.FileExists(strTarFile))
		{
			DeleteFile(strTarFile);
			CTokenString TokenSv(strTarFile, strTarFile.GetLength(), '\\', 1);
			WriteLogN("[%s] remove ptn deploy unit : [%d]-[%s]", m_strLogicName.c_str(), pdata->nType, TokenSv.NextToken());
		}
		return 0;
	}

	if(tFileUtil.FileExists(strTarFile))	
	{		
		if(ChkFileHash(strTarFile, pdata->strSvFileHash) == 0)
		{
			CTokenString TokenSv(strTarFile, strTarFile.GetLength(), '\\', 1);
			
			WriteLogN("[%s] check ptn deploy confirm : [%d]-[%s]", m_strLogicName.c_str(), pdata->nType, TokenSv.NextToken());
			return 0;
		}
	}
	if(nChkOnly)	return -1;

	AddDownloadPtnDeployUnit(pdata);
*/
	return -1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPtnPatch::AddDownloadPtnDeployUnit(PDB_PTN_DEPLOY pdata)
{
/*
	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName);
		m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_WAIT;
		return -1;
	}
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName);
		m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_WAIT;
		return -2;
	}

	String strDnFilePath, strDnFileName, strDnFileHash;
	strDnFilePath = pdata->strDnFilePath;
	strDnFileName = pdata->strDnFileName;
	strDnFileHash = pdata->strDnFileHash;

	if(strDnFileName.empty() || strDnFileHash.empty())
	{
		WriteLogE("[%s] invalid ptn deploy file info [%s][%s]", m_strLogicName.c_str(), strDnFileName.c_str(), strDnFileHash.c_str());
		m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_FAIL;
		return -4;
	}

	{
		m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_CHANGE;

		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = SS_DN_FILE_TYPE_PPTN_DEPLOY;
		tAFI.nItemID = pdata->nID;	
		tAFI.nPolicyID = 0;
		tAFI.nRefCntMax = 1;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		{	
			strcpy_ext(tAFI.szDLPath, strDnFilePath, MAX_PATH);
			strcpy_ext(tAFI.szFileName, strDnFileName, MAX_PATH);
			strcpy_ext(tAFI.szFileHash, strDnFileHash, MAX_PATH);
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists ptn deploy download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
			return 0;
		}

		WriteLogN("start ptn deploy download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
		t_ManageFileDown->AddItem(tAFI);
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnPatch::ApplyPtnDeploy(UINT32 nID)
{	
/*
	PDB_PTN_DEPLOY pdata = t_MMPPPatch->t_ManagePtnDeploy->FindItem(nID);
	if(!pdata)		return 0;

	if(ChkPtnDeployUnit(pdata, 1))
	{
		m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_WAIT;
		return 0;
	}

	m_nInitPtnDeploy = SS_PTN_DEPLOY_STATUS_NONE;
*/
	return 0;
}
//---------------------------------------------------------------------------
