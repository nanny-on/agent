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

#ifndef ManagePoInPtnSPRuleH
#define ManagePoInPtnSPRuleH
//---------------------------------------------------------------------------

class CManagePoInPtnSPRule : public CManagePoBase<DB_PO_IN_PTN_SP_RULE>
{
public:
	INT32					LoadDBMS();
	CMutexExt				m_tDataMutex;
public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data);
    INT32					EditPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data);
	INT32					DelPoInPtnSPRule(UINT32 nID);
	INT32					ApplyPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data);
public:
	String					GetName(UINT32 nID);    
	UINT32					GetNextLocalID();
	INT32					GetItemCopy(TListDBPoInPtnSPRule& tRuleList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_PTN_SP_RULE pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_SP_RULE& data);

	INT32					SetPktSync(TListPVOID& tIDList);
public:
	CManagePoInPtnSPRule();
    ~CManagePoInPtnSPRule();

};

extern CManagePoInPtnSPRule*	t_ManagePoInPtnSPRule;

#endif
