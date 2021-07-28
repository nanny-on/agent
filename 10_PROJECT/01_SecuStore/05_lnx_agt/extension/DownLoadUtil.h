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


#ifndef _DOWNLOAD_UTIL_H__
#define _DOWNLOAD_UTIL_H__

#include "HttpFileGetUtil.h"
#include "TokenString.h"

class CDownLoadUtil  
{
private:	
	CMutexExt			m_cDIMutex;
	ASI_FDL_INIT		m_tAFDLI;	
	pthread_t			m_tDownLoadThread;
	INT32				m_nDownloadThreadContinue;		
//	CASIPTPDLLUtil		t_PtoPFileGetUtil;
	CHttpFileGetUtil	t_HttpFileGetUtil;
	TListStr			m_tHomePathList;


private:
	CMutexExt			m_tDLSvrInfoMutex;
	TListDLSvrInfo		m_tDLSiteSvrInfoList;
	TListDLSvrInfo		m_tDLPubSvrInfoList;

private:
	void				MakeDLSiteSvrInfoList();
	void				MakeDLPubSvrInfoList();

private:
	INT32				m_nContinueDownPatch;

private:
	TMapAFDLInfo		t_AFDLInfoMap;	

private:
	static	void*		DownloadFileThread(LPVOID lParam);
	static	void		NotifyDownloadStatus(CDownLoadUtil* tDownloadUtil, ASI_FDL_INIT& afdli, PASI_FDL_INFO pafdl_info, UINT32 nStatus);

private:
	INT32			RemoveAFDLInfo(PASI_FDL_INFO pafi);
	INT32			IsUsedDLSvrTypePtoP();
	INT32			IsUsedDLSvrTypeSite();
	INT32			IsUsedDLSvrTypePublic();
	INT32			CheckHashValue(LPCTSTR pszFileName, LPCTSTR pszFileHash);

private:
	INT32			TryDownloadFile(PASI_FDL_INFO pafi);

	INT32			DownLoadSuccess(PASI_FDL_INFO pafi);
	PASI_FDL_INFO	GetNextDownLoadInfo();
	INT32			ChgDLSvrType(PASI_FDL_INFO pafi);
	INT32			GetDLSvrPath(LPSTR lpBuf, INT32 nMaxLen, PASI_FDL_INFO pafi);

public:
	INT32			SetHomePath(LPCTSTR lpHomePathList, UINT32 nLen);
	INT32			SetDLLogPath(char *pLogPath, char *pLogFile, INT32 nRemainLog, UINT32 nFileLogRetention);

public:
	INT32			AddDownLoadInfo(PASI_FDL_INFO pafi);
	INT32			FindDownLoadInfo(UINT32 nID, PASI_FDL_INFO pafi);
	INT32			DelDownLoadInfo(UINT32 nItemType, UINT32 nItemID);
	INT32			SetDownloadInit(PASI_FDL_INIT pafdli);
	INT32			SetDLSvrType(UINT32 nDLSvrType);
	INT32			SetDLSvrInfo(UINT32 nItemType, LPCTSTR szDLSvrInfoList);
	INT32			RunDownload();
	INT32			StopDownload();

public:
	INT32			m_nPtoPInitDLL;

public:
	INT32 IsRunDownload();
	CDownLoadUtil();
	virtual ~CDownLoadUtil();
};

#endif /*_DOWNLOAD_UTIL_H__*/ 
