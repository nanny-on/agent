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
#include "DBMgrHostStatusPo.h"

//---------------------------------------------------------------------------

CDBMgrHostStatusPo*		t_DBMgrHostStatusPo = NULL;

//---------------------------------------------------------------------------

CDBMgrHostStatusPo::CDBMgrHostStatusPo()
{
	m_strDBTName = "host_status_po";
}
//---------------------------------------------------------------------------

CDBMgrHostStatusPo::~CDBMgrHostStatusPo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPo::LoadDB(TListDBHostStatusPo& tDBHostStatusPoList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_STATUS_PO dhsp;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, host_id"
						", fao_drv, fac_unit"
						", df_run, df_unit"
						", feop_drv"
						", felo_drv, felo_run, felo_ptn"
						", fegb_drv, fegb_run, fegb_ptn"
						", fegw_drv, fegw_run, fegw_ptn"
						" FROM host_status_po WHERE used_flag <> 0;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dhsp.nID				= GetDBField_Int(nIndex++) ;
		dhsp.nUsedFlag			= GetDBField_Int(nIndex++) ;
		
		dhsp.nHostID			= GetDBField_Int(nIndex++);
		dhsp.nFaODrv			= GetDBField_Int(nIndex++);
		dhsp.nFaCUnit			= GetDBField_Int(nIndex++);
		dhsp.nDfRun				= GetDBField_Int(nIndex++);
		dhsp.nDfRun				= GetDBField_Int(nIndex++);

		dhsp.nFeOpDrv			= GetDBField_Int(nIndex++);
		dhsp.nFeLoDrv			= GetDBField_Int(nIndex++);
		dhsp.nFeLoRun			= GetDBField_Int(nIndex++);
		dhsp.strFeLoPtn			= GetDBField_String(nIndex++);
		dhsp.nFeGbDrv			= GetDBField_Int(nIndex++);
		dhsp.nFeGbRun			= GetDBField_Int(nIndex++);
		dhsp.strFeGbPtn			= GetDBField_String(nIndex++);
		dhsp.nFeGwDrv			= GetDBField_Int(nIndex++);
		dhsp.nFeGwRun			= GetDBField_Int(nIndex++);
		dhsp.strFeGwPtn			= GetDBField_String(nIndex++);

        tDBHostStatusPoList.push_back(dhsp);
        if(m_nLoadMaxID < UINT32(dhsp.nID))
			m_nLoadMaxID = dhsp.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostStatusPoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPo::InsertHostStatusPo(DB_HOST_STATUS_PO& dhsp)
{
	m_strQuery = SPrintf("INSERT INTO host_status_po (used_flag, host_id"
									", fao_drv, fac_unit"
									", df_run, df_unit"
									", feop_drv"
									", felo_drv, felo_run, felo_ptn"
									", fegb_drv, fegb_run, fegb_ptn"
									", fegw_drv, fegw_run, fegw_ptn"
									") VALUES ( %u, %u"
									", %u, %u"
									", %u, %u"
									", %u"
									", %u, %u, '%s'"
									", %u, %u, '%s'"
									", %u, %u, '%s'"
									");",
                                    dhsp.nUsedFlag,	dhsp.nHostID, 
									dhsp.nFaODrv, dhsp.nFaCUnit, 
									dhsp.nDfRun, dhsp.nDfUnit,
									dhsp.nFeOpDrv,
									dhsp.nFeLoDrv, dhsp.nFeLoRun, dhsp.strFeLoPtn.c_str(),
									dhsp.nFeGbDrv, dhsp.nFeGbRun, dhsp.strFeGbPtn.c_str(),
									dhsp.nFeGwDrv, dhsp.nFeGwRun, dhsp.strFeGwPtn.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(dhsp.nID == 0)
		dhsp.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPo::UpdateHostStatusPo(DB_HOST_STATUS_PO& dhsp)
{
	m_strQuery = SPrintf("UPDATE host_status_po SET "
						" used_flag=%u, host_id=%u"
						", fao_drv=%u, fac_unit=%u"
						", df_run=%u, df_unit=%u"
						", feop_drv=%u"
						", felo_drv=%u, felo_run=%u, felo_ptn='%s'"
						", fegb_drv=%u, fegb_run=%u, fegb_ptn='%s'"
						", fegw_drv=%u, fegw_run=%u, fegw_ptn='%s'"
						" WHERE id=%u;",
						dhsp.nUsedFlag,	dhsp.nHostID, 
						dhsp.nFaODrv, dhsp.nFaCUnit, 
						dhsp.nDfRun, dhsp.nDfUnit,
						dhsp.nFeOpDrv,
						dhsp.nFeLoDrv, dhsp.nFeLoRun, dhsp.strFeLoPtn.c_str(),
						dhsp.nFeGbDrv, dhsp.nFeGbRun, dhsp.strFeGbPtn.c_str(),
						dhsp.nFeGwDrv, dhsp.nFeGwRun, dhsp.strFeGwPtn.c_str(), 
						dhsp.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

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
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPo::LoadExecute(PVOID lpTempletList)
{
	TListDBHostStatusPo* ptDBList = (TListDBHostStatusPo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrHostStatusPo::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_STATUS_PO pd_t = (PDB_HOST_STATUS_PO)lpTemplet;

    return InsertHostStatusPo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPo::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_STATUS_PO pd_t = (PDB_HOST_STATUS_PO)lpTemplet;

    return UpdateHostStatusPo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






