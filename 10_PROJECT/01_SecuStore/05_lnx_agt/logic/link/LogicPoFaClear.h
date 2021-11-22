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

#ifndef LogicPoFaClearH
#define LogicPoFaClearH
//---------------------------------------------------------------------------

class CLogicPoFaClear	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromLink_Ext();
	INT32		AnalyzePkt_FromLink_Ext_Scan();

	INT32		AnalyzePkt_FromLink_Del_Ext();

public:
	void		SendPkt_Init(UINT32 nID, UINT32 nLastChkTime);
	void		SendPkt_Find(PDB_LOG_DOC pdld);
	void		SendPkt_Progress(PMEM_FIND_ORDER_INFO pMFOI);
	void		SendPkt_End(MEM_FIND_ORDER_INFO& tMFOI);

	void		SendPkt_Del_Last(MEM_FIND_ORDER_INFO& tMFOI);
	void		SendPkt_Del_Last(UINT32 nPolicyType, UINT32 nOpType, UINT32 nTotalDelCnt);

public:
	CLogicPoFaClear();
    ~CLogicPoFaClear();
};

extern CLogicPoFaClear*		t_LogicPoFaClear;

#endif
