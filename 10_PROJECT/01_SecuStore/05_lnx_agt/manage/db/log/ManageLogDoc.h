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

#ifndef ManageLogDocH
#define ManageLogDocH
//---------------------------------------------------------------------------

class CManageLogDoc : public CManageBase<DB_LOG_DOC>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddLogDoc(DB_LOG_DOC&	dld);
    INT32					EditLogDoc(DB_LOG_DOC&	dld);
    INT32					DelLogDoc(UINT32 nID);

public:
	INT32					AddLogDocWithHost(DB_LOG_DOC&	dld);
	INT32					EditLogDocWithHost(DB_LOG_DOC&	dld);

public:
	INT32					IsExistLogDoc(DB_LOG_DOC&	dld);
	PDB_LOG_DOC				GetExistLogDoc(DB_LOG_DOC&	dld);

	INT32					IsExistLogDocByBkName(String strBkName, UINT32 nChkRmTime = 0, UINT32 nChkBkTime = 0);
	INT32					ResetDocLog(BOOL bDelOnlyDB = FALSE);

public:
	INT32					SetPktSync(TListPVOID& tIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(PDB_LOG_DOC pdld, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_DOC& dld);

	INT32					SetPkt_Link(MemToken& SendToken);
	INT32					SetPkt_Link(PDB_LOG_DOC pdld, MemToken& SendToken);
	INT32					GetPkt_Link(MemToken& RecvToken, DB_LOG_DOC& dld);

public:
	CManageLogDoc();
    ~CManageLogDoc();

};

extern CManageLogDoc*	t_ManageLogDoc;

#endif
