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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 * * 
 */

// ThreadPoInPtnFile.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoInCreateLog.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInCreateLog

CThreadPoInCreateLog*	t_ThreadPoInCreateLog = NULL;

CThreadPoInCreateLog::CThreadPoInCreateLog()
{
	m_nTestTime = 0;
	m_fTotalDiffTime = 0;
	m_nTestCount = 0;
	pthread_mutex_init(&m_EventMutex, NULL);
}

CThreadPoInCreateLog::~CThreadPoInCreateLog()
{
	pthread_mutex_destroy(&m_EventMutex);
}

INT32 CThreadPoInCreateLog::LoadWhitePattern()
{
	INT32 nRetVal = 0;
	CHAR acPath[MAX_PATH] = {0, };

	snprintf(acPath, MAX_PATH-1, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath.c_str());
	acPath[MAX_PATH-1] = 0;

	nRetVal = m_tWEDLLUtil.LoadLibraryExt(acPath);
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load library  : [%s][%d][%d]", m_strThreadName.c_str(), acPath, nRetVal, errno);
		return -2;
	}
	nRetVal = m_tWEDLLUtil.Init();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we dll  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -3;
	}
	nRetVal = m_tWEDLLUtil.InitDB();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we db  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -4;
	}
	return 0;
}


//---------------------------------------------------------------------------

VOID CThreadPoInCreateLog::UnloadWhitePattern()
{
	m_tWEDLLUtil.ClearFile();
	m_tWEDLLUtil.FreeLibraryExt();
}

BOOL CThreadPoInCreateLog::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInCreateLog::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

INT32 CThreadPoInCreateLog::GetCreateFileListCnt()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&m_EventMutex);
	nCount = m_tCreateFileList.size();
	pthread_mutex_unlock (&m_EventMutex);
	return nCount;
}

INT32 CThreadPoInCreateLog::AddCreateFileList(PASI_CHK_FILE_PROC pCreateFile)
{
	INT32 nOpMode = 0;
	if(pCreateFile == NULL)
	{
		return -1;
	}
	pthread_mutex_lock (&m_EventMutex);
	m_tCreateFileList.push_back(*pCreateFile);
	pthread_mutex_unlock (&m_EventMutex);
	return 0;
}

INT32 CThreadPoInCreateLog::GetCreateFileList(TListChkFileProc &tListCreateFile) 
{
	INT32 nCount = 0;
	TListChkFileProcItor begin, end;
	pthread_mutex_lock (&m_EventMutex);
	begin = m_tCreateFileList.begin();	end = m_tCreateFileList.end();
	for(begin; begin != end; begin++)
	{
		tListCreateFile.push_back(*begin);
	}
	m_tCreateFileList.clear();
	pthread_mutex_unlock (&m_EventMutex);
	nCount = tListCreateFile.size();
	return nCount;
}

