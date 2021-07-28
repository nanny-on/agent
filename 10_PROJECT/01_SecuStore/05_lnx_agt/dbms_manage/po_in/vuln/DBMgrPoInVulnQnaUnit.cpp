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
#include "DBMgrPoInVulnQnaUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaUnit*		t_DBMgrPoInVulnQnaUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaUnit::CDBMgrPoInVulnQnaUnit()
{
	m_strDBTName = "po_in_vuln_qna_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaUnit::~CDBMgrPoInVulnQnaUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_qna_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaUnit::LoadDB(TListDBPoInVulnQnaUnit& tDBPoInVulnQnaUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_QNA_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", msg_question, msg_answer"
						" FROM po_in_vuln_qna_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.strQuestion			= GetDBField_String(nIndex++);
		data.strAnswer				= GetDBField_String(nIndex++);
		
        tDBPoInVulnQnaUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnQnaUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaUnit::InsertPoInVulnQnaUnit(DB_PO_IN_VULN_QNA_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_qna_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", msg_question, msg_answer"
						") VALUES (%s"
						", '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strQuestion.c_str(), data.strAnswer.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaUnit::UpdatePoInVulnQnaUnit(DB_PO_IN_VULN_QNA_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_qna_unit SET %s"
						", msg_question='%s', msg_answer='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strQuestion.c_str(), data.strAnswer.c_str(), 
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

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

INT32	CDBMgrPoInVulnQnaUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnQnaUnit* ptDBList = (TListDBPoInVulnQnaUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_QNA_UNIT pd_t = (PDB_PO_IN_VULN_QNA_UNIT)lpTemplet;

    return InsertPoInVulnQnaUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_QNA_UNIT pd_t = (PDB_PO_IN_VULN_QNA_UNIT)lpTemplet;

    return UpdatePoInVulnQnaUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






