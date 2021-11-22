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

// ThreadPoFePtnLo.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoFePtnLo.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFePtnLo

CThreadPoFePtnLo*	t_ThreadPoFePtnLo = NULL;

CThreadPoFePtnLo::CThreadPoFePtnLo()
{
	m_nOrderID = FF_START_ORDER_ID_PO_FE_LOCAL;
}

CThreadPoFePtnLo::~CThreadPoFePtnLo()
{
}

BOOL CThreadPoFePtnLo::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoFePtnLo::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFePtnLo message handlers

void			CThreadPoFePtnLo::AddWork(UINT64 nOrderID)
{
	m_tOrderMutex.Lock();

	if(m_tOrderIDMap.find(nOrderID) == m_tOrderIDMap.end())
	{
		m_tOrderIDList.push_back(nOrderID);
		m_tOrderIDMap[nOrderID] = m_nOrderID++;
	}
	m_tOrderMutex.UnLock();
	return;
}
//-------------------------------------------------------------------------------

UINT32			CThreadPoFePtnLo::IsWork()
{
	UINT32 nRtn = 0;
	m_tOrderMutex.Lock();
	nRtn = m_tOrderIDList.size();
	m_tOrderMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------

UINT32			CThreadPoFePtnLo::NextWork()
{
	UINT32 nRtn = 0;
	m_tOrderMutex.Lock();

	if(m_tOrderIDList.size())
	{
		UINT64 nOrderID = *(m_tOrderIDList.begin());		m_tOrderIDList.pop_front();

		TMapID64Itor find = m_tOrderIDMap.find(nOrderID);
		if(find != m_tOrderIDMap.end())
		{
			nRtn = UINT32(find->second);
			m_tOrderIDMap.erase(find);
		}

		if(!nRtn)
		{
			m_tOrderIDList.clear();
			m_tOrderIDMap.clear();
		}
	}

	m_tOrderMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

int CThreadPoFePtnLo::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();

	if(pMainDlg == NULL)
	{
		WriteLogE("invalid po fe ptn lo class");
		return 1;
	}

	m_nRunFlag = 1;

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(IsWork())
		{
			m_nWorkOrderID = NextWork();
			if(!m_nWorkOrderID)		continue;

			if(InitEnv())
			{
				WriteLogE("init environment fail : [%s]", m_strThreadName.c_str());
				continue;
			}

			if(m_tWEngWLExMap.size())
			{
				SavePtnFile();

				pMainDlg->OnThreadEnd(G_TYPE_PO_FE_PTN_LO, &m_tCurPolicy);
			}

			FreeEnv();
		}
		Sleep(10);
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFePtnLo::InitEnv()
{
	{
		CHAR szDLLName[CHAR_MAX_SIZE] = {0, };

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_ff.so", t_EnvInfo->m_strDLLPath.c_str());
		szDLLName[CHAR_MAX_SIZE-1] = 0;
		if(m_tFFDLLUtil.LoadLibraryExt(szDLLName))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, errno);
			return -15;
		}
	}

	{
		ASI_FF_INIT_INFO tAFII;
		t_EnvInfoOp->GetFFPtnInitData(tAFII);
		m_tFFDLLUtil.ASIFF_Init(&tAFII);
	}

	PDB_PO_FE_PTN_LO pCurPolicy = NULL;
	{
		pCurPolicy = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_LO);
		if(!pCurPolicy)
		{
			WriteLogE("[%s] not find po_fe_ptn_lo current policy", m_strThreadName.c_str());
			return -3;
		}

		m_tCurPolicy = *pCurPolicy;
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFePtnLo::FreeEnv()
{
	m_tFFDLLUtil.FreeLibraryExt();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFePtnLo::ChkScanFile()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFePtnLo::SavePtnFile()
{
	return 0;
}
//---------------------------------------------------------------------------
