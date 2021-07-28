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

// ThreadPoFaClear.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoFaClear.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaClear

CThreadPoFaClear*	t_ThreadPoFaClear = NULL;

CThreadPoFaClear::CThreadPoFaClear()
{
	m_nOrderID = FF_START_ORDER_ID_PO_FA_CLEAR;
}

CThreadPoFaClear::~CThreadPoFaClear()
{
}

BOOL CThreadPoFaClear::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoFaClear::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaClear message handlers

int CThreadPoFaClear::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	INT32 nRet = 0;
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			nRet = ChkScanFileList();
			switch(nRet)
			{
				case 0:			
					m_nPause = 1;
					break;
				case 1:
					Sleep(1000);
					break;
				default:		
				{	
					WriteLogA("[%s] find file list default case value: [%d]", m_strThreadName.c_str(), nRet); 
					Sleep(10);
					break;
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}

	if(!t_EnvInfoOp)							
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaClear::AddOrderID(PMEM_FIND_ORDER_INFO pMFOI)
{
	m_tOrderIDMap[pMFOI->nID] = *pMFOI;
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaClear::ChkScanFileList()
{
	TListID tDelIDList;
	INT32 nFinding = 0;

	{
		INT32 nRet = 0;
		TMapMemFindOrderInfoItor begin, end;
		begin = m_tOrderIDMap.begin();	end = m_tOrderIDMap.end();
		for(begin; begin != end; begin++)
		{
			nRet = ChkScanFile(&(begin->second));
			switch(nRet)
			{
				case -1:
				{
					if(t_ASIFFDLLUtil)
					{
						t_ASIFFDLLUtil->ASIFF_ClearFileMask(begin->first);
						t_ASIFFDLLUtil->ASIFF_ClearFileDateTime(begin->first);
						t_ASIFFDLLUtil->ASIFF_ClearExceptPath(begin->first);
						t_ASIFFDLLUtil->ASIFF_ClearSearchDirPath(begin->first);
						t_ASIFFDLLUtil->ASIFF_ClearFileFindOption(begin->first);
					}
					WriteLogN("[%s] find file stop and clear operation : [%d]", m_strThreadName.c_str(), begin->first);
					tDelIDList.push_back(begin->first);
					break;
				}
				case 1:
				{
					nFinding = 1;
					break;
				}
				default:
				{
					WriteLogN("[%s] find file default case value : [%d]", m_strThreadName.c_str(), nRet);
				}
			}
		}
	}

	{
		TListIDItor begin, end;
		begin = tDelIDList.begin();	end = tDelIDList.end();
		for(begin; begin != end; begin++)
		{
			m_tOrderIDMap.erase(*begin);
		}
	}

	if(m_tOrderIDMap.size() == 0)
		return 0;

	if(nFinding)	return 1;
	
	return 2;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaClear::ChkScanFile(PMEM_FIND_ORDER_INFO pMFOI)
{	
	ASI_FF_FILE_ITEM tAFFI[1];
	ASI_FF_FILE_RESULT tAFFR;
	CFileUtil tFileUtil;
	UINT32 nAFFINum = 1;
	INT32 nRetVal = 0;
	CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();

	memset(tAFFI, 0, sizeof(ASI_FF_FILE_ITEM) * nAFFINum);
	if(t_ASIFFDLLUtil)
		t_ASIFFDLLUtil->ASIFF_GetFindFileItem(pMFOI->nID, tAFFI, &nAFFINum, &tAFFR);

	if(tAFFR.nContinue == 0 && tAFFR.nMoreFileItem == 0)  
	{
		WriteLogN("[%s] find file work end : [%d][%d:%d]", m_strThreadName.c_str(), pMFOI->nID, tAFFR.nFileWorkedNum, tAFFR.nFileTotalNum);
		if(pMainDlg != NULL)
		{
			pMainDlg->OnThreadPoFaClearEnd(G_TYPE_PO_FA_CLEAR_UNIT, pMFOI);
		}
		return -1;
	}

	if(pMFOI->nNotiPg && (GetTickCount() - pMFOI->nNotiPgSend) > 1000)
	{
		pMFOI->nNotiPgSend = GetTickCount();
		
		MEM_FIND_ORDER_INFO tMFOI = *pMFOI;
		tMFOI.nNotiPgTotal	= tAFFR.nFileTotalNum;
		tMFOI.nNotiPgEnd	= tAFFR.nFileWorkedNum;

		if(pMainDlg != NULL)
		{
			pMainDlg->OnThreadPoFaClearProgress(G_TYPE_PO_FA_CLEAR_UNIT, &tMFOI);
		}
	}

	{
		t_ThreadChkHkNoti->SetMemFile_ExitWinProgress(tAFFR.nDirTotalNum, tAFFR.nDirSearchedNum, tAFFR.nFileTotalNum, tAFFR.nFileWorkedNum);		
	}

	if(!nAFFINum)
	{
 		WriteLogN("[%s] find file working: [%d][%d:%d]:[%d:%d]-[%d:%d]-[%d:%d]-[%d:%d]", 
 													m_strThreadName.c_str(), pMFOI->nID, 
 													tAFFR.nContinue, tAFFR.nMoreFileItem,
 													tAFFR.nSearchPathNum, tAFFR.nSearchedPathNum,
 													tAFFR.nDirTotalNum, tAFFR.nDirSearchedNum,
 													tAFFR.nDirSubSearchNum, tAFFR.nDirSubSearchedNum,
 													tAFFR.nFileTotalNum, tAFFR.nFileWorkedNum);
		return 1; 
	}

	DB_LOG_DOC tDLD;
	String strObjectFullPath;
	PKT_DATA pkt_data;
	MemToken SendToken(2048);
	UINT32 nIdx = 0;
	while(nIdx < nAFFINum && GetContinue())
	{
		memset(&tDLD, 0, sizeof(DB_LOG_DOC));
		memset(&pkt_data, 0, sizeof(PKT_DATA));
		SendToken.Clear();
		
		tDLD.strObjectPath	= tAFFI[nIdx].szFilePath;
		tDLD.strObjectName	= tAFFI[nIdx].szFileName;
		tDLD.strSubjectName	= STR_SERVICE_NAME;
		if(t_EnvInfoOp != NULL)
			tDLD.strSubjectPath	= SPrintf("%s/sbin", t_EnvInfoOp->GetRootPath().c_str());

		strObjectFullPath = SPrintf("%s/%s", tDLD.strObjectPath.c_str(), tDLD.strObjectName.c_str());

		if(tFileUtil.GetFileDateTime(strObjectFullPath.c_str(), &tDLD.nFileCrTime, &tDLD.nFileMdTime, &tDLD.nFileAcTime) != 0)
		{
			tDLD.nFileCrTime = tDLD.nFileMdTime = tDLD.nFileAcTime = 0;
		}

		tDLD.nOpType		= pMFOI->nOpType;
		tDLD.nPolicyType	= (pMFOI->nPoID ? pMFOI->nPoID : pMFOI->nID) + ASI_EPS_APP_POLICY_GROUP_ID_FA_CLEAR;
		
		SendToken.TokenAdd_32(ERR_SUCCESS);
		t_ManageLogDoc->SetPkt_Link(&tDLD, SendToken);

		pkt_data.body.data = (PBYTE)SendToken.GetData();
		pkt_data.hdr.length = SendToken.GetLength();
		pkt_data.hdr.type = G_TYPE_LOG_DOC;
		pkt_data.hdr.code = UINT16(pMFOI->nNextOp);

		WriteLogN("[%s] [%02d] send message : %s", m_strThreadName.c_str(), nIdx, strObjectFullPath.c_str());
		if(pMainDlg != NULL)
		{
			nRetVal = pMainDlg->OnKernelFileLog(ASI_EPS_APP_LOG_TYPE_DOC, &pkt_data);
			if(nRetVal != 0)
			{
				WriteLogE("[%s] fail to add file log (%d)", m_strThreadName.c_str(), nRetVal);
			}
		}
		nIdx += 1;
	}	

	{
		pMFOI->nNotiTotalFind += nIdx;
	}

	return 0;
}
//---------------------------------------------------------------------------
