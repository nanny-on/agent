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

#ifndef LogicAuthH
#define LogicAuthH
//---------------------------------------------------------------------------

class CLogicAuth	: CLogicBase
{
private:
	void		AnalyzePkt_Auth_Login();

public:
	void		AnalyzePkt_Auth(PPKT_DATA pkt_data);

public:
	void		SendLinkInitData();

public:
	CLogicAuth();
    ~CLogicAuth();
};

extern CLogicAuth*		t_LogicAuth;

#endif