VOID CThreadPoInCreateLog::ClearCreateFileList()
{
	pthread_mutex_lock (&m_EventMutex);
	m_tCreateFileList.clear();
	pthread_mutex_unlock (&m_EventMutex);
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInCreateLog message handlers

INT32 CThreadPoInCreateLog::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	TListChkFileProc tListCreateFile;
	PASI_CHK_PTN_FILE pChkPtnFile = NULL;

	nRetVal = LoadWhitePattern();
	if(nRetVal < 0)
	{
		WriteLogE("fail to load white pattern : [%s][%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	pChkPtnFile = (PASI_CHK_PTN_FILE)malloc(sizeof(ASI_CHK_PTN_FILE));
	if(pChkPtnFile == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		UnloadWhitePattern();
		return -2;
	}

	m_nRunFlag = 1;

	while(GetContinue())
	{
		nCount = GetCreateFileList(tListCreateFile);
		if(nCount > 0)
		{
			nRetVal = CheckCreateNotifyFile(tListCreateFile, pChkPtnFile);
			if(nRetVal < 0)
			{
				WriteLogE("[%s] fail to check create notify file : [%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
			}
			tListCreateFile.clear();
		}
		else
		{
			Sleep(100);
		}
	}
	WriteLogN("[%s] stop thread by sub continue flag", m_strThreadName.c_str());
	UnloadWhitePattern();
	ClearCreateFileList();
	safe_free(pChkPtnFile);
	return 0;
}

INT32	CThreadPoInCreateLog::CreateSendFile(PASI_CHK_PTN_FILE pChkPtnFile)
{
	FILE *fp = NULL;
	char acPath[MAX_PATH] = {0,};
	if(pChkPtnFile == NULL)
	{
		return -1;
	}
	if(_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFile, "tc", 2))
	{
		return 0;
	}
	strncpy(acPath, "/tmp/tcfile", MAX_PATH-1);
	if(CreateDirectory(acPath, NULL) == FALSE)
	{
		return -2;
	}
	snprintf(acPath, MAX_PATH-1, "/tmp/tcfile/%s", pChkPtnFile->stCHKFILE.stFileInfo.acFile);
	if(is_file(acPath) == REG_FILE)
	{
		return 0;
	}

	fp = fopen(acPath, "w");
	if(fp == NULL)
	{
		return -3;
	}

	fprintf(fp, "%s", pChkPtnFile->stCHKFILE.stFileInfo.acFile);
	fclose(fp);

	return 0;
}

INT32	CThreadPoInCreateLog::AnalyzeCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 i, nRetVal = 0;
	DWORD dwFileType = AS_INVALID_FILE;
	memset(&pChkPtnFile->stAWWE, 0, sizeof(ASI_WENG_WL_EX));
	
	if(pChkPtnFile == NULL)
		return -1;

	for(i=0; i<3; i++)
	{
		nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
		if(nRetVal == 0 || dwFileType != AS_INVALID_FILE)
		{
			break;
		}
		Sleep(10);
	}
	if(pChkPtnFile->stAWWE.acWhiteHash[0] == 0 || dwFileType == AS_INVALID_FILE)
	{
		return -2;
	}

	strncpy(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash, pChkPtnFile->stAWWE.acWhiteHash, SHA512_BLOCK_SIZE+1);
	pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
	nRetVal = t_LogicMgrSiteFile->CheckSiteCreateFile(&pChkPtnFile->stCHKFILE);
	if(nRetVal == 0)
	{

		if(t_ThreadPoInAccFile != NULL)
		{
			t_ThreadPoInAccFile->SendAddCreateFile(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, pChkPtnFile->stAWWE.acWhiteHash);
		}
		CreateSendFile(pChkPtnFile);
	}
	return 0;
}

INT32	CThreadPoInCreateLog::AnalyzeElfFile(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
#ifdef _PERP_TEST_LOG
	BOOL bIsTestPgm = FALSE;
	double fDiffTime = 0;
	struct timeval stStartTime;
#endif
	if(pChkPtnFile == NULL || t_LogicMgrSiteFile == NULL)
	{
		return -1;
	}
#ifdef _PERP_TEST_LOG
	if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFile, "tc5_file_", 9))
	{
		bIsTestPgm = TRUE;
		gettimeofday(&stStartTime, NULL);
	}
#endif
	nRetVal = AnalyzeCreateEvent(pChkPtnFile);
#ifdef _PERP_TEST_LOG
	if(nRetVal == 0 && bIsTestPgm == TRUE)
	{
		fDiffTime = diff_time(stStartTime);
		UINT32 nTime = (UINT32)time(NULL);
		if(nTime - m_nTestTime > 30)
		{
			m_nTestCount = 0;
			m_fTotalDiffTime = 0;
		}
		else
			m_nTestCount++;
		m_nTestTime = nTime;
		m_fTotalDiffTime += fDiffTime;
		WritePerfTest5Log("[%03d]\ttest the interval time for detection of created file [%s] [detection time : %.02f ms]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, fDiffTime/1000);
		if(m_nTestCount == 19)
		{
			fDiffTime = m_fTotalDiffTime/20;
			WritePerfTest5Log("[total]\ttest the interval time for detection of created file [total : %d files] [average time : %.02f ms]", m_nTestCount+1, fDiffTime/1000);
		}
	}
#endif
	return nRetVal;
}

//---------------------------------------------------------------------------

INT32		CThreadPoInCreateLog::CheckCreateNotifyFile(TListChkFileProc tListChkFileProc, PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	char acSockPath[MAX_PATH] = {0,};
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	TListChkFileProcItor begin, end;
	if(pChkPtnFile == NULL)
		return -1;

	begin = tListChkFileProc.begin();	end = tListChkFileProc.end();
	for(begin; begin != end; begin++)
	{
		pChkFileProc = (PASI_CHK_FILE_PROC)&(*begin);
		if(pChkFileProc == NULL)
			continue;
		memcpy(&pChkPtnFile->stCHKFILE, pChkFileProc, nSize);
		AnalyzeElfFile(pChkPtnFile);
	}
	return 0;
}

