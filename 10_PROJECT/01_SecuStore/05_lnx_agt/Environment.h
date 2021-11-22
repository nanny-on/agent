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

//---------------------------------------------------------------------------

#ifndef EnvironmentH
#define EnvironmentH
//---------------------------------------------------------------------------

class   CEnvironment
{
private:
	CRegUtil	t_RegUtil;
	char 		m_pszBuf[CHAR_MAX_SIZE];
	INT32		m_nValue;

public:
	String     	m_strRootPath;
	String     	m_strUtilPath;
	String		m_strInsPath;
	String		m_strLogPath;
	String		m_strDLLPath;

public:
	String		m_strPemCertFileName;
	String		m_strPemKeyFileName;

public:
	HANDLE      m_hMainDlg;
	HANDLE      m_hThreadEvent;
	UINT32      m_nContinueOP;
	DWORD		m_dwEvntThreadID;

public:
	String		m_strHostKey;
	String		m_strHostBinVer;
	String		m_strHostPtnVer;
	UINT32		m_nHostRegTime;
	UINT32		m_nBootChkTime;
	UINT32		m_nLastOffType;
	UINT32		m_nLastOffTime;
	INT32		m_nTsDebugMode;
	UINT32		m_nCMDebugMode;
	UINT32		m_nDbLogRetention;
	UINT32		m_nFileLogRetention;
	UINT32		m_nHostSysType;

public://Con
	String		m_strLgnSvrInfoList;
	UINT32		m_nLgnSvrInfoListMode;
	UINT32		m_nRemainSockLog;
	UINT32		m_nIFPort;
	UINT32		m_nIFPortApply;
	UINT32		m_nDLPTPPort;
	String		m_strLogicalMacAddr;

public://DBMS
	UINT32		m_nRemainDBFQOpTime;
	String		m_strDBFilePath;
	DBMS_ACCOUNT_INFO	t_DBAcInfoF;
	

public:
	INT32       LoadEnv();
	INT32		LoadEnv_Local();
	INT32		LoadEnv_Con();

private:
	INT32		LoadEnv_DB();

public:
	INT32		SetReg_HostRegTime(UINT32 nTime);
	INT32		SetReg_BootChkTime(UINT32 nTime);
	
	INT32		GetReg_LastOffInfo();
	INT32		SetReg_LastOffInfo(ASI_BOOT_TYPE eBootType, UINT32 nLastOffTime = 0);
	INT32		GetReg_IsMountedEncfs(INT32 &nMountedEncfs);
	INT32		SetReg_IsMountedEncfs(INT32 nMountedEncfs);

	INT32		GetReg_WatchDocMode(INT32 &nMode);
	INT32		SetReg_WatchDocMode(INT32 nMode);

	INT32		SetReg_HostKey(String strHostKey);
	INT32		SetReg_RmKey(String strRmKey);
	INT32		SetReg_LgnSvrInfoListMode(UINT32 nMode);
	INT32		GetReg_EnableServiceOpt();
	INT32		SetReg_EnableServiceOpt(UINT32 nMode);
	INT32		SetReg_LastOffHookInfo(UINT32 nMode = 1);

	INT32		SetReg_IFPortApply(UINT32 nIFPort);
	INT32		GetReg_IFPortApply();

	INT32		SetReg_ShutdownOption(UINT32 nMode = 1);
	INT32		SetReg_AllowBlockingAppsAtShutdownByThread(UINT32 nMode = 1);
	INT32		SetReg_AllowBlockingAppsAtShutdown(UINT32 nMode = 1);
	INT32		GetReg_AllowBkicjubgAoosAtShutodwn();
	INT32		SetReg_AllUserAutoEndTasks(UINT32 nMode = 1);
	INT32		GetReg_AllUserAutoEndTasks();

	INT32		GetReg_HiberbootEnabled();
	INT32		SetReg_HiberbootEnabled(UINT32 nEnable);

public:
	INT32		GetReg_LocalEnv_TSProtectMode();
	INT32		SetReg_LocalEnv_TSProtectMode(UINT32 nMode);

	INT32		GetReg_LocalEnv_TSDebugMode();
	INT32		SetReg_LocalEnv_TSDebugMode(UINT32 nMode);

public:
	INT32		GetReg_ExPopup_OpType();
	INT32		SetReg_ExPopup_OpType(UINT32 nOpType);
	
public:
	String		GetReg_DbgEnv_DLLPath();
	UINT32		GetReg_DbgEnv_SysBoot();
	UINT32		GetReg_DbgEnv_SkipPM();
	UINT32		GetReg_DbgEnv_SkipPMApplyEnv();
	INT32		SetReg_TimeStamp(INT64 nTimeStamp);

public:
	CEnvironment();
	~CEnvironment();
};

extern CEnvironment *t_EnvInfo;

#endif
