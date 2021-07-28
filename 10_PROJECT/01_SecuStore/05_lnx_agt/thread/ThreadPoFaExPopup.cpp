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

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoFaExPopup.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaExPopup

CThreadPoFaExPopup*	t_ThreadPoFaExPopup = NULL;

CThreadPoFaExPopup::CThreadPoFaExPopup()
{
//	m_tRegUtil.SetRootPath(STR_REG_DEFAULT_SVC_EX_POPUP_SECU_PATH);
//	m_tRegUtil.SetWowDefaultKey();
}

CThreadPoFaExPopup::~CThreadPoFaExPopup()
{
}

BOOL CThreadPoFaExPopup::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoFaExPopup::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaExPopup message handlers

int CThreadPoFaExPopup::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	INT32 nRet = 0;
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			nRet = ChkExPopupRegList();
			if(!nRet)
			{
				Sleep(100);
			}
		}
		else
		{
			Sleep(10);
		}
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return CThreadBase::Run();
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaExPopup::ChkExPopupRegList()
{
/*
	TListRegValueInfo tValueList;

	m_tRegUtil.GetEnumSubValueName(HKEY_LOCAL_MACHINE, "", tValueList);
	if(tValueList.size() == 0)	return 0;

	{
		TListRegValueInfoItor begin, end;
		begin = tValueList.begin();	end = tValueList.end();
		for(begin; begin != end; begin++)
		{
			ChkExPopupRegInfo(*begin);
		}
	}
*/	
	return 1;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaExPopup::ChkExPopupRegInfo(REG_VALUE_INFO& tRVI)
{
/*
	StringW strFileNameNew, strFileNameOrg;
	StringW strRegInfo;
	TMapStrW tFileMap;
	CFileUtil tFileUtil;

	{
		BYTE szRegInfo[TCHAR_MAX_SIZE] = {0, };
		if(m_tRegUtil.RegReadBinary(HKEY_LOCAL_MACHINE, "", tRVI.strValue, szRegInfo, TCHAR_MAX_SIZE))
		{
			return 0;
		}

		m_tRegUtil.RegDeleteValueExt(HKEY_LOCAL_MACHINE, "", tRVI.strValue);
		strRegInfo = (WCHAR*)szRegInfo;
	}

	{
		CTokenStringW Token(strRegInfo, strRegInfo.GetLength(), L'|');
		strFileNameNew = Token.NextToken();
		strFileNameOrg = Token.NextToken();
	}

	if(tFileUtil.FileExistsW(strFileNameNew))
	{
		tFileMap[strFileNameNew] = strFileNameOrg;
	}
	else
	{
		TListGatherFileInfoW tFileNameList;
		tFileUtil.GetFileNameListW(strFileNameNew, L"", L"", tFileNameList, 0);

		TListGatherFileInfoWItor begin, end;
		begin = tFileNameList.begin();	end = tFileNameList.end();
		for(begin; begin != end; begin++)
		{
			StringW strFilePathOrg = begin->strFullPath;
			strFilePathOrg.Replace(strFileNameNew, strFileNameOrg);
			
			tFileMap[begin->strFullPath] = strFilePathOrg;
		}
	}


	{
		TMapStrWRItor begin, end;
		begin = tFileMap.rbegin();	end = tFileMap.rend();
		for(begin; begin != end; begin++)
		{
			ChkExPopupFileInfo(begin->first, begin->second);
		}
	}		
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoFaExPopup::ChkExPopupFileInfo(String strFileNameNew, String strFileNameOrg)
{
/*
	CFileUtil tFileUtil;
	CProcUtil tProcUtil;

	if(tFileUtil.DirectoryExists(strFileNameNew.c_str()))
	{
		RemoveDirectory(strFileNameNew.c_str());
		return 0;
	}

	{
		DB_LOG_DOC tDLD;
		tDLD.strObjectPath	= strFileNameNew;
		tDLD.strObjectName	= strFileNameOrg;
		tDLD.strSubjectName	= STR_PROC_NAME_WIN_EXPLORER;


		INT32 nChgUserToken = 0;
		tDLD.nFileCrTime	= tFileUtil.GetFileDateTime(tDLD.strObjectPath, 0);
		tDLD.nFileMdTime	= tFileUtil.GetFileDateTime(tDLD.strObjectPath, 1);
		tDLD.nFileAcTime	= tFileUtil.GetFileDateTime(tDLD.strObjectPath, 2);

		tDLD.nOpType		= SS_LOG_DOC_OP_TYPE_EX_POPUP;
		tDLD.nPolicyType	= t_DeployPolicyUtil->GetCurPoID(SS_POLICY_TYPE_FA_CLEAR) + ASI_EPS_APP_POLICY_GROUP_ID_FA_CLEAR;

		{
			MemToken SendToken(1024);
			SendToken.TokenAdd_32(ERR_SUCCESS);
			t_ManageLogDoc->SetPkt_Link(&tDLD, SendToken);

			PKT_DATA pkt_data;
			pkt_data.body.data = (PBYTE)SendToken.GetData();
			pkt_data.hdr.length = SendToken.GetLength();
			pkt_data.hdr.type = G_TYPE_LOG_DOC;
			pkt_data.hdr.code = G_CODE_COMMON_DEL;

			WriteLogN("[%s] send message : [%x]", m_strThreadName.c_str(), t_EnvInfoOp->GetMainHandle());
			UINT32 nTickTime = GetTickCount();
			//DWORD_PTR dwError = -1;
			//LRESULT hRst = SendMessageTimeout(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_KERNEL_LOG_EVT, (WPARAM)&pkt_data, (LPARAM)ASI_EPS_APP_LOG_TYPE_DOC, SMTO_NORMAL, SENDMESSAGE_DEFAULT_WAITING, &dwError);
			LRESULT hRst = SendMessage(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_KERNEL_LOG_EVT, (WPARAM)&pkt_data, (LPARAM)ASI_EPS_APP_LOG_TYPE_DOC);
			WriteLogN("[%s] return send message : [%x][%d]:opt[%u]", m_strThreadName.c_str(), t_EnvInfoOp->GetMainHandle(), hRst, GetTickCount() - nTickTime);
		}

		if(nChgUserToken)	tProcUtil.RevertToCurToken();
	}	
*/
	return 0;
}
//---------------------------------------------------------------------------
