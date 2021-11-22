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
 */
//---------------------------------------------------------------------------

#ifndef LogicMgrPtnVulnH
#define LogicMgrPtnVulnH
//---------------------------------------------------------------------------

class CLogicMgrPtnVuln	: public CLogicBase
{
private:
//	CASIDBFQDLLUtil*	m_tASIDBFQDLLUtil;
	INT32				m_nInitLogic;
	String				m_strDBFName;
//	CSystemInfo			m_tSysInfo;

	INT32				m_nInitPatchUtil;
	INT32				m_nInitPatchWsus;
	INT32				m_nInitPtnDeploy;

	String				m_strWScanFileName;
	INT32				m_nIsLiveUpdate;

	volatile  LONG		m_nIsRunRBMode;

private:
	INT32	OpenDBFQ();
	INT32	CloseDBFQ();

public:
	INT32	InitPtnVuln();
	INT32	FreePtnVuln();

	INT32	ReloadPtnVuln();
	INT32	IsInitLogic()	{	return m_nInitLogic;	};

	INT32	IsEnableRunMode(UINT32 nType);
	INT32	SetEnableRunMode(UINT32 nType, UINT32 nMode);

public:
	INT32	InitPtnVulnWsus();
	INT32	ChkPtnVulnWsus();
	INT32	ApplyPtnVulnWsus(UINT32 nID);

	INT32	GetVulnWsusStatus()						{	return m_nInitPatchWsus;		};
	INT32	SetVulnWsusStatus(UINT32 nStatus)		{	m_nInitPatchWsus = nStatus;	};
	String	GetLastVulnWsusFName()					{	return m_strWScanFileName;		};

	INT32	IsVulnWsusLive();

private:
	INT32	ChkPtnVulnWsusUnit(PDB_PTN_PATCH_WSUS pdata, UINT32 nChkOnly = 0);
	INT32	AddDpDownInfoWsus(PDB_PTN_PATCH_WSUS pdata);

public:
	INT32	InitPtnDeploy();
	INT32	ChkPtnDeploy();
	INT32	ApplyPtnDeploy(UINT32 nID);

	INT32	GetPtnDeployStatus()					{	return m_nInitPtnDeploy;	};
	INT32	SetPtnDeployStatus(UINT32 nStatus)		{	m_nInitPtnDeploy = nStatus;	};
	
private:
	INT32	ChkPtnDeployUnit(PDB_PTN_DEPLOY pdata, UINT32 nChkOnly = 0);
	INT32	AddDownloadPtnDeployUnit(PDB_PTN_DEPLOY pdata);

public:
	CLogicMgrPtnVuln();
    ~CLogicMgrPtnVuln();
};

extern CLogicMgrPtnVuln*		t_LogicMgrPtnVuln;

#endif
