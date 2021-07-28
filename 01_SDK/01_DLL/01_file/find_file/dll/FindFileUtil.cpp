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
#include "com_struct_find_work.h"
#include "ThreadFinder.h"
#include "FindFileUtil.h"
//--------------------------------------------------------------

CFindFileUtil::CFindFileUtil(void)
{
	m_tASIDFFDLLUtil = NULL;
	m_tASIFIDLLUtil = NULL;
	m_nRemainDebugLog = 1;
	m_nFileLogRetention = 5;
	memset(m_acLogPath, 0, MAX_PATH);
	memset(m_acLogFile, 0, MAX_PATH);
	pthread_mutex_init(&m_mutex, NULL);

}
//--------------------------------------------------------------

CFindFileUtil::~CFindFileUtil(void)
{
	SAFE_DELETE(m_tASIDFFDLLUtil);
	SAFE_DELETE(m_tASIFIDLLUtil);
	pthread_mutex_destroy(&m_mutex);
}

void CFindFileUtil::SetLogPath(char *pLogPath, char *pLogFile, INT32 nRemainLog, UINT32 nFileLogRetention)
{
	pthread_mutex_lock(&m_mutex);
	m_nRemainDebugLog = nRemainLog;
	m_nFileLogRetention = nFileLogRetention;
	if(pLogPath)
		strncpy(m_acLogPath, pLogPath, MAX_PATH-1);
	if(pLogFile)
		strncpy(m_acLogFile, pLogFile, MAX_PATH-1);
	pthread_mutex_unlock(&m_mutex);
}

