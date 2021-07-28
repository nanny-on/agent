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

#ifndef LogicLogDocDScanH
#define LogicLogDocDScanH
//---------------------------------------------------------------------------

class CLogicLogDocDScan	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromLink_Ext();

public:
	INT32		AnalyzePkt_FromLink_Ext_Scan();
	INT32		AnalyzePkt_FromLink_Ext_Del();

public:
	void		SendPkt_Find(PDB_LOG_DOC pdld);
	void		SendPkt_Progress(PMEM_FIND_ORDER_INFO pMFOI);
	void		SendPkt_End(MEM_FIND_ORDER_INFO& tMFOI);

	void		SendPkt_Del_Last(MEM_FIND_ORDER_INFO& tMFOI);

public:
	CLogicLogDocDScan();
    ~CLogicLogDocDScan();
};

extern CLogicLogDocDScan*		t_LogicLogDocDScan;

#endif
