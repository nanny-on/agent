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

// EnvironmentOp.h: interface for the CEnvironmentOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENTOP_H__B01BCC89_1DA6_4BA2_AF0E_DB38CCACC9E8__INCLUDED_)
#define AFX_ENVIRONMENTOP_H__B01BCC89_1DA6_4BA2_AF0E_DB38CCACC9E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEnvironmentOp  
{

private:
	UINT32					m_nGlobalID;
	HWND					m_hMainDlg;
	INT32					m_nMainContinue;
	INT32					m_nLgnSvrInfoIdx;
	INT32					m_nMgrSvrInfoIdx;
	INT32					m_nUdtChkSvrInfoIdx;
	INT32					m_nUdtDataSvrInfoIdx;
	pthread_mutex_t			m_env_mutex;
	INT32					m_nMgrSvrAuthStatus;
	TMapMemSvrInfo			m_tMgrSvrInfoMap;
	TMapMemSvrInfo			m_tUdtChkSvrInfoMap;
	TMapMemSvrInfo			m_tUdtDataSvrInfoMap;

	TMapMemAgtUpFileInfo	m_tMemAgtUpFileInfoMap;

public:
	String					m_strPriIPAddr;
	String					m_strPriMacAddr;

public:
	INT32					m_nApplyPolicyStatus;
	INT32					m_nSysBootMode;
	INT32					m_nModuleUpdateMode;
	BOOL					m_bSkipReConnectLgnSvr;

public:
	INT32					m_nSysOffMode;
	UINT32					m_nSysOffFlag;
	DWORD					m_nSysOffReason;
	UINT32					m_nSysOffTime;
	UINT32					m_nSysLocaleID;

private:
	UINT32					m_nInjectFlag;
	UINT32					m_nStopOpBySysOff;

public:
	INT32			InitEnvOp();

public:
	UINT32			GetGlobalID()							{	return m_nGlobalID++;		};
	void			SetMainHandle(HWND hMainDlg)			{	m_hMainDlg = hMainDlg;		};
	HWND			GetMainHandle()							{	return m_hMainDlg;			};
	void			SetMainContinue(INT32 nFlag = 0)		{	m_nMainContinue = nFlag;	};
	INT32			GetMainContinue()						{	return m_nMainContinue;		};	
	void			SetStopOpBySysOff(UINT32 nFlag = 0)		{	m_nStopOpBySysOff = nFlag;	};
	UINT32&			GetStopOpBySysOff()						{	return m_nStopOpBySysOff;	};

public:
	String			GetComputerName();
	String			GetRootPath();
	String			GetUserUniCode();
    UINT32			GetGUIThreadLocale();


private:
	INT32			ResetEnvPath();

public:
	void			SetMgrSvrAuthStatus(INT32 nMgrSvrAuthStatus);
	void			GetMgrSvrAuthStatus(INT32 &nMgrSvrAuthStatus);
	void			SetMgrSvrInfo(String strSvrInfoList);
	void			SetUdtChkSvrInfo(String strSvrInfoList);
	void			SetUdtDataSvrInfo(String strSvrInfoList);
	String			GetUdtDataSvrInfo();

	void			SetAgtUpFileInfo(TMapMemAgtUpFileInfo& tMemAgtUpFileInfoMap);
	ASI_FDL_INFO	GetAgtUpFileInfo();

public:
	SOCK_INIT_DATA	GetCltLgnSID(INT32 nSSLMode = 0);
	SOCK_INIT_DATA	GetCltMgrSID(INT32 nSSLMode = 0);
	SOCK_INIT_DATA	GetCltUdtSID(INT32 nSSLMode = 0);
	SOCK_INIT_DATA	GetSvrLinkSID(INT32 nSSLMode = 0, UINT32 nIFPort = 0);
	void			NextCltSvrIdx(UINT32 nType);

public:
	INT32			SetServiceRecoveryOption(INT32 nResetCount = 86400 /*sec*/, INT32 nActionMilisecond = 1800000 /*milisec*/);
	INT32			SetSysBootChkMode(UINT32 nChkType = 0, UINT32 nBootTime = 0);
	INT32			SetSysBootMode(UINT32 nSysBootMode = 1);
	INT32			IsSysBootTime();

public:
	INT32			SetSysOffMode(UINT32 nLogOffMode = 1, UINT32 nFlag = 0, DWORD dwReason = 0);
	INT32			IsSysOffMode()	{	return m_nSysOffMode;	};

public:
	void			GetDLFileInitData(ASI_FDL_INIT& tAFI);
	void			GetTSInitData(ASI_TS_INIT& tATI);
//	void			GetEPSAPPInitData(ASI_EPS_APP_INIT& tAEAI);
	void			GetDMInitData(ASIDM_INIT& tAI);
	void			GetFFInitData(ASI_FF_INIT_INFO& tAFII);
	void			GetFFPtnInitData(ASI_FF_INIT_INFO& tAFII);
	void			GetFFRsBkInitData(ASI_FF_INIT_INFO& tAFII);
	void			GetASInitData(ASISI_INIT& tASI);
	void			GetASIPMSPatchInitData(ASI_PMS_INIT& tAPI);
	void			GetASIPMSVulnInitData(ASI_PMS_INIT& tAPI);
	void			GetASIWebSockInitData(ASI_WEBSOCK_INIT& tAWSI);

public:
	void			SetIni_NotifyInfo_TimeStamp(String strSection, String strKey, UINT32 nTimeStamp);
	INT32			GetIni_NotifyInfo_TimeStamp(String strSection, String strKey);

public:
	void			SetInjectFlag(UINT32 nInjectFlag);
	UINT32			GetInjectFlag();

public:
	CEnvironmentOp();
	virtual ~CEnvironmentOp();

};

extern CEnvironmentOp*	t_EnvInfoOp;

#endif // !defined(AFX_ENVIRONMENTOP_H__B01BCC89_1DA6_4BA2_AF0E_DB38CCACC9E8__INCLUDED_)