void CFindFileUtil::WriteLog(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	pthread_mutex_lock(&m_mutex);
	do{
		if(m_nRemainDebugLog == 0)
			break;
		GetCurrentDateTime(0, acTimeBuf);

		if(m_acLogPath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(m_acLogPath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		if(m_acLogFile[0] == 0)
			strncpy(m_acLogFile, "/nanny_agt_ff_log_", MAX_PATH-1);
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", m_acLogPath, m_acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(m_acLogPath, m_acLogFile, m_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[Info]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
}



//--------------------------------------------------------------

INT32		CFindFileUtil::Init(PASI_FF_INIT_INFO pAfii)
{
	String strTFName;
	INT32 nTFMaxNum = 0;
	INT32 nCpuCount = 0;
	pthread_t tid = 0;
	ASI_DFILE_FMT_INIT tADFI;
	if(pAfii == NULL)
		return -1;
	
	memcpy(&m_tAFII, pAfii, sizeof(m_tAFII));
	if(m_tAFII.szDocFileFmtDLLPath[0] == 0 || m_tAFII.szFileInfoDLLPath[0] == 0)	
		return -2;

	SetLogPath(m_tAFII.szLogPath, m_tAFII.szLogFile, m_tAFII.nRemainLog, m_tAFII.nFileLogRetention);

	m_tASIDFFDLLUtil = new CASIDFFDLLUtil();
	if(m_tASIDFFDLLUtil == NULL)
		return -3;

	if(m_tASIDFFDLLUtil->LoadLibraryExt(m_tAFII.szDocFileFmtDLLPath) != 0)
	{
		SAFE_DELETE(m_tASIDFFDLLUtil);
		return -4;
	}

	m_tASIFIDLLUtil = new CASIFIDLLUtil();
	if(m_tASIFIDLLUtil == NULL)
	{
		SAFE_DELETE(m_tASIDFFDLLUtil);
		return -5;
	}
	
	if(m_tASIFIDLLUtil->LoadLibraryExt(m_tAFII.szFileInfoDLLPath) != 0)
	{
		SAFE_DELETE(m_tASIDFFDLLUtil);
		SAFE_DELETE(m_tASIFIDLLUtil);
		return -6;
	}

	strncpy(tADFI.szLogPath , m_tAFII.szLogPath, CHAR_MAX_SIZE-1);
	strncpy(tADFI.szLogFile, m_tAFII.szLogFile, CHAR_MAX_SIZE-1);
	tADFI.nRemainLog = m_tAFII.nRemainLog;
	tADFI.nFileLogRetention = m_tAFII.nFileLogRetention;
	m_tASIDFFDLLUtil->ASIDFF_SetDFFmtInit(&tADFI);
	
	nTFMaxNum = m_tAFII.nFinderThreadMaxNum;
	if(!nTFMaxNum)
	{
		nCpuCount = get_cpu_core_num();
		nTFMaxNum = 1;
		if(nCpuCount > 2)
			nTFMaxNum = nCpuCount - 2;
		WriteLog("auto detect number of process in system : [%d]", nTFMaxNum);
	}

	m_nPreSearchLevel = 0;

	while(nTFMaxNum--)
	{
		strTFName = SPrintf("finder %.2d", nTFMaxNum);

		CThreadFinder* tTF = new CThreadFinder();
		if(tTF != NULL)
		{
			tTF->SetOwnerClass(this);
			tTF->CreateThreadExt(strTFName, &tid);
			m_tFinderThreadList.push_back((UINT64)tTF);
		}
	}

	return 0;
}

//--------------------------------------------------------------

INT32		CFindFileUtil::Release()
{
	{
		TListID64Itor begin, end;
		begin = m_tFinderThreadList.begin();	end = m_tFinderThreadList.end();
		for(begin; begin != end; begin++)
		{
			CThreadFinder* tTF = (CThreadFinder*)(*begin);
			if(tTF != NULL)
			{
				tTF->SetContinue(0);
				StopThread_Common(tTF);
				delete tTF;
			}
		}
	}

	{
		if(m_tASIFIDLLUtil)
			m_tASIFIDLLUtil->FreeLibraryExt();
		if(m_tASIDFFDLLUtil)
			m_tASIDFFDLLUtil->FreeLibraryExt();
	}

	{
		ClearFindSubDirItem();		WriteLog("clear find sub dir items..");
		ClearFindFileDirItem();		WriteLog("clear find find dir items..");
		ClearFindFileWork();		WriteLog("clear find file work..");
	}
	return 0;
}

INT32		CFindFileUtil::Stop()
{
	CMutexExt* tMutexExt = NULL;
	TMapFindFileWorkItor begin, end;
	begin = m_tFindFileWorkMap.begin();	end = m_tFindFileWorkMap.end();
	for(begin; begin != end; begin++)
	{
		PFIND_FILE_WORK pSFFW = GetFindFileWork(begin->first);
		if(!pSFFW)
			continue;

		tMutexExt = (CMutexExt*)pSFFW->tMutexExt;
		if(tMutexExt == NULL)
			continue;

		tMutexExt->Lock();
		{
			pSFFW->tFFIList.clear();
			pSFFW->nContinue = 0;
		}
		tMutexExt->UnLock();
	}

	{
		ClearFindSubDirItem();		WriteLog("clear find sub dir items..");
		ClearFindFileDirItem();		WriteLog("clear find find dir items..");
	}

	WriteLog("stop find file work all..");

	return 0;
}
//--------------------------------------------------------------

INT32		CFindFileUtil::Stop(UINT32 nOrder)
{
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrder);
	if(!pSFFW)
	{
		return 0;
	}

	tMutexExt = (CMutexExt*)pSFFW->tMutexExt;
	if(tMutexExt == NULL)
	{
		return 0;
	}

	tMutexExt->Lock();
	{
		pSFFW->tFFIList.clear();
		pSFFW->nContinue = 0;
	}
	tMutexExt->UnLock();

	{
		ClearFindSubDirItem(nOrder);		WriteLog("clear find sub dir items.. [id:%d]", nOrder);
		ClearFindFileDirItem(nOrder);		WriteLog("clear find find dir items.. [id:%d]", nOrder);
	}

	WriteLog("stop find file work [id:%u]..", nOrder);

	return 0;
}

//--------------------------------------------------------------

INT32		CFindFileUtil::StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime)
{
	INT32 nLWaitTime = 0;
	INT32 nStatus = 0;

	if(tThreadObject == NULL)
		return 0;
	if(m_SemExt.CreateEvent() == FALSE)
		return -1;
	
	nLWaitTime = nWaitTime * 100;

	while(m_SemExt.WaitForSingleObject(100) == WAIT_TIMEOUT)
	{
		if(tThreadObject->GetContinue() == 0)
			break;
		if(!nLWaitTime)
			break;

		--nLWaitTime;
	}
	m_SemExt.CloseHandle(); 
	if(!nLWaitTime)
	{
		WriteLog("thread terminate fail : over wait time [%d]", nWaitTime);
		return -10;
	}
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::SetPreSearchLevel(UINT32 nLevel)
{
	m_nPreSearchLevel = nLevel;
	return 0;
}
//--------------------------------------------------------------------

UINT32		CFindFileUtil::GetPreSearchLevel(String strSearchPath)
{
	if(m_nPreSearchLevel)	return m_nPreSearchLevel;

	if(strSearchPath.empty())
		return 3;

	CTokenString Token((BYTE *)strSearchPath.c_str(), strSearchPath.length(), '/');
	INT32	nRtn = Token.GetRemainTokenCount();

	if(UINT32(nRtn) > m_tAFII.nAutoSearchDirLevel)
		nRtn = 0;
	else
		nRtn = m_tAFII.nAutoSearchDirLevel - nRtn;

	WriteLog("auto pre-search level is : [%d/level:%d][%s]", nRtn, m_tAFII.nAutoSearchDirLevel, strSearchPath.c_str());
	return nRtn;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddSearchDirPath(UINT32 nOrderID, LPCTSTR lpSearchRoot)
{
	m_nFindFileCount = 0;
	m_nCheckFileCount = 0;
	TMapIDListStrItor find = m_tSearchListMap.find(nOrderID);
	if(find == m_tSearchListMap.end())
	{
		TListStr tDirList;
		m_tSearchListMap[nOrderID] = tDirList;
		find = m_tSearchListMap.find(nOrderID);
	}
	WriteLog("add search dir path : [id:%d][%s]", nOrderID, lpSearchRoot);
	find->second.push_back(lpSearchRoot);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearSearchDirPath(UINT32 nOrderID)
{
	m_tSearchListMap.erase(nOrderID);
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddFileFindOption(UINT32 nOrderID, UINT32 nFindOption)
{
	m_tFindOptionMap[nOrderID] = nFindOption;
	WriteLog("add file find option : [id:%d][ot:%d]", nOrderID, nFindOption);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearFileFindOption(UINT32 nOrderID)
{
	m_tFindOptionMap.erase(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

UINT32		CFindFileUtil::GetFileFindOption(UINT32 nOrderID)
{
	TMapIDItor find = m_tFindOptionMap.find(nOrderID);
	if(find == m_tFindOptionMap.end())	return 0;

	return find->second;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::SearchDirFile(UINT32 nOrderID)
{
//	String strSR;
	INT32 nLen = 0;
	TListStrItor begin, end;
	TMapIDListStrItor find = m_tSearchListMap.find(nOrderID);
	if(find == m_tSearchListMap.end())	return -1;

	if(InitFindFileWork(nOrderID))
	{
		return -1;
	}

	SetFindFileWork_SearchPath(nOrderID, 0, find->second.size());

	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
//		strSR = _strlwr(begin->c_str());
//		strSR = begin->c_str();
		nLen = begin->length();
		if(nLen < 1)
			continue;

		PreSearchDir(nOrderID, *begin);	
		SetFindFileWork_SearchPath(nOrderID, 1, 1);
	}
	return 0;	
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::SearchDirFileThread(UINT32 nOrderID)
{
//	String strSR;
	INT32 nLen = 0;
	TListStrItor begin, end;
	TMapIDListStrItor find = m_tSearchListMap.find(nOrderID);
	if(find == m_tSearchListMap.end())	return -1;

	if(InitFindFileWork(nOrderID))
	{
		return -11;
	}

	SetFindFileWork_SearchPath(nOrderID, 0, find->second.size());
	
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
//		strSR = _strlwr(begin->c_str());
//		strSR = begin->c_str();
		nLen = (INT32)begin->length();
		if(nLen < 1)
		{
			continue;
		}

		if(PreSearchDirThread(nOrderID, *begin))
		{
			SetFindFileWork_SearchPath(nOrderID, 1, 1);
			continue;
		}
		SetFindFileWork_SearchPath(nOrderID, 1, 1);
	}	
	return 0;	
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::PreSearchDir(UINT32 nOrderID, String strRootPath)
{	
	CHAR szFindDir[MAX_PATH] = {0, };
	UINT32 nDirNum = 0;
	INT32 nSubDirSearch = 0;
	int nLen;
	TListFindFileItem tAFFIList;
	TListStr tNameList, tLastNameList;
	nLen = strRootPath.length();
	
	if(strRootPath.length() < 1)
		return -1;
	
	strncpy(szFindDir, strRootPath.c_str(), MAX_PATH-1);
	szFindDir[MAX_PATH-1] = 0;

	if(szFindDir[nLen - 1] == '*')
	{
		nSubDirSearch = 2;
		szFindDir[nLen - 2] = 0;
	}
	Recursive_SearchDirFile(nOrderID, szFindDir, "", nSubDirSearch, nDirNum, tAFFIList);
	if(tAFFIList.size())
	{
		AddFindFileItemList(nOrderID, 0, tAFFIList);
	}
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::PreSearchDirThread(UINT32 nOrderID, String strRootPath)
{	
	CHAR szFindDir[MAX_PATH] = {0, };
	INT32 nSubDirSearch = 0;
	INT32 nLen = 0;
	INT32 nPreSubDirSearchLevel = 0;
	TListStr tNameList, tLastNameList;
	TListFindFileItem tFindFileItemList;

	nLen = strRootPath.length();
	if(nLen < 1)
	{
		WriteLog("[Error] [PreSearchDirThread] invalid root path");
		return -1;
	}

	strncpy(szFindDir, strRootPath.c_str(), MAX_PATH-1);
	if(szFindDir[nLen - 1] == '*')
	{
		nPreSubDirSearchLevel = GetPreSearchLevel(strRootPath);
		nSubDirSearch = 1;
		szFindDir[nLen - 1] = 0;
		if(nLen != 2)
			szFindDir[nLen - 2] = 0;
	}

	nLen = (INT32)strlen(szFindDir);
	if(nLen > 1)
	{
		if(DirectoryExists(szFindDir) == FALSE)
		{
			WriteLog("[Error] [PreSearchDirThread] fail to find %s (%d)", szFindDir, errno);
			return -3;
		}
	}

	Recursive_SearchDir(nOrderID, szFindDir, "", nPreSubDirSearchLevel, tNameList, &tLastNameList, &tFindFileItemList);
	WriteLog("pre search dir : [%s] total (%d) sub (%d) file (%d)", szFindDir, tNameList.size(), tLastNameList.size(), tFindFileItemList.size());
	SetFindFileWork_TotalDir(nOrderID, tNameList.size());
	AddFindSubDirItem(nOrderID, nSubDirSearch, tLastNameList);
	AddFindFileItemList(nOrderID, tNameList.size(), tFindFileItemList);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::Recursive_SearchDir(UINT32 nOrderID, char *pcRootPath, char *pcSubDir, INT32 nSubDirSearch, TListStr& tNameList, TListStr* tLastNameList, TListFindFileItem* tFindFileItemList, PUINT32 pnContinue)
{	
	INT32 nExceptAllPath = (GetFileFindOption(nOrderID) & ASI_FF_FIND_OPTION_EXCLUDE_ALL_PATH);
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	UINT32 nMatchType = ASI_FF_FILE_FIND_TYPE_PATTERN;
	UINT32 dwPathLen = 0;
	UINT32 dwFileLen = 0;
	BOOL nContinue = TRUE;
	FIND_FILE_ITEM tAFFI;	
	char *pcFindDirA = NULL;
	char *pcChkFileA = NULL;
	char *pcSubAddPathA = NULL;
	INT32 nRetVal = 0;

	memset(&tAFFI, 0, sizeof(tAFFI));

	if (pcRootPath == NULL || pcRootPath[0] == 0)
	{
		WriteLog( "[Error] [Recursive_SearchDir] invalid root path");
		return -1;
	}

	dwPathLen = (UINT32)strlen(pcRootPath);
	if(dwPathLen == 0)
	{
		WriteLog( "[Error] [Recursive_SearchDir] invalid root path");
		return -2;
	}

	pcFindDirA = (char *)malloc(CHAR_MAX_SIZE);
	if(pcFindDirA == NULL)
	{
		WriteLog( "[Error] [Recursive_SearchDir] fail to allocate memory (%d)", errno);
		return -3;
	}
	memset(pcFindDirA, 0, CHAR_MAX_SIZE);

	do{
		pcChkFileA = (char *)malloc(MAX_PATH);
		if(pcChkFileA == NULL)
		{
			WriteLog("[Error] [Recursive_SearchDir] fail to allocate memory (%d)", errno);
			nRetVal = -4;
			break;
		}
		memset(pcChkFileA, 0, MAX_PATH);

		pcSubAddPathA = (char *)malloc(CHAR_MAX_SIZE);
		if(pcSubAddPathA == NULL)
		{
			WriteLog("[Error] [Recursive_SearchDir] fail to allocate memory (%d)", errno);
			nRetVal = -5;
			break;
		}
		memset(pcSubAddPathA, 0, CHAR_MAX_SIZE);

		if(pcSubDir != NULL && pcSubDir[0] != 0)
		{
			if(dwPathLen == 1 && pcRootPath[0] == '/')
			{
				snprintf(pcFindDirA, CHAR_MAX_SIZE-1, "%s%s", pcRootPath, pcSubDir);
			}
			else
			{
				snprintf(pcFindDirA, CHAR_MAX_SIZE-1, "%s/%s", pcRootPath, pcSubDir);
			}
		}
		else
		{
			snprintf(pcFindDirA, CHAR_MAX_SIZE-1, "%s", pcRootPath);
		}
		pcFindDirA[CHAR_MAX_SIZE-1] = 0;

		if(!_stricmp(pcFindDirA, "/sys"))
		{
			WriteLog("skip path : [%s]", pcFindDirA);
			nRetVal = 0;
			break;
		}

		if (nExceptAllPath)
		{
			if(IsExistExceptDir(nOrderID, pcFindDirA))
			{
				nSubDirSearch = 1;
				tNameList.push_back(pcFindDirA);
			}
		}
		else
		{
			if(IsExistExceptDir(nOrderID, pcFindDirA))
			{
				WriteLog("exist except dir mask : [%s]", pcFindDirA);
				nRetVal = 0;
				break;
			}
			if(!nSubDirSearch)
			{
				if(tLastNameList)
					(*tLastNameList).push_back(pcFindDirA);
				else
				{
					tNameList.push_back(pcFindDirA);
				}
				nRetVal = 0;
				break;
			}
			tNameList.push_back(pcFindDirA);
		}

		pDir = opendir(pcFindDirA);
		if (pDir == NULL)
		{
			WriteLog("[Error] [Recursive_SearchDir] fail to open %s (%d)", pcFindDirA, errno);
			nRetVal = -6;
			break;
		}

		while((pnContinue ? *pnContinue : 1) && (pDirEnt = readdir(pDir)) != NULL && nContinue)
		{			
			if(!_stricmp(pDirEnt->d_name, ".") || !_stricmp(pDirEnt->d_name, ".."))
			{
				continue;
			}
			memset(pcSubAddPathA, 0, CHAR_MAX_SIZE);
			memset(pcChkFileA, 0, MAX_PATH);
			strncpy(pcChkFileA, (char *)pDirEnt->d_name, MAX_PATH-1);
			pcChkFileA[MAX_PATH-1] = 0;
			if(DT_DIR == pDirEnt->d_type)
			{
				if(nSubDirSearch)
				{
					dwPathLen = (INT32)strlen(pcChkFileA);
					if(dwPathLen > MAX_FILE_NAME)
					{
						WriteLog("skip %s/%s", pcFindDirA, pcChkFileA);
						continue;
					}

					if(pcSubDir != NULL && pcSubDir[0] != 0)
						snprintf(pcSubAddPathA, CHAR_MAX_SIZE-1, "%s/%s", pcSubDir, pcChkFileA);
					else
						snprintf(pcSubAddPathA, CHAR_MAX_SIZE-1, "%s", pcChkFileA);
					dwPathLen = (INT32)strlen(pcSubAddPathA);
					if(dwPathLen > CHAR_MAX_SIZE-MAX_FILE_NAME)
					{
						WriteLog("skip %s/%s", pcFindDirA, pcChkFileA);
						continue;
					}

					Recursive_SearchDir(nOrderID, pcRootPath, pcSubAddPathA, (nSubDirSearch - 1), tNameList, tLastNameList, tFindFileItemList, pnContinue);
				}
			}
			else if(DT_REG == pDirEnt->d_type)
			{
				if(tFindFileItemList)
				{
					if(!nExceptAllPath && IsExistExceptDirFileMask(nOrderID, pcFindDirA, pcChkFileA) || (nExceptAllPath && !IsExistExceptDir(nOrderID, pcFindDirA)))
					{
						continue;
					}
					else if(IsExistFileMask(nOrderID, pcFindDirA, pcChkFileA, nMatchType) && IsExistFileDateTime(nOrderID, pcFindDirA, pcChkFileA))
					{
						tAFFI.strFilePath	= pcFindDirA;
						tAFFI.strFileName	= pcChkFileA;
						tAFFI.nFindType		= nMatchType;

						dwPathLen = (UINT32)strlen(pcFindDirA);
						dwFileLen = (UINT32)strlen(pcChkFileA);
						if(dwPathLen + dwFileLen > CHAR_MAX_SIZE-1)
						{
							WriteLog("skip %s/%s", pcFindDirA,pcChkFileA);
							continue;
						}
						if(dwPathLen == 1 && pcFindDirA[0] == '/')
						{
							snprintf(pcSubAddPathA,CHAR_MAX_SIZE-1, "%s%s", pcFindDirA, pcChkFileA);
						}
						else
						{
							snprintf(pcSubAddPathA,CHAR_MAX_SIZE-1, "%s/%s", pcFindDirA, pcChkFileA);
						}
						dwFileLen = 0;
						get_file_size(pcSubAddPathA, &dwFileLen);
						tAFFI.nFileSize		= dwFileLen;
						tFindFileItemList->push_back(tAFFI);
						WriteLog("find to file %s (%d) (%d) (%d)", pcSubAddPathA, nMatchType, tFindFileItemList->size(), (INT32)gettid());

						if(tFindFileItemList->size() == 10)
						{
							AddFindFileItemList(nOrderID, 0, *tFindFileItemList); 
							tFindFileItemList->clear();
						}					
					}
				}
			}
		}
		nRetVal = 0;
	}while(FALSE);
	if(pDir != NULL)
	{
		closedir(pDir);
		pDir = NULL;
	}
	safe_free(pcFindDirA);
	safe_free(pcChkFileA);
	safe_free(pcSubAddPathA);
	return nRetVal;
}


INT32		CFindFileUtil::Recursive_SearchFile(UINT32 nOrderID, String strSearchPath, TListFindFileItem& tFindFileItemList, PUINT32 pnContinue)
{
	String strDirA;	
	String strFileNameA;
	String strChkDirA = strSearchPath;
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	BOOL nContinue = TRUE;
	UINT32 nMatchType = 0;
	UINT32 dwFileLen = 0;
	FIND_FILE_ITEM tAFFI;

	memset(&tAFFI, 0, sizeof(tAFFI));
	pDir = opendir(strChkDirA.c_str());
	if (pDir == NULL)
	{
		WriteLog( "[Error] [Recursive_SearchFile] fail to open %s (%d)", strChkDirA.c_str(), errno);
		return 0 ;
	}

	while((pnContinue ? *pnContinue : 1) && (pDirEnt = readdir(pDir)) != NULL && nContinue)
	{
		if(!_stricmp(pDirEnt->d_name, ".") || !_stricmp(pDirEnt->d_name, ".."))
		{
			continue;
		}
		strFileNameA = pDirEnt->d_name;

		nMatchType = ASI_FF_FILE_FIND_TYPE_PATTERN;
		if(DT_DIR == pDirEnt->d_type)
		{
			continue;
		}
		else if(DT_REG == pDirEnt->d_type)
		{
			if(IsExistExceptDirFileMask(nOrderID, strChkDirA, strFileNameA))
			{
				continue;
			}
			else if(!IsExistFileMask(nOrderID, strChkDirA, strFileNameA, nMatchType))
			{
				continue;
			}
			else if(!IsExistFileDateTime(nOrderID, strChkDirA, strFileNameA))
			{
				continue;
			}
			else
			{	
				strDirA = strChkDirA + "/" + strFileNameA;
				get_file_size(strDirA.c_str(), &dwFileLen);
				tAFFI.nFileSize		= dwFileLen;
				tAFFI.strFilePath	= strChkDirA;
				tAFFI.strFileName	= strFileNameA;
				tAFFI.nFindType		= nMatchType;
				tFindFileItemList.push_back(tAFFI);
				if(m_tAFII.nOnceFindFileNum == tFindFileItemList.size())
				{
					AddFindFileItemList(nOrderID, 0, tFindFileItemList);
					tFindFileItemList.clear(); 
				}
			}
		}
	}

	closedir(pDir);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::Recursive_SearchDirFile(UINT32 nOrderID, String strSearchPath, String strSubDir, INT32 nSubDirSearch, UINT32& nDirNum, TListFindFileItem& tFindFileItemList)
{
	String strFindDirA;
	String strChkDirA;
	String strFileNameA;
	String strPathA;
	UINT32 nMatchType = 0;
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	BOOL nContinue = TRUE;
	UINT32 dwFileLen = 0;
	FIND_FILE_ITEM tAFFI;
	memset(&tAFFI, 0, sizeof(tAFFI));
	if(strSubDir.empty() == FALSE)
	{
		strFindDirA = SPrintf("%s/%s", strSearchPath.c_str(), strSubDir.c_str());
	}
	else
	{
		strFindDirA = strSearchPath;
	}
	
	strChkDirA = strFindDirA;
	
	if(IsExistExceptDir(nOrderID, strChkDirA.c_str()))
	{
		WriteLog("except dir : [%d][%s]", nOrderID, strChkDirA.c_str());
		return 0;
	}
	nDirNum++;

	pDir = opendir(strChkDirA.c_str());
	if (pDir == NULL)
	{
		WriteLog("[Error] [Recursive_SearchDirFile] fail to open %s (%d)", strChkDirA.c_str(), errno);
		return 0 ;
	}	

	while((pDirEnt = readdir(pDir)) != NULL && nContinue)
	{			
		if(_stricmp(pDirEnt->d_name, ".") && _stricmp(pDirEnt->d_name, ".."))
		{
			strFileNameA = pDirEnt->d_name;
			nMatchType = ASI_FF_FILE_FIND_TYPE_PATTERN;
			
			if(DT_DIR == pDirEnt->d_type)
			{
				dwFileLen = (UINT32)strFileNameA.length();
				if(dwFileLen > MAX_FILE_NAME)
				{
					WriteLog("skip %s/%s", strChkDirA.c_str(), strFileNameA.c_str());
					continue;
				}
				if(nSubDirSearch)
				{
					strPathA = (strSubDir.empty() ? strFileNameA : strSubDir + "/" + strFileNameA);
					dwFileLen = (UINT32)strPathA.length();
					if(dwFileLen > CHAR_MAX_SIZE-MAX_FILE_NAME)
					{
						WriteLog("skip %s/%s", strChkDirA.c_str(), strFileNameA.c_str());
						continue;
					}
					Recursive_SearchDirFile(nOrderID, strSearchPath, strPathA, nSubDirSearch, nDirNum, tFindFileItemList);	
				}
			}
			else if(DT_REG == pDirEnt->d_type)
			{
				if(IsExistExceptDirFileMask(nOrderID, strChkDirA, strFileNameA))
				{
					continue;
				}
				else if(!IsExistFileMask(nOrderID, strChkDirA, strFileNameA, nMatchType) || !IsExistFileDateTime(nOrderID, strChkDirA, strFileNameA))
				{
					continue;
				}
				else
				{
					strPathA = strChkDirA + "/" + strFileNameA;

					dwFileLen = (UINT32)strPathA.length();
					if(dwFileLen + dwFileLen > CHAR_MAX_SIZE-1)
					{
						WriteLog("skip %s", strPathA.c_str());
						continue;
					}

					dwFileLen = 0;
					get_file_size(strPathA.c_str(), &dwFileLen);
					tAFFI.nFileSize		= dwFileLen;
					tAFFI.strFilePath	= strChkDirA;
					tAFFI.strFileName	= strFileNameA;
					tAFFI.nFindType		= nMatchType;
					tFindFileItemList.push_back(tAFFI);
					WriteLog( "find to dir file %s/%s (%d) (%d)", strChkDirA.c_str(), strFileNameA.c_str(), nMatchType, tFindFileItemList.size());
				}
			}
		}
	}
	
	closedir(pDir);
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddFileMask(UINT32 nOrderID, LPCTSTR lpFileMask)
{
	TMapStr tKeyMap;
	CHAR szFileExt[MAX_PATH] = {0, };
	INT32 nFileLen = 0;
	if(!lpFileMask || lpFileMask[0] == 0)
		return -1;
	
	strncpy(szFileExt, lpFileMask, MAX_PATH-3);
	szFileExt[MAX_PATH-3] = 0;
//	_strlwr(szFileExt);
	nFileLen = (INT32)strlen(szFileExt);
	if(nFileLen < 1)
		return -2;

	if(szFileExt[0] != '*')
	{
		ReverseLPTSTR(szFileExt);
		strcat(szFileExt, ".");
		strcat(szFileExt, "*");
		szFileExt[MAX_PATH-1] = 0;
		ReverseLPTSTR(szFileExt);
	}

	TMapIDMapStrItor find = m_tFileMaskMap.find(nOrderID);
	if(find == m_tFileMaskMap.end())
	{
		m_tFileMaskMap[nOrderID] = tKeyMap;
		find = m_tFileMaskMap.find(nOrderID);
	}

	CTokenString Token(lpFileMask, nFileLen, '.', 1);
	String strDFFmt = Token.NextToken();

	find->second[szFileExt] = strDFFmt;
	WriteLog("add file mask filter : [id:%d][%s]:[%s]", nOrderID, szFileExt, strDFFmt.c_str());
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::DelFileMask(UINT32 nOrderID, LPCTSTR lpFileMask)
{
	CHAR szFileExt[MAX_PATH] = {0, };
	INT32 nFileLen = 0;

	if(!lpFileMask || lpFileMask[0] == 0)
		return -1;

	strncpy(szFileExt, lpFileMask, MAX_PATH-3);
	szFileExt[MAX_PATH-3] = 0;
	_strlwr(szFileExt);
	
	nFileLen = (INT32)strlen(szFileExt);
	if(nFileLen < 1)
		return -2;

	if(szFileExt[0] != '*')
	{
		ReverseLPTSTR(szFileExt);
		strcat(szFileExt, ".");
		strcat(szFileExt, "*");
		szFileExt[MAX_PATH-1] = 0;
		ReverseLPTSTR(szFileExt);
	}

	TMapIDMapStrItor find = m_tFileMaskMap.find(nOrderID);
	if(find == m_tFileMaskMap.end())
	{
		return 0;
	}

	TMapStrItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; )
	{
		if(strcmp(begin->first.c_str(), szFileExt))
		{
			begin++;
		}
		else
		{
			find->second.erase(begin);
			break;
		}
	}
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearFileMask(UINT32 nOrderID)
{
	TMapIDMapStrItor find = m_tFileMaskMap.find(nOrderID);
	if(find == m_tFileMaskMap.end())
	{
		return 0;
	}

	find->second.clear();
	m_tFileMaskMap.erase(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::IsExistFileMask(UINT32 nOrderID, String strFilePath, String strFileName, UINT32& nMatchType, INT32 nEmptyOption)
{
	String strFileNameA;
	String strFileFullPathA;
	UINT32 nOption = 0;
	TMapIDMapStrItor find = m_tFileMaskMap.find(nOrderID);
	if(find == m_tFileMaskMap.end())
	{
		if(nEmptyOption == 0)
			return 0;
		else
			return 1;
	}

	if(find->second.empty())
	{
		if(nEmptyOption == 0)
			return 0;
		else
			return 1;
	}
	if(strFilePath.length() == 0 || strFileName.length() == 0)
	{
		return 0;
	}

	strFileNameA = strFileName;

	if(strFilePath.length() == 1)	
	{
		strFileFullPathA = SPrintf("%s%s", strFilePath.c_str(), strFileNameA.c_str());
	}
	else
	{
		strFileFullPathA = SPrintf("%s/%s", strFilePath.c_str(), strFileNameA.c_str());
	}
	nOption = GetFileFindOption(nOrderID);
	if(nOption & ASI_FF_FIND_OPTION_USED_DOC_FILE_FORMAT)
	{
		if(IsExistFileMaskByDFF(nOrderID, strFileFullPathA))
		{
			nMatchType = ASI_FF_FILE_FIND_TYPE_DOC_FILE_FORMAT;
			return 1;
		}
	}
	else
	{
		TMapStrItor begin, end;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end; begin++)
		{
			if(StringMatchSpec(strFileNameA.c_str(), (begin->first.c_str())))
			{
				WriteLog("match ext string (id:%d) (%s:%s)", nOrderID, strFileFullPathA.c_str(), begin->first.c_str());
				nMatchType = ASI_FF_FILE_FIND_TYPE_PATTERN;
				return 1;
			}
		}
	}
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

BOOL		CFindFileUtil::IsExistFileMaskByDFF(UINT32 nOrderID, String strFileFullName)
{
	ASI_DFILE_FMT_INFO tADFI;
	INT32 nRetVal = 0;
	char acLogMsg[MAX_LOGMSG+1] = {0,};
	if(m_tASIFIDLLUtil->ASIFI_GetFileElfMagic((char *)strFileFullName.c_str()))	
	{
		return FALSE;
	}

	strncpy(tADFI.szFileName, strFileFullName.c_str(), CHAR_MAX_SIZE-1);
	tADFI.szFileName[CHAR_MAX_SIZE-1] = '\0';

	nRetVal = m_tASIDFFDLLUtil->ASIDFF_GetDFFmtInfo(&tADFI, acLogMsg);
	if(nRetVal != 0)
	{
		WriteLog("[IsExistFileMaskByDFF] fail to find file format (%d) : %s", nRetVal, acLogMsg);
		return FALSE;
	}
	if(!_strnicmp(acLogMsg, "more check", 10))
	{
		WriteLog("%s", acLogMsg);
	}

	if(tADFI.nFmtType == ASIDFF_FILE_FMT_TYPE_UNKNOWN || tADFI.nFmtType == ASIDFF_FILE_FMT_TYPE_ZIP)
	{
		return FALSE;
	}
	if(tADFI.szFmtType[0] == 0)
	{
		WriteLog("[IsExistFileMaskByDFF] fail to get fmt type %s", nRetVal, tADFI.szFileName);
		return FALSE;
	}

	TMapIDMapStrItor find = m_tFileMaskMap.find(nOrderID);
	if(find == m_tFileMaskMap.end())
	{
		WriteLog("[IsExistFileMaskByDFF] (id: %d) invalid file mask", nOrderID);
		return FALSE;
	}

	if(find->second.empty())
	{
		WriteLog("[IsExistFileMaskByDFF] (id: %d) invalid file mask", nOrderID);
		return FALSE;
	}

	TMapStrItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		if(!_stricmp(begin->second.c_str(), tADFI.szFmtType))
		{
			WriteLog("match file format (id:%d) (%s:%s)", nOrderID, strFileFullName.c_str(), tADFI.szFmtType);
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddExceptDir(UINT32 nOrderID, LPCTSTR lpDirPath)
{
	String strFileMask;
	TMapStrID tKeyMap;
	INT32 nIncludeSubPath = 0;
	INT32 nLen = 0;
	char *pExt = NULL;
	CHAR szDirPath[MAX_PATH] = {0, };
	CHAR acCompFileName[MAX_FILE_NAME] = {0, };
	if(!lpDirPath || lpDirPath[0] == 0)
		return -1;

	nLen = strlen(lpDirPath);
	if(nLen < 3)
		return -2;

	strncpy(szDirPath, lpDirPath, MAX_PATH-1);
	szDirPath[MAX_PATH-1] = 0;

	if(get_basename(szDirPath, acCompFileName, MAX_FILE_NAME-1) != NULL)
	{
		if(acCompFileName[0] != '.')
		{
			pExt = strrchr(acCompFileName, '.');
			if(pExt != NULL)
				strFileMask = SPrintf("%s", pExt);
		}
	}

//	_strlwr(szDirPath);
	if(!strFileMask.empty())
	{
		return AddExceptDirFileMask(nOrderID, lpDirPath);
	}
	else if(szDirPath[nLen - 1] == '*')
	{
		szDirPath[nLen - 2] = 0;
		nIncludeSubPath = 1;
	}

	TMapIDMapStrIDItor find = m_tExceptDirMap.find(nOrderID);
	if(find == m_tExceptDirMap.end())
	{
		m_tExceptDirMap[nOrderID] = tKeyMap;
		find = m_tExceptDirMap.find(nOrderID);
	}

	find->second[szDirPath] = nIncludeSubPath;
	WriteLog("add file exclude dir filter : [id:%d][%s]", nOrderID, lpDirPath);

	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::DelExceptDir(UINT32 nOrderID, LPCTSTR lpDirPath)
{
	String strFileMask;
	INT32 nIncludeSubPath = 0;
	INT32 nLen = 0;
	char *pExt = NULL;
	CHAR szDirPath[MAX_PATH] = {0, };
	if(!lpDirPath || lpDirPath[0] == 0)
		return -1;

	nLen = strlen(lpDirPath);

	if(nLen < 3)
		return -2;

	strncpy(szDirPath, lpDirPath, MAX_PATH-1);
	szDirPath[MAX_PATH-1] = 0;
//	_strlwr(szDirPath);
	
	pExt = strrchr(szDirPath, '.');
	if(pExt != NULL)
		strFileMask = SPrintf("%s", pExt);

	if(!strFileMask.empty())
	{
		return DelExceptDirFileMask(nOrderID, lpDirPath);
	}
	if(szDirPath[nLen - 1] == '*')
	{
		szDirPath[nLen - 2] = 0;
	}

	TMapIDMapStrIDItor find = m_tExceptDirMap.find(nOrderID);
	if(find == m_tExceptDirMap.end())
	{
		return 0;
	}

	find->second.erase(szDirPath);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearExceptDir(UINT32 nOrderID)
{
	TMapIDMapStrIDItor find = m_tExceptDirMap.find(nOrderID);
	if(find == m_tExceptDirMap.end())
	{
		return 0;
	}

	find->second.clear();	
	m_tExceptDirMap.erase(nOrderID);

	ClearExceptDirFileMask(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::IsExistExceptDir(UINT32 nOrderID, LPCTSTR lpDirPath)
{
	TMapIDMapStrIDItor find = m_tExceptDirMap.find(nOrderID);
	if(find == m_tExceptDirMap.end())
	{
		return 0;
	}

	TMapStrIDItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		if(StringMatchSpec(lpDirPath, begin->first.c_str()))
		{
			return 1;			
		}
	}
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddExceptDirFileMask(UINT32 nOrderID, LPCTSTR lpDirPathFileMask)
{
	String strFileMask;
	INT32 nIncludeSubPath = 0;
	INT32 nLen = 0;
	char *pExt = NULL;
	TMapIDMapStrItor find;
	TMapStr tKeyMap;
	CHAR szDirPath[MAX_PATH] = {0, };
	if(!lpDirPathFileMask || lpDirPathFileMask[0] == 0)
		return -1;

	nLen = strlen(lpDirPathFileMask);
	if(nLen < 3)
		return -2;

	strncpy(szDirPath, lpDirPathFileMask, MAX_PATH-1);
	szDirPath[MAX_PATH-1] = 0;
//	_strlwr(szDirPath);

	pExt = strrchr(szDirPath, '.');
	if(pExt != NULL)
		strFileMask = SPrintf("%s", pExt);

	if(!strFileMask.empty())
	{
		szDirPath[nLen - strFileMask.length() - 1] = 0;
		strFileMask = SPrintf("%s", &pExt[1]);
	}

	find = m_tExceptDirFileMaskMap.find(nOrderID);
	if(find == m_tExceptDirFileMaskMap.end())
	{
		m_tExceptDirFileMaskMap[nOrderID] = tKeyMap;
		find = m_tExceptDirFileMaskMap.find(nOrderID);
	}

	find->second[szDirPath] = strFileMask;
	WriteLog("add file exclude dir : [id:%d][%s]", nOrderID, lpDirPathFileMask);

	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::DelExceptDirFileMask(UINT32 nOrderID, LPCTSTR lpDirPathFileMask)
{
	String strFileMask;
	INT32 nIncludeSubPath = 0;
	INT32 nLen = 0;
	char *pExt = NULL;
	CHAR szDirPath[MAX_PATH] = {0, };
	if(!lpDirPathFileMask || lpDirPathFileMask[0] == 0)
		return -1;

	nLen = strlen(lpDirPathFileMask);
	if(nLen < 3)
		return -2;

	strncpy(szDirPath, lpDirPathFileMask, MAX_PATH-1);
//	_strlwr(szDirPath);
	pExt = strrchr(szDirPath, '.');
	if(pExt != NULL)
		strFileMask = SPrintf("%s", pExt);
	if(!strFileMask.empty())
	{
		szDirPath[nLen - strFileMask.length() - 1] = 0;
	}

	TMapIDMapStrItor find = m_tExceptDirFileMaskMap.find(nOrderID);
	if(find == m_tExceptDirFileMaskMap.end())
	{
		return 0;
	}

	find->second.erase(szDirPath);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearExceptDirFileMask(UINT32 nOrderID)
{
	TMapIDMapStrItor find = m_tExceptDirFileMaskMap.find(nOrderID);
	if(find == m_tExceptDirFileMaskMap.end())
	{
		return 0;
	}

	find->second.clear();	
	m_tExceptDirFileMaskMap.erase(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::IsExistExceptDirFileMask(UINT32 nOrderID, String strFilePath, String strFileName)
{
	TMapStrItor begin, end;
	String strFileMask;
	char *pExt = NULL;

	TMapIDMapStrItor find = m_tExceptDirFileMaskMap.find(nOrderID);
	if(find == m_tExceptDirFileMaskMap.end())
	{
		return 0;
	}

	if(find->second.empty())
	{
		return 1;
	}

	pExt = strrchr((char *)strFileName.c_str(), '.');
	if(pExt == NULL)
	{
		return 0;
	}

	strFileMask = SPrintf("%s", &pExt[1]);

	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		if(StringMatchSpec(strFilePath.c_str(), begin->first.c_str()) && StringMatchSpec(strFileMask.c_str(), begin->second.c_str()))
		{
			WriteLog("match : [%s : %s] [%s : %s]", strFilePath.c_str(), begin->first.c_str(), strFileMask.c_str(), begin->second.c_str());
			return 1;
		}
	}
	return 0;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::AddFileDateTime(UINT32 nOrderID, UINT32 nType, UINT32 nChkDT)
{
	TMapID tFDMap;
	TMapIDMapItor find = m_tFileDTMap.find(nOrderID);
	if(find == m_tFileDTMap.end())
	{
		m_tFileDTMap[nOrderID] = tFDMap;
		find = m_tFileDTMap.find(nOrderID);
	}

	find->second[nType] = nChkDT;
	WriteLog("add file date time filter : [id:%d][tp:%d][dt:%d]", nOrderID, nType, nChkDT);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::DelFileDateTime(UINT32 nOrderID, UINT32 nType)
{
	TMapIDMapItor find = m_tFileDTMap.find(nOrderID);
	if(find == m_tFileDTMap.end())
	{
		return 0;
	}

	find->second.erase(nType);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearFileDateTime(UINT32 nOrderID)
{
	TMapIDMapItor find = m_tFileDTMap.find(nOrderID);
	if(find == m_tFileDTMap.end())
	{
		return 0;
	}

	find->second.clear();	
	m_tFileDTMap.erase(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::IsExistFileDateTime(UINT32 nOrderID, String strFilePath, String strFileName)
{
	String strChkFileNameA;
	UINT32 nCurDT = 0;
	UINT32 nCurCDT = 0, nCurADT = 0, nCurMDT = 0;
//	UINT32 nExistInfo = 0;
	TMapIDItor begin, end;
	INT32 nRetVal = 0;

	pthread_mutex_lock(&m_mutex);
	do{
		TMapIDMapItor find = m_tFileDTMap.find(nOrderID);
		if(find == m_tFileDTMap.end())
		{
			nRetVal = 1;
			break;
		}

		if(find->second.empty())
		{
			nRetVal = 1;
			break;
		}

		if(strFilePath.length() == 0)
		{
			nRetVal = 0;
			break;
		}
		
		if(strFileName.length() != 0)
		{
			if(strFilePath.length() == 1)
				strChkFileNameA = SPrintf("%s%s", strFilePath.c_str(), strFileName.c_str());
			else
				strChkFileNameA = SPrintf("%s/%s", strFilePath.c_str(), strFileName.c_str());
		}
		else
			strChkFileNameA = strFilePath;

		nCurDT = GetCurrentDateTimeInt();

		if(GetFileTimeInfo(strChkFileNameA.c_str(), &nCurCDT, &nCurMDT, &nCurADT) != 0)
		{
			nRetVal = 0;
			break;
		}

		nRetVal = 0;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end && !nRetVal; begin++)
		{
			switch(begin->first)
			{
				case ASI_FF_FILE_DT_CHK_TYPE_CREATE:
				case ASI_FF_FILE_DT_CHK_TYPE_WRITE:
				{
					if(begin->second == 0 || nCurMDT && (nCurMDT < (nCurDT - begin->second)))
					{
						nRetVal = 1;
					}
					break;
				}
				case ASI_FF_FILE_DT_CHK_TYPE_ACCESS:
				{
					if(begin->second == 0 || nCurADT && (nCurADT < (nCurDT - begin->second)))
					{
						nRetVal = 1;
					}
					break;
				}
			}
		}
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}


BOOL CFindFileUtil::FindExactWildString(char *pBaseName, PFILE_WILD_INFO pEntry)
{
	char *ptr = NULL;
	INT32 nBaseLen = 0;
	if(pEntry == NULL || pBaseName == NULL || pBaseName[0] == 0)
		return FALSE;

	if (pEntry->nWildType == FILE_WILD_ALL)
	{
		return TRUE;
	}
	else if (pEntry->nWildType == FILE_WILD_PRE)
	{
		nBaseLen = (INT32)strlen(pBaseName);
		if(nBaseLen < pEntry->nWildLen)
		{
			return FALSE;
		}
		ptr = pBaseName + nBaseLen - pEntry->nWildLen;
		if (ptr >= pBaseName)
		{
			if (!_stricmp(ptr, pEntry->acBaseName))
				return TRUE;
		}
	}
	else if (pEntry->nWildType == FILE_WILD_POST)
	{
		if (!_strnicmp(pBaseName, pEntry->acBaseName, pEntry->nWildLen))
			return TRUE;
	}
	else
	{
		if (!_stricmp(pBaseName, pEntry->acBaseName))
			return TRUE;
	}
	return FALSE;
}


BOOL CFindFileUtil::SetWildString(char *pBaseName, PFILE_WILD_INFO pEntry)
{
	char *pStart = NULL;
	char *pLast = NULL;
	INT32 nLen = 0;

	if(pEntry == NULL || pBaseName == NULL || pBaseName[0] == 0)
		return FALSE;

	pLast = pBaseName + (INT32)strlen(pBaseName) - 1;
	pStart = pBaseName;

	if (pStart == pLast)
	{
		if (*pStart == '*')  /* * 형식 */
		{
			pEntry->nWildType = FILE_WILD_ALL;
			return TRUE;
		}
	}
	else if (*pStart == '*')  /* *.txt 형식 */
	{
		pEntry->nWildType = FILE_WILD_PRE;
		strncpy(pEntry->acBaseName, pStart + 1, MAX_FILE_NAME-1);
		pEntry->acBaseName[MAX_FILE_NAME-1] = 0;
		pEntry->nWildLen = (INT32)strlen(pEntry->acBaseName);
		return TRUE;
	}
	else if (*pLast == '*')  /* aa.*' 형식 */
	{
		pEntry->nWildType = FILE_WILD_POST;
		strncpy(pEntry->acBaseName, pStart, MAX_FILE_NAME-1);
		pEntry->acBaseName[MAX_FILE_NAME-1] = 0;
		nLen = (INT32)strlen(pEntry->acBaseName);
		if (pEntry->acBaseName[nLen - 1] == '*')
			pEntry->acBaseName[nLen - 1] = '\0';
		pEntry->nWildLen = (INT32)strlen(pEntry->acBaseName);
		return TRUE;
	}
	else
	{
		pEntry->nWildType = FILE_WILD_NONE;
		strncpy(pEntry->acBaseName, pStart, MAX_FILE_NAME-1);
		pEntry->acBaseName[MAX_FILE_NAME-1] = 0;
		pEntry->nWildLen = (INT32)strlen(pEntry->acBaseName);
		return TRUE;
	}
	return FALSE;
}



INT32		CFindFileUtil::StringMatchBaseName(LPCSTR pszTarget, LPCSTR pszSpec) 
{
	PFILE_WILD_INFO pWildInfo = NULL;
	INT32 nRetVal = 0;
	if(pszTarget == NULL || pszTarget[0] == 0 || pszSpec == NULL || pszSpec[0] == 0)
	{
		WriteLog("[StringMatchBaseName] invalid input data");
		return 0;
	}

	do{
		pWildInfo = (PFILE_WILD_INFO)malloc(sizeof(FILE_WILD_INFO));
		if(pWildInfo == NULL)
		{
			WriteLog("[StringMatchBaseName] fail to allocate memory (%d)", errno);
			nRetVal = 0;
			break;
		}
		memset(pWildInfo, 0, sizeof(FILE_WILD_INFO));

		if(SetWildString((char *)pszSpec, pWildInfo) == FALSE)
		{
			WriteLog("[StringMatchBaseName] fail to set wild string (%s)", pszSpec);
			nRetVal = 0;
			break;
		}


	}while(FALSE);
	safe_free(pWildInfo);
	return nRetVal;
}


//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CFindFileUtil::StringMatchSpec(LPCTSTR pszTarget, LPCTSTR pszSpec) 
{
	char *cp = NULL, *mp = NULL;
	INT32 nRetVal = 0;
	pthread_mutex_lock(&m_mutex);
	if(pszTarget == NULL || pszSpec == NULL)
	{
		pthread_mutex_unlock(&m_mutex);
		return 0;
	}
	while (*pszTarget)
	{
		if (*pszSpec == '*') 
		{
			if (!(*(++pszSpec))) 
			{
				pthread_mutex_unlock(&m_mutex);
				return 1;
			}
			mp = (char *)pszSpec;
			cp = (char *)(pszTarget+1);
		} 
//		else if (((TOLOWER(*pSpec) == TOLOWER(*pTarget)) && (*pSpec != '#')) || (*pSpec == '?') || ((*pSpec == '#') && isdigit(*pTarget))) 
		else if (((*pszSpec == *pszTarget) && (*pszSpec != '#')) || (*pszSpec == '?') || ((*pszSpec == '#') && isdigit(*pszTarget))) 
		{
			pszSpec++;
			pszTarget++;
		} 
		else 
		{
			if (mp)
			{
				pszSpec = (char *)mp;
				pszTarget = (char *)cp++;
			}
			else
			{
				pthread_mutex_unlock(&m_mutex);
				return 0;
			}
		}
	}

	while (*pszSpec == '*')
	{
		pszSpec++;
	}
	if(!(*pszSpec))
		nRetVal = 1;
	else
		nRetVal = 0;
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32		CFindFileUtil::AddFindSubDirItem(UINT32 nOrderID, UINT32 nSubSearch, TListStr& tNameList)
{
	TListStrItor begin, end;
	PFIND_FILE_WORK pAFFI = NULL;
	FIND_DIR_ITEM tFDI;
	memset(&tFDI, 0, sizeof(tFDI));
	m_tFindSubDirItemMutex.Lock();
	begin = tNameList.begin();	end = tNameList.end();
	for(begin; begin != end; begin++)
	{
		tFDI.nOrderID		= nOrderID;
		tFDI.strSearchDir	= *begin;
//		tFDI.strSearchDirW = ConvertWideString(tFDI.strSearchDir);
		tFDI.nSubSearch		= nSubSearch;
		m_tFindSubDirItemList.push_back(tFDI);
	}

	pAFFI = GetFindFileWork(nOrderID);
	if(pAFFI)
	{
		pAFFI->nDirSubSearchNum += tNameList.size();
	}

	m_tFindSubDirItemMutex.UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::GetFindSubDirItem(FIND_DIR_ITEM& tFDI)
{
	INT32 nRtn = 0;
	TListFindDirItemItor begin, end;
	m_tFindSubDirItemMutex.Lock();
	begin = m_tFindSubDirItemList.begin();		end = m_tFindSubDirItemList.end();
	if(begin != end)
	{
		tFDI = *begin;
		m_tFindSubDirItemList.pop_front();
		nRtn = 1;
	}
	m_tFindSubDirItemMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::ClearFindSubDirItem()
{
	m_tFindSubDirItemMutex.Lock();
	m_tFindSubDirItemList.clear();
	m_tFindSubDirItemMutex.UnLock();
	return 0;
}

INT32		CFindFileUtil::ClearFindSubDirItem(UINT32 nOrderID)
{
	INT32 nRtn = 0;
	m_tFindSubDirItemMutex.Lock();

	TListFindDirItemItor begin, end;
	begin = m_tFindSubDirItemList.begin();		end = m_tFindSubDirItemList.end();
	for(; begin != end;)
	{
		FIND_DIR_ITEM tFDI = *begin;
		if(nOrderID != tFDI.nOrderID)
		{
			begin++;
			continue;
		}

		m_tFindSubDirItemList.erase(begin++);
		nRtn = 1;
	}

	m_tFindSubDirItemMutex.UnLock();
	return nRtn;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32		CFindFileUtil::AddFindFileDirItem(UINT32 nOrderID, TListStr& tNameList, PUINT32 pnWorkNum)
{
	TListStrItor begin, end;
	FIND_DIR_ITEM tFDI;
	m_tFindFileDirItemMutex.Lock();
	begin = tNameList.begin();	end = tNameList.end();
	for(begin; begin != end; begin++)
	{
		tFDI.nOrderID		= nOrderID;
		tFDI.strSearchDir	= *begin;	
//		tFDI.strSearchDirW	= ConvertWideString(tFDI.strSearchDir);
		m_tFindFileDirItemList.push_back(tFDI);
	}
	if(pnWorkNum)	
		*pnWorkNum = m_tFindFileDirItemList.size();

	m_tFindFileDirItemMutex.UnLock();
	return 0;	
}
//-------------------------------------------------------------

INT32		CFindFileUtil::GetFindFileDirItem(FIND_DIR_ITEM& tFDI)
{
	INT32 nRtn = 0;
	TListFindDirItemItor begin, end;
	m_tFindFileDirItemMutex.Lock();
	begin = m_tFindFileDirItemList.begin();		end = m_tFindFileDirItemList.end();
	if(begin != end)
	{
		tFDI = *begin;
		m_tFindFileDirItemList.pop_front();
		nRtn = 1;
	}
	m_tFindFileDirItemMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::GetFindFileDirItem(TListFindDirItem& tSFDWList, UINT32& nOrderID, UINT32 nLimited)
{
	INT32 nRtn = 0;
	TListFindDirItemItor begin, end;

	m_tFindFileDirItemMutex.Lock();
	begin = m_tFindFileDirItemList.begin();		end = m_tFindFileDirItemList.end();
	for(begin; begin != end; )
	{
		if(!nOrderID)
			nOrderID = begin->nOrderID;
		if(begin->nOrderID != nOrderID)
		{
			begin++;
			continue;
		}

		tSFDWList.push_back(*begin);
		m_tFindFileDirItemList.erase(begin++);
		nRtn = 1;
		nLimited--;

		if(!nLimited)
			break;
	}
	m_tFindFileDirItemMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::ClearFindFileDirItem()
{
	m_tFindFileDirItemMutex.Lock();

	WriteLog("clear find file dir item num : [%d]", m_tFindFileDirItemList.size());
	m_tFindFileDirItemList.clear();

	m_tFindFileDirItemMutex.UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::ClearFindFileDirItem(UINT32 nOrderID)
{
	INT32 nRtn = 0;
	m_tFindFileDirItemMutex.Lock();

	TListFindDirItemItor begin, end;
	begin = m_tFindFileDirItemList.begin();		end = m_tFindFileDirItemList.end();
	for(; begin != end;)
	{
		if(begin->nOrderID != nOrderID)
		{
			begin++;
			continue;
		}

		m_tFindFileDirItemList.erase(begin++);
		nRtn = 1;
	}

	m_tFindFileDirItemMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32		CFindFileUtil::InitFindFileWork(UINT32 nOrderID)
{
	FIND_FILE_WORK tSFFW;
	if(m_tFindFileWorkMap.find(nOrderID) != m_tFindFileWorkMap.end())
	{
		WriteLog("already exist order id : [%d]", nOrderID);
		return -1;
	}

	tSFFW.tMutexExt		= new CMutexExt;
	tSFFW.nOrderID		= nOrderID;
	tSFFW.nContinue		= 1;

	m_tFindFileWorkMap[nOrderID] = tSFFW;
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::DeleteFindFileWork(UINT32 nOrderID)
{
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
		return 0;

	CMutexExt* tMutexExt = (CMutexExt*)(pSFFW->tMutexExt);
	if(tMutexExt)
		delete tMutexExt;

	pSFFW->tFFIList.clear();
	pSFFW->nContinue = 0;
	m_tFindFileWorkMap.erase(nOrderID);

	ClearFileMask(nOrderID);
	ClearExceptDir(nOrderID);
	return 0;
}
//--------------------------------------------------------------------

INT32		CFindFileUtil::ClearFindFileWork()
{
	TMapFindFileWorkItor begin, end;
	CMutexExt* tMutexExt;
	begin = m_tFindFileWorkMap.begin();	end = m_tFindFileWorkMap.end();
	for(begin; begin != end; begin++)
	{
		tMutexExt = (CMutexExt*)(begin->second.tMutexExt);
		if(tMutexExt)
			delete tMutexExt;
		begin->second.tFFIList.clear();
		begin->second.nContinue = 0;
	}
	m_tFindFileWorkMap.clear();
	return 0;
}
//--------------------------------------------------------------------

UINT32		CFindFileUtil::GetFindFileWorkContinue(UINT32 nOrderID)
{
	UINT32 nRtn = 0;

	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
	{
		return 0;
	}

	((CMutexExt*)(pSFFW->tMutexExt))->Lock();
	{
		nRtn = pSFFW->nContinue;
	}
	((CMutexExt*)(pSFFW->tMutexExt))->UnLock();

	return nRtn;
}
//--------------------------------------------------------------------

PFIND_FILE_WORK	CFindFileUtil::GetFindFileWork(UINT32 nOrderID)
{
	TMapFindFileWorkItor find = m_tFindFileWorkMap.find(nOrderID);
	if(find == m_tFindFileWorkMap.end())
		return NULL;
	return &(find->second);
}
//-------------------------------------------------------------

INT32		CFindFileUtil::SetFindFileWork_TotalDir(UINT32 nOrderID, UINT32 nDirNum, UINT32 nDirSubSearchedNum)
{
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
		return -1;

	tMutexExt = (CMutexExt*)pSFFW->tMutexExt;
	if(tMutexExt == NULL)
		return -2;

	tMutexExt->Lock();
	pSFFW->nDirTotalNum			+= nDirNum;
	pSFFW->nDirSubSearchedNum	+= nDirSubSearchedNum;
	tMutexExt->UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::SetFindFileWork_SearchPath(UINT32 nOrderID, UINT32 nType, UINT32 nValue)
{
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
		return -1;
	tMutexExt = (CMutexExt*)pSFFW->tMutexExt;
	if(tMutexExt == NULL)
		return -2;

	tMutexExt->Lock();
	switch(nType)
	{
		case 0:
			pSFFW->nSearchPathNum = nValue;
			break;
		case 1:
			pSFFW->nSearchedPathNum += nValue;
			break;
	}
	tMutexExt->UnLock();
	return 0;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32		CFindFileUtil::AddFindFileItemList(UINT32 nOrderID, UINT32 nSearchDirNum, TListFindFileItem& tFindFileItemList)
{
	TListFindFileItemItor begin, end;
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
		return -1;

	tMutexExt = (CMutexExt*)pSFFW->tMutexExt;
	if(tMutexExt == NULL)
		return -2;

	tMutexExt->Lock();
	begin = tFindFileItemList.begin();	end = tFindFileItemList.end();
	for(begin; begin != end; begin++)
	{
		pSFFW->tFFIList.push_back(*begin);
	}
	pSFFW->nDirSearchedNum += nSearchDirNum;
	pSFFW->nFileTotalNum += tFindFileItemList.size();
	tMutexExt->UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::AddFindFileItemList(UINT32 nOrderID, UINT32 nSearchDirNum, PFIND_FILE_ITEM pAFFI)
{
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
		return -1;
	tMutexExt = (CMutexExt*)(pSFFW->tMutexExt);
	if(tMutexExt == NULL)
		return -2;
	
	tMutexExt->Lock();
	if(pAFFI)
	{
		pSFFW->tFFIList.push_back(*pAFFI);
		pSFFW->nFileTotalNum += 1;
	}
	pSFFW->nDirSearchedNum += nSearchDirNum;	
	tMutexExt->UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::GetFindFileItem(UINT32 nOrderID, PASI_FF_FILE_ITEM pAFFI, PUINT32 nBufNum, PASI_FF_FILE_RESULT pAFFR)
{
	UINT32 nIdx = 0;
	TListFindFileItemItor begin, end;
	CMutexExt* tMutexExt = NULL;
	PFIND_FILE_WORK pSFFW = NULL;

	if(nBufNum == NULL)
	{
		WriteLog("[GetFindFileItem] invalid input data");
		return -1;
	}
	
	pSFFW = GetFindFileWork(nOrderID);
	if(!pSFFW)
	{
		WriteLog("[GetFindFileItem] fail to find file work (%d)", nOrderID);
		return -2;
	}

	tMutexExt = (CMutexExt*)(pSFFW->tMutexExt);
	if(tMutexExt == NULL)
	{
		WriteLog("[GetFindFileItem] invalid mutex lock");
		return -3;
	}
	
	tMutexExt->Lock();
	if(pAFFI)
	{
		begin = pSFFW->tFFIList.begin();	end = pSFFW->tFFIList.end();
		for(begin; begin != end && (*nBufNum) > nIdx; )
		{
			strncpy(pAFFI[nIdx].szFilePath, begin->strFilePath.c_str(), CHAR_MAX_SIZE-1);
			pAFFI[nIdx].szFilePath[CHAR_MAX_SIZE-1] = '\0';
			strncpy(pAFFI[nIdx].szFileName, begin->strFileName.c_str(), MAX_HBUFF-1);
			pAFFI[nIdx].szFileName[MAX_HBUFF-1] = '\0';
			pAFFI[nIdx].nFileSize = begin->nFileSize;
			pAFFI[nIdx].nFindType = begin->nFindType;

			pSFFW->tFFIList.erase(begin++);
			pSFFW->nFileWorkedNum += 1;
			nIdx++;
		}

		*nBufNum = nIdx;
	}	

	if(pAFFR)
	{
		pAFFR->nContinue = 1;
		pAFFR->nMoreFileItem = 1;
		pAFFR->nSearchPathNum		= pSFFW->nSearchPathNum;
		pAFFR->nSearchedPathNum		= pSFFW->nSearchedPathNum;
		pAFFR->nDirSubSearchNum		= pSFFW->nDirSubSearchNum;
		pAFFR->nDirSubSearchedNum	= pSFFW->nDirSubSearchedNum;
		pAFFR->nDirTotalNum			= pSFFW->nDirTotalNum;
		pAFFR->nDirSearchedNum		= pSFFW->nDirSearchedNum;
		pAFFR->nFileTotalNum		= pSFFW->nFileTotalNum;
		pAFFR->nFileWorkedNum		= pSFFW->nFileWorkedNum;
		if(pSFFW->nDirSearchedNum != pSFFW->nDirOldSearchedNum)
		{
			pSFFW->nDirOldSearchedNum = pSFFW->nDirSearchedNum;
			pSFFW->nSameSearcheCnt = 0;
		}
		else
		{
			pSFFW->nSameSearcheCnt++;
		}

		if((pAFFR->nSearchPathNum + pAFFR->nSearchedPathNum + pAFFR->nDirSearchedNum + pAFFR->nDirSubSearchedNum + pAFFR->nDirTotalNum + pAFFR->nDirSearchedNum + pAFFR->nFileTotalNum + pAFFR->nFileWorkedNum))
		{
			if(pAFFR->nSearchPathNum == pAFFR->nSearchedPathNum && pAFFR->nDirTotalNum == pAFFR->nDirSearchedNum && pAFFR->nDirSubSearchNum == pAFFR->nDirSubSearchedNum)
			{
				pAFFR->nContinue = 0;
				if((*nBufNum) == 0 && pAFFR->nFileTotalNum == pAFFR->nFileWorkedNum)
					pAFFR->nMoreFileItem = 0;
				if(!pAFFR->nContinue && !pAFFR->nMoreFileItem)
    	            pSFFW->nContinue = 0;
			}
			if(pSFFW->nSameSearcheCnt > 300)
			{
				pAFFR->nContinue = 0;
				pAFFR->nMoreFileItem = 0;
			}
			if(!pSFFW->nContinue)
			{
				pAFFR->nContinue = 0;
				pAFFR->nMoreFileItem = 0;
			}
		}
	}

	tMutexExt->UnLock();
	return 0;
}
//-------------------------------------------------------------

INT32		CFindFileUtil::IsDocFileFormat(LPCTSTR pFilePath, INT32 *pnFileType)
{
	ASI_DFILE_FMT_INFO tADFI;
	if(pFilePath == NULL || pnFileType == NULL)
		return -1;

	char acLogMsg[MAX_LOGMSG+1] = {0,};
	if(m_tASIFIDLLUtil->ASIFI_GetFileElfMagic((char *)pFilePath))	
	{
		return -2;
	}

	strncpy(tADFI.szFileName, pFilePath, CHAR_MAX_SIZE-1);
	tADFI.szFileName[CHAR_MAX_SIZE-1] = '\0';
//	mbstowcs((wchar_t*)tADFI.wszFileName, tADFI.szFileName, CHAR_MAX_SIZE-1);
//	tADFI.wszFileName[CHAR_MAX_SIZE-1] = L'\0';

	if(m_tASIDFFDLLUtil->ASIDFF_GetDFFmtInfo(&tADFI, acLogMsg) != 0)
	{
		if(acLogMsg[0] != 0)
			WriteLog("fail to get file (%s) fmt info : %s", tADFI.szFileName, acLogMsg);
	}

	if(tADFI.nFmtType == ASIDFF_FILE_FMT_TYPE_UNKNOWN || tADFI.nFmtType == ASIDFF_FILE_FMT_TYPE_ZIP)
		return -3;

	*pnFileType = tADFI.nFmtType;

	return 0;
}





