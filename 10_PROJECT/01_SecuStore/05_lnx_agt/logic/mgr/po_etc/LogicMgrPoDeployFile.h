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

#ifndef LogicMgrPoDeployFileH
#define LogicMgrPoDeployFileH
//---------------------------------------------------------------------------

class CLogicMgrPoDeployFile	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();

public:
	INT32		ApplyPolicy();
	INT32		OnTimer_Logic();

	INT32		ChkEndDpFile();

private:
	INT32		IsValidSchedule(UINT32 nUnitID, TListID& tValidSchIDList);
	INT32		IsValidCheck(UINT32 nUnitID);
	INT32		IsValidTarget(UINT32 nUnitID);
	INT32		AddDpDownInfo(UINT32 nPoID, UINT32 nUnitID, TListID& tSchPkgIDList);

private:
	INT32		ComppareInfo_AppWiz(UINT32 nChkUnitID);
	INT32		ComppareInfo_Registry(UINT32 nChkUnitID);
	INT32		ComppareInfo_FilePath(UINT32 nChkUnitID);
	INT32		ComppareInfo_FileVersion(UINT32 nChkUnitID);
	INT32		ComppareInfo_Process(UINT32 nChkUnitID);

public:
	static VOID	GetSoftwareInfo(PASISI_SW_INFO pasi, INT32 nSize, PVOID lParam);

public:
	CLogicMgrPoDeployFile();
    ~CLogicMgrPoDeployFile();
};

extern CLogicMgrPoDeployFile*		t_LogicMgrPoDeployFile;

#endif
