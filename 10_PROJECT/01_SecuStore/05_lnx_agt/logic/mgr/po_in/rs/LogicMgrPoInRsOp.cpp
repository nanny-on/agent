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


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPoInRsOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoInRsOp*		t_LogicMgrPoInRsOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInRsOp::CLogicMgrPoInRsOp()
{
	t_ManagePoInRsOp			= new CManagePoInRsOp();
	t_ManagePoInRsOpPkg			= new CManagePoInRsOpPkg();
	t_ManagePoInRsOpUnit		= new CManagePoInRsOpUnit();

	t_ManagePoInRsOpUnitObjPkg	= new CManagePoInRsOpUnitObjPkg();
	t_ManagePoInRsOpUnitSubPkg	= new CManagePoInRsOpUnitSubPkg();
	
	t_ManagePoInRsOpObjUnit	= new CManagePoInRsOpObjUnit();
	t_ManagePoInRsOpSubUnit	= new CManagePoInRsOpSubUnit();


	t_ManagePoInRsOp->LoadDBMS();
	t_ManagePoInRsOpPkg->LoadDBMS();
	t_ManagePoInRsOpUnit->LoadDBMS();

	t_ManagePoInRsOpUnitObjPkg->LoadDBMS();
	t_ManagePoInRsOpUnitSubPkg->LoadDBMS();

	t_ManagePoInRsOpObjUnit->LoadDBMS();
	t_ManagePoInRsOpSubUnit->LoadDBMS();

	{
		t_ManagePoInRsOpUnitObjPkg->InitPkg();
		t_ManagePoInRsOpUnitSubPkg->InitPkg();
	}

	t_ManagePoInRsOp->InitHash();

	m_strLogicName		= "mgr agt po in rs op";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_RS_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_RS_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_RS_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoInRsOp::~CLogicMgrPoInRsOp()
{
	SAFE_DELETE(t_ManagePoInRsOpObjUnit);
	SAFE_DELETE(t_ManagePoInRsOpSubUnit);

	SAFE_DELETE(t_ManagePoInRsOpUnitObjPkg);
	SAFE_DELETE(t_ManagePoInRsOpUnitSubPkg);

	SAFE_DELETE(t_ManagePoInRsOpUnit);
	SAFE_DELETE(t_ManagePoInRsOpPkg);
	SAFE_DELETE(t_ManagePoInRsOp);

}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInRsOp::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInRsOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_RS_OP pdata = NULL;
	DB_PO_IN_RS_OP data;

	m_tDPH = &(data.tDPH);

	TListDBPoInRsOpPkg	tPkgList;
	TListDBPoInRsOpUnit	tUnitList;

	TListDBPoInRsOpUnitObjPkg	tObjPkgList;
	TListDBPoInRsOpUnitSubPkg	tSubPkgList;

	TMapDBPoInRsOpObjUnit		tObjMap;
	TMapDBPoInRsOpSubUnit		tSubMap;

	if( t_ManagePoInRsOp->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_RS_OP_PKG	datap;		
		DB_PO_IN_RS_OP_UNIT datau;

		if( t_ManagePoInRsOpPkg->GetPkt(RecvToken, datap))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInRsOpUnit->GetPkt(RecvToken, datau))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_IN_RS_OP_UNIT_SUB_PKG sub_pkg;
				DB_PO_IN_RS_OP_SUB_UNIT sub_unit;

				if( t_ManagePoInRsOpUnitSubPkg->GetPkt(RecvToken, sub_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoInRsOpSubUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tSubPkgList.push_back(sub_pkg);
				tSubMap[sub_unit.tDPH.nID] = sub_unit;

				datau.tDPH._add_key_id_value(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block();
		}

		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_IN_RS_OP_UNIT_OBJ_PKG sub_pkg;
				DB_PO_IN_RS_OP_OBJ_UNIT sub_unit;

				if( t_ManagePoInRsOpUnitObjPkg->GetPkt(RecvToken, sub_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoInRsOpObjUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tObjPkgList.push_back(sub_pkg);
				tObjMap[sub_unit.tDPH.nID] = sub_unit;

				datau.tDPH._add_key_id_value(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block();
		}
		RecvToken.TokenSkip_Block();	
		
		tPkgList.push_back(datap);
		tUnitList.push_back(datau);

		data.tDPH._add_id_value(datap.tDPH.nID, datap.tDPH.nUnitID);
	}
	
	pdata = (PDB_PO_IN_RS_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInRsOpPkg->ClearPkgUnitByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInRsOp->DelPoInRsOp(pdata->tDPH.nID);
	}

	{
		{
			{
				TMapDBPoInRsOpSubUnitItor begin, end;
				begin = tSubMap.begin();	end = tSubMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpSubUnit->ApplyPoInRsOpSubUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub subject unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TMapDBPoInRsOpObjUnitItor begin, end;
				begin = tObjMap.begin();	end = tObjMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpObjUnit->ApplyPoInRsOpObjUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub object unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}
		}

		{
			{
				TListDBPoInRsOpUnitSubPkgItor begin, end;
				begin = tSubPkgList.begin();	end = tSubPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpUnitSubPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub subject pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoInRsOpUnitSubPkg->AddPoInRsOpUnitSubPkg(*begin);
				}
			}
			{
				TListDBPoInRsOpUnitObjPkgItor begin, end;
				begin = tObjPkgList.begin();	end = tObjPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpUnitObjPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub object pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoInRsOpUnitObjPkg->AddPoInRsOpUnitObjPkg(*begin);
				}
			}
		}	

		{
			{
				TListDBPoInRsOpUnitItor begin, end;
				begin = tUnitList.begin();	end = tUnitList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpUnit->ApplyPoInRsOpUnit(*begin))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TListDBPoInRsOpPkgItor begin, end;
				begin = tPkgList.begin();	end = tPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoInRsOpPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoInRsOpPkg->AddPoInRsOpPkg(*begin);
				}
			}

			if(SetER(t_ManagePoInRsOp->ApplyPoInRsOp(data)))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
			}
		}
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

