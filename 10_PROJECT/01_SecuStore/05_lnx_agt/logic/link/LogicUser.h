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

#ifndef LogicUserH
#define LogicUserH
//---------------------------------------------------------------------------

class CLogicUser	: public CLogicBase
{
private:	
	INT32		AnalyzePkt_FromLink_Ext_Auth();
	INT32		AnalyzePkt_FromLink_Ext_End();

	INT32		AnalyzePkt_FromLink_Ext_ChgPW();

public:
	INT32		AnalyzePkt_FromLink_User(PPKT_DATA pkt_data);

public:
	void		SendPkt_Auth(UINT32 nRtn, DB_USER& du);
	void		SendPkt_Logout(UINT32 nRtn);
	
	void		SendPkt_UserChgPW(UINT32 nRtn, DB_USER& du);

public:
	CLogicUser();
    ~CLogicUser();
};

extern CLogicUser*		t_LogicUser;

#endif
