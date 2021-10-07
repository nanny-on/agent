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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *  * 
 */

#ifndef _THREAD_PO_IN_ACC_FILE_H__
#define _THREAD_PO_IN_ACC_FILE_H__


#define AS_SEND_POLICY_INFO 0x01
#define AS_SEND_WHITE_FILE	0x02
#define AS_SEND_BLACK_FILE	0x04
#define AS_SEND_POLICY_ALL	AS_SEND_POLICY_INFO | AS_SEND_WHITE_FILE | AS_SEND_BLACK_FILE

class CThreadPoInAccFile : public CThreadBase
{
public:
	CThreadPoInAccFile();
	virtual ~CThreadPoInAccFile();

private:
	UINT32					m_nOrderID;
	TMapMemFindOrderInfo	m_tOrderIDMap;

	INT32				m_nServerFd;
	INT32				m_nClientFd;
	INT32				m_nCheckThread;
	pthread_mutex_t 	m_SockMutex;

private:
	INT32				CheckThread(INT32 nClientFd);
	INT32				InitUnixSock(INT32 &nSrvFd);
	VOID				UninitUnixSock();
	INT32				GetCheckThreadState();
	VOID				SetCheckThreadState(INT32 nState);
	INT32				GetClientFd();
	VOID				SetClientFd(INT32 nFd);
	INT32				InitNotifyEvent(INT32 &nSrvFd);
	INT32				CheckSockEvent(INT32 nClientFd, PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize);
	INT32				SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize);
	INT32				IsInitLogic();

	VOID				SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType);
	INT32				ChkInPtn(PASI_CHK_PTN_FILE pChkPtnFile, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType);
	INT32				SetLogAccEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				BypassObjectPath(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32				AnalyzeAccEvent(PASI_CHK_PTN_FILE pChkPtnFile);

	INT32				SendPolicyInfo(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendWhiteClear(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendWhiteFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendBlackClear(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendBlackFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SendEndPolicy(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
// Operations
public:
	INT32				CheckWhitePatternFile();
	INT32				SendExitThreadCmd();
	INT32				SendPolicy(INT32 nSendFlag);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoInAccFile)
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

extern	CThreadPoInAccFile*	t_ThreadPoInAccFile;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_ACC_FILE_H__
