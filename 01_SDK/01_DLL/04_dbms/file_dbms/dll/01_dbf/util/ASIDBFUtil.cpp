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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "asi_glb_w32_file.h"
#include "ASIDBFUtil.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIDBFUtil::CASIDBFUtil()
{	
 	m_nDBFLastFP			= 0;
	m_nAuthResult			= 0;

// 	m_nLastInsID			= 0;

	m_nChkOpTickCnt			= 0;	
}
//---------------------------------------------------------------------

CASIDBFUtil::~CASIDBFUtil()
{
	CloseDBF();
	ClearMember();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetASyncMode(UINT32 nMode)
{
	if(m_tFHMap.size())		return -1;

	return 0;
}
//---------------------------------------------------------------------

HANDLE		CASIDBFUtil::GetFileHandle(UINT32 nMode)
{
	nMode = ASI_DBF_FILE_HANDLE_ALL;

	TMapPVOIDItor find = m_tFHMap.find(nMode);
	if(find == m_tFHMap.end())
		return 0;		

	return find->second;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetFileHandle(HANDLE hFile, UINT32 nMode)
{
	m_tFHMap[ASI_DBF_FILE_HANDLE_ALL] = (PVOID)hFile;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ReadDBF(PVOID lpBuff, UINT32 nSize, UINT32 nMode)
{
	HANDLE hFile = GetFileHandle(nMode);	
	if(!hFile)
		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	PBYTE lpReadBuf = (PBYTE)lpBuff;	
	DWORD dwReadSize = 0;
	while(dwReadSize != nSize)
	{
		DWORD dwOnceReadSize = 0;
		if(ReadFile(hFile, lpReadBuf + dwReadSize, nSize - dwReadSize, &dwOnceReadSize, NULL) == FALSE)
		{
			if(errno != EINTR && errno != EAGAIN)
			{
				return ASI_DBF_ERROR_TYPE_FILE_HANDLE_EOF;
			}
		}
		else if(dwOnceReadSize == 0)
		{
			return ASI_DBF_ERROR_TYPE_FILE_HANDLE_EOF;
		}
		dwReadSize += dwOnceReadSize;

	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::WriteDBF(PVOID lpBuff, UINT32 nSize)
{
	HANDLE hFileW = GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE);
	if(!hFileW)	return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	FastWriteFileBuff(lpBuff, nSize);
	
	PBYTE lpWriteBuff = (PBYTE)lpBuff;	
	INT32 dwWritedSize = 0;
	while((UINT32)dwWritedSize != nSize)
	{
		DWORD dwWritedByte = 0;

		WriteFile(hFileW, &(lpWriteBuff[dwWritedSize]), nSize - dwWritedSize, &dwWritedByte, NULL);
		dwWritedSize += dwWritedByte;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::MoveDBF(UINT32 nMode, UINT64 nMoveSize)
{
	if(!GetFileHandle(nMode))	return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	SetFilePointerEx(nMode, nMoveSize, FILE_CURRENT);
	return 0;
}
//---------------------------------------------------------------------

LPTSTR		CASIDBFUtil::GetFieldName(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	return pdts->tDT.tColInfo[nIdx].szColName;
}
//---------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFUtil::GetField(INT32 nIdx)
{
	CASIDBFMember* pMember = GetMember();	
	if(!pMember->m_tCurSelTable)		return NULL;
	
	return GetField(pMember->m_tCurSelTable, nIdx);
}
//---------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFUtil::GetField(LPCTSTR lpName)
{
	CASIDBFMember* pMember = GetMember();
	if(!pMember->m_tCurSelTable)		return NULL;
	return GetField(pMember->m_tCurSelTable, lpName);
}
//---------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFUtil::GetField(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{	
	INT32 nIdx = pdts->_find_idx(lpName);
	return GetField(pdts, nIdx);
}
//---------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFUtil::GetField(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	CASIDBFMember* pMember = GetMember();
	
	//UINT64 nItemLen = GetFieldLength(pdts, nIdx);
	UINT64 nItemLenPair = GetBuffLengthAll(pMember->m_tCurDBFILenMap, nIdx);
	
	UINT32 nItemLen = LOUINT64(nItemLenPair);
	UINT32 nItemSPos = HIUINT64(nItemLenPair);

	pMember->m_tCurDBFIR._clear();

	pMember->m_tCurDBFIR.nRtnType = GetFieldType(pdts, nIdx);
	pMember->m_tCurDBFIR.nRtnLen = nItemLen;

	CopyField(&(pMember->m_tCurDBFIR), pMember->m_pCurItemBuff, nItemSPos);

	return &(pMember->m_tCurDBFIR);
}
//---------------------------------------------------------------------

DBF_TABLE_ITEM	CASIDBFUtil::GetFieldDefValue(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	DBF_TABLE_ITEM dfir;
	dfir.nRtnType	= pdts->tDT.tColInfo[nIdx].nMemberType;
	switch(dfir.nRtnType)
	{
		case ASI_DBF_MEMBER_TYPE_UINT16:		dfir.nRtnUINT16	= ((UINT16)pdts->tDT.tColInfo[nIdx].nDefaultValue);		break;
		case ASI_DBF_MEMBER_TYPE_UINT32:		dfir.nRtnUINT32	= ((UINT32)pdts->tDT.tColInfo[nIdx].nDefaultValue);		break;
		case ASI_DBF_MEMBER_TYPE_UINT64:		dfir.nRtnUINT64	= ((UINT64)pdts->tDT.tColInfo[nIdx].nDefaultValue);		break;
	}
	return dfir;
}
//---------------------------------------------------------------------

DBF_TABLE_ITEM	CASIDBFUtil::GetFieldDefValue(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{
	DBF_TABLE_ITEM dfir;

	INT32 nItemIdx = pdts->_find_idx(lpName);
	if(nItemIdx < 0)	return dfir;

	return GetFieldDefValue(pdts, nItemIdx);
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::GetFieldType(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	return pdts->tDT.tColInfo[nIdx].nMemberType;	
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::GetFieldType(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{
	INT32 nItemIdx = pdts->_find_idx(lpName);
	if(nItemIdx < 0)	return 0;

	return GetFieldType(pdts, nItemIdx);
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::GetFieldLength(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	CASIDBFMember* pMember = GetMember();
	return GetBuffLength(pMember->m_tCurDBFILenMap, nIdx);
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::GetFieldLength(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{
	INT32 nItemIdx = pdts->_find_idx(lpName);
	if(nItemIdx < 0)	return 0;

	return GetFieldLength(pdts, nItemIdx);
}
//---------------------------------------------------------------------

UINT32				CASIDBFUtil::GetFieldInitLength(PDBF_TABLE_SEARCH pdts, INT32 nIdx)
{
	UINT32 nRtn = 0;
	switch(pdts->tDT.tColInfo[nIdx].nMemberType)
	{
		case ASI_DBF_MEMBER_TYPE_UINT16:		nRtn = sizeof(UINT16);							break;
		case ASI_DBF_MEMBER_TYPE_UINT32:		nRtn = sizeof(UINT32);							break;
		case ASI_DBF_MEMBER_TYPE_UINT64:		nRtn = sizeof(UINT64);							break;
		case ASI_DBF_MEMBER_TYPE_STRING:		nRtn = pdts->tDT.tColInfo[nIdx].nInitStrLen;				break;
		case ASI_DBF_MEMBER_TYPE_LSTRING:		nRtn = GetLStrLen(pdts->tDT.tColInfo[nIdx].nInitStrLen);	break;
		case ASI_DBF_MEMBER_TYPE_OBJECT:		nRtn = pdts->tDT.tColInfo[nIdx].nInitStrLen;				break;
	}	
	return nRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CopyField(PDBF_ITEM_RTN pDIR, PBYTE pOrBuffer, INT32 nCpSPos)
{
	switch(pDIR->nRtnType)
	{		
		case ASI_DBF_MEMBER_TYPE_OBJECT:
		{
			if(pDIR->nRtnLen)
			{
				pDIR->lpRtnObject = pOrBuffer + nCpSPos;
			}
			break;
		}
		case ASI_DBF_MEMBER_TYPE_STRING:
		{
			if(pDIR->nRtnLen)	
			{
				pDIR->szRtnString = (PCHAR)(pOrBuffer + nCpSPos);
			}
			break;
		}
		case ASI_DBF_MEMBER_TYPE_LSTRING:
		{			
			if(pDIR->nRtnLen)
			{
				pDIR->szRtnString = (PCHAR)(pOrBuffer + nCpSPos);
			}
			break;
		}
		case ASI_DBF_MEMBER_TYPE_UINT16:
		{
			pDIR->pRtnUINT16 = (PUINT16)(pOrBuffer + nCpSPos);
			break;
		}
		case ASI_DBF_MEMBER_TYPE_UINT32:
		{
			pDIR->pRtnUINT32 = (PUINT32)(pOrBuffer + nCpSPos);
			break;
		}
		case ASI_DBF_MEMBER_TYPE_UINT64:
		{
			pDIR->pRtnUINT64 = (PUINT64)(pOrBuffer + nCpSPos);
			break;
		}
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::MakeField(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember)
{
	{
		INT32 nMaxIdx = pdts->tDT.tHInfo.nColNumber;
		if(pMember->m_nSelColNum)		nMaxIdx = pMember->m_nSelColNum;

		INT32 nChkIdx = 0;
		while(nChkIdx < nMaxIdx)
		{
			INT32 nColIdx = nChkIdx;
			if(pMember->m_nSelColNum)	nColIdx = pMember->m_tSelColIDArray[nChkIdx];

			UINT64 nItemLenPair = GetBuffLengthAll(pMember->m_tCurDBFILenMap, nColIdx);
			UINT32 nItemLen = LOUINT64(nItemLenPair);
			UINT32 nItemSPos = HIUINT64(nItemLenPair);

			DBF_ITEM_RTN tDIR;
			tDIR.nRtnType = GetFieldType(pdts, nColIdx);
			tDIR.nRtnLen = nItemLen;

			CopyField(&tDIR, pMember->m_pCurItemBuff, nItemSPos);
			pMember->m_tCurDBFIRArray[nChkIdx] = tDIR;

			nChkIdx += 1;
		}
	}
	return 0;
}
//---------------------------------------------------------------------

UINT64		CASIDBFUtil::GetFieldFP()
{
	CASIDBFMember* pMember = GetMember();
	return pMember->m_tCurDBFIH.nCurFP;
}
//---------------------------------------------------------------------

UINT32				CASIDBFUtil::GetTableNextID(LPCTSTR lpTName, PUINT64 pNextID)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(lpTName);
	if(!pdts)
	{
		WriteLogN("not find table info : [%s]", lpTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	pdts->tDT.tHInfo.nLastID += 1;
	if(pNextID)	*pNextID = pdts->tDT.tHInfo.nLastID;
	return 0;
}
//---------------------------------------------------------------------

UINT32				CASIDBFUtil::GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(lpTName);
	if(!pdts)
	{
		WriteLogN("not find table info : [%s]", lpTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	if(pnFP)		*pnFP = pdts->_find_fp(nKey);
	return 0;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::IsExistField(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{
	INT32 nItemIdx = pdts->_find_idx(lpName);
	if(nItemIdx < 0)	return 0;

	return 1;
}
//---------------------------------------------------------------------

PDBF_TABLE_COLUMN	CASIDBFUtil::FindFieldColum(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName)
{
	INT32 nItemIdx = pdts->_find_idx(lpName);
	if(nItemIdx < 0)	return NULL;

	return &(pdts->tDT.tColInfo[nItemIdx]);
}
//---------------------------------------------------------------------

CASIDBFMember*			CASIDBFUtil::GetMember()
{
	CASIDBFMember*	pRtn = NULL;
	m_tMutexMember.Lock();

	UINT64 nThreadID = (UINT64)gettid();
	TMapASIDBFMemberItor find = m_tDbfUtilMemberMap.find(nThreadID);
	if(find == m_tDbfUtilMemberMap.end())
	{
		CASIDBFMember* pMember = new CASIDBFMember();;
		m_tDbfUtilMemberMap[nThreadID] = pMember;
		find = m_tDbfUtilMemberMap.find(nThreadID);	
	}

	pRtn = (CASIDBFMember*)(find->second);
	m_tMutexMember.UnLock();
	return pRtn;
}
//---------------------------------------------------------------------

void					CASIDBFUtil::ClearMember()
{
	TMapASIDBFMemberItor begin, end;
	begin = m_tDbfUtilMemberMap.begin();	end = m_tDbfUtilMemberMap.end();
	for(begin; begin != end; begin++)
	{
		CASIDBFMember* pdata = (CASIDBFMember*)(begin->second);
		if(!pdata)	continue;
		
		//pdata->StopThread();
		pdata->ClearMember();

		delete pdata;
	}
	m_tDbfUtilMemberMap.clear();
	return;
}
//---------------------------------------------------------------------
