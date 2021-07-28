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

#ifndef LogicMgrPoHostWebSockH
#define LogicMgrPoHostWebSockH
//---------------------------------------------------------------------------

class CLogicMgrPoHostWebSock	: public CLogicBase
{

private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();

public:
	INT32		ApplyPolicy();
	INT32		ChkPolicy(PASI_WEBSOCK_MSG pdata);

public:
	INT32		ChkPolicy_InVulnOp(PDB_PO_HOST_WEB_SOCK pdata_pol, String& strMsg);
	INT32		ChkPolicy_InPtnOp(PDB_PO_HOST_WEB_SOCK pdata_pol, String& strMsg);

public:
	INT32		SendMsg(PASI_WEBSOCK_MSG pdata, String strMsg);
	
public:
	CLogicMgrPoHostWebSock();
    ~CLogicMgrPoHostWebSock();
};

extern CLogicMgrPoHostWebSock*		t_LogicMgrPoHostWebSock;

#endif
