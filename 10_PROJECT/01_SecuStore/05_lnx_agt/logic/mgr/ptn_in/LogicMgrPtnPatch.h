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
//---------------------------------------------------------------------------

#ifndef LogicMgrPtnPatchH
#define LogicMgrPtnPatchH
//---------------------------------------------------------------------------

class CLogicMgrPtnPatch	: public CLogicBase
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

	volatile  LONG		m_nIsRunRBMode;

private:
	INT32	OpenDBFQ();
	INT32	CloseDBFQ();

public:
	INT32	InitPtnPatch();
	INT32	FreePtnPatch();

	INT32	ReloadPtnPatch();
	INT32	IsInitLogic()	{	return m_nInitLogic;	};

	INT32	IsEnableRunMode(UINT32 nType);
	INT32	SetEnableRunMode(UINT32 nType, UINT32 nMode);

public:
	INT32	ChkPtnPatchWsus();
	INT32	ApplyPtnPatchWsus(UINT32 nID);

	INT32	GetPatchWsusStatus()					{	return m_nInitPatchWsus;		};
	INT32	SetPatchWsusStatus(UINT32 nStatus)		{	m_nInitPatchWsus = nStatus;	};
	String	GetLastPatchWsusFName()					{	return m_strWScanFileName;		};

private:
	INT32	ChkPtnPatchWsusUnit(PDB_PTN_PATCH_WSUS pdata, UINT32 nChkOnly = 0);
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

// public:
// 	INT32	ChkPtnPatchUtil();
// 	INT32	ApplyPtnPatchUtil(UINT32 nID);
// 
// 	INT32	GetPatchUtilStatus()					{	return m_nInitPatchUtil;	};
// 	INT32	SetPatchUtilStatus(UINT32 nStatus)		{	m_nInitPatchUtil = nStatus;	};
// 	INT32	IsWaitBootByPatchUtil();
// 
// private:
// 	INT32	ChkPtnPatchUtilUnit(PDB_PTN_PATCH_UTIL pdata, UINT32 nChkOnly = 0);
// 	INT32	AddDpDownInfoUtil(PDB_PTN_PATCH_UTIL pdata);
// 


public:
	CLogicMgrPtnPatch();
    ~CLogicMgrPtnPatch();
};

extern CLogicMgrPtnPatch*		t_LogicMgrPtnPatch;

#endif
