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

#ifndef LogicPoInPtnSPRuleH
#define LogicPoInPtnSPRuleH
//---------------------------------------------------------------------------

class CLogicPoInPtnSPRule	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromLink_Ext();
	INT32		AnalyzePkt_FromLink_Ext_Req();

	INT32		AnalyzePkt_FromLink_Del_Ext();

private:
	INT32		ChkAutoConfirm(PDB_PO_IN_PTN_SP_RULE pdata);
	INT32		IsExistMatchInfo(PDB_PO_IN_PTN_SP_RULE pdata);

public:
	CLogicPoInPtnSPRule();
    ~CLogicPoInPtnSPRule();
};

extern CLogicPoInPtnSPRule*		t_LogicPoInPtnSPRule;

#endif
