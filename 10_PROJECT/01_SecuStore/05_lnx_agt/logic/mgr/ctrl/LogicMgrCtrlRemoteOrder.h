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

#ifndef LogicMgrCtrlRemoteOrderH
#define LogicMgrCtrlRemoteOrderH
//---------------------------------------------------------------------------

class CLogicMgrCtrlRemoteOrder	: public CLogicBase
{ 
private:
	INT32		AnalyzePkt_FromMgr_Ext();

	INT32		AnalyzePkt_FromMgr_Ext_Ctrl();

private:
	INT32		ApplyRemotePolicy(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_FEScan(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NCScan(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NCReSend(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NCMakeLoPtn(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NOScan(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NOReSend(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_ReGetSysInfo();
	INT32		ApplyRemotePolicy_NPScan(PMEM_CTRL_REMOTE_ORDER pMCRO);
	INT32		ApplyRemotePolicy_NO_VUINScan(PMEM_CTRL_REMOTE_ORDER pMCRO);


public:
	CLogicMgrCtrlRemoteOrder();
    ~CLogicMgrCtrlRemoteOrder();
};

extern CLogicMgrCtrlRemoteOrder*		t_LogicMgrCtrlRemoteOrder;

#endif
