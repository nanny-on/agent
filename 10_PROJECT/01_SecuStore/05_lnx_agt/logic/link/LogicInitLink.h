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

#ifndef LogicInitLinkH
#define LogicInitLinkH
//---------------------------------------------------------------------------

class CLogicInitLink	: CLogicBase
{

public:
	void		AnalyzePkt_Init(PPKT_DATA pkt_data);

private:
	void		AnalyzePkt_Init_Request();
	void		AnalyzePkt_Init_Last();

private:
	void		SendPkt_Init_Common();

	void		SendPkt_Init_Admin();
	void		SendPkt_Init_Host();
	void		SendPkt_Init_Group();
	void		SendPkt_Init_User();
	void		SendPkt_Init_Env();
	void		SendPkt_Init_Ptn();
	void		SendPkt_Init_LocalEnv();
	void		SendPkt_Init_Policy();
	void		SendPkt_Init_Info();
	void		SendPkt_Init_Data();

private:
	void		SendPkt_Init_Policy_SA();
	void		SendPkt_Init_Policy_AC();
	void		SendPkt_Init_Policy_FA();
	void		SendPkt_Init_Policy_FE();
	void		SendPkt_Init_Policy_IN();
	void		SendPkt_Init_Policy_IN_VULN();
	void		SendPkt_Init_Policy_IN_RS();
	void		SendPkt_Init_Policy_IN_DEVO();
	void		SendPkt_Init_Policy_PM();
	void		SendPkt_Init_Policy_NC();

private:
	void		SendPkt_InitLast_DelLastNotify();
    
public:
	CLogicInitLink();
    ~CLogicInitLink();
};

extern CLogicInitLink*		t_LogicInitLink;

#endif
