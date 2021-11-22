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


#include "stdafx.h"
#include "com_struct.h"
#include "as_sha1.h"
#include "as_sha256.h"
#include "DownLoadUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
CDownLoadUtil::CDownLoadUtil()
{
	m_tDownLoadThread = 0;
	m_nDownloadThreadContinue = 0;
	m_nContinueDownPatch = 1;

	m_nPtoPInitDLL	= 0;
}
//--------------------------------------------------------------------

CDownLoadUtil::~CDownLoadUtil()
{
	StopDownload();
}

//--------------------------------------------------------------------

INT32	CDownLoadUtil::SetHomePath(LPCSTR lpHomePathList, UINT32 nLen)
{
	if(lpHomePathList == NULL)
		return -1;

	m_tHomePathList.clear();

	CTokenString Token(lpHomePathList, nLen, ';');
	while(Token.IsNextToken())
	{
		String strPath = Token.NextToken();

		m_tHomePathList.push_back(strPath);
	}
	return 0;
}

INT32	CDownLoadUtil::SetDLLogPath(char *pLogPath, char *pLogFile, INT32 nRemainLog, UINT32 nFileLogRetention)
{
	if(pLogPath == NULL || pLogFile == NULL)
		return -1;
	t_HttpFileGetUtil.SetLogPath(pLogPath, pLogFile, nRemainLog, nFileLogRetention);
	return 0;
}

//--------------------------------------------------------------------

