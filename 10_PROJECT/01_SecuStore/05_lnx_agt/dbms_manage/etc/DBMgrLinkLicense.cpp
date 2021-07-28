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
#include "DBMgrLinkLicense.h"

//---------------------------------------------------------------------------

CDBMgrLinkLicense*		t_DBMgrLinkLicense;

//---------------------------------------------------------------------------

CDBMgrLinkLicense::CDBMgrLinkLicense()
{
	m_strDBTName = "link_license";
}
//---------------------------------------------------------------------------

CDBMgrLinkLicense::~CDBMgrLinkLicense()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLinkLicense::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLinkLicense::LoadDB(TListDBLinkLicense& tDBLinkLicenseList)
{
    UINT32 nReadCnt = 0;
    DB_LINK_LICENSE dl;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", license_key "
						" FROM link_license WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		
		DB_PO_HEADER& tDPH = dl.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		dl.strLicenseKey		= GetDBField_String(nIndex++);		
        tDBLinkLicenseList.push_back(dl);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBLinkLicenseList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLinkLicense::InsertLinkLicense(DB_LINK_LICENSE& dl)
{
	DB_PO_HEADER& tDPH = dl.tDPH;
	
	m_strQuery = SPrintf("INSERT INTO link_license("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", license_key"
						") VALUES (%s"
						", '%s'"
						");",                        
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dl.strLicenseKey.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLinkLicense::UpdateLinkLicense(DB_LINK_LICENSE& dl)
{
	DB_PO_HEADER& tDPH = dl.tDPH;

	m_strQuery = SPrintf("UPDATE link_license SET %s"
						", license_key='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dl.strLicenseKey.c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLinkLicense::DeleteLinkLicense(UINT32 nID)
{
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

INT32	CDBMgrLinkLicense::LoadExecute(PVOID lpTempletList)
{
	TListDBLinkLicense* ptDBList = (TListDBLinkLicense*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLinkLicense::InsertExecute(PVOID lpTemplet)
{
	PDB_LINK_LICENSE pd_t = (PDB_LINK_LICENSE)lpTemplet;

    return InsertLinkLicense(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLinkLicense::UpdateExecute(PVOID lpTemplet)
{
	PDB_LINK_LICENSE pd_t = (PDB_LINK_LICENSE)lpTemplet;

    return UpdateLinkLicense(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLinkLicense::DeleteExecute(UINT32 nID)
{
	return DeleteLinkLicense(nID);
}
//---------------------------------------------------------------------------






