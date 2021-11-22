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

#ifndef _THREAD_PO_IN_ACC_FILE_H__
#define _THREAD_PO_IN_ACC_FILE_H__


#define AS_SEND_POLICY_INFO		0x0001
#define AS_SEND_SP_FILE			0x0002
#define AS_SEND_EX_FILE			0x0004
#define AS_SEND_WHITE_FILE		0x0008
#define AS_SEND_BLACK_FILE		0x0010
#define AS_SEND_CREATE_FILE		0x0020

#define AS_SEND_START_POLICY	0x1000
#define AS_SEND_END_POLICY		0x2000
#define AS_SEND_POLICY_ALL	AS_SEND_START_POLICY | AS_SEND_POLICY_INFO | AS_SEND_SP_FILE | AS_SEND_EX_FILE | AS_SEND_WHITE_FILE | AS_SEND_BLACK_FILE | AS_SEND_END_POLICY

class CThreadPoInAccFile : public CThreadBase
{
public:
	CThreadPoInAccFile();
	virtual ~CThreadPoInAccFile();

private:
	INT32				m_nShmId;
	INT32				m_nSendingMode;
	INT32				m_nClientFd;
	char *				m_pString;
	UINT32				m_nTestTime;
	INT32				m_nTestCount;
	double				m_fTotalDiffTime;
	UINT32				m_nSendTime;
	CASIWEngDLLUtil		m_tWEDLLUtil;
	pthread_mutex_t 	m_SockMutex;
private:
	INT32				InitShm();
	VOID				UninitShm();
	INT32				InitNotifyEvent();
	INT32				LoadWhitePattern();
	VOID				UnloadWhitePattern();
	INT32				CheckShmEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				ShmRecv(PVOID pRecvData, INT32 nReqSize);
	INT32				ShmWrite(PVOID pWriteData, INT32 nReqSize);
	VOID				GetSockId(INT32 &nClientFd);
	VOID				SetSockId(INT32 nClientFd);
	INT32				InitSockEnv();
	VOID				UninitSockEnv();
	INT32				SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize);
	INT32				IsInitLogic();

	INT32				SetLogAccEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				BypassObjectPath(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				AnalyzeAccEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	VOID				GetSendTime(UINT32 &nSendTime);
	INT32				SendAliveMessage();
	INT32				GetSendingMode();
	VOID				SetSendingMode(INT32 nSendingMode);
	INT32				SendCmdInfoWithLock(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendStartPolicy(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendPolicyInfo(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendSpClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendSpFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32& nSendCnt);
	INT32				SendExClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendExFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32& nSendCnt);
	INT32				SendWhiteClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendWhiteFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32& nSendCnt);
	INT32				SendBlackClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendBlackFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32& nSendCnt);
	INT32				SendEndPolicy(PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	// Operations
public:
	INT32				CheckWhitePatternFile();
	INT32				SendPolicy(INT32 nSendFlag);
	INT32				SendAddCreateFile(char *acFullPath, char *acWhiteHash);
	INT32				SendClearCreateFile();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoInAccFile)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	INT32		IsEndWork()							{	return m_nPause;	};

	// Implementation
protected:

};

extern	CThreadPoInAccFile*	t_ThreadPoInAccFile;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_ACC_FILE_H__
