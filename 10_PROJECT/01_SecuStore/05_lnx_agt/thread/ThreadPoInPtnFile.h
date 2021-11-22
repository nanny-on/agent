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
 *  * 
 */

#ifndef _THREAD_PO_IN_PTN_FILE_H__
#define _THREAD_PO_IN_PTN_FILE_H__

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile thread

#define ASI_FANOTIFY_BUFF_MAX	8192


#define FAN_CLOSE_WRITE		0x00000008
#define FAN_OPEN_PERM		0x00010000

//#define FAN_OPEN_EXEC_PERM	0x00040000

#define ASI_FANOTIFY_PATH "/"

#define ASI_DISCONNECT_STATE	0
#define ASI_ACCEPT_STATE		1
#define ASI_CONNECT_STATE		2

typedef struct _asi_chk_info
{
	INT32 nLen;
	INT32 nPtn;
	char acFile[MAX_FILE_NAME];
	char acPath[MAX_PATH];
	char acFullPath[MAX_PATH];
}ASI_CHK_INFO, *PASI_CHK_INFO;

typedef struct _asi_ret_info
{
	INT32 nBlockMode;
	INT32 nIsWarning;
	INT32 nPolicyType;
	INT32 nAcVal;
	char acWhiteHash[SHA512_BLOCK_SIZE+HASH_PADDING_LEN];
}ASI_RET_INFO, *PASI_RET_INFO;

typedef struct _asi_chk_proc_file
{
	INT32 nState;
	INT32 nCmdId;
	ASI_CHK_INFO stFileInfo;
	ASI_CHK_INFO stProcInfo;
	ASI_RET_INFO stRetInfo;
} ASI_CHK_FILE_PROC, *PASI_CHK_FILE_PROC;

typedef struct _asi_chk_ptn_file
{
	ASI_CHK_FILE_PROC stCHKFILE;
	ASI_WENG_WL_EX stAWWE;
} ASI_CHK_PTN_FILE, *PASI_CHK_PTN_FILE;

typedef struct _asi_policy_info
{
	INT32 nOpMode;
	INT32 nOpID;
	INT32 nOpBlockMode;
	INT32 nExUsedMode;
	INT32 nSpUsedMode;
	INT32 nWLUsedMode;
	INT32 nWLID;
	INT32 nBLUsedMode;
	INT32 nBLID;
	INT32 nReserved;
}ASI_POLICY_INFO, *PASI_POLICY_INFO;

typedef struct _asi_acc_notify_policy
{
	INT32 nState;
	INT32 nCmdId;
	ASI_POLICY_INFO stPolInfo;
	ASI_FILE_HASH stFileHash;
} ASI_ACC_NOTIFY_POLICY, *PASI_ACC_NOTIFY_POLICY;

typedef list<ASI_CHK_FILE_PROC>					TListChkFileProc;
typedef TListChkFileProc::iterator				TListChkFileProcItor;


typedef map<String, ASI_RET_INFO>			TMapPtnRetEx;
typedef TMapPtnRetEx::iterator				TMapPtnRetExItor;
typedef TMapPtnRetEx::reverse_iterator		TMapPtnRetExRItor;

class CThreadPoInPtnFile : public CThreadBase
{
public:
	CThreadPoInPtnFile();
	virtual ~CThreadPoInPtnFile();

private:
	UINT32					m_nOrderID;
	TMapMemFindOrderInfo	m_tOrderIDMap;
	CASIWEngDLLUtil		m_tWEDLLUtil;
	CASIFIDLLUtil		m_tFIDLLUtil;
	INT32				m_nShmId;
	char *				m_pString;
	UINT32				m_nTestTime;
	INT32				m_nTestCount;
	double				m_fTotalDiffTime;
	INT32				m_nCheckThread;
	pthread_mutex_t 	m_ShmMutex;

private:
	INT32				InitShm();
	VOID				UninitShm();
	INT32				InitNotifyEvent();
	INT32				CheckShmEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				IsInitLogic();

	INT32				ShmRecv(PVOID pRecvData, INT32 nReqSize);
	BOOL				IsMatchFile(char *pFilePath, char *pcFeKey, DB_FILE_INFO& tSpecFI, UINT32 nChkType, CHAR chTok);
	INT32				ChkInPtnEx(char *pFilePath, char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType, UINT32& nExtOption);
	INT32				ChkInPtnSP(char *pPath, char *pFile, char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType);
	INT32				ChkInPtn(char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType);
	VOID				SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType);
	INT32				SetLogExecEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				AnalyzeAccEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	VOID				SetLogCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				BypassObjectPath(PASI_CHK_PTN_FILE pChkPtnFile);
	DWORD				AnalyzeCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				AnalyzeExecEvent(PASI_CHK_PTN_FILE pChkPtnFile);
// Operations
public:
	INT32				LoadWhitePattern();
	VOID				UnloadWhitePattern();
	INT32				CheckWhitePatternFile();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoInPtnFile)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	UINT32		GetNextOrderID()					{	return ++m_nOrderID;			};
	INT32		AddOrderID(PVOID pPFOI);
	INT32		IsEndWork()							{	return m_nPause;	};

// Implementation
protected:

};

extern	CThreadPoInPtnFile*	t_ThreadPoInPtnFile;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_PTN_FILE_H__
