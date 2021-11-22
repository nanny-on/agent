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

#ifndef ManageProcessInfoH
#define ManageProcessInfoH

class CManageProcessInfo : public CManageBase<MEM_PROCESS_INFO>
{
private:
	
public:
	void	AddProcessInfo(MEM_PROCESS_INFO& data);
	void	DelProcessInfo(UINT32 nID);


public:
	INT32	SetPkt(MemToken& SendToken);
	INT32	SetPkt(PMEM_PROCESS_INFO pdata, MemToken& SendToken);
	INT32	GetPkt(MemToken& RecvToken, MEM_PROCESS_INFO& data);

public:
	CManageProcessInfo(void);
	~CManageProcessInfo(void);
};

extern CManageProcessInfo*	t_ManageProcessInfo;

#endif