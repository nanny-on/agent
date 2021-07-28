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
#include "LogicMgrSiteFileAlias.h"

//---------------------------------------------------------------------------

CLogicMgrSiteFileAlias*		t_LogicMgrSiteFileAlias = NULL;

//---------------------------------------------------------------------------

CLogicMgrSiteFileAlias::CLogicMgrSiteFileAlias()
{	
	t_ManageSiteFileAlias	= new CManageSiteFileAlias();

	t_ManageSiteFileAlias->LoadDBMS();

	m_strLogicName		= "logic mgr site file alias";

	m_nControlType		= SS_CONTROL_TYPE_COMMON_SITE_FILE_ALIAS;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_COMMON;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_COMMON_SITE_FILE_ALIAS;
}
//---------------------------------------------------------------------------

CLogicMgrSiteFileAlias::~CLogicMgrSiteFileAlias()
{
	SAFE_DELETE(t_ManageSiteFileAlias);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFileAlias::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		nRtn = AnalyzePkt_FromMgr_Ext_Sync();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------



INT32		CLogicMgrSiteFileAlias::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_SITE_FILE_ALIAS data;

	if(t_ManageSiteFileAlias->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

	CheckSiteFileAlias(&data);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFileAlias::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFileAlias::AnalyzePkt_FromMgr_Del_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
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

INT32		CLogicMgrSiteFileAlias::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBSiteFileAlias tDBSiteFileAliasList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_SITE_FILE_ALIAS tDSFA;
		if(t_ManageSiteFileAlias->GetPktSync(RecvToken, tDSFA))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBSiteFileAliasList.push_back(tDSFA);
	}

	{
		TListDBSiteFileAliasItor begin, end;
		begin = tDBSiteFileAliasList.begin();	end = tDBSiteFileAliasList.end();
		for(begin; begin != end; begin++)
		{			
			PDB_SITE_FILE_ALIAS pdata = t_ManageSiteFileAlias->FindSiteFileAliasKey((*begin));
			if(!pdata)		continue;

			DB_SITE_FILE_ALIAS tDSFA = *pdata;
			{
				tDSFA.nRegSvrID		= begin->nRegSvrID;

				if((tDSFA.nSyncSvrStep & begin->nSyncSvrStep))
				{
					tDSFA.nSyncSvrStep	-= begin->nSyncSvrStep;
				}
			}
			
			t_ManageSiteFileAlias->EditSiteFileAlias(tDSFA);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String		CLogicMgrSiteFileAlias::GetWEngKey(String strFilePath)
{
	ASI_WENG_WL_EX tAWWE;
	DWORD dwFileType = 0;
	String strFeKey;
	if(t_ASIWENGDLLUtil->GetWL(strFilePath.c_str(), &tAWWE, sizeof(tAWWE), &dwFileType) != ERROR_SUCCESS)
	{
		return "";
	}

	if(dwFileType == AS_INVALID_FILE)
	{
		return "";
	}

//	strFeKey = BinToHexLen((PBYTE)(tAWWE.acWhiteHash), SHA512_BLOCK_SIZE);
	strFeKey = tAWWE.acWhiteHash;
	return strFeKey;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFileAlias::CheckSiteFileAlias(String strFilePath)
{
	String strFeKey = GetWEngKey(strFilePath);
	if(strFeKey.empty())	return;

	DB_SITE_FILE_ALIAS tDSF;
	{
		CTokenString Token(strFilePath.c_str(), strFilePath.length(), '/', 1);

		tDSF.strFeKey = strFeKey;				
		tDSF.strFileName = Token.NextToken();
		tDSF.strFilePath = Token.RemainValue();
	}
	return CheckSiteFileAlias(&tDSF);
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFileAlias::CheckSiteFileAlias(PDB_SITE_FILE_ALIAS pdata_src)
{
	DB_SITE_FILE_ALIAS data;
	PDB_SITE_FILE_ALIAS pdata = NULL;
	CHAR szFileInfo[CHAR_MAX_SIZE] = {0, };
	
	data = *pdata_src;
	pdata = t_ManageSiteFileAlias->FindSiteFileAliasKey(data);

	if(pdata)
	{
		data = *pdata;
	}
	else
	{
		data.nRegDate		= GetCurrentDateTimeInt();
		data.nUsedMode		= STATUS_USED_MODE_ON;
		data.nSyncSvrStep	= SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
	}

	{
		SetSiteFileAlias(data);		
	}

	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFileAlias::SetSiteFileAlias(DB_SITE_FILE_ALIAS& data)
{
	{
		WriteLogN("[%s] remain site file alias to file : fp[%s]:fn[%s]", 
					m_strLogicName.c_str(),
					data.strFilePath.c_str(), data.strFileName.c_str());
	}
		
	INT32 nOldItem = data.nID;
	t_ManageSiteFileAlias->ApplySiteFileAlias(data);
	
	{	
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageSiteFileAlias->SetPkt(&data, SendToken);
/*
		if(!nOldItem)
		{
			t_ASIEPSAPPDLLUtil->AddSiteFileAlias(SendToken.GetData(), SendToken.GetLength());			
		}
*/
		do 
		{
			{
				PDB_PO_FE_PTN_OP pdata = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
				if(pdata && (pdata->nNewFileSendType & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
							((data.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE_ALIAS, G_CODE_COMMON_SYNC, SendToken);
					break;
				}
			}

			{
				PDB_PO_IN_PTN_OP pdata = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
				if(pdata && 
					(pdata->nRTFGMode & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
					((data.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE_ALIAS, G_CODE_COMMON_SYNC, SendToken);
					break;
				}
			}

			{
				PDB_PO_NC_PTN_OP pdata = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_OP);
				if(pdata && 
					(pdata->nRTFGMode & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
					((data.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE_ALIAS, G_CODE_COMMON_SYNC, SendToken);
					break;
				}
			}
		} while (FALSE);

		SendToken.Clear();
	}

	{

	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFileAlias::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageSiteFileAlias->SetPktSync(tSendList);

		if(tSendList.empty())	return;
	}

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;

	TListPVOIDItor begin, end;
	begin = tSendList.begin();	end = tSendList.end();

	while(nSendNum < tSendList.size())
	{
		nOnceNum = (((tSendList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tSendList.size() - nSendNum));

		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManageSiteFileAlias->SetPkt((PDB_SITE_FILE_ALIAS)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_SITE_FILE_ALIAS, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFileAlias::SendPkt_ReSend(INT32 nOnceMaxNum)
{
	TListID tIDList;
	t_ManageSiteFileAlias->GetItemIDList(tIDList);

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();

	while(nSendNum < tIDList.size())
	{
		nOnceNum = (((tIDList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tIDList.size() - nSendNum));

		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManageSiteFileAlias->SetPkt((*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_SITE_FILE_ALIAS, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
