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

#ifndef ManageLogDocHostH
#define ManageLogDocHostH
//---------------------------------------------------------------------------

class CManageLogDocHost : public CManageBase<DB_LOG_DOC_HOST>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddLogDocHost(DB_LOG_DOC_HOST&	data);
    INT32					EditLogDocHost(DB_LOG_DOC_HOST&	data);
    INT32					DelLogDocHost(UINT32 nID);

public:
	UINT32					GetLogDocHostEvtTime(UINT32 nLogID);

public:
	CManageLogDocHost();
    ~CManageLogDocHost();

};

extern CManageLogDocHost*	t_ManageLogDocHost;

#endif
