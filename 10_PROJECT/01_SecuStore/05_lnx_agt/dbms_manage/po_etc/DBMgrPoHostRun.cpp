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
#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoHostRun.h"

//---------------------------------------------------------------------------

CDBMgrPoHostRun*		t_DBMgrPoHostRun = NULL;

//---------------------------------------------------------------------------

CDBMgrPoHostRun::CDBMgrPoHostRun()
{
	m_strDBTName = "po_host_run";
}
//---------------------------------------------------------------------------

CDBMgrPoHostRun::~CDBMgrPoHostRun()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRun::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRun::LoadDB(TListDBPoHostRun& tDBPoHostRunList)
{
    UINT32 nReadCnt = 0;
    DB_PO_HOST_RUN dphr;
    String strMSOPkg, strMSOCls;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", run_option, rm_log_day, lgn_con_period, mgr_show_type, show_log_day, boot_chk_type"
						", integrity_mode, cross_auth_mode "
						", once_pkt_num"
						", mgr_show_order_pkg, mgr_show_order_cls"
						", conn_manage, log_reset"
						",lnx_run_option, lnx_rm_log_day, lnx_lgn_con_period, lnx_boot_chk_type, lnx_once_pkt_num, lnx_conn_manage"
						" FROM po_host_run WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dphr.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		if(tDPH.nID != 0)
		{
			dphr.nRunOption				= GetDBField_UInt(nIndex++);
			dphr.nRmLogDay				= GetDBField_UInt(nIndex++);
			dphr.nLgnConPeriod			= GetDBField_UInt(nIndex++);
			dphr.nMgrShowType			= GetDBField_UInt(nIndex++);
			dphr.nShowLogDay			= GetDBField_UInt(nIndex++);
			dphr.nBootChkType			= GetDBField_UInt(nIndex++);
			dphr.nIntegrityMode			= GetDBField_UInt(nIndex++);
			dphr.nCrossAuthMode			= GetDBField_UInt(nIndex++);
			dphr.nOncePktNum			= GetDBField_UInt(nIndex++);

			strMSOPkg					= GetDBField_String(nIndex++);
			HexToMap_64(strMSOPkg, dphr.tMgrShowOrderPkgMap, SS_PO_HOST_RUN_MRG_SHOW_ORDER_PKG_NUM);
			strMSOCls					= GetDBField_String(nIndex++);
			HexToMap_64(strMSOCls, dphr.tMgrShowOrderClsMap, SS_PO_HOST_RUN_MRG_SHOW_ORDER_CLS_NUM);

			dphr.nConnManage			= GetDBField_UInt(nIndex++);
			dphr.nLogReset				= GetDBField_UInt(nIndex++);

			dphr.nLnxRunOption			= GetDBField_UInt(nIndex++);
			dphr.nLnxRmLogDay			= GetDBField_UInt(nIndex++);
			dphr.nLnxLgnConPeriod		= GetDBField_UInt(nIndex++);
			dphr.nLnxBootChkType		= GetDBField_UInt(nIndex++);
			dphr.nLnxOncePktNum			= GetDBField_UInt(nIndex++);
			dphr.nLnxConnManage			= GetDBField_UInt(nIndex++);
        
			tDBPoHostRunList.push_back(dphr);
			if(m_nLoadMaxID < UINT32(tDPH.nID))
				m_nLoadMaxID = tDPH.nID;
			nReadCnt++;
		}
    }while(Next());
	m_nLoadNumber = tDBPoHostRunList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRun::InsertPoHostRun(DB_PO_HOST_RUN& dphr)
{
	DB_PO_HEADER& tDPH = dphr.tDPH;
	String strMSOPkg, strMSOCls;
	{
		MapToHex_64(dphr.tMgrShowOrderPkgMap, strMSOPkg, SS_PO_HOST_RUN_MRG_SHOW_ORDER_PKG_NUM);
		MapToHex_64(dphr.tMgrShowOrderClsMap, strMSOCls, SS_PO_HOST_RUN_MRG_SHOW_ORDER_CLS_NUM);
	}


	m_strQuery = SPrintf("INSERT INTO po_host_run("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", run_option, rm_log_day, lgn_con_period, mgr_show_type, show_log_day, boot_chk_type"
						", integrity_mode, cross_auth_mode "
						", once_pkt_num"
						", mgr_show_order_pkg, mgr_show_order_cls"
						", conn_manage, log_reset"
						",lnx_run_option, lnx_rm_log_day, lnx_lgn_con_period, lnx_boot_chk_type"
						", lnx_once_pkt_num, lnx_conn_manage"
						") VALUES (%s"
                        ", %u, %u, %u, %u, %u, %u"
						", %u, %u"
						", %u"
						", '%s', '%s'"
						", %u, %u"
						", %u, %u, %u, %u"
						", %u, %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dphr.nRunOption, dphr.nRmLogDay, dphr.nLgnConPeriod, dphr.nMgrShowType, dphr.nShowLogDay, dphr.nBootChkType, 
						dphr.nIntegrityMode, dphr.nCrossAuthMode,
						dphr.nOncePktNum,
						strMSOPkg.c_str(), strMSOCls.c_str(),
						dphr.nConnManage, dphr.nLogReset,
						dphr.nLnxRunOption, dphr.nLnxRmLogDay, dphr.nLnxLgnConPeriod, dphr.nLnxBootChkType,
						dphr.nLnxOncePktNum, dphr.nLnxConnManage);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRun::UpdatePoHostRun(DB_PO_HOST_RUN& dphr)
{
	DB_PO_HEADER& tDPH = dphr.tDPH;
	String strMSOPkg, strMSOCls;
	{
		MapToHex_64(dphr.tMgrShowOrderPkgMap, strMSOPkg, SS_PO_HOST_RUN_MRG_SHOW_ORDER_PKG_NUM);
		MapToHex_64(dphr.tMgrShowOrderClsMap, strMSOCls, SS_PO_HOST_RUN_MRG_SHOW_ORDER_CLS_NUM);
	}

	m_strQuery = SPrintf("UPDATE po_host_run SET %s"
						", run_option=%u, rm_log_day=%u, lgn_con_period=%u, mgr_show_type=%u"
						", show_log_day=%u, boot_chk_type=%u"
						", integrity_mode=%u, cross_auth_mode=%u "
						", once_pkt_num=%u"
						", mgr_show_order_pkg='%s', mgr_show_order_cls='%s'"
						", conn_manage=%u, log_reset=%u"
						",lnx_run_option=%u, lnx_rm_log_day=%u, lnx_lgn_con_period=%u, lnx_boot_chk_type=%u"
						", lnx_once_pkt_num=%u, lnx_conn_manage=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dphr.nRunOption, dphr.nRmLogDay, dphr.nLgnConPeriod, dphr.nMgrShowType, 
						dphr.nShowLogDay, dphr.nBootChkType,
						dphr.nIntegrityMode, dphr.nCrossAuthMode,
						dphr.nOncePktNum,
						strMSOPkg.c_str(), strMSOCls.c_str(),
						dphr.nConnManage, dphr.nLogReset,
						dphr.nLnxRunOption, dphr.nLnxRmLogDay, dphr.nLnxLgnConPeriod, dphr.nLnxBootChkType,
						dphr.nLnxOncePktNum, dphr.nLnxConnManage,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRun::DeletePoHostRun(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------  

INT32			CDBMgrPoHostRun::ClearPoHostRun()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------  
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRun::LoadExecute(PVOID lpTempletList)
{
	TListDBPoHostRun* ptDBList = (TListDBPoHostRun*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRun::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_RUN pd_t = (PDB_PO_HOST_RUN)lpTemplet;

    return InsertPoHostRun(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRun::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_RUN pd_t = (PDB_PO_HOST_RUN)lpTemplet;

    return UpdatePoHostRun(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRun::DeleteExecute(UINT32 nID)
{
	return DeletePoHostRun(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRun::ClearExecute()
{
	return ClearPoHostRun();
}
//---------------------------------------------------------------------------






