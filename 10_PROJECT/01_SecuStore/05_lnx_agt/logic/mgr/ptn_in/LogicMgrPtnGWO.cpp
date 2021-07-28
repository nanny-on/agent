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
#include "LogicMgrPtnGWO.h"

//---------------------------------------------------------------------------

CLogicMgrPtnGWO*		t_LogicMgrPtnGWO = NULL;

//---------------------------------------------------------------------------

CLogicMgrPtnGWO::CLogicMgrPtnGWO()
{
	t_MMPPGWO = new MEM_MGR_PTN_PKG;
	if(t_MMPPGWO != NULL)
		t_MMPPGWO->_init();
	
	m_strLogicName		= "mgr agt ptn gwo";
	
	m_nPackageType		= SS_PACKAGE_TYPE_NOMS;
	m_nPolicyType		= SS_POLICY_TYPE_IN_PTN_WL;
	m_nSSPoSeqIdx		= 0;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;

	m_tASIDBFQDLLUtil	= NULL;
	m_nInitLogic		= 0;	
}
//---------------------------------------------------------------------------

CLogicMgrPtnGWO::~CLogicMgrPtnGWO()
{
	if(t_MMPPGWO)
	{
		t_MMPPGWO->_free();
		SAFE_DELETE(t_MMPPGWO);
	}
	SAFE_DELETE(m_tASIDBFQDLLUtil);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWO::InitPtnGWO()
{	
	if(OpenDBFQ())
		return -1;

	{
		t_MMPPGWO->_set_dbfq(m_tASIDBFQDLLUtil);
	}

	{
		INT32 nLoadEnd = 0;
		do 
		{
			if(t_MMPPGWO && t_MMPPGWO->t_ManagePtnVersion->LoadDBMS())
				break;
			if(t_MMPPGWO && t_MMPPGWO->t_ManagePtnProcFile->LoadDBMS())
				break;
			nLoadEnd = 1;
		} while (FALSE);

		if(!nLoadEnd)
			return -2;
	}
	CloseDBFQ();
	m_nInitLogic = 1;

	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWO::FreePtnGWO()
{
	m_nInitLogic = 0;
	
	{
		if(t_MMPPGWO)
			t_MMPPGWO->_clear();
	}
	{
		CloseDBFQ();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWO::ReloadPtnGWO()
{
	FreePtnGWO();
	return InitPtnGWO();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWO::OpenDBFQ()
{
	if(m_tASIDBFQDLLUtil)
		return 0;
	PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
	if(!pdata)
		return -1;

	m_tASIDBFQDLLUtil = new CASIDBFQDLLUtil();	
	CHAR szDLLName[TCHAR_MAX_SIZE] = {0, };
	snprintf(szDLLName, TCHAR_MAX_SIZE-1, "%s/asi_dbfq.so", t_EnvInfo->m_strDLLPath.c_str());
	if(m_tASIDBFQDLLUtil->LoadLibraryExt(szDLLName))
	{
		WriteLogE("[%s] load library fail : [%s][%d]", m_strLogicName.c_str(), szDLLName, errno);
		return -51;
	}

	ASIDBFQ_INIT_INFO tSII;
	memset(&tSII, 0, sizeof(ASIDBFQ_INIT_INFO));
	tSII.nRemainLog		= 1;
	tSII.nOpenWaitCnt	= 20;
	tSII.nRemainQOpTime = t_EnvInfo->m_nRemainDBFQOpTime;
	snprintf(tSII.szLogFileName, TCHAR_MAX_SIZE-1, "/nanny_ptn_gwo_dbfq_sys_log_");
	snprintf(tSII.szLogFilePath, TCHAR_MAX_SIZE-1, "%s/log", t_EnvInfo->m_strRootPath.c_str());
	snprintf(tSII.szASIDBFDLLPath, TCHAR_MAX_SIZE-1, "%s", t_EnvInfo->m_strDLLPath.c_str());
	
	{
		m_strDBFName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());
	}


	if(m_tASIDBFQDLLUtil->ASIDBFQ_Create())
	{
		WriteLogE("[%s] create asi_dbfq object fail", m_strLogicName.c_str());
		return -2;
	}
	
	if(m_tASIDBFQDLLUtil->ASIDBFQ_Init(&tSII))
	{
		WriteLogE("[%s] init asi_dbfq fail", m_strLogicName.c_str());
		return -3;
	}
	
	if(m_tASIDBFQDLLUtil->ASIDBFQ_OpenDatabase(m_strDBFName.c_str(), "root", "tmzosjfnxm"))
	{
		WriteLogE("[%s] open file database fail : [%s]", m_strLogicName.c_str(), m_strDBFName.c_str());
		return -4;
	}
	else
	{
		WriteLogN("[%s] open file database success : [%s]", m_strLogicName.c_str(), m_strDBFName.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrPtnGWO::CloseDBFQ()
{
	if(!m_tASIDBFQDLLUtil)
		return 0;
	if(m_strDBFName.empty())
		return 0;

	m_tASIDBFQDLLUtil->ASIDBFQ_CloseDatabase(m_strDBFName.c_str());
	m_tASIDBFQDLLUtil->FreeLibraryExt();
	SAFE_DELETE(m_tASIDBFQDLLUtil);
	m_strDBFName.empty();

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------