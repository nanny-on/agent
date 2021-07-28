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

#ifndef ManageDocDeleteInfoH
#define ManageDocDeleteInfoH
//---------------------------------------------------------------------------

class CManageDocDeleteInfo : public CManageBase<DB_DOC_DELETE_INFO>
{
public:
	INT32				LoadDBMS();
	INT32				AddDocDeleteInfo(DB_DOC_DELETE_INFO&	dddi);
	INT32				EditDocDeleteInfo(DB_DOC_DELETE_INFO&	dddi);

public:
	void				UpdateDocScanTime();
	void				DocDeleteCount(UINT32 nOpType);
	void				UpdateDocDeleteInfo();
	void				GetData(DB_DOC_DELETE_INFO& tData)	{	tData = m_tData;	};

public:
	INT32				SetPkt(MemToken& SendToken);
    INT32				SetPkt(PDB_DOC_DELETE_INFO pddi, MemToken& SendToken);
private:
	UINT32				m_nDocCnt;
	UINT32				m_nSchCnt;
	UINT32				m_nMenualCnt;
	DB_DOC_DELETE_INFO	m_tData;
    
public:
	CManageDocDeleteInfo();
    ~CManageDocDeleteInfo();

};

extern CManageDocDeleteInfo*	t_ManageDocDeleteInfo;

#endif
