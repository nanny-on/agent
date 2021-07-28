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

#ifndef LogicMgrLogDocH
#define LogicMgrLogDocH
//---------------------------------------------------------------------------

#define		ASI_EPS_APP_POLICY_GROUP_ID_FA_OP				30000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FA_CLEAR		31000

class CLogicMgrLogDoc	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Add_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();
	INT32		AnalyzePkt_FromMgr_Del_Ext();

	INT32		AnalyzePkt_FromMgr_Ext_Find();
	INT32		AnalyzePkt_FromMgr_Ext_Scan();
	INT32		AnalyzePkt_FromMgr_Ext_Sync();
	INT32		AnalyzePkt_FromMgr_Ext_RollBack();
	INT32		AnalyzePkt_FromMgr_Ext_RollBack_Each_Host();

public:
	INT32		DeleteLogDoc(DB_LOG_DOC& dld);

public:
	void		SetLogDoc(DB_LOG_DOC& dld);
	void		SetLogDoc_Mgr(DB_LOG_DOC& dld);
	INT32		ChkDelBackupOp(UINT32& nContinue);
	INT32		ChkBackupOp(UINT32 nDelMethod /*0*/, UINT32 nDelCnt /*7*/, UINT32 nLimitSize /*0*/, UINT32 nLimitDelCnt /*0*/, UINT32 nChkFDTType /*0*/, UINT32 nDelAfterDay /*0*/, UINT32 nUsedMode, UINT32& nContinue);

public:
	void		SendPkt_Sync(INT32 nOnceMaxNum = 1000);

public:
	CLogicMgrLogDoc();
    ~CLogicMgrLogDoc();
};

extern CLogicMgrLogDoc*		t_LogicMgrLogDoc;

#endif