INT32	CDownLoadUtil::AddDownLoadInfo(PASI_FDL_INFO pafi)
{
	INT32 nRtn;
	m_cDIMutex.Lock();

	ASI_FDL_INFO tAFI = *pafi;

	t_AFDLInfoMap[tAFI.nID] = tAFI;	
	nRtn = t_AFDLInfoMap.size();

	WriteLogN("add download info : [%d]:[%d][%d]:[%d]", tAFI.nID, tAFI.nItemType, tAFI.nItemID, t_AFDLInfoMap.size());

	m_cDIMutex.UnLock();

	return nRtn;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::FindDownLoadInfo(UINT32 nID, PASI_FDL_INFO pafi)
{
	INT32 nRtn = 0;
	m_cDIMutex.Lock();

	do 
	{
		if(!pafi)
			break;

		TMapAFDLInfoItor find = t_AFDLInfoMap.find(nID);
		if(find != t_AFDLInfoMap.end())
		{
			*pafi = find->second;
		}
		else
		{
			nRtn = -1;
		}

	} while (FALSE);

	m_cDIMutex.UnLock();

	return nRtn;
}
//--------------------------------------------------------------------


INT32	CDownLoadUtil::DelDownLoadInfo(UINT32 nItemType, UINT32 nItemID)
{
	INT32 nRtn = -1;
	m_cDIMutex.Lock();
	
	TMapAFDLInfoItor begin, end;
	begin = t_AFDLInfoMap.begin();	end = t_AFDLInfoMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nItemType != nItemType || begin->second.nItemID != nItemID)
		{
			continue;
		}

		begin->second.nRemoveReserved = 1;
		nRtn = 0;
		WriteLogN("del download info reserved : [%d]:[%d][%d][%d]:[rst:%d]", begin->second.nID, nItemType, nItemID, t_AFDLInfoMap.size(), nRtn);
		break;
	}	
	m_cDIMutex.UnLock();	
	return nRtn;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::SetDownloadInit(PASI_FDL_INIT pafdli)
{
	char acPtoPHomePath[CHAR_MAX_SIZE] = {0, };
	char acPtoPTempPath[CHAR_MAX_SIZE] = {0, };
	
	if(pafdli == NULL)
		return -1;

	m_tAFDLI = *pafdli;	

	strncpy(acPtoPHomePath, pafdli->szHomePath, CHAR_MAX_SIZE-1);
	strncpy(acPtoPTempPath, pafdli->szTempPath, CHAR_MAX_SIZE-1);

	if(DirectoryExists(acPtoPHomePath) == FALSE)
		CreateDirectory(acPtoPHomePath, NULL);
	if(DirectoryExists(acPtoPTempPath) == FALSE)
		CreateDirectory(acPtoPTempPath, NULL);		
	return 0;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::SetDLSvrType(UINT32 nDLSvrType)
{
	WriteLogN("change dl svr type : [%d]->[%d]", m_tAFDLI.nDLSvrUsedFlag, nDLSvrType);
	m_tAFDLI.nDLSvrUsedFlag = nDLSvrType;
	return 0;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::SetDLSvrInfo(UINT32 nItemType, LPCSTR szDLSvrInfoList)
{
	WriteLogN("change dl svr info : [%d]:[%s]", nItemType, szDLSvrInfoList);

	switch(nItemType)
	{
		case ASIFDL_DL_SVR_TYPE_PUBLIC:
		{
			memset(m_tAFDLI.szDLPubSvrInfo, 0, MAX_PATH);
			strncpy(m_tAFDLI.szDLPubSvrInfo, szDLSvrInfoList, MAX_PATH-1);
			MakeDLPubSvrInfoList();
			break;
		}
		case ASIFDL_DL_SVR_TYPE_SITE:
		{
			memset(m_tAFDLI.szDLSiteSvrInfo, 0, MAX_PATH);
			strncpy(m_tAFDLI.szDLSiteSvrInfo, szDLSvrInfoList, MAX_PATH-1);
			MakeDLSiteSvrInfoList();
			break;
		}
	}
	return 0;
}
//--------------------------------------------------------------------

void	CDownLoadUtil::MakeDLSiteSvrInfoList()
{
	m_tDLSvrInfoMutex.Lock();

	m_tDLSiteSvrInfoList.clear();
	CTokenString Token(m_tAFDLI.szDLSiteSvrInfo, strlen(m_tAFDLI.szDLSiteSvrInfo), ';');

	while(Token.IsNextToken())
	{
		String strSvrInfo = Token.NextToken();

		WriteLogN("dl site svr info : [%s]", strSvrInfo.c_str());
		CTokenString TokenSub(strSvrInfo.c_str(), strSvrInfo.length(), ':');

		DL_SVR_INFO tDSI;
		tDSI.strAddress = TokenSub.NextToken();
		tDSI.nPort = TokenSub.NextToken_Int();

		m_tDLSiteSvrInfoList.push_back(tDSI);
	}

	m_tDLSvrInfoMutex.UnLock();
	return;
}
//--------------------------------------------------------------------

void				CDownLoadUtil::MakeDLPubSvrInfoList()
{
	m_tDLSvrInfoMutex.Lock();

	m_tDLPubSvrInfoList.clear();
	CTokenString Token(m_tAFDLI.szDLPubSvrInfo, strlen(m_tAFDLI.szDLPubSvrInfo), ';');

	while(Token.IsNextToken())
	{
		String strSvrInfo = Token.NextToken();

		WriteLogN("dl pub svr info : [%s]", strSvrInfo.c_str());
		CTokenString TokenSub(strSvrInfo.c_str(), strSvrInfo.length(), ':');

		DL_SVR_INFO tDSI;
		tDSI.strAddress = TokenSub.NextToken();
		tDSI.nPort = TokenSub.NextToken_Int();

		m_tDLPubSvrInfoList.push_back(tDSI);
	}

	m_tDLSvrInfoMutex.UnLock();
	return;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::RunDownload()
{
	pthread_attr_t attr;
	if(m_tDownLoadThread)
		return 1;

	m_nDownloadThreadContinue = 1;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if (pthread_create(&m_tDownLoadThread, NULL, DownloadFileThread, (void *)this) != 0)
	{
		m_nDownloadThreadContinue = 0;
		m_tDownLoadThread = 0;
		return -1;
	}

	pthread_attr_destroy(&attr);
	
	return 0;
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::StopDownload()
{
	if(m_tDownLoadThread == 0)
		return 1;

	m_nDownloadThreadContinue = 0;
	if( m_tDownLoadThread )
	{
		pthread_cancel(m_tDownLoadThread);
		m_tDownLoadThread = 0;
	}

	return 0;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::IsRunDownload()
{
	return m_nDownloadThreadContinue;
}
//--------------------------------------------------------------------

void		CDownLoadUtil::NotifyDownloadStatus(CDownLoadUtil* tDownLoadUtil, ASI_FDL_INIT& afdli, PASI_FDL_INFO pafdl_info, UINT32 nStatus)
{
	if(afdli.tAFDLR.fDLResult)
	{
		ASI_FDL_RESULT_ITEM tAFRI;
		tAFRI.nItemID			= pafdl_info->nItemID;
		tAFRI.lParam			= afdli.tAFDLR.lParam;
		tAFRI.nItemType			= pafdl_info->nItemType;
		tAFRI.nID				= pafdl_info->nID;
		tAFRI.nStatus			= nStatus;
		tAFRI.nRemainFileNum	= tDownLoadUtil->t_AFDLInfoMap.size();
		tAFRI.nDLSvrType		= pafdl_info->nDLSvrType;
		tAFRI.nRefCount			= pafdl_info->nRefCnt;
		strncpy(tAFRI.pszFileName, pafdl_info->szFileName, CHAR_MAX_SIZE-1);

		afdli.tAFDLR.fDLResult(&tAFRI);
	}
	return;
}
//--------------------------------------------------------------------

void *CDownLoadUtil::DownloadFileThread(LPVOID lParam)
{
	PASI_FDL_INFO pafi = NULL;
	CDownLoadUtil *tDownLoadUtil = (CDownLoadUtil*)lParam;
	CMainDlg *pMainDlg = NULL;
	if(tDownLoadUtil == NULL)
		return (void *)NULL;

	pMainDlg = (CMainDlg *)tDownLoadUtil->m_tAFDLI.hOwner;
	if(pMainDlg == NULL)
		return (void *)NULL;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	
	while(tDownLoadUtil->m_nDownloadThreadContinue && *(tDownLoadUtil->m_tAFDLI.pnMainContinue))
	{
		if(pafi == NULL)
		{
			if( (pafi = tDownLoadUtil->GetNextDownLoadInfo()) == NULL)
			{
				Sleep(500);
//				ProcessMessages();
				continue;
			}
			pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_START,  pafi->nID);
			NotifyDownloadStatus(tDownLoadUtil, tDownLoadUtil->m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_START);
		}

		INT32 nChangeResult = tDownLoadUtil->ChgDLSvrType(pafi);
//		WriteLogN("change download svr type result : %d", nChangeResult);
		
        switch(nChangeResult)
		{
			case 0:
			{
				pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END, pafi->nID);
				NotifyDownloadStatus(tDownLoadUtil, tDownLoadUtil->m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END);

				INT32 nRemoveRtn = tDownLoadUtil->RemoveAFDLInfo(pafi);
				if( nRemoveRtn == -1)
				{
					WriteLogE("Remove download info fails : [%d]", pafi->nID);
				}
				else if(nRemoveRtn == 0)
				{
					pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_ALL_END, tDownLoadUtil->t_AFDLInfoMap.size());				
				} 
				pafi = NULL;

				break;
			}
			case 1:
			{
				if(tDownLoadUtil->TryDownloadFile(pafi) == ASIFDL_DL_RST_TYPE_SUCCESS)
				{
					if(tDownLoadUtil->DownLoadSuccess(pafi) == 0)
					{
						pafi = NULL;
					}
				}
				break;
			}
			case 2:
			{
				pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END, pafi->nID);
				NotifyDownloadStatus(tDownLoadUtil, tDownLoadUtil->m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END);
				pafi->nRefCnt++;
				pafi = NULL;
				break;
			}
			case 3:
			{
				pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_CANCEL_REF_OVER, pafi->nID);
				NotifyDownloadStatus(tDownLoadUtil, tDownLoadUtil->m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_CANCEL_REF_OVER);

				INT32 nRemoveRtn = tDownLoadUtil->RemoveAFDLInfo(pafi);
				if( nRemoveRtn == -1)
				{
					WriteLogE("Remove download info fails because ref over: [%d]", pafi->nID);
				}
				else if(nRemoveRtn == 0)
				{
					pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_ALL_END, tDownLoadUtil->t_AFDLInfoMap.size());				
				} 
				pafi = NULL;

				break;
			}
		}
	}
	
	pMainDlg->PostMessage(tDownLoadUtil->m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_LOGIC_END, 0);
	Sleep(500);
	return (void *)NULL;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::ChgDLSvrType(PASI_FDL_INFO pafi)
{
    if(pafi == NULL)
        return 0;

	if(pafi->nRefCntMax && pafi->nRefCntMax <= pafi->nRefCnt)
	{
		WriteLogN("Change download svr type break because try dn op over (%d) : id[%d],type[%d],id[%d],ref[%d/%d]",
			pafi->nDLSvrType, pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt, pafi->nRefCntMax);
		return 3;
	}

	UINT32 nCurChkDLSvrType = pafi->nDLSvrType;
	if((nCurChkDLSvrType & ASIFDL_DL_SVR_TYPE_ALL) == 0)
		nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_UNKNOWN;
	do 
	{
		if(nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_UNKNOWN)
		{
			if(pafi->nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_LOCAL)
			{
				nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_LOCAL;
				break;
			}
			nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_LOCAL;
		}

		if(nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_LOCAL)
		{
			if(pafi->nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_PTOP && IsUsedDLSvrTypePtoP())
			{
				nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_PTOP;
				break;
			}
			nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_PTOP;
		}

		if(nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_PTOP)
		{
			if(pafi->nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_SITE && IsUsedDLSvrTypeSite())
			{
				nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_SITE;
				break;
			}
			nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_SITE;
		}

		if(nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_SITE)
		{
			if(pafi->nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_PUBLIC && IsUsedDLSvrTypePublic())
			{
				nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_PUBLIC;
				break;
			}
			nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_PUBLIC;
		}

		if(nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_PUBLIC)
		{
			nCurChkDLSvrType = ASIFDL_DL_SVR_TYPE_UNKNOWN;
		}

	}while (FALSE);
        
	pafi->nDLSvrType = nCurChkDLSvrType;
		
//    WriteLogN("Change download svr type (%d) : id[%d],type[%d],id[%d],ref[%d]", pafi->nDLSvrType, pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt);
	return (nCurChkDLSvrType == ASIFDL_DL_SVR_TYPE_UNKNOWN ? 2 : 1);
}
//--------------------------------------------------------------------

