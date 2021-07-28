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

#ifndef LogicMgrPtnGWCH
#define LogicMgrPtnGWCH
//---------------------------------------------------------------------------

class CLogicMgrPtnGWC	: public CLogicBase
{
private:
//	CASIDBFQDLLUtil*	m_tASIDBFQDLLUtil;
	INT32				m_nInitLogic;
	String				m_strDBFName;
		
private:
	INT32	OpenDBFQ();
	INT32	CloseDBFQ();

public:
	INT32	InitPtnGWC();
	INT32	FreePtnGWC();

	INT32	ReloadPtnGWC();
	INT32	IsInitLogic()	{	return m_nInitLogic;	};

public:
	CLogicMgrPtnGWC();
    ~CLogicMgrPtnGWC();
};

extern CLogicMgrPtnGWC*		t_LogicMgrPtnGWC;

#endif
