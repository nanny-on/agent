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

#ifndef _ACC_NOTIFY_DLG_H__

#define _ACC_NOTIFY_DLG_H__

class CAccNotifyDlg
{
public:
	CAccNotifyDlg();
	virtual ~CAccNotifyDlg();
	int			Run();
	BOOL		GetMessage(MSG* pMsg);
	BOOL		PostMessage(UINT dwMsgID, WPARAM wParam, LPARAM lParam);
	void		CloseMain();

private:
    INT32 		m_nCloseMode;
	INT32 		m_nFileLogRetention;
	DWORD		m_dwThreadID;
	char		m_acRootPath[MAX_PATH];
	CSemExt 	m_SemExt;
	pthread_mutex_t 	m_PolMutex;
	TMapCStr			m_tWLPathStrMap;
	TMapCStr			m_tBLPathStrMap;
	INT32			m_nIsSetPolicy;
	ASI_POLICY_INFO m_stPolicyInfo;

private:
	INT32		StartModule();
	void		StopModule();
	INT32		StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime = 5/*sec*/);

private:
	INT32		PreCreateSubClass();
	INT32		CreateSubClass();
	INT32		InitTimerID();
	INT32		GetWatchDocMode(INT32 &nMode);
	INT32		SendAliveMessage();
	INT32		StartSubClass();
	VOID		StopSubClass();
	VOID		DeleteSubClass();

public:
	void		WM_AgentInitialize(WPARAM wParam, LPARAM lParam);
	void		WM_MainThreadTerminated(WPARAM wParam, LPARAM lParam);
	void		WM_AgentAutoDestory(WPARAM wParam, LPARAM lParam);
	DWORD		OnThreadTimer(WPARAM wParam, LPARAM lParam);
	void		SetPolicyInfo(PASI_POLICY_INFO pPolicyInfo);
	void		GetPolicyInfo(PASI_POLICY_INFO pPolicyInfo);
	void		ClearPolicyInfo();
	void		AddWLPathStr(CString strPath, CString strHash);
	UINT32		FindWLPathStr(CString strPath, CString &strHash);
	void		ClearWLPathStr();
	UINT32		GetWLPathCnt();
	void		AddBLPathStr(CString strPath, CString strHash);
	UINT32		FindBLPathStr(CString strPath, CString &strHash);
	void		ClearBLPathStr();
	UINT32		GetBLPathCnt();
	void		SetPolicyState(INT32 nIsSetPolicy);
	INT32		GetPolicyState();
	void		ClearPolicy();
	void		WriteLogN(char *fmt,...);
	void		WriteLogE(char *fmt,...);
};

extern CAccNotifyDlg* t_AccNotifydDlg;

#endif /*_ACC_NOTIFY_DLG_H__*/


