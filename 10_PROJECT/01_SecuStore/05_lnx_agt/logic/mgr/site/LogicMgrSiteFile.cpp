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
	PDB_SITE_FILE pDsf = NULL;
	PDB_SITE_FILE pFindDsf = NULL;
	char acPath[MAX_PATH] = {0, }; 
	INT32 nSize = sizeof(DB_SITE_FILE);
	CHAR szSha256[ASIHASH_SHA_TYPE_256_LEN+2] = {0, };

	if(pdsf_src == NULL || nSize < 1)
		return;

	pDsf = (PDB_SITE_FILE)malloc(nSize);
	if(pDsf == NULL)
		return;

	pDsf->strFeKey = pdsf_src->strFeKey;
	pDsf->strFilePath = pdsf_src->strFilePath;
	pDsf->strFileName = pdsf_src->strFileName;
	pDsf->nPeBit = 0;
	pDsf->nPeType = 0;
	pDsf->nUsedFlag = 1;

	pFindDsf = t_ManageSiteFile->FindPosSKeyID_Item(pDsf->strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
	if(pFindDsf)
	{
		pDsf->nID = pFindDsf->nID;
		pDsf->nRegSvrID = pFindDsf->nRegSvrID;
		pDsf->nRegDate = pFindDsf->nRegDate;
		pDsf->nUsedMode	= pFindDsf->nUsedMode;
		pDsf->nSyncSvrStep = pFindDsf->nSyncSvrStep;
		pDsf->strFileHash = pFindDsf->strFileHash;
		pDsf->nFileSize = pFindDsf->nFileSize;
	}
	else
	{
		pDsf->nID = 0;
		pDsf->nRegSvrID = 0;
		pDsf->nRegDate = GetCurrentDateTimeInt();
		pDsf->nUsedMode = STATUS_USED_MODE_ON;
		pDsf->nSyncSvrStep = SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
		snprintf(acPath, MAX_PATH-1, "%s/%s", pDsf->strFilePath.c_str(), pDsf->strFileName.c_str());
		acPath[MAX_PATH-1] = 0;
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, acPath, szSha256, ASIHASH_SHA_TYPE_256_LEN+1);
		szSha256[ASIHASH_SHA_TYPE_256_LEN] = 0;
		pDsf->strFileHash = szSha256;
		pDsf->nFileSize = GetFileSizeExt(acPath);
	}

	SetSiteFile(pDsf, FALSE);
	safe_free(pDsf);
}

void		CLogicMgrSiteFile::CheckSiteCreateFile(PASI_CHK_FILE_PROC pChkFile)
{
	DB_SITE_FILE dsf;
	PDB_SITE_FILE pFindDsf = NULL;
	INT32 nSize = sizeof(DB_SITE_FILE);
	CHAR szSha256[ASIHASH_SHA_TYPE_256_LEN+2] = {0, };

	if(pChkFile == NULL || nSize < 1)
		return;

	dsf.strFeKey = pChkFile->stRetInfo.acWhiteHash;
	dsf.strFilePath = pChkFile->stFileInfo.acPath;
	dsf.strFileName = pChkFile->stFileInfo.acFile;
	dsf.nPeBit = 0;
	dsf.nPeType = 0;
	dsf.nUsedFlag = 1;

	pFindDsf = t_ManageSiteFile->FindPosSKeyID_Item(dsf.strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
	if(pFindDsf)
	{
		dsf.nID = pFindDsf->nID;
		dsf.nRegSvrID = pFindDsf->nRegSvrID;
		dsf.nRegDate = pFindDsf->nRegDate;
		dsf.nUsedMode	= pFindDsf->nUsedMode;
		dsf.nSyncSvrStep = pFindDsf->nSyncSvrStep;
		dsf.strFileHash = pFindDsf->strFileHash;
		dsf.nFileSize = pFindDsf->nFileSize;
	}
	else
	{
		dsf.nID = 0;
		dsf.nRegSvrID = 0;
		dsf.nRegDate = GetCurrentDateTimeInt();
		dsf.nUsedMode = STATUS_USED_MODE_ON;
		dsf.nSyncSvrStep = SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, pChkFile->stFileInfo.acFullPath, szSha256, ASIHASH_SHA_TYPE_256_LEN+1);
		szSha256[ASIHASH_SHA_TYPE_256_LEN] = 0;
		dsf.strFileHash = szSha256;
		dsf.nFileSize = GetFileSizeExt(pChkFile->stFileInfo.acFullPath);
	}

	SetSiteFile(&dsf, TRUE);
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFile::SetSiteFile(PDB_SITE_FILE pDsf, BOOL bRealTime)
{
	INT32 nOldItem = 0;

	if(pDsf == NULL)
		return;
	if(bRealTime)
		WriteLogN("[%s] remain real-time created file to server : fp[%s]:fn[%s]", m_strLogicName.c_str(), pDsf->strFilePath.c_str(), pDsf->strFileName.c_str());
	else
		WriteLogN("[%s] remain created file to server : fp[%s]:fn[%s]", m_strLogicName.c_str(), pDsf->strFilePath.c_str(), pDsf->strFileName.c_str());
	nOldItem = pDsf->nID;
	t_ManageSiteFile->ApplySiteFile(*pDsf);
	
	SendToken.Set(1024);
	SendToken.TokenAdd_32(1);
	t_ManageSiteFile->SetPkt(pDsf, SendToken);
		
	if(!nOldItem)
	{
		SendData_Link(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
	}

	do 
	{
		{
			PDB_PO_FE_PTN_OP pdata = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
			if(pdata && (pdata->nNewFileSendType & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
						((pDsf->nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
			{
				SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
				break;
			}
		}
		{
			PDB_PO_IN_PTN_OP pdata = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
			if(pdata && 
				(pdata->nRTFGMode & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
				((pDsf->nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
			{
				SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
				break;
			}
		}
		{
			PDB_PO_NC_PTN_OP pdata = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_OP);
			if(pdata && 
				(pdata->nRTFGMode & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && 
				((pDsf->nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
			{
				SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
				break;
			}
		}
	} while (FALSE);

	SendToken.Clear();
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
