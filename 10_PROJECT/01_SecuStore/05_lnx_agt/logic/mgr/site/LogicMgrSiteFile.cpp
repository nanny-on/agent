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
	DB_SITE_FILE stDsf;
	PDB_SITE_FILE pFindDsf = NULL;
	char acPath[MAX_PATH] = {0, }; 
	INT32 nSize = sizeof(DB_SITE_FILE);
	CHAR szSha256[ASIHASH_SHA_TYPE_256_LEN+2] = {0, };

	if(pdsf_src == NULL || nSize < 1)
		return;

	stDsf.strFeKey = pdsf_src->strFeKey;
	stDsf.strFilePath = pdsf_src->strFilePath;
	stDsf.strFileName = pdsf_src->strFileName;
	stDsf.nPeBit = 0;
	stDsf.nPeType = 0;
	stDsf.nUsedFlag = 1;

	pFindDsf = t_ManageSiteFile->FindPosSKeyID_Item(stDsf.strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
	if(pFindDsf)
	{
		stDsf.nID = pFindDsf->nID;
		stDsf.nRegSvrID = pFindDsf->nRegSvrID;
		stDsf.nRegDate = pFindDsf->nRegDate;
		stDsf.nUsedMode	= pFindDsf->nUsedMode;
		stDsf.nSyncSvrStep = pFindDsf->nSyncSvrStep;
		stDsf.strFileHash = pFindDsf->strFileHash;
		stDsf.nFileSize = pFindDsf->nFileSize;
	}
	else
	{
		stDsf.nID = 0;
		stDsf.nRegSvrID = 0;
		stDsf.nRegDate = GetCurrentDateTimeInt();
		stDsf.nUsedMode = STATUS_USED_MODE_ON;
		stDsf.nSyncSvrStep = SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
		snprintf(acPath, MAX_PATH-1, "%s/%s", stDsf.strFilePath.c_str(), stDsf.strFileName.c_str());
		acPath[MAX_PATH-1] = 0;
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, acPath, szSha256, ASIHASH_SHA_TYPE_256_LEN+1);
		szSha256[ASIHASH_SHA_TYPE_256_LEN] = 0;
		stDsf.strFileHash = szSha256;
		stDsf.nFileSize = GetFileSizeExt(acPath);
	}

	SetSiteFile(&stDsf, FALSE);
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
	BOOL bSendServer = FALSE;
	if(pDsf == NULL)
		return;
	nOldItem = pDsf->nID;
	t_ManageSiteFile->ApplySiteFile(*pDsf);
	PDB_PO_IN_PTN_OP pdata = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pdata && (pdata->nRTFGMode & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO) && ((pDsf->nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO)))
	{
		bSendServer = TRUE;
		if(bRealTime)
			WriteLogN("[%s] remain real-time created file to server with in_ptn_op : fp[%s/%s] key[%s]", m_strLogicName.c_str(), pDsf->strFilePath.c_str(), pDsf->strFileName.c_str(), pDsf->strFeKey.c_str());
		else
			WriteLogN("[%s] remain created file to server with in_ptn_op : fp[%s/%s] key[%s]", m_strLogicName.c_str(), pDsf->strFilePath.c_str(), pDsf->strFileName.c_str(), pDsf->strFeKey.c_str());
	}


	m_tMutex.Lock();
	SendToken.Set(1024);
	SendToken.TokenAdd_32(1);
	t_ManageSiteFile->SetPkt(pDsf, SendToken);
		
	if(!nOldItem)
	{
		SendData_Link(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
	}
	if(bSendServer == TRUE)
	{
		SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
	}
	SendToken.Clear();
	m_tMutex.UnLock();
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
	INT32 nListCount = 0;
	PDB_SITE_FILE pdsf = NULL;
	nListCount = tSendList.size();
	if(nListCount < 1)
		return;

	begin = tSendList.begin();	end = tSendList.end();
	while(nSendNum < nListCount)
	{
		nOnceNum = (((nListCount - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (nListCount - nSendNum));
		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			pdsf = (PDB_SITE_FILE)(*begin);
			if(pdsf != NULL)
			{
				t_ManageSiteFile->SetPkt(pdsf, SendToken);
				nSendNum += 1;
				nOnceNum -= 1;
			}
		}
		if(nSendNum != 0)
		{
			SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
		}
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrSiteFile::SendPkt_ReSend(INT32 nOnceMaxNum)
{
	TListID tIDList;
	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;
	INT32 nListCount = 0;
	TListIDItor begin, end;

	t_ManageSiteFile->GetItemIDList(tIDList);
	nListCount = tIDList.size();
	begin = tIDList.begin();	end = tIDList.end();
	if(*begin == 0)
	{
		nListCount--;
	}
	if(nListCount < 1)
		return;
	while(nSendNum < nListCount)
	{
		nOnceNum = (((nListCount - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (nListCount - nSendNum));
		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			if(*begin != 0)
			{
				t_ManageSiteFile->SetPkt(*begin, SendToken);
				nSendNum += 1;
				nOnceNum -= 1;
			}
		}
		if(nSendNum != 0)
		{
			SendData_Mgr(G_TYPE_SITE_FILE, G_CODE_COMMON_SYNC, SendToken);
		}
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
