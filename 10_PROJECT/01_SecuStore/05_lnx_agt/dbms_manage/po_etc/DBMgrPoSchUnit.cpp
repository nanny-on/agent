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
#include "DBMgrPoSchUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoSchUnit*		t_DBMgrPoSchUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoSchUnit::CDBMgrPoSchUnit()
{
	m_strDBTName = "po_host_po_sch_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoSchUnit::~CDBMgrPoSchUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchUnit::LoadDB(TListDBPoSchUnit& tDBPoSchUnitList)
{
	TMapStr tStrMap;
    UINT32 nReadCnt = 0;
    DB_PO_SCH_UNIT dphpsu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", package_type, policy_info, start_time, end_time"
						" FROM po_sch_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dphpsu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dphpsu.nPackageType			= GetDBField_UInt64(nIndex++);
		
		{
			tStrMap.clear();
			dphpsu.tPoInfoMap.clear();

			m_strValue				= GetDBField_String(nIndex++);		
			StrToMapStr(m_strValue, tStrMap);
			MapStrToMap_64(tStrMap, dphpsu.tPoInfoMap);
		}

		dphpsu.nStDate				= GetDBField_UInt(nIndex++);
		dphpsu.nEdDate				= GetDBField_UInt(nIndex++);
		
        tDBPoSchUnitList.push_back(dphpsu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoSchUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchUnit::InsertPoSchUnit(DB_PO_SCH_UNIT& dphpsu)
{
	DB_PO_HEADER& tDPH = dphpsu.tDPH;

	String strPolicyInfo;
	TMapStr tStrMap;
	MaptoMapStr_64(dphpsu.tPoInfoMap, tStrMap);
	strPolicyInfo = MapStrToStr(tStrMap);

	m_strQuery = SPrintf("INSERT INTO po_sch_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", package_type, policy_info, start_time, end_time"
						") VALUES (%s"
                        ", '%llu', '%s', %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dphpsu.nPackageType, strPolicyInfo.c_str(), dphpsu.nStDate, dphpsu.nEdDate);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchUnit::UpdatePoSchUnit(DB_PO_SCH_UNIT& dphpsu)
{
	DB_PO_HEADER& tDPH = dphpsu.tDPH;

	String strPolicyInfo;
	TMapStr tStrMap;
	MaptoMapStr_64(dphpsu.tPoInfoMap, tStrMap);
	strPolicyInfo = MapStrToStr(tStrMap);

	m_strQuery = SPrintf("UPDATE po_sch_unit SET %s"
						", package_type='%llu', policy_info='%s', start_time=%u, end_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dphpsu.nPackageType, strPolicyInfo.c_str(), dphpsu.nStDate, dphpsu.nEdDate,
                        tDPH.nID);

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

INT32	CDBMgrPoSchUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoSchUnit* ptDBList = (TListDBPoSchUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_SCH_UNIT pd_t = (PDB_PO_SCH_UNIT)lpTemplet;

    return InsertPoSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_SCH_UNIT pd_t = (PDB_PO_SCH_UNIT)lpTemplet;

    return UpdatePoSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






