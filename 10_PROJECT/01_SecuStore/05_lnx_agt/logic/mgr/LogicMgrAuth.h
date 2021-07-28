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

#ifndef LogicMgrAuthH
#define LogicMgrAuthH
//---------------------------------------------------------------------------

class CLogicMgrAuth	: CLogicBase
{
private:
	void		AnalyzePkt_Auth_Login();
	void		AnalyzePkt_Auth_Last();

public:
	void		AnalyzePkt_Auth(PPKT_DATA pkt_data);

public:
	void		SendAuthLogin();
	void		SendMgrInitData(UINT32 nOpt = 0);
	void		SendMgrInitData_Polcy();
	void		SendMgrInitData_Sync();

public:
	CLogicMgrAuth();
    ~CLogicMgrAuth();
};

extern CLogicMgrAuth*		t_LogicMgrAuth;

#endif
