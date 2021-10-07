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
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetER(INT32 nRtn)
{
	CASIDBFMember* pMember = GetMember();
	pMember->m_nErrRtn = nRtn;
	return pMember->m_nErrRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetER()
{
	CASIDBFMember* pMember = GetMember();
	return pMember->m_nErrRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetOpTickCntLog(UINT32 nChkOpTickCnt)
{
	m_nChkOpTickCnt = nChkOpTickCnt;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::InitOpTickCntLog()
{
	if(!m_nChkOpTickCnt)	return 0;

	m_nChkOpTickID	= 0;
	m_nChkOpTickCntIdx = 0;
	memset(m_tChkOpTickCntMap, 0, sizeof(UINT64) * 100);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ChkOpTickCntLog(UINT32 nKey, UINT32 nValue)
{
	if(!m_nChkOpTickCnt)	return 0;
	if(m_nChkOpTickCntIdx > 98)		return 0;

	if(!nKey)						nKey = ++m_nChkOpTickID;
	else if(nKey > m_nChkOpTickID)	m_nChkOpTickID = nKey;

	if(!nValue)		nValue = GetTickCount();	

	m_tChkOpTickCntMap[m_nChkOpTickCntIdx++] = MAKEUINT64(nKey, nValue);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::WriteLogOpTickCnt(CString strTName)
{
	if(!m_nChkOpTickCnt)	return 0;

	UINT32 nLTickCnt = GetTickCount();
	UINT32 nSTickCnt = 0;
	{
		nSTickCnt = HIUINT64(m_tChkOpTickCntMap[1]);
		if(m_nChkOpTickCnt > (nLTickCnt - nSTickCnt))	return 0;		
	}
	
	{
		CString strTickLog, strTickTemp;		
		UINT32 nIdx = 0;
		UINT32 nKey, nValue;

		for(nIdx; nIdx < m_nChkOpTickCntIdx; nIdx++)
		{
			strTickTemp.Empty();
			nKey = LOUINT64(m_tChkOpTickCntMap[nIdx]);
			nValue = HIUINT64(m_tChkOpTickCntMap[nIdx]);
			
			switch(nKey)
			{
				case 1:		strTickTemp.Format("[%u:%u]", nKey, nValue);		break;
				case 2:		
				{
					strTickTemp.Format("[%u:%d]", nKey, nLTickCnt - nValue);
					nSTickCnt = nValue;
					break;
				}
				default:
				{
					if(nValue - nSTickCnt)
					{
						strTickTemp.Format("[%u:%d]", nKey, nValue - nSTickCnt);
						nSTickCnt = nValue;
					}
					break;
				}
			}
			if(strTickTemp.IsEmpty())	continue;

			strTickLog += strTickTemp;
		}
		WriteLogN("dbf opt info [%s] : %s", (LPCSTR)strTName, (LPCSTR)strTickLog);
	}
	
	return 0;	
}
//---------------------------------------------------------------------

PBYTE		CASIDBFUtil::MakeCommonBuff(UINT32 nSize)
{
	PBYTE lpBuff = (PBYTE)malloc(sizeof(BYTE) * (nSize + 2));
	if(!lpBuff)	return NULL;

	memset(lpBuff, 0, (sizeof(BYTE) * (nSize + 2)));
	return lpBuff;
}
//---------------------------------------------------------------------

void		CASIDBFUtil::FreeCommonBuff(PBYTE* lpBuff)
{
	if(*lpBuff)
	{
		free(*lpBuff);
		*lpBuff = NULL;
	}
	return;
}
//---------------------------------------------------------------------

void		CASIDBFUtil::SetBuffLength(TMapID64& tHLenMap, UINT32 nLen, INT32 nIdx)
{
    tHLenMap[nIdx] = MAKEUINT64(nLen, 0);
    return;
}
//---------------------------------------------------------------------------

UINT32		CASIDBFUtil::GetBuffLength(TMapID64& tHLenMap, INT32 nIdx)
{
	TMapID64Itor find = tHLenMap.find(nIdx);
	if(find == tHLenMap.end())	return 0;

	return LOUINT64(find->second);
}
//---------------------------------------------------------------------------

UINT64		CASIDBFUtil::GetBuffLengthAll(TMapID64& tLenMap, INT32 nIdxNum)
{
	TMapID64Itor find = tLenMap.find(nIdxNum);
	if(find == tLenMap.end())	return 0;

	return find->second;	
}
//---------------------------------------------------------------------------

INT32		CASIDBFUtil::CopyHLenBuffToMap(PBYTE lpHLenBuff, TMapID64& tHLenMap, PDBF_TABLE_SEARCH pdts)
{
	INT32 nItemIdx = 0;
	INT32 nBufPos = 0;
	INT32 nSvLen = 0;
	
	UINT32 nLen = 0;
	while(pdts->tDT.tHInfo.nColNumber > (UINT32)nItemIdx)
	{
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		nLen = 0;

		if(tDTC.nMemberType == ASI_DBF_MEMBER_TYPE_LSTRING)
		{
			memcpy(&nLen, lpHLenBuff + nBufPos, 4);
			nBufPos += 4;
		}
		else
		{
			memcpy(&nLen, lpHLenBuff + nBufPos, 2);
			nBufPos += 2;
		}

		tHLenMap[nItemIdx] = MAKEUINT64(nLen, nSvLen);
		nSvLen += nLen;
		nItemIdx += 1;
	}
	return nSvLen;
}
//---------------------------------------------------------------------------

void		CASIDBFUtil::CopyHLenMapToBuff(TMapID64& tHLenMap, PBYTE lpHLenBuff, PDBF_TABLE_SEARCH pdts)
{
	INT32 nItemIdx = 0;
	INT32 nBufPos = 0;

	UINT32 nLen = 0;

	while(pdts->tDT.tHInfo.nColNumber > (UINT32)nItemIdx)
	{	
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		nLen = 0;

		{
			TMapID64Itor find = tHLenMap.find(nItemIdx);
			if(find != tHLenMap.end())	nLen = LOUINT64(find->second);
		}

		if(tDTC.nMemberType == ASI_DBF_MEMBER_TYPE_LSTRING)
		{
			memcpy(lpHLenBuff + nBufPos, &nLen, 4);
			nBufPos += 4;
		}
		else
		{
			memcpy(lpHLenBuff + nBufPos, &nLen, 2);
			nBufPos += 2;
		}
		nItemIdx += 1;
	}
	return;
}
//---------------------------------------------------------------------------

UINT64		CASIDBFUtil::GetFilePointerEx(UINT32 nMode)
{
	HANDLE hFile = GetFileHandle(nMode);	
	if(!hFile)		return 0;
	    
	LONG nHigh = 0;
    UINT32 nLow = SetFilePointer(hFile, 0, &nHigh, FILE_CURRENT);
	return MAKEUINT64(nLow, nHigh);	
}
//---------------------------------------------------------------------

UINT64		CASIDBFUtil::SetFilePointerEx(UINT32 nMode, UINT64 nFileFP, INT32 nMoveMethod)
{
	HANDLE hFile = GetFileHandle(nMode);	
	if(!hFile)		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
		
    LONG nHigh = HILONG(nFileFP);
    LONG nLow = LOLONG(nFileFP);	
    if(SetFilePointer(hFile, nLow, &nHigh, nMoveMethod) == 0xFFFFFFFF)
	{
		WriteLogE("set file pointer fail : [%I64u][%d]", nFileFP, GetLastError());
		return ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
	}

	return 0;
}
//---------------------------------------------------------------------

UINT64		CASIDBFUtil::SetFileEOF(UINT64 nFP)
{
	HANDLE hFileW = GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE);	
	if(!hFileW)		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	CHAR chEOF = ASI_DBF_EOF;
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nFP);
	WriteDBF(&chEOF, sizeof(CHAR));
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
	return GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetAutoIncrementID(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))	return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	CASIDBFMember* pMember = GetMember();
	
	pMember->m_nLastInsID = 0;
	UINT32 nItemIdx = 0;
	UINT32 nBuffPos = 0;
	while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
	{
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		DBF_TABLE_ITEM tDTI;
		
		if((tDTC.nColOption & ASI_DBF_COL_OPTION_FLAG_AUTO_INC))
		{
			PBYTE lpChkBuff = lpBuff + nBuffPos;
			switch(tDTC.nMemberType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
				{
					memcpy(&(tDTI.nRtnUINT16), lpChkBuff, sizeof(UINT16));
					pMember->m_nLastInsID = tDTI.nRtnUINT16;

					if(!tDTI.nRtnUINT16)	
					{
						pdts->tDT.tHInfo.nLastID += 1;
						tDTI.nRtnUINT16 = (UINT16)pdts->tDT.tHInfo.nLastID;
						memcpy(lpChkBuff, &(tDTI.nRtnUINT16), sizeof(UINT16));
						pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
					}
					else if(tDTI.nRtnUINT16 > (UINT16)pdts->tDT.tHInfo.nLastID)
					{
						pdts->tDT.tHInfo.nLastID = tDTI.nRtnUINT16;
					}
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT32:
				{
					memcpy(&(tDTI.nRtnUINT32), lpChkBuff, sizeof(UINT32));		
					pMember->m_nLastInsID = tDTI.nRtnUINT32;

					if(!tDTI.nRtnUINT32)	
					{
						pdts->tDT.tHInfo.nLastID += 1;
						tDTI.nRtnUINT32 = (UINT32)pdts->tDT.tHInfo.nLastID;
						memcpy(lpChkBuff, &(tDTI.nRtnUINT32), sizeof(UINT32));
						pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
					}
					else if(tDTI.nRtnUINT32 > (UINT32)pdts->tDT.tHInfo.nLastID)
					{
						pdts->tDT.tHInfo.nLastID = tDTI.nRtnUINT32;
					}
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT64:
				{
					memcpy(&(tDTI.nRtnUINT64), lpChkBuff, sizeof(UINT64));
					pMember->m_nLastInsID = tDTI.nRtnUINT64;

					if(!tDTI.nRtnUINT64)	
					{
						pdts->tDT.tHInfo.nLastID += 1;
						tDTI.nRtnUINT64 = (UINT64)pdts->tDT.tHInfo.nLastID;
						memcpy(lpChkBuff, &(tDTI.nRtnUINT64), sizeof(UINT64));
						pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
					}
					else if(tDTI.nRtnUINT64 > pdts->tDT.tHInfo.nLastID)
					{
						pdts->tDT.tHInfo.nLastID = tDTI.nRtnUINT64;
					}
					break;
				}
			}
			break;
		}
		nBuffPos += GetBuffLength(tHLenMap, nItemIdx);
		nItemIdx++;
	}
	return 0;
}
