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

#ifndef LogicMgrLogPatchH
#define LogicMgrLogPatchH
//---------------------------------------------------------------------------

class CLogicMgrLogPatch	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Add_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();
	INT32		AnalyzePkt_FromMgr_Del_Ext();

	INT32		AnalyzePkt_FromMgr_Ext_Sync();

public:
	INT32		StartDownLoad(TListDBLogPatch& tLogList, UINT32 nPoUnitID = 0);
	INT32		AddDpDownInfoPatch(PDB_LOG_PATCH pdata, UINT32 nPoUnitID = 0);

public:
	INT32		ApplyExecuteEnd(UINT32 nWorkID);

public:
	INT32		IsWaitBootByPatch(UINT32 nID);

public:
	void		SetLogPatch(DB_LOG_PATCH& data);
	void		SetLogPatch(TListDBLogPatch& tLogList);

public:
	CLogicMgrLogPatch();
    ~CLogicMgrLogPatch();
};

extern CLogicMgrLogPatch*		t_LogicMgrLogPatch;

#endif