INT32			CDownLoadUtil::TryDownloadFile(PASI_FDL_INFO pafi)
{
	INT32 nResult = ASIFDL_DL_RST_TYPE_FAIL;
	CMainDlg *pMainDlg = NULL;
	if(pafi == NULL)
		return ASIFDL_DL_RST_TYPE_FAIL;

	switch(pafi->nDLSvrType)
	{
		case ASIFDL_DL_SVR_TYPE_LOCAL:
		{
			char acLocalFileName[CHAR_MAX_SIZE] = {0, };
			snprintf(acLocalFileName, CHAR_MAX_SIZE-1, "%s/%s", m_tAFDLI.szHomePath, pafi->szFileName);
			if(is_file(acLocalFileName) == 0)
			{
				nResult	= ASIFDL_DL_RST_TYPE_SUCCESS;
			}		
			break;
		}
		case ASIFDL_DL_SVR_TYPE_SITE:
		{
			if(m_tDLSiteSvrInfoList.size() == 0)
			{
				break;
			}
			INT32 nHttpRtn = 1;
			CHAR szSaveFilePath[CHAR_MAX_SIZE] = {0, };
			CHAR szDLSvrPath[CHAR_MAX_SIZE] = {0, };

			snprintf(szSaveFilePath, CHAR_MAX_SIZE-1, "%s/%s", m_tAFDLI.szTempPath, pafi->szFileName);								
			if(GetDLSvrPath(szDLSvrPath, CHAR_MAX_SIZE, pafi))
			{
				break;
			}

			m_tDLSvrInfoMutex.Lock();
			TListDLSvrInfoItor begin, end;
			begin = m_tDLSiteSvrInfoList.begin();		end = m_tDLSiteSvrInfoList.end();
			for(begin; begin != end; begin++)
			{
				nHttpRtn = t_HttpFileGetUtil.GetFile_Host((char *)begin->strAddress.c_str(), szDLSvrPath, szSaveFilePath, begin->nPort);
				if( nHttpRtn == 0 )
				{
					WriteLogN("dl file success from site svr : gid[%d],type[%d],item_id[%d],ref[%d]", pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt);
					nResult	= ASIFDL_DL_RST_TYPE_SUCCESS;						
					break;
				}
				else
				{
					if(_stricmp(pafi->szDLPath, "/wptn"))
						WriteLogE("dl file fails from site svr : id[%d],type[%d],item_id[%d],ref[%d],rtn[%d]:[%s]", pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt, nHttpRtn, pafi->szDLPath);
				}
			}
			m_tDLSvrInfoMutex.UnLock();
			break;
		}
		case ASIFDL_DL_SVR_TYPE_PUBLIC:
		{
			if(m_tDLPubSvrInfoList.size() == 0)
			{
				break;
			}

			INT32 nHttpRtn = 1;
			CHAR szSaveFilePath[CHAR_MAX_SIZE] = {0, };
			CHAR szDLSvrPath[CHAR_MAX_SIZE] = {0, };
			snprintf(szSaveFilePath, CHAR_MAX_SIZE-1, "%s/%s", m_tAFDLI.szTempPath, pafi->szFileName);								
			if(GetDLSvrPath(szDLSvrPath, CHAR_MAX_SIZE, pafi))
			{
				break;
			}

			m_tDLSvrInfoMutex.Lock();
			TListDLSvrInfoItor begin, end;
			begin = m_tDLPubSvrInfoList.begin();		end = m_tDLPubSvrInfoList.end();
			for(begin; begin != end; begin++)
			{
				nHttpRtn = t_HttpFileGetUtil.GetFile_Host((char *)begin->strAddress.c_str(), szDLSvrPath, szSaveFilePath, begin->nPort);
				if( nHttpRtn == 0 )
				{
					WriteLogN("dl file success from pub svr : gid[%d],type[%d],item_id[%d],ref[%d]", 
						pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt);
					nResult	= ASIFDL_DL_RST_TYPE_SUCCESS;						
					break;
				}
				else
				{
					WriteLogE("dl file fails from pub svr : id[%d],type[%d],item_id[%d],ref[%d],rtn[%d]:[%s]", 
						pafi->nID, pafi->nItemType, pafi->nItemID, pafi->nRefCnt, nHttpRtn, pafi->szDLPath);
				}
			}
			m_tDLSvrInfoMutex.UnLock();

			if(nResult != ASIFDL_DL_RST_TYPE_SUCCESS)
			{
				pMainDlg = (CMainDlg *)m_tAFDLI.hOwner;
				if(pMainDlg != NULL)
					pMainDlg->PostMessage(m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END, pafi->nID);
				NotifyDownloadStatus(this, m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_FAIL_END);
				Sleep(500);
			}
			break;
		}
	}

	return nResult;
}
//--------------------------------------------------------------------

