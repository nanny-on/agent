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


#ifndef ManagePoDvDefaultH
#define ManagePoDvDefaultH
//---------------------------------------------------------------------------

class CManagePoDvDefault : public CManagePoBase<DB_PO_DV_DEFAULT>
{
public:
	INT32					LoadDBMS();

public:
	
public:
	INT32					AddPoDvDefault(DB_PO_DV_DEFAULT&	didi);
    INT32					EditPoDvDefault(DB_PO_DV_DEFAULT&	didi);
    INT32					DelPoDvDefault(UINT32 nID);
	INT32					ApplyPoDvDefault(DB_PO_DV_DEFAULT&	didi);
	INT32					ResetPoDvDefault();
	String					FindDvPdName(TListDeviceInfo& tList, String strDvInstanceID);
	INT32					DeleteAllDvDefault();
	INT32					DeleteDvDefault(UINT32 nID);
	INT32					FindDvPolID(String strDvInstanceID, String strDvPaInstanceID, UINT32 nUsed = 1);
    INT32					FindDvPolID(String strDvPaInstanceID, UINT32 nUsed = 1);
	UINT32					GetDvDefaultCnt();


public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken, BOOL nIsUse = FALSE);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_DV_DEFAULT pdidi, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_DV_DEFAULT& didi);

public:
	CManagePoDvDefault();
    ~CManagePoDvDefault();

};

extern CManagePoDvDefault*	t_ManagePoDvDefault;

#endif
