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
#include "ManageEnvLicense.h"

//---------------------------------------------------------------------------

CManageEnvLicense*	t_ManageEnvLicense = NULL;

//---------------------------------------------------------------------------

CManageEnvLicense::CManageEnvLicense()
{
	t_DBMgrEnvLicense	= new CDBMgrEnvLicense();	
}
//---------------------------------------------------------------------------

CManageEnvLicense::~CManageEnvLicense()
{
	if(t_DBMgrEnvLicense)	{	delete t_DBMgrEnvLicense;	t_DBMgrEnvLicense = NULL;	}	
}
//---------------------------------------------------------------------------

INT32		CManageEnvLicense::LoadDBMS()
{
	DB_ENV_LICENSE del;
	TListDBEnvLicense tDBEnvLicenseList;
	TListDBEnvLicenseItor begin, end;
	
	if(SetER(t_DBMgrEnvLicense->LoadExecute(&tDBEnvLicenseList)))
	{
		if(tDBEnvLicenseList.size() == 0)
		{
			memset(&del, 0, sizeof(DB_ENV_LICENSE));
			del.tDPH.nID = 1;
			AddEnvLicense(del);
		}	
		return g_nErrRtn;
	}

	begin = tDBEnvLicenseList.begin();	end = tDBEnvLicenseList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}

	if(tDBEnvLicenseList.size() == 0)
	{
		memset(&del, 0, sizeof(DB_ENV_LICENSE));
		del.tDPH.nID = 1;
		AddEnvLicense(del);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::InitHash()
{
	String strOrgValue;

	PDB_ENV_LICENSE pdel 			= NULL;
	{
		if( (pdel = FirstItem()) == NULL)
		{
			WriteLogE("not find env_license by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strRightPackage, strRightClass, strRightPolicy, strRightControl;

		MapToHex_64(pdel->tRightPackageMap, strRightPackage, SS_ADMIN_RIGHT_TYPE_CLASS_NUM_PACKAGE);
		MapToHex_64(pdel->tRightClassMap, strRightClass, SS_ADMIN_RIGHT_TYPE_CLASS_NUM_PACKAGE);
		MapToHex_64(pdel->tRightPolicyMap,  strRightPolicy, SS_ADMIN_RIGHT_TYPE_CLASS_NUM_POLICY);
		MapToHex_64(pdel->tRightControlMap, strRightControl, SS_ADMIN_RIGHT_TYPE_CLASS_NUM_CONTROL);


		strOrgValue = SPrintf("%s,"
							"%s,%u,%s,%s,%s,%s,", 
							GetHdrHashInfo(pdel).c_str(),
							pdel->strLicenseKey.c_str(), pdel->nSupportOs, strRightPackage.c_str(), strRightClass.c_str(), strRightPolicy.c_str(), strRightControl.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdel->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvLicense::GetHash()
{
	PDB_ENV_LICENSE pdel 			= NULL;
	{
		if( (pdel = FirstItem()) == NULL)
		{
			WriteLogE("not find env_license by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return "";
		}
	}

	return pdel->tDPH.strHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::AddEnvLicense(DB_ENV_LICENSE&	del)
{	
	if(SetER(t_DBMgrEnvLicense->InsertExecute(&del)))
	{
		return g_nErrRtn;
	}

	AddItem(del.tDPH.nID, del);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::EditEnvLicense(DB_ENV_LICENSE&	del)
{
	PDB_ENV_LICENSE pdel = FindItem(del.tDPH.nID);
	if(!pdel)	return ERR_DBMS_UPDATE_FAIL;	

	if(SetER(t_DBMgrEnvLicense->UpdateExecute(&del)))
	{
		return g_nErrRtn;
	}

	EditItem(del.tDPH.nID, del);
	InitHash();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::DelEnvLicense(UINT32 nID)
{
	PDB_ENV_LICENSE pdel = FindItem(nID);
	if(!pdel)	return ERR_DBMS_DELETE_FAIL;


	if(SetER(t_DBMgrEnvLicense->DeleteExecute(pdel->tDPH.nID)))
	{
		return g_nErrRtn;
	}
	
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::ApplyEnvLicense(DB_ENV_LICENSE&	del)
{
	if(FindItem(del.tDPH.nID))
	{
		return EditEnvLicense(del);
	}
	return AddEnvLicense(del);
}
//---------------------------------------------------------------------------

String					CManageEnvLicense::GetName(UINT32 nID)
{
	PDB_ENV_LICENSE pdel = FindItem(nID);
    if(!pdel)	return "";
    return pdel->strLicenseKey;
}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsValidRight(UINT64 nPackage, UINT64 nPolicy, UINT64 nControl)
{
	return !(IsInvalidRight(nPackage, nPolicy, nControl));
}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsValidRight(UINT64 nPackage, UINT64 nClass, UINT64 nPolicy, UINT64 nControl)
{
	return !(IsInvalidRight(nPackage, nClass, nPolicy, nControl));
}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsInvalidRight(UINT64 nPackage, UINT64 nPolicy, UINT64 nControl)
{
	PDB_ENV_LICENSE pdel = FirstItem();
	if(!pdel)	return 1;

	return IsInvalidRight(pdel, nPackage, nPolicy, nControl);	
}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsInvalidRight(UINT64 nPackage, UINT64 nClass, UINT64 nPolicy, UINT64 nControl)
{
	PDB_ENV_LICENSE pdel = FirstItem();
	if(!pdel)	return 1;

	TMapID64Itor find;	
	UINT64 nRightValue;
	if(nClass)
	{
		nRightValue = 0;
		find = pdel->tRightClassMap.find(GET_RIGHT_POS(nClass));
		if(find != pdel->tRightClassMap.end())
		{
			nRightValue = GET_RIGHT_VALUE(find->second);
		}
		if((GET_RIGHT_VALUE(nClass) & nRightValue) == 0)	return 1;
	}

	return IsInvalidRight(nPackage, nPolicy, nControl);

}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsValidRight(PDB_ENV_LICENSE pdata, UINT64 nPackage, UINT64 nPolicy, UINT64 nControl)
{
	return !(IsInvalidRight(pdata, nPackage, nPolicy, nControl));
}
//---------------------------------------------------------------------------

UINT64				CManageEnvLicense::IsInvalidRight(PDB_ENV_LICENSE pdata, UINT64 nPackage, UINT64 nPolicy, UINT64 nControl)
{
	TMapID64Itor find;	
	UINT64 nRightValue;;
	if(nPackage)
	{
		nRightValue = 0;
		find = pdata->tRightPackageMap.find(GET_RIGHT_POS(nPackage));
		if(find != pdata->tRightPackageMap.end())
		{
			nRightValue = GET_RIGHT_VALUE(find->second);
		}
		if((GET_RIGHT_VALUE(nPackage) & nRightValue) == 0)	return 1;
	}
	if(nPolicy)
	{
		nRightValue = 0;
		find = pdata->tRightPolicyMap.find(GET_RIGHT_POS(nPolicy));
		if(find != pdata->tRightPolicyMap.end())
		{
			nRightValue = GET_RIGHT_VALUE(find->second);
		}
		if((GET_RIGHT_VALUE(nPolicy) & nRightValue) == 0)	return 1;
	}
	if(nControl)
	{
		nRightValue = 0;
		find = pdata->tRightControlMap.find(GET_RIGHT_POS(nControl));
		if(find != pdata->tRightControlMap.end())
		{
			nRightValue = GET_RIGHT_VALUE(find->second);
		}
		if((GET_RIGHT_VALUE(nControl) & nRightValue) == 0)	return 1;
	}
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
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvLicenseItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_LICENSE pdel = FindItem(nID);
	if(!pdel)	return -1;

	return SetPkt(pdel, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::SetPkt(PDB_ENV_LICENSE pdel, MemToken& SendToken)
{

	SendToken.TokenAdd_DPH(pdel->tDPH);

	SendToken.TokenAdd_String(pdel->strLicenseKey);
	SendToken.TokenAdd_32(pdel->nSupportOs);

	SendToken.TokenAdd_ID64Map(pdel->tRightPackageMap);
	SendToken.TokenAdd_ID64Map(pdel->tRightClassMap);
	SendToken.TokenAdd_ID64Map(pdel->tRightPolicyMap);
	SendToken.TokenAdd_ID64Map(pdel->tRightControlMap);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLicense::GetPkt(MemToken& RecvToken, DB_ENV_LICENSE& del)
{
	if (!RecvToken.TokenDel_DPH(del.tDPH))						goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(del.strLicenseKey) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(del.nSupportOs))					goto	INVALID_PKT;
	
	if ( RecvToken.TokenDel_ID64Map(del.tRightPackageMap) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(del.tRightClassMap) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(del.tRightPolicyMap) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(del.tRightControlMap) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



