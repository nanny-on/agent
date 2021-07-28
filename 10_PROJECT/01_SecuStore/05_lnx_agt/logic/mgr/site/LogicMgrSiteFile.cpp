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
#include "LogicMgrSiteFile.h"

//---------------------------------------------------------------------------

CLogicMgrSiteFile*		t_LogicMgrSiteFile = NULL;

//---------------------------------------------------------------------------

CLogicMgrSiteFile::CLogicMgrSiteFile()
{	
	t_ManageSiteFile	= new CManageSiteFile();
	t_LogicSiteFile		= new CLogicSiteFile();

	t_ManageSiteFile->LoadDBMS();

	m_strLogicName		= "logic mgr site file";

	m_nControlType		= SS_CONTROL_TYPE_COMMON_SITE_FILE;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_COMMON;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_COMMON_SITE_FILE;
}
//---------------------------------------------------------------------------

CLogicMgrSiteFile::~CLogicMgrSiteFile()
{
	SAFE_DELETE(t_LogicSiteFile);
	SAFE_DELETE(t_ManageSiteFile);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFile::AnalyzePkt_FromMgr_Ext()
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



INT32		CLogicMgrSiteFile::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_SITE_FILE dsf;

	if(t_ManageSiteFile->GetPkt(RecvToken, dsf))		return AZPKT_CB_RTN_PKT_INVALID;

	CheckSiteFile(&dsf);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFile::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteFile::AnalyzePkt_FromMgr_Del_Ext()
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

INT32		CLogicMgrSiteFile::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBSiteFile tDBSiteFileList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_SITE_FILE tDLS;
		if(t_ManageSiteFile->GetPktSync(RecvToken, tDLS))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBSiteFileList.push_back(tDLS);
	}

	{
		TListDBSiteFileItor begin, end;
		begin = tDBSiteFileList.begin();	end = tDBSiteFileList.end();
		for(begin; begin != end; begin++)
		{
			PDB_SITE_FILE pdsf = t_ManageSiteFile->FindPosSKeyID_Item(begin->strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
			if(!pdsf)		continue;

			DB_SITE_FILE tDSF = *pdsf;
			{
				tDSF.nRegSvrID		= begin->nRegSvrID;

				if((tDSF.nSyncSvrStep & begin->nSyncSvrStep))
				{
					tDSF.nSyncSvrStep	-= begin->nSyncSvrStep;
				}
			}
			
			t_ManageSiteFile->EditSiteFile(tDSF);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String		CLogicMgrSiteFile::GetWEngKey(String strFilePath)
{
	ASI_WENG_WL_EX tAWWE;
	String strFeKey;

	DWORD dwFileType = 0;
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

void		CLogicMgrSiteFile::CheckSiteFile(String strFilePath)
{
	String strFeKey = GetWEngKey(strFilePath);
	if(strFeKey.empty())	return;

	DB_SITE_FILE tDSF;
	{
		CTokenString Token(strFilePath.c_str(), strFilePath.length(), '\\', 1);

		tDSF.strFeKey = strFeKey;				
		tDSF.strFileName = Token.NextToken();
		tDSF.strFilePath = Token.RemainValue();
	}
	return CheckSiteFile(&tDSF);
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFile::CheckSiteFile(PDB_SITE_FILE pdsf_src)
{
	DB_SITE_FILE dsf;
	PDB_SITE_FILE pdsf = NULL;
	String strFullFileName;
	CHAR szFileInfo[CHAR_MAX_SIZE] = {0, };

	if(pdsf_src == NULL)
		return;
	
	dsf = *pdsf_src;
	pdsf = t_ManageSiteFile->FindPosSKeyID_Item(dsf.strFeKey, SS_SITE_FILE_KEY_TYPE_FE);

	if(pdsf)
	{
		dsf = *pdsf;
	}
	else
	{
		strFullFileName = SPrintf("%s/%s", dsf.strFilePath.c_str(), dsf.strFileName.c_str());

		dsf.nRegDate		= GetCurrentDateTimeInt();
		dsf.nUsedMode		= STATUS_USED_MODE_ON;
		dsf.nSyncSvrStep	= SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO;

		{
			ASI_FILE_CERT_INFO tAFCI;
			strncpy(tAFCI.szCheckFileName, strFullFileName.c_str(), MAX_PATH-1);

			dsf.strPubName		= tAFCI.szPubName;
			//dsf.strPubName.Remove('\'');
			remove_char_in_str(dsf.strPubName, '\'');
			dsf.strPubSN		= tAFCI.szPubSN;			//dsf.strPubName.Remove('\'');			
			dsf.strCntName		= tAFCI.szCntPubName;
			//dsf.strCntName.Remove('\'');
			remove_char_in_str(dsf.strCntName, '\'');
			dsf.strCntSN		= tAFCI.szCntPubSN;			//dsf.strPubName.Remove('\'');			
		}

		{
			ZeroMemoryExt(szFileInfo);
//			t_ASIFIDLLUtil->ASIFI_GetFileDescription(strFullFileName.c_str(), szFileInfo);
			dsf.strFileDescr = szFileInfo;
			//dsf.strFileDescr.Remove('\'');
			remove_char_in_str(dsf.strFileDescr, '\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
//			t_ASIFIDLLUtil->ASIFI_GetFileVersion(strFullFileName.c_str(), szFileInfo);
			dsf.strFileVersion = szFileInfo;
//			dsf.strFileVersion.Remove('\'');
			remove_char_in_str(dsf.strFileVersion, '\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
//			t_ASIFIDLLUtil->ASIFI_GetFileSpecialValue(strFullFileName.c_str(), "CompanyName", szFileInfo);
			dsf.strPublisherName = szFileInfo;
//			dsf.strPublisherName.Remove('\'');
			remove_char_in_str(dsf.strPublisherName, '\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
//			t_ASIFIDLLUtil->ASIFI_GetFileSpecialValue(strFullFileName.c_str(), "ProductName", szFileInfo);
			dsf.strProductName = szFileInfo;
//			dsf.strProductName.Remove('\'');
			remove_char_in_str(dsf.strProductName, '\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
			SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFileName.c_str(), szFileInfo, CHAR_MAX_SIZE-1);
			dsf.strFileHash = szFileInfo;
		}
		{
			dsf.nFileSize = GetFileSizeExt(strFullFileName.c_str());
		}
		{
//			dsf.nPeBit = t_ASIFIDLLUtil->ASIFI_GetFilePEMagic(strFullFileName.c_str());
//			dsf.nPeType = t_ASIFIDLLUtil->ASIFI_GetFilePEType(strFullFileName.c_str());
		}		
	}

	SetSiteFile(dsf);
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFile::SetSiteFile(DB_SITE_FILE& dsf)
{
	{
		WriteLogN("[%s] remain site file to file : fp[%s]:fn[%s]", 
					m_strLogicName.c_str(),
					dsf.strFilePath.c_str(), dsf.strFileName.c_str());
	}
		
	INT32 nOldItem = dsf.nID;
	t_ManageSiteFile->ApplySiteFile(dsf);
	
	{	
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageSiteFile->SetPkt(&dsf, SendToken);
		
		if(!nOldItem)
		{
//			t_ASIEPSAPPDLLUtil->AddSiteFile(SendToken.GetData(), SendToken.GetLength());
			SendData_Link(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
		}

		do 
		{
			{
				PDB_PO_FE_PTN_OP pdata = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
				if(pdata && (pdata->nNewFileSendType & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
							((dsf.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
					break;
				}
			}

			{
				PDB_PO_IN_PTN_OP pdata = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
				if(pdata && 
					(pdata->nRTFGMode & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
					((dsf.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
					break;
				}
			}

			{
				PDB_PO_NC_PTN_OP pdata = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_OP);
				if(pdata && 
					(pdata->nRTFGMode & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
					((dsf.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
				{
					SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
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

void		CLogicMgrSiteFile::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageSiteFile->SetPktSync(tSendList);

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
			t_ManageSiteFile->SetPkt((PDB_SITE_FILE)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFile::SendPkt_ReSend(INT32 nOnceMaxNum)
{
	TListID tIDList;
	t_ManageSiteFile->GetItemIDList(tIDList);

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
			t_ManageSiteFile->SetPkt((*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
