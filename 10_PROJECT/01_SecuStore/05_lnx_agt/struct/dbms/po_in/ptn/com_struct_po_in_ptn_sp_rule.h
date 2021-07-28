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

#ifndef DB_PO_IN_PTN_SP_RULE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_IN_PTN_SP_RULE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_in_ptn_sp_rule
{
	_db_po_in_ptn_sp_rule()
	{
		nTargetType		= SS_PO_IN_PTN_SP_TARGET_TYPE_HOST;
		nTargetID		= 0;
		nFileID			= 0;

		nReqLevel		= SS_PO_IN_PTN_SP_REQ_LEVEL_NORMAL;
		nReqDay			= 0;

		nConfirmType	= SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_WAIT;
		nConfirmID		= 0;
		nConfirmRst		= SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_WAIT;

		nUseDay			= SS_PO_IN_PTN_SP_CONFIRM_DAY_UNLIMITED;
		nEvtDate		= 0;
		nEndDate		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nTargetType;
	UINT32				nTargetID;
	UINT32				nFileID;
	UINT32				nReqLevel;
	UINT32				nReqDay;
	UINT32				nConfirmType;
	UINT32				nConfirmID;
	UINT32				nConfirmRst;
	UINT32				nUseDay;
	String				strFileKey;
	String				strFilePath;
	String				strFileName;
	UINT32				nEvtDate;
	UINT32				nEndDate;

}DB_PO_IN_PTN_SP_RULE, *PDB_PO_IN_PTN_SP_RULE;

typedef list<DB_PO_IN_PTN_SP_RULE>				TListDBPoInPtnSPRule;
typedef TListDBPoInPtnSPRule::iterator			TListDBPoInPtnSPRuleItor;

typedef map<UINT32, DB_PO_IN_PTN_SP_RULE>		TMapDBPoInPtnSPRule;
typedef TMapDBPoInPtnSPRule::iterator			TMapDBPoInPtnSPRuleItor;
//---------------------------------------------------------------------------

#endif //DB_PO_IN_PTN_SP_RULE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



