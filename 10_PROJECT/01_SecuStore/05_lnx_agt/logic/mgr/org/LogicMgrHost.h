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

#ifndef LogicMgrHostH
#define LogicMgrHostH
//---------------------------------------------------------------------------

class CLogicMgrHost	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();

	INT32		AnalyzePkt_FromMgr_Ext_Sync();
	INT32		AnalyzePkt_FromMgr_Ext_Used();
	INT32		AnalyzePkt_FromMgr_Ext_SetName();
	INT32		AnalyzePkt_FromMgr_Ext_SyncEnd();

public:
	void		SendPkt_Edit();
	void		SendPkt_InIt();

public:
	void		SetUninstallHost();
	void		SendPkt_SyncEnd();

public:
	CLogicMgrHost();
    ~CLogicMgrHost();
};

extern CLogicMgrHost*		t_LogicMgrHost;

#endif
