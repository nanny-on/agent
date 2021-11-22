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
#include "ManagePoInVulnOp.h"

//---------------------------------------------------------------------------

CManagePoInVulnOp*	t_ManagePoInVulnOp = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnOp::CManagePoInVulnOp()
{
	t_DBMgrPoInVulnOp	= new CDBMgrPoInVulnOp();
}
//---------------------------------------------------------------------------

CManagePoInVulnOp::~CManagePoInVulnOp()
{
	if(t_DBMgrPoInVulnOp)	{	delete t_DBMgrPoInVulnOp;	t_DBMgrPoInVulnOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnOp::LoadDBMS()
{
	TListDBPoInVulnOp tDBPoInVulnOpList;
    TListDBPoInVulnOpItor begin, end;
	if(SetER(t_DBMgrPoInVulnOp->LoadExecute(&tDBPoInVulnOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnOpList.begin();	end = tDBPoInVulnOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::InitHash()
{
	t_ManagePoInVulnOpPkg->InitPkg();
	
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_VULN_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,"
							"%s,%s,%s,%s,"
							"%u,%s,%s,",
							GetHdrHashInfo(pdata).c_str(),
							pdata->nEnvScanMode,
							pdata->tDPIVOPF.strPtnFileSvName.c_str(), pdata->tDPIVOPF.strPtnVersion.c_str(), pdata->tDPIVOPF.strPtnFileName.c_str(), pdata->tDPIVOPF.strPtnFileHash.c_str(),
							pdata->tDPIVOPF.nPtnDnFileType, pdata->tDPIVOPF.strPtnDnFileName.c_str(), pdata->tDPIVOPF.strPtnDnFileHash.c_str());

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoInVulnOpPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::AddPoInVulnOp(DB_PO_IN_VULN_OP&	data)
{
	if(SetER(t_DBMgrPoInVulnOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::EditPoInVulnOp(DB_PO_IN_VULN_OP&	data)
{
	PDB_PO_IN_VULN_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::DelPoInVulnOp(UINT32 nID)
{
	PDB_PO_IN_VULN_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::ApplyPoInVulnOp(DB_PO_IN_VULN_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnOp(data);
	}
	return AddPoInVulnOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnOp::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_IN_VULN_OP pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;	

	return IsValidPtnFile(pdata);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::IsValidPtnFile(PDB_PO_IN_VULN_OP pdata)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE, pdata->tDPIVOPF.strPtnFileName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE);
		if(_stricmp(pdata->tDPIVOPF.strPtnFileHash.c_str(), szHash))
		{
			WriteLogE("po in vuln op ptn invalid hash : [%s][%s]", szHash, pdata->tDPIVOPF.strPtnFileHash.c_str());
			return 0;
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInVulnOpPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPkt(PDB_PO_IN_VULN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nEnvScanMode);

	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileSvName);

	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnVersion);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileName);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileHash);

	SendToken.TokenAdd_32(pdata->tDPIVOPF.nPtnDnFileType);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnDnFileName);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnDnFileHash);


	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInVulnOpPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPktHost(PDB_PO_IN_VULN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nEnvScanMode);

	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileSvName);

	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnVersion);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileName);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnFileHash);

	SendToken.TokenAdd_32(pdata->tDPIVOPF.nPtnDnFileType);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnDnFileName);
	SendToken.TokenAdd_String(pdata->tDPIVOPF.strPtnDnFileHash);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::SetPktEPS(UINT32 nID, MemToken& SendToken)
{
	TListID tList;
	TListIDItor begin, end;
	t_ManagePoInVulnOpPkg->GetItemIDList(tList);

	TListMemAsiEpsRegInfo tMemRegList;
	if(t_ManageEpsRegInfo)
		t_ManageEpsRegInfo->Clear();
//	CSimpleSysCheckUtil tSimpleSysChkUtil;
	UINT32 nTSafe = 1;

	begin = tList.begin();	end = tList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_OP_PKG pdata_pkg = t_ManagePoInVulnOpPkg->FindItem(*begin);
		if (!pdata_pkg)	continue;

		UINT32 nLockValue = pdata_pkg->tDPH.nExtOption & SS_PTN_VULN_SUPPORT_OPTION_LOCK_SAFE_VALUE;
		UINT32 nRepairValue = pdata_pkg->tDPH.nExtOption & SS_PTN_VULN_SUPPORT_OPTION_AUTO_REPAIR;

		if(!nLockValue && !nRepairValue)	continue;

//		UINT8	nType = LOTYPEID32(pdata_pkg->tDPH.nUnitID);
//		UINT32	nUID = HITYPEID32(pdata_pkg->tDPH.nUnitID);
/*
		switch(nType)
		{
			case SS_COMMON_UNIT_TYPE_GLOBAL:
			{
				PDB_PTN_VULN pdata = t_MMPPVuln->t_ManagePtnVuln->FindItem(nUID);
				if(!pdata)		break;

				TListID tVulnScanIDList;
				t_MMPPVuln->t_ManagePtnVulnScan->GetKeyIDList(nUID, tVulnScanIDList);
				
				TListStr tSIDList;
				t_ManageWinSession.GetSIDList(tSIDList);

				{
					TListStrItor begin_sid, end_sid;
					begin_sid = tSIDList.begin();	end_sid = tSIDList.end();
					for(begin_sid; begin_sid != end_sid; begin_sid++)
					{
						tSimpleSysChkUtil.ChkVuln(pdata_pkg->tDPH.nUnitID, tVulnScanIDList, *begin_sid, nRepairValue, nLockValue);
					}
				}
				break;
			}
		}
*/
	}	

	if(t_ManageEpsRegInfo)
		t_ManageEpsRegInfo->SetPkt(SendToken);
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnOp::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_OP& data)
{
	if(!RecvToken.TokenDel_DPH(data.tDPH))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nEnvScanMode))	goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnFileSvName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnVersion) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnFileName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnFileHash) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.tDPIVOPF.nPtnDnFileType))			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnDnFileName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDPIVOPF.strPtnDnFileHash) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



