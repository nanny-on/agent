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



#ifndef LogicMgrPoHostNotifyH
#define LogicMgrPoHostNotifyH
//---------------------------------------------------------------------------

class CLogicMgrPoHostNotify	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();

public:
	INT32		OnTimer_Logic();

private:
	void		ExecHostNotify(PDB_PO_HOST_NOTIFY_UNIT pdphnu);
	UINT32		GetSkipTimeInfo(String strPath);
	void		ClearSkipTimeInfo(DB_PO_HOST_NOTIFY_UNIT&	dphnu);
	void		DeleteInfoFile();
	INT32		AddStartMenu(DB_PO_HOST_NOTIFY_UNIT&	dphnu);
	void		StartRunRegClear();

public:
	CLogicMgrPoHostNotify();
    ~CLogicMgrPoHostNotify();
};

extern CLogicMgrPoHostNotify*		t_LogicMgrPoHostNotify;

#endif