PASI_FDL_INFO	CDownLoadUtil::GetNextDownLoadInfo()
{
	TListID tDelIDList;
	PASI_FDL_INFO pafi = NULL;		
	INT32 nRefCnt = -1;
		
	m_cDIMutex.Lock();
	{
		TMapAFDLInfoItor begin, end;
		begin = t_AFDLInfoMap.begin();	end = t_AFDLInfoMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.nRemoveReserved)
			{
				tDelIDList.push_back(begin->first);				
				WriteLogN("del download info : [%d]:[%d][%d]", begin->second.nID, begin->second.nItemType, begin->second.nItemID);
				continue;
			}
			
			if(nRefCnt == -1 || nRefCnt > begin->second.nRefCnt)
			{
				nRefCnt = begin->second.nRefCnt;
				pafi = &(begin->second);
			}
		}	
	}
	{
		TListIDItor begin, end;
		begin = tDelIDList.begin();	end = tDelIDList.end();
		for(begin; begin != end; begin++)
		{
			t_AFDLInfoMap.erase(*begin);
		}
	}
	m_cDIMutex.UnLock();

	UINT32 nCurTime = GetCurrentDateTimeInt();
	if(pafi)
	{
		if(nCurTime - pafi->nFileStatus < 60)
			return NULL;
		pafi->nFileStatus = nCurTime;
	}
	return pafi;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::DownLoadSuccess(PASI_FDL_INFO pafi)
{
	String strSaveFilePath;
	CMainDlg *pMainDlg = NULL;

	if(pafi == NULL)
    {
        WriteLogE("file information empty when file download success [down success]");
        return 0;
    }

    if(pafi->nDLSvrType == ASIFDL_DL_SVR_TYPE_PTOP || pafi->nDLSvrType == ASIFDL_DL_SVR_TYPE_LOCAL)
        strSaveFilePath = SPrintf("%s/%s", m_tAFDLI.szHomePath, pafi->szFileName);
    else
        strSaveFilePath = SPrintf("%s/%s", m_tAFDLI.szTempPath, pafi->szFileName);

    if(CheckHashValue(strSaveFilePath.c_str(), pafi->szFileHash))
    {
        if(pafi->nDLSvrType != ASIFDL_DL_SVR_TYPE_PTOP)
        {
            if(unlink(strSaveFilePath.c_str()) == -1)
            {
                WriteLogE("fail to delete file %s : [%d]", strSaveFilePath.c_str(), errno);
            }

			NotifyDownloadStatus(this, m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_FAIL_HASH_END);
        }
        else
        {
			WriteLogE("Invalid file hash : gid[%d],uid[%d],type[%d],ref[%d]", pafi->nID, pafi->nItemID, pafi->nItemType, pafi->nRefCnt);	
			NotifyDownloadStatus(this, m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_FAIL_HASH_END);
        }
        return 1;
    }
	else if(pafi->nDLSvrType != ASIFDL_DL_SVR_TYPE_LOCAL)
	{
		String strTargetFileName = SPrintf("%s/%s", m_tAFDLI.szHomePath, pafi->szFileName);
		CopyFile(strSaveFilePath.c_str(), strTargetFileName.c_str(), FALSE);
		unlink(strSaveFilePath.c_str());

	} 
	pMainDlg = (CMainDlg *)m_tAFDLI.hOwner;
	if(pMainDlg != NULL)
		pMainDlg->PostMessage(m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END, pafi->nID);
	NotifyDownloadStatus(this, m_tAFDLI, pafi, ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END);

	INT32 nRemoveRtn = RemoveAFDLInfo(pafi);
    if( nRemoveRtn == -1)
	{
		WriteLogE("Remove download info fails : [%d]", pafi->nID);
	}
	else if(nRemoveRtn == 0)
	{
		if(pMainDlg != NULL)
			pMainDlg->PostMessage(m_tAFDLI.nNotifyMsgID, ASIFDL_DL_RST_STATUS_TYPE_ALL_END, t_AFDLInfoMap.size());
		WriteLogN("notify download end message : [%.8x], [%d]", m_tAFDLI.hOwner, t_AFDLInfoMap.size());
	}
	else
	{
		WriteLogN("remain download items [%d], continue download operation..", nRemoveRtn);
	}
	pafi = NULL;
	return 0;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::CheckHashValue(LPCSTR pszFileName, LPCSTR pszFileHash)
{
	CHAR szHashValue[CHAR_MAX_SIZE] = {0, };
    BOOL bHashCheckPassed = TRUE;
	if(pszFileName == NULL || pszFileHash == NULL)
	{
		WriteLogE("fail to check file Integrity : invalid input data");
		return -1;
	}

	switch(strlen(pszFileHash))
	{
		case ASIFDL_DL_FILE_HASH_TYPE_SHA1_40:
			get_sha1_hash_from_file((char *)pszFileName, szHashValue, CHAR_MAX_SIZE);
			break;
		case ASIFDL_DL_FILE_HASH_TYPE_SHA2_64:
			get_sha256_hash_from_file((char *)pszFileName, szHashValue, CHAR_MAX_SIZE);
			break;
		default:
		{
			WriteLogE("not define file hash algorithm : [%d][%s][%d]", strlen(pszFileHash), pszFileHash, pszFileHash[40]);
			break;
		}
	}
    if(pszFileHash == NULL || pszFileHash[0] == NULL)
    {
        bHashCheckPassed = TRUE;
    }
    else
    {
        if(!_stricmp(szHashValue, pszFileHash))
            bHashCheckPassed = TRUE;
        else
            bHashCheckPassed = FALSE;
    }

    if(bHashCheckPassed == FALSE)
    {
		WriteLogE("fail to check %s Integrity : [or:%s][chk:%s]", pszFileName, pszFileHash, szHashValue);
		return -3;
	}
	return 0;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::IsUsedDLSvrTypePtoP()
{
	return ( m_tAFDLI.nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_PTOP ? 1 : 0);
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::IsUsedDLSvrTypeSite()
{
	return ( m_tAFDLI.nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_SITE ? 1 : 0);
}
//--------------------------------------------------------------------

INT32	CDownLoadUtil::IsUsedDLSvrTypePublic()
{
	return ( m_tAFDLI.nDLSvrUsedFlag & ASIFDL_DL_SVR_TYPE_PUBLIC ? 1 : 0);
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::RemoveAFDLInfo(PASI_FDL_INFO pafi)
{
	INT32 nRtn = -1;
	INT32 nIndex = 1;
	WriteLogN("remove download info item : [%u]", pafi->nID);
	m_cDIMutex.Lock();
	TMapAFDLInfoItor begin, end;
	TMapAFDLInfoItor find = t_AFDLInfoMap.find(pafi->nID);
	end = t_AFDLInfoMap.end();
	if(find != end)
	{
		t_AFDLInfoMap.erase(find);
		nRtn = t_AFDLInfoMap.size();
	}
	if(nRtn > 1)
	{
		begin = t_AFDLInfoMap.begin();	end = t_AFDLInfoMap.end();
		for(begin; begin != end; begin++)
		{
			WriteLogN("remain download info [%d] : [%s][%s]", nIndex++, begin->second.szDLPath, begin->second.szFileName);
		}
	}
	m_cDIMutex.UnLock();

	return nRtn;
}
//--------------------------------------------------------------------

INT32 CDownLoadUtil::GetDLSvrPath(LPSTR lpBuf, INT32 nMaxLen, PASI_FDL_INFO pafi)
{   
	INT32 i, nNum = 0;
	if(lpBuf == NULL || nMaxLen < 1 || pafi == NULL)
		return -1;

	snprintf(lpBuf, nMaxLen-1, "%s/%s", pafi->szDLPath, pafi->szFileName);

    nNum = (INT32)strlen(lpBuf);
    for(i = 0 ; i < nNum ; i++)
    {
        if(lpBuf[i] == ' ')
            lpBuf[i] = '%20';
    }
    return 0;
}
