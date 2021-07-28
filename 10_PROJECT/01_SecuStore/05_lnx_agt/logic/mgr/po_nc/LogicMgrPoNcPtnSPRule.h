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

#ifndef LogicMgrPoNcPtnSPRuleH
#define LogicMgrPoNcPtnSPRuleH
//---------------------------------------------------------------------------

class CLogicMgrPoNcPtnSPRule	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();
	INT32		AnalyzePkt_FromMgr_Ext_Sync();

public:
	INT32		OnTimer_Logic();

public:
	void		SendPkt_Req(PDB_PO_NC_PTN_SP_RULE pdata);
	void		SendPkt_Sync(INT32 nOnceMaxNum = 1000);

public:
	CLogicMgrPoNcPtnSPRule();
    ~CLogicMgrPoNcPtnSPRule();
};

extern CLogicMgrPoNcPtnSPRule*		t_LogicMgrPoNcPtnSPRule;

#endif
