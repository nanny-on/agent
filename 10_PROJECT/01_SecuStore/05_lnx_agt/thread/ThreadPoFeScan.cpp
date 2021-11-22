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

// ThreadPoFeScan.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoFeScan.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFeScan

CThreadPoFeScan*	t_ThreadPoFeScan = NULL;

CThreadPoFeScan::CThreadPoFeScan()
{
	m_nOrderID = 10000;
	m_nWorkID = 0;
}

CThreadPoFeScan::~CThreadPoFeScan()
{
}

BOOL CThreadPoFeScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoFeScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFeScan message handlers



INT32		CThreadPoFeScan::AddWork(PMEM_FIND_ORDER_INFO pMFOI)
{
	m_tWorkMutex.Lock();	
	m_tWorkInfoMap[pMFOI->nID] = *pMFOI;
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFeScan::DelWork(UINT32 nID)
{
	m_tWorkMutex.Lock();	
	m_tWorkInfoMap.erase(nID);
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

UINT32			CThreadPoFeScan::GetNextWork()
{
	UINT32 nRtn = 0;
	m_tWorkMutex.Lock();
	if(m_tWorkInfoMap.size())
	{
		nRtn = m_tWorkInfoMap.begin()->first;
	}
	m_tWorkMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

int CThreadPoFeScan::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		m_nWorkID = GetNextWork();
		if(m_nWorkID)
		{	
			do 
			{
				if(StartScan())
				{
					WriteLogE("init environment fail : [%s][oid:%u]", m_strThreadName.c_str(), m_nWorkID);
					break;
				}

				if(m_tWEngWLExMap.size())
				{
					ProcessScanResult();
				}
			} while (FALSE);						

			StopScan();
			DelWork(m_nWorkID);
			WriteLogE("stop scan : [%s][oid:%u]", m_strThreadName.c_str(), m_nWorkID);
		}
		Sleep(10);
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFeScan::StartScan()
{
	CHAR szDLLName[CHAR_MAX_SIZE] = {0, };

	snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_ff.so", t_EnvInfo->m_strDLLPath.c_str());
	szDLLName[CHAR_MAX_SIZE-1] = 0;
	if(m_tFFDLLUtil.LoadLibraryExt(szDLLName))
	{
		WriteLogE("load library fail : [%s][%d]", szDLLName, errno);
		return -15;
	}
	{
		ASI_FF_INIT_INFO tAFII;
		t_EnvInfoOp->GetFFPtnInitData(tAFII);
		m_tFFDLLUtil.ASIFF_Init(&tAFII);
	}

	PDB_PO_FE_PTN_OP pCurPolicy = NULL;
	{
		pCurPolicy = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
		if(!pCurPolicy)	
		{
			WriteLogE("[%s] not find po_fe_ptn_op current policy", m_strThreadName.c_str());
			return -3;
		}

		m_tCurPolicy = *pCurPolicy;
	}
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFeScan::StopScan()
{
	m_tFFDLLUtil.FreeLibraryExt();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFeScan::ChkScanFile()
{	
	ASI_FF_FILE_ITEM tAFFI[1];
	ASI_FF_FILE_RESULT tAFFR;

	UINT32 nAFFINum = 1;
	memset(tAFFI, 0, sizeof(ASI_FF_FILE_ITEM) * nAFFINum);
	m_tFFDLLUtil.ASIFF_GetFindFileItem(m_nWorkID, tAFFI, &nAFFINum, &tAFFR);

	if(tAFFR.nContinue == 0 && tAFFR.nMoreFileItem == 0)  
	{
		WriteLogN("[%s] find file work end : [%d][%d:%d]", m_strThreadName.c_str(), m_nWorkID, tAFFR.nFileWorkedNum, tAFFR.nFileTotalNum);
		return -1;
	}

	if(!nAFFINum)	return 1; 

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFeScan::ProcessScanResult()
{
	CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();	
	PDB_PO_FE_PTN_OP pCurPolicy = NULL;

	{
		pCurPolicy = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
		if(!pCurPolicy)	
		{
			WriteLogE("[%s] not find po_fe_ptn_op current policy", m_strThreadName.c_str());
			return -1;
		}
	}

	{
		TMapWEngWLExItor begin, end;
		begin = m_tWEngWLExMap.begin();	end = m_tWEngWLExMap.end();
		for(begin; begin != end; begin++)
		{
			if(pCurPolicy->nNewFileSendType & SS_PO_FE_PTN_LO_NEW_FILE_SEND_TYPE_INFO)
			{
//				String strFeKey = BinToHexLen((PBYTE)(begin->second.acWhiteHash), SHA512_BLOCK_SIZE);
				String strFeKey = begin->second.acWhiteHash;

				PDB_SITE_FILE pdsf = t_ManageSiteFile->FindPosSKeyID_Item(strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
				if(pdsf && (pdsf->nSyncSvrStep & SS_PO_FE_PTN_LO_NEW_FILE_SEND_TYPE_INFO))		continue;
				
				CTokenString Token(begin->second.acFilePath, strlen_ext(begin->second.acFilePath), '/', 1);

				DB_SITE_FILE tDSF;
				tDSF.strFeKey = strFeKey;				
				tDSF.strFileName = Token.NextToken();
				tDSF.strFilePath = Token.RemainValue();

				if(pMainDlg != NULL)
					pMainDlg->OnThreadLog(ASI_EPS_APP_LOG_TYPE_SITE_FILE, (PVOID)&tDSF);

			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
