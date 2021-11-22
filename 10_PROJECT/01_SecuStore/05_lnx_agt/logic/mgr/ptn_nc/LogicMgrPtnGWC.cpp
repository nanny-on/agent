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
#include "LogicMgrPtnGWC.h"

//---------------------------------------------------------------------------

CLogicMgrPtnGWC*		t_LogicMgrPtnGWC = NULL;

//---------------------------------------------------------------------------

CLogicMgrPtnGWC::CLogicMgrPtnGWC()
{
//	t_MMPPGWC = new MEM_MGR_PTN_PKG;
//	t_MMPPGWC->_init();
	
	m_strLogicName		= "mgr agt ptn gwc";
	
	m_nPackageType		= SS_PACKAGE_TYPE_NCMS;
	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_WL;
	m_nSSPoSeqIdx		= 0;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;

//	m_tASIDBFQDLLUtil	= NULL;
	m_nInitLogic		= 0;	
}
//---------------------------------------------------------------------------

CLogicMgrPtnGWC::~CLogicMgrPtnGWC()
{
//	t_MMPPGWC->_free();
//	SAFE_DELETE(t_MMPPGWC);

//	SAFE_DELETE(m_tASIDBFQDLLUtil);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWC::InitPtnGWC()
{	
/*
	if(OpenDBFQ())	return -1;

	{
		t_MMPPGWC->_set_dbfq(m_tASIDBFQDLLUtil);
	}

	{
		INT32 nLoadEnd = 0;
		do 
		{
			if(t_MMPPGWC->t_ManagePtnVersion->LoadDBMS())			break;
			if(t_MMPPGWC->t_ManagePtnProcFile->LoadDBMS())			break;

			nLoadEnd = 1;
		} while (FALSE);

		if(!nLoadEnd)		return -2;
	}
	
	CloseDBFQ();
*/
	m_nInitLogic = 1;
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWC::FreePtnGWC()
{
	m_nInitLogic = 0;
/*	
	{
		t_MMPPGWC->_clear();
	}

	{
		CloseDBFQ();
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWC::ReloadPtnGWC()
{
	FreePtnGWC();
	return InitPtnGWC();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWC::OpenDBFQ()
{
/*
	if(m_tASIDBFQDLLUtil)	return 0;
	PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_WL);
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
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szLogFileName, "\\sa_ptn_gwc_dbfq_sys_log_");
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szLogFilePath, "%s\\log", t_EnvInfo->m_strRootPath);
	sprintf_ext(TCHAR_MAX_SIZE, tSII.szASIDBFDLLPath, "%s", t_EnvInfo->m_strDLLPath);
	
	{
		m_strDBFName.Format("%s\\%s\\%s", t_EnvInfo->m_strRootPath, STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName);
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

INT32	CLogicMgrPtnGWC::CloseDBFQ()
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