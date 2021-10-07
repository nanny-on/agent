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

INT32				CASIDBFUtil::FastSetCurTable(CString strTName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName); 
	if(!pdts)
	{
		WriteLogE("not find table name (pre set dbf table item) : [%s]", (char *)(LPCSTR)strTName);
		return -1;
	}

	CASIDBFMember* pMember = GetMember();
	if(!pMember)
	{
		return -2;
	}

	pMember->m_tFastCurTable = pdts;
	return 0;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastPreSetDBF_TableItem(CString strColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen)
{
	CASIDBFMember* pMember = GetMember();
	if(!pMember)
	{
		return -1;
	}
	
	if(!pMember->m_tFastCurTable)
	{
		WriteLogE("not find table name (pre set dbf table item)");
		return -1;
	}

	DBF_TABLE_ITEM_SET_VALUE dfisv;

	switch(nMemberType)
	{
		case ASI_DBF_MEMBER_TYPE_UINT16:		dfisv.pUINT16	= (PUINT16)lpBuff;		nBufLen = 2;	break;
		case ASI_DBF_MEMBER_TYPE_UINT32:		dfisv.pUINT32	= (PUINT32)lpBuff;		nBufLen = 4;	break;
		case ASI_DBF_MEMBER_TYPE_UINT64:		dfisv.pUINT64	= (PUINT64)lpBuff;		nBufLen = 8;	break;
		case ASI_DBF_MEMBER_TYPE_STRING:
		case ASI_DBF_MEMBER_TYPE_LSTRING:
		{
			dfisv.pString = (LPCTSTR)lpBuff;
			dfisv.nBuffLen = nBufLen;
			break;
		}
		case ASI_DBF_MEMBER_TYPE_OBJECT:
		{
			dfisv.lpObject = lpBuff;
			dfisv.nBuffLen = nBufLen;
			break;
		}
		default:
		{
			WriteLogE("not define member type pre set dbf table item : [%d]", nMemberType);
			return -2;
			break;
		}
	}	

	pMember->m_tFastCurTableItemMap[strColName] = dfisv;
	pMember->m_nFastItemLen += nBufLen;
	return 0;
}
//---------------------------------------------------------------------

PDBF_TABLE_ITEM_SET_VALUE	CASIDBFUtil::FastFindPreSetTItemValue(CASIDBFMember* pMember, CString strColName)
{
	TMapTItemSetValueItor find = pMember->m_tFastCurTableItemMap.find(strColName);
	if(find == pMember->m_tFastCurTableItemMap.end())		return 0;

	return &(find->second);
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastMakeBuffer(PDBF_FAST_BUFF_INFO pDFBI, INT32 nMakeSize)
{
	pDFBI->nMkBufSize = nMakeSize;
	INT32 nMkSize = nMakeSize + 1;

	pDFBI->pBuffer = (PBYTE)malloc(nMkSize);
	if(!pDFBI->pBuffer)	return -1;
	
	//memset(pDFBI->pBuffer, 0, 1+nMakeSize);
	*(pDFBI->pBuffer + nMakeSize) = 0;

	pDFBI->nBuffLen = nMakeSize;
	return 0;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastCopyBuffer(PDBF_FAST_BUFF_INFO pDFBI, PVOID pCpBuff, INT32 nCpBufLen, INT32 nDefLen)
{
	if(pDFBI->pBuffer == NULL)	return -1;
	
	INT32 nChkBuffLen = max(nDefLen, nCpBufLen);

	do 
	{
		if(pDFBI->nBuffPos + nChkBuffLen > pDFBI->nBuffLen)
		{
			INT32 nNewBuffLen = pDFBI->nBuffLen + pDFBI->nMkBufSize;
			PBYTE pNewBuff = (PBYTE)malloc(nNewBuffLen + 1);
			//memset(pNewBuff, 0, nNewBuffLen + 1);
			*(pNewBuff + nNewBuffLen) = 0;
			memcpy(pNewBuff, pDFBI->pBuffer, pDFBI->nBuffPos);
			free(pDFBI->pBuffer);

			pDFBI->pBuffer = pNewBuff;
			pDFBI->nBuffLen = nNewBuffLen;
		}
	} while (pDFBI->nBuffPos + nChkBuffLen > pDFBI->nBuffLen);

	if(pCpBuff)
	{		
		memcpy(pDFBI->pBuffer + pDFBI->nBuffPos, (PBYTE)pCpBuff, nCpBufLen);

		if(nDefLen > nCpBufLen)
		{
			INT32 nZeroIdx = 0;
			while(nDefLen > (nCpBufLen + nZeroIdx))
			{
				*(pDFBI->pBuffer + pDFBI->nBuffPos + nCpBufLen + nZeroIdx) = 0;
				nZeroIdx += 1;
			}
		}
	}
	pDFBI->nBuffPos += nChkBuffLen;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastSetAutoIncrementID(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))	return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	CASIDBFMember* pMember = GetMember();	
	pMember->m_nLastInsID = 0;

	UINT32 nItemIdx = 0;
	UINT32 nBuffPos = 0;

	TListIDItor begin, end;
	begin = pdts->m_tAutoColList.begin();	end = pdts->m_tAutoColList.end();
	for(begin; begin != end; begin++)
	{
		nItemIdx = *begin;
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		
		if((tDTC.nColOption & ASI_DBF_COL_OPTION_FLAG_AUTO_INC) == 0)	continue;
		
		UINT64 nBufInfo = GetBuffLengthAll(tHLenMap, nItemIdx);
		PBYTE lpChkBuff = lpBuff + HIUINT64(nBufInfo);
		DBF_ITEM_RTN tDIR;

		switch(tDTC.nMemberType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:
			{
				tDIR.pRtnUINT16 = (PUINT16)lpChkBuff;
				pMember->m_nLastInsID = (UINT16)*(tDIR.pRtnUINT16);
				if(!*(tDIR.pRtnUINT16))	
				{
					pdts->tDT.tHInfo.nLastID += 1;
					*(tDIR.pRtnUINT16) = (UINT16)pdts->tDT.tHInfo.nLastID;
					pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
				}
				else if(*(tDIR.pRtnUINT16) > (UINT16)pdts->tDT.tHInfo.nLastID)
				{
					pdts->tDT.tHInfo.nLastID = *(tDIR.pRtnUINT16);
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_UINT32:
			{
				tDIR.pRtnUINT32 = (PUINT32)lpChkBuff;
				pMember->m_nLastInsID = (UINT32)*(tDIR.pRtnUINT32);
				if(!*(tDIR.pRtnUINT32))	
				{
					pdts->tDT.tHInfo.nLastID += 1;
					*(tDIR.pRtnUINT32) = (UINT32)pdts->tDT.tHInfo.nLastID;
					pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
				}
				else if(*(tDIR.pRtnUINT32) > (UINT32)pdts->tDT.tHInfo.nLastID)
				{
					pdts->tDT.tHInfo.nLastID = *(tDIR.pRtnUINT32);
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				tDIR.pRtnUINT64 = (PUINT64)lpChkBuff;
				pMember->m_nLastInsID = (UINT64)*(tDIR.pRtnUINT64);
				if(!*(tDIR.pRtnUINT64))	
				{
					pdts->tDT.tHInfo.nLastID += 1;
					*(tDIR.pRtnUINT64) = (UINT64)pdts->tDT.tHInfo.nLastID;
					pMember->m_nLastInsID = pdts->tDT.tHInfo.nLastID;
				}
				else if(*(tDIR.pRtnUINT64) > (UINT64)pdts->tDT.tHInfo.nLastID)
				{
					pdts->tDT.tHInfo.nLastID = *(tDIR.pRtnUINT64);
				}
				break;
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastExeSelectDBF(CString strTName, UINT32 nID)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		CASIDBFMember* pMember = GetMember();

		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		UINT64 nSFP = pdts->_find_fp(nID);
		if(!nSFP)
		{
			WriteLogN("not find item fp info : [fast select]");
			nRtn = ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
			break;
		}

		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);	
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, nSFP);
		
		FreeCommonBuff(&(pMember->m_pCurItemBuff));

		if(SetER(ReadDBF(&(pMember->m_tCurDBFIH), sizeof(DBF_TABLE_ITEM_HEADER))))
		{
			WriteLogN("ExeSelectDBF read item header fail [empty data]: [%.8x]", GetER());
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}	

		if(pMember->m_tCurDBFIH.nTID != pdts->tDT.tHInfo.nTID)
		{
			WriteLogN("ExeSelectDBF : current field is not request table item..[%d] != [%d]", pMember->m_tCurDBFIH.nTID, pdts->tDT.tHInfo.nTID);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = -10;		
			break;
		}

		if(pMember->m_tCurDBFIH.nDelFlag)
		{
			WriteLogN("ExeSelectDBF : current field status is delete..[%I64u][%I64u][%I64u]:[%u][%u]", nSFP, nCurFP, GetFilePointerEx(ASI_DBF_FILE_HANDLE_READ), pMember->m_tCurDBFIH.nCurBlockSize, pMember->m_tCurDBFIH.nInitBlockSize);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_V2_ALREADY_DELETE_ITEM;		
			break;
		}

		UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
		if(SetER(ReadDBF(pIHLBuff, pdts->m_nItemHLen)))
		{
			WriteLogN("ExeSelectDBF : read header fail [empty data]: [%.8x]", GetER());
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}	
		INT32 nSvLen = CopyHLenBuffToMap(pIHLBuff, pMember->m_tCurDBFILenMap, pdts);

		if(pMember->m_tCurDBFIH.nCurBlockSize == 0)
		{
			WriteLogN("ExeSelectDBF cur block size is zero");
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}

		if(pMember->m_tCurDBFIH.nCurBlockSize < (UINT32)nSvLen)
		{
			WriteLogN("NextDBF block size invalid : cur block size[%d] and sv size[%d]", pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}

		pMember->m_pCurItemBuff = (PBYTE)malloc(pMember->m_tCurDBFIH.nCurBlockSize + 1);
		if(!pMember->m_pCurItemBuff)
		{
			WriteLogE("ExeSelectDBF : make malloc fail : [%d]", pMember->m_tCurDBFIH.nCurBlockSize);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		*(pMember->m_pCurItemBuff + pMember->m_tCurDBFIH.nCurBlockSize + 1) = 0;

		if(SetER(ReadDBF(pMember->m_pCurItemBuff, pMember->m_tCurDBFIH.nCurBlockSize)))
		{
			WriteLogE("ExeSelectDBF : Item fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}

		if(SetER(ChgTableItemDec(&(pMember->m_tCurDBFIH), &(pMember->m_pCurItemBuff))))
		{
			WriteLogE("ExeSelectDBF : change decrypt fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}

		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		pMember->m_tCurSelTable = pdts;

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	
	m_tMutex.Lock();
	do 
	{
		CASIDBFMember* pMember = GetMember();

		if(pMember->m_tFastCurTable == NULL || pMember->m_tFastCurTableItemMap.size() == 0)
		{
			WriteLogN("not find table info : [fast insert]");
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		PDBF_TABLE_SEARCH pdts = pMember->m_tFastCurTable;
		DBF_FAST_BUFF_INFO tDFBI;

		if(FastMakeBuffer(&tDFBI, pMember->m_nFastItemLen))
		{
			nRtn = -2;
			break;
		}

	
		DBF_TABLE_ITEM_HEADER tDTIH;	
		TMapID64 tHLenMap;

		UINT32 nPriKeyType = 0;
		CString strPriKey = "";

		UINT32 nItemIdx = 0;
		while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
		{
			PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
			PDBF_TABLE_ITEM_SET_VALUE pdtisv = FastFindPreSetTItemValue(pMember, pdtc->szColName);
			
			UINT32 nInitLen = 0;
			switch(pdtc->nMemberType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
				{
					nInitLen = sizeof(UINT16);	

					if(!pdtisv)	FastCopyBuffer(&tDFBI, &(pdtc->nDefaultValue), nInitLen);
					else		FastCopyBuffer(&tDFBI, pdtisv->pUINT16, nInitLen);

					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT32:
				{
					nInitLen = sizeof(UINT32);	

					if(!pdtisv)	FastCopyBuffer(&tDFBI, &(pdtc->nDefaultValue), nInitLen);
					else		FastCopyBuffer(&tDFBI, pdtisv->pUINT32, nInitLen);
					
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT64:
				{
					nInitLen = sizeof(UINT64);			

					if(!pdtisv)	FastCopyBuffer(&tDFBI, &(pdtc->nDefaultValue), nInitLen);
					else		FastCopyBuffer(&tDFBI, pdtisv->pUINT64, nInitLen);

					break;
				}
				case ASI_DBF_MEMBER_TYPE_STRING:
				{
					if(!pdtisv)
					{
						nInitLen = pdtc->nInitStrLen;
						FastCopyBuffer(&tDFBI, NULL, 0, nInitLen);
					}
					else
					{
						nInitLen = max(pdtisv->nBuffLen, pdtc->nInitStrLen);
						FastCopyBuffer(&tDFBI, (PVOID)pdtisv->pString, pdtisv->nBuffLen, nInitLen);
					}
					break;
				}
				case ASI_DBF_MEMBER_TYPE_LSTRING:
				{
					if(!pdtisv)
					{
						nInitLen = GetLStrLen(pdtc->nInitStrLen);
						FastCopyBuffer(&tDFBI, NULL, 0, nInitLen);
					}
					else
					{
						nInitLen = max((INT32)pdtisv->nBuffLen, GetLStrLen(pdtc->nInitStrLen));
						FastCopyBuffer(&tDFBI, (PVOID)pdtisv->pString, pdtisv->nBuffLen, nInitLen);
					}	
					break;
				}
				case ASI_DBF_MEMBER_TYPE_OBJECT:
				{
					if(!pdtisv)
					{
						nInitLen = pdtc->nInitStrLen;
						FastCopyBuffer(&tDFBI, NULL, 0, nInitLen);
					}
					else
					{
						nInitLen = max(pdtisv->nBuffLen, pdtc->nInitStrLen);
						FastCopyBuffer(&tDFBI, pdtisv->lpObject, pdtisv->nBuffLen, nInitLen);
					}
					break;
				}
			}			
			SetBuffLength(tHLenMap, nInitLen, nItemIdx);
			tDTIH.nCurBlockSize += nInitLen;

			if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
			{
				switch(pdtc->nMemberType)
				{
					case ASI_DBF_MEMBER_TYPE_UINT16:				
					case ASI_DBF_MEMBER_TYPE_UINT32:
					case ASI_DBF_MEMBER_TYPE_UINT64:
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_UINT64;
						break;
					}
					case ASI_DBF_MEMBER_TYPE_STRING:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_STRING;
						if(pdtisv)	strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_LSTRING:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_LSTRING;
						if(pdtisv)	strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_OBJECT:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_OBJECT;
						//if(pdtisv)	strPriKey = pdtisv->lpObject;
						break;
					}
				}
			}
			nItemIdx++;
		}

		pMember->m_tFastCurTableItemMap.clear();
		pMember->m_nFastItemLen = 0;

		if(SetER(FastInsertDBF_TItem(pdts, &tDTIH, tHLenMap, tDFBI)))
		{
			tDFBI._clear();
			nRtn = GetER();
			break;
		}

		tDFBI._clear();
		if(pnNewFP)		*pnNewFP	= tDTIH.nCurFP;
		if(pnLastID)	*pnLastID	= pMember->m_nLastInsID;

		switch(nPriKeyType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:
			case ASI_DBF_MEMBER_TYPE_UINT32:
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				pdts->_apply_fp(pdts->tDT.tHInfo.nLastID, tDTIH.nCurFP);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			{
				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
				break;
			}
		}

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastExeUpdateDBF(UINT32 nID)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		CASIDBFMember* pMember = GetMember();		
		UINT64 nFP = 0;

		PDBF_TABLE_SEARCH pdts = pMember->m_tFastCurTable;
		if(!pdts)
		{
			WriteLogN("not find table info : [fast update]");
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		nFP = pdts->_find_fp(nID);
		if(!nFP)
		{
			WriteLogN("not find item fp info : [fast update]");
			nRtn = ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
			break;
		}
		
		DBF_TABLE_ITEM_HEADER tDTIH;
		DBF_FAST_BUFF_INFO tDFBI;
		TMapID64 tHLenMap;
		UINT32 nItemIdx = 0;

		UINT64	nPriKey = 0;
		CString strPriKey = "";

		if(FastMakeBuffer(&tDFBI, pMember->m_nFastItemLen))
		{
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);		
		{
			if(SetER(ExeSelectDBF(pdts->tDT.tHInfo.szName, nFP)))
			{
				WriteLogE("ExeUpdateDBF fail : old item read fail : [%s][%.8x]", pdts->tDT.tHInfo.szName, nFP);
				nRtn = GetER();
				break;
			}

			{
				tDTIH	= pMember->m_tCurDBFIH;
				tDTIH.nCurBlockSize	= 0;
			}
		}
		
		{
			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
			{
				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
				PDBF_ITEM_RTN pdir = GetField(pdts, nItemIdx);
				PDBF_TABLE_ITEM_SET_VALUE pdtisv = FastFindPreSetTItemValue(pMember, pdtc->szColName);

				UINT32 nInitLen = 0;
				switch(pdtc->nMemberType)
				{
					case ASI_DBF_MEMBER_TYPE_UINT16:
					{
						nInitLen = sizeof(UINT16);	

						if(!pdtisv)		FastCopyBuffer(&tDFBI, pdir->pRtnUINT16, nInitLen);
						else			FastCopyBuffer(&tDFBI, pdtisv->pUINT16, nInitLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_UINT32:
					{
						nInitLen = sizeof(UINT32);	

						if(!pdtisv)		FastCopyBuffer(&tDFBI, pdir->pRtnUINT32, nInitLen);
						else			FastCopyBuffer(&tDFBI, pdtisv->pUINT32, nInitLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_UINT64:	
					{
						nInitLen = sizeof(UINT64);	

						if(!pdtisv)		FastCopyBuffer(&tDFBI, pdir->pRtnUINT64, nInitLen);
						else			FastCopyBuffer(&tDFBI, pdtisv->pUINT64, nInitLen);		
						break;
					}
					case ASI_DBF_MEMBER_TYPE_STRING:		
					{
						if(!pdtisv)
						{
							nInitLen = max(pdtc->nInitStrLen, pdir->nRtnLen);
							FastCopyBuffer(&tDFBI, pdir->szRtnString, pdir->nRtnLen, nInitLen);
						}
						else
						{
							nInitLen = max(pdtc->nInitStrLen, pdtisv->nBuffLen);
							FastCopyBuffer(&tDFBI, (PVOID)pdtisv->pString, pdtisv->nBuffLen, nInitLen);
						}
						break;
					}
					case ASI_DBF_MEMBER_TYPE_LSTRING:		
					{
						if(!pdtisv)
						{
							nInitLen = max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdir->nRtnLen);
							FastCopyBuffer(&tDFBI, pdir->szRtnString, pdir->nRtnLen, nInitLen);
						}
						else
						{
							nInitLen = max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdtisv->nBuffLen);
							FastCopyBuffer(&tDFBI, (PVOID)pdtisv->pString, pdtisv->nBuffLen, nInitLen);
						}
					}
					case ASI_DBF_MEMBER_TYPE_OBJECT:		
					{
						if(!pdtisv)
						{
							nInitLen = max(pdtc->nInitStrLen, pdir->nRtnLen);
							FastCopyBuffer(&tDFBI, pdir->lpRtnObject, pdir->nRtnLen, nInitLen);
						}
						else
						{
							nInitLen = max(pdtc->nInitStrLen, pdtisv->nBuffLen);
							FastCopyBuffer(&tDFBI, pdtisv->lpObject, pdtisv->nBuffLen, nInitLen);
						}
						break;
					}
				}	

				if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
				{
					switch(pdtc->nMemberType)
					{
						case ASI_DBF_MEMBER_TYPE_UINT16:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT16);
							else			nPriKey = *(pdtisv->pUINT16);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT32:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT32);
							else			nPriKey = *(pdtisv->pUINT32);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT64:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT64);
							else			nPriKey = *(pdtisv->pUINT64);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_STRING:		
						{
							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
							else			strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_LSTRING:		
						{
							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
							else			strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_OBJECT:		
						{						
							break;
						}
					}
				}
				SetBuffLength(tHLenMap, nInitLen, nItemIdx);
				tDTIH.nCurBlockSize += nInitLen;
				nItemIdx++;
			}
		}

		{
			pMember->m_tFastCurTableItemMap.clear();
			pMember->m_nFastItemLen = 0;
		}

		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);		

			if(SetER(FastUpdateDBF_TItem(pdts, &tDTIH, tHLenMap, tDFBI)))
			{
				tDFBI._clear();
				FreeCommonBuff(&(pMember->m_pCurItemBuff));
				nRtn = GetER();
				break;
			}
			tDFBI._clear();
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
		}

		if(nPriKey)
		{
			pdts->_apply_fp(nPriKey, tDTIH.nCurFP);
		}
		else if(strPriKey.IsEmpty() == FALSE)
		{
			pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
		}

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastInsertDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, DBF_FAST_BUFF_INFO tDFBI)
{
	//TRACE("InsertDBF 1: [%I64u:%I64u][%d]\n", m_nCurrentFP, GetFilePointerEx(), pdfih->nCurBlockSize);
	PBYTE lpEncBuff = NULL;
	INT32 nRtn = 0;

	{
		FastSetAutoIncrementID(pdts, pdtih, tHLenMap, tDFBI.pBuffer);
	}

	ChkOpTickCntLog();
	if(SetER(ChgTableItemEnc(pdtih, tDFBI.pBuffer, &lpEncBuff)))
	{
		WriteLogE("InsertDBF_TItem fail by encrypt fail : [%.8x]", GetER());
	}
	if(lpEncBuff)	tDFBI.pBuffer = lpEncBuff;

	//WriteLogN("insert dbf file point : [%I64u]", m_nDBFLastFP);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, m_nDBFLastFP);		
	ChkOpTickCntLog();

	if(m_nDBFLastFP == pdts->tDT.tHInfo.nLastItemFP)
	{
		DBF_TABLE_ITEM_HEADER tDTIHLast;
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
		if(SetER(ReadDBF(&tDTIHLast, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
		{
			WriteLogE("InsertDBF_TItem fail by old item read : [%.8x]", GetER());
			SetFileEOF(m_nDBFLastFP);
			nRtn = GetER();
			return nRtn;
		}

		pdts->tDT.tHInfo.nLastItemFP = tDTIHLast.nCurFP;
	}

	DBF_TABLE_ITEM_HEADER tDTIH;
	pdtih->nCurFP			= m_nDBFLastFP;	
	pdtih->nPreFP			= pdts->tDT.tHInfo.nLastItemFP;
	pdtih->nNextFP			= 0;
	pdtih->nTID				= pdts->tDT.tHInfo.nTID;
	pdtih->nInitBlockSize	= pdtih->nCurBlockSize;
	UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
	CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);


	DBF_FAST_BUFF_INFO tDFBI_Item;
	do 
	{
		INT32 nMakeBufLen = sizeof(DBF_TABLE_ITEM_HEADER) + pdts->m_nItemHLen + pdtih->nInitBlockSize;
		FastMakeBuffer(&tDFBI_Item, nMakeBufLen);
		FastCopyBuffer(&tDFBI_Item, pdtih, sizeof(DBF_TABLE_ITEM_HEADER));
		FastCopyBuffer(&tDFBI_Item, pIHLBuff, pdts->m_nItemHLen);
		FastCopyBuffer(&tDFBI_Item, tDFBI.pBuffer, pdtih->nInitBlockSize);
		
		
		if(SetER(WriteDBF(tDFBI_Item.pBuffer, tDFBI_Item.nBuffPos)))
		{
			WriteLogE("InsertDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}		

		if(pdts->tDT.tHInfo.nLastItemFP)
		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
			if(SetER(ReadDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
			{
				WriteLogE("InsertDBF_TItem fail by old item read : [%.8x]", GetER());
				SetFileEOF(m_nDBFLastFP);
				nRtn = GetER();
				break;
			}
			tDTIH.nNextFP = m_nDBFLastFP;
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
			if(SetER(WriteDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER))))
			{
				WriteLogE("InsertDBF_TItem fail by old item write : [%.8x]", GetER());
				SetFileEOF(m_nDBFLastFP);
				nRtn = GetER();
				break;
			}
		}

		{
			if(pdts->tDT.tHInfo.nFirstItemFP == 0)	pdts->tDT.tHInfo.nFirstItemFP = m_nDBFLastFP;
			pdts->tDT.tHInfo.nLastItemFP	= m_nDBFLastFP;
			pdts->tDT.tHInfo.nItemCnt	+= 1;
			pdts->tDT.tHInfo.nItemTCnt	+= 1;

			if(SetER(UpdateDBF_Table(pdts)))
			{
				WriteLogE("InsertDBF_TItem fail by update table fail : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}
		}

		//WriteLogN("asidbfutil _ [%d]:[%I64u][%I64u:%I64u][%I64u]", pdtih->nTID, pdtih->nPreFP, pdtih->nCurFP, pdt->tHInfo.nLastItemFP, tDTIH.nNextFP);

		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
		m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	} while (FALSE);

	tDFBI_Item._clear();
	if(lpEncBuff)	FreeCommonBuff(&lpEncBuff);
	//TRACE("InsertDBF 3: [%I64u][%d]\n", m_nCurrentFP, pdfih->nInitBlockSize);
	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::FastUpdateDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, DBF_FAST_BUFF_INFO tDFBI)
{
	PBYTE lpEncBuff = NULL;
	INT32 nRtn = 0;

	if(SetER(ChgTableItemEnc(pdtih, tDFBI.pBuffer, &lpEncBuff)))
	{
		WriteLogE("InsertDBF_TItem fail by encrypt fail : [%.8x]", GetER());
	}
	if(lpEncBuff)	tDFBI.pBuffer = lpEncBuff;
	ChkOpTickCntLog();

	DBF_TABLE_ITEM_HEADER tDTIH;
	UINT8 tCurDFILen[ASI_DBF_MAX_COLUMN_NUMBER * 2] = {0, };
	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdtih->nCurFP);
	DBF_FAST_BUFF_INFO tDFBI_Item;

	do 
	{
		if(SetER(ReadDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
		{
			WriteLogE("UpdateDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}

		if(tDTIH.nTID != pdts->tDT.tHInfo.nTID)
		{
			WriteLogN("UpdateDBF_TItem : current field is not request table item..[%d] != [%d]", tDTIH.nTID, pdts->tDT.tHInfo.nTID);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = -10;
			break; 
		}

		if(tDTIH.nDelFlag)
		{
			WriteLogE("UpdateDBF_TItem fail : already delete row : [%s][%.8x]", pdts->tDT.tHInfo.szName, pdtih->nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_V2_ALREADY_DELETE_ITEM;
			break;
		}

		if(pdtih->nCurBlockSize > tDTIH.nInitBlockSize)
		{		
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, m_nDBFLastFP);
			WriteLogN("auto delete item by update item : [%s][%I64u][%u][%u]", pdts->tDT.tHInfo.szName, m_nDBFLastFP, pdtih->nCurBlockSize, tDTIH.nInitBlockSize);
			{
				pdtih->nCurFP			= m_nDBFLastFP;	
				pdtih->nPreFP			= tDTIH.nCurFP;
				pdtih->nNextFP			= tDTIH.nNextFP;

				pdtih->nUpCnt			= tDTIH.nUpCnt + 1;
				pdtih->nTID				= pdts->tDT.tHInfo.nTID;
				pdtih->nInitBlockSize	= pdtih->nCurBlockSize;

				UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
				CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);


				INT32 nMakeBufLen = sizeof(DBF_TABLE_ITEM_HEADER) + pdts->m_nItemHLen + pdtih->nInitBlockSize;
				FastMakeBuffer(&tDFBI_Item, nMakeBufLen);
				FastCopyBuffer(&tDFBI_Item, pdtih, sizeof(DBF_TABLE_ITEM_HEADER));
				FastCopyBuffer(&tDFBI_Item, pIHLBuff, pdts->m_nItemHLen);
				FastCopyBuffer(&tDFBI_Item, tDFBI.pBuffer, pdtih->nInitBlockSize);


				if(SetER(WriteDBF(tDFBI_Item.pBuffer, tDFBI_Item.nBuffPos)))
				{
					WriteLogE("UpdateDBF_TItem fail : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}	

				if(tDTIH.nNextFP)
				{
					DBF_TABLE_ITEM_HEADER tDTIH_Next;

					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH.nNextFP);
					if(SetER(ReadDBF(&tDTIH_Next, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
					{
						WriteLogE("UpdateDBF_TItem fail by old item read : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
					tDTIH_Next.nPreFP = m_nDBFLastFP;
					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH_Next.nCurFP);
					if(SetER(WriteDBF(&tDTIH_Next, sizeof(DBF_TABLE_ITEM_HEADER))))
					{
						WriteLogE("UpdateDBF_TItem fail by old item write : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
				}

				{
					tDTIH.nDelFlag = 1;
					tDTIH.nNextFP = m_nDBFLastFP;
					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH.nCurFP);
					if(SetER(WriteDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER))))
					{
						WriteLogE("UpdateDBF_TItem fail by old item update for del flag : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
					//WriteLogN("auto delete item by update item : [%I64u]->[%I64u]", tDTIH.nCurFP, tDTIH.nNextFP);
				}
			}

			{
				if(pdts->tDT.tHInfo.nLastItemFP == tDTIH.nCurFP)
				{
					pdts->tDT.tHInfo.nLastItemFP = m_nDBFLastFP;
				}

				pdts->tDT.tHInfo.nItemTCnt	+= 1;

				if(SetER(UpdateDBF_Table(pdts)))
				{
					WriteLogE("UpdateDBF_TItem fail by update table fail : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}
			}

			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
			m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
		}
		else
		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdtih->nCurFP);	
			pdtih->nInitBlockSize	= tDTIH.nInitBlockSize;
			UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
			CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);

			INT32 nMakeBufLen = sizeof(DBF_TABLE_ITEM_HEADER) + pdts->m_nItemHLen + pdtih->nInitBlockSize;
			FastMakeBuffer(&tDFBI_Item, nMakeBufLen);
			FastCopyBuffer(&tDFBI_Item, pdtih, sizeof(DBF_TABLE_ITEM_HEADER));
			FastCopyBuffer(&tDFBI_Item, pIHLBuff, pdts->m_nItemHLen);
			FastCopyBuffer(&tDFBI_Item, tDFBI.pBuffer, pdtih->nInitBlockSize);

			if(SetER(WriteDBF(tDFBI_Item.pBuffer, tDFBI_Item.nBuffPos)))
			{
				WriteLogE("UpdateDBF_TItem fail : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}	
		}
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		nRtn = 0;
	} while (FALSE);

	tDFBI_Item._clear();
	if(lpEncBuff)	FreeCommonBuff(&lpEncBuff);

	return nRtn;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastMakeFileBuff(CASIDBFMember* pMember, UINT32 nMode, UINT64 nFileFP, INT32 nBuffSize, INT32 nMoveMethod)
{
	HANDLE hFile = GetFileHandle(nMode);	
	if(!hFile)		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	pMember->ClearFastFileBuff();
	INT32 nNextBufSize = 0;
	INT32 nBackBufSize = 0;
	INT32 nTotalBuffSize = 0;
	UINT64 nReadFileFP = nFileFP;

	if(pMember->m_nCurSelDirection == 1)
	{
		nNextBufSize = ASI_DBF_FAST_FILE_BUFF_MAX_SIZE;
		if(nNextBufSize < nBuffSize)			nNextBufSize = nBuffSize;
	}
	else
	{
		nNextBufSize = ASI_DBF_FAST_FILE_BUFF_MIN_SIZE;
		if(nNextBufSize < nBuffSize)			nNextBufSize = nBuffSize;

		nBackBufSize = ASI_DBF_FAST_FILE_BUFF_MAX_SIZE;
		if(nBackBufSize < nBuffSize)		nBackBufSize = nBuffSize;

		if(nReadFileFP > nBackBufSize)	nReadFileFP -= nBackBufSize;
		else							nReadFileFP = 0;
	}

	LONG nHigh = HILONG(nReadFileFP);
	LONG nLow = LOLONG(nReadFileFP);
	if(SetFilePointer(hFile, nLow, &nHigh, nMoveMethod) == 0xFFFFFFFF)
	{
		WriteLogE("set file pointer fail : [%I64u][%d]", nReadFileFP, GetLastError());
		return ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
	}

	nTotalBuffSize = (nNextBufSize + nBackBufSize);
	INT32 nBuffLen = (INT32)(m_nDBFLastFP - nReadFileFP > nTotalBuffSize ? nTotalBuffSize : m_nDBFLastFP - nReadFileFP);

	pMember->m_tMutexFastFileBuff.Lock();

	pMember->m_pFastFileBuff = (PBYTE)malloc(nBuffLen + 1);
	if(SetER(ReadDBF(pMember->m_pFastFileBuff, nBuffLen, nMode)))
	{
		pMember->m_tMutexFastFileBuff.UnLock();
		pMember->ClearFastFileBuff();
		return GetER();
	}
	
	pMember->m_nFastFileReadNum += 1;
	pMember->m_nFastFileMode = nMode;
	pMember->m_nFastFileBuffLen = nBuffLen;
	pMember->m_nFastCFilePoint = nFileFP;
	pMember->m_nFastSFilePoint = nReadFileFP;
	pMember->m_nFastEFilePoint = nReadFileFP + nBuffLen;
	pMember->m_tMutexFastFileBuff.UnLock();
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastSetFilePointerEx(CASIDBFMember* pMember, UINT32 nMode, UINT64 nFileFP, INT32 nMoveMethod)
{
	HANDLE hFile = GetFileHandle(nMode);	
	if(!hFile)		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

//	LONG nHigh = HILONG(nFileFP);
//	LONG nLow = LOLONG(nFileFP);

	if(nFileFP < pMember->m_nFastSFilePoint || pMember->m_nFastEFilePoint <= nFileFP)
	{
		if(SetER(FastMakeFileBuff(pMember, nMode, nFileFP, 0, nMoveMethod)))
		{
			WriteLogE("set make file buff fail : [%I64u][%d]", nFileFP, GetLastError());
			return GetER();
		}
	}
	pMember->m_nFastCFilePoint = nFileFP;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastReadDBF(CASIDBFMember* pMember, PVOID lpBuff, UINT32 nSize, UINT32 nMode)
{
	if(!pMember)
	{
		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
	}
	INT32 nRtn = 0;
	pMember->m_tMutexFastFileBuff.Lock();
	do 
	{
		if(pMember->m_pFastFileBuff == NULL)
		{
			nRtn = ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
			break;
		}

		if(pMember->m_nFastEFilePoint <= pMember->m_nFastCFilePoint + nSize)
		{
			if(SetER(FastMakeFileBuff(pMember, pMember->m_nFastFileMode, pMember->m_nFastCFilePoint, nSize)))
			{
				WriteLogE("set make file buff fail : [%I64u][%d]", pMember->m_nFastCFilePoint, GetLastError());
				nRtn = GetER();
				break;
			}
		}

		{
			UINT32 nSPos = UINT32(pMember->m_nFastCFilePoint - pMember->m_nFastSFilePoint);
			memcpy(lpBuff, pMember->m_pFastFileBuff + (nSPos), nSize);

			pMember->m_nFastCFilePoint += nSize;
		}
	} while (FALSE);


	pMember->m_tMutexFastFileBuff.UnLock();

	return nRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastWriteFileBuff(PVOID lpBuff, INT32 nBuffLen, INT32 nMode)
{
	UINT64 nCurFP = GetFilePointerEx(nMode);

	m_tMutexMember.Lock();
	TMapASIDBFMemberItor begin, end;
	begin = m_tDbfUtilMemberMap.begin();	end = m_tDbfUtilMemberMap.end();
	for(begin; begin != end; begin++)
	{
		CASIDBFMember* pMember = (CASIDBFMember*)(begin->second);

		pMember->m_tMutexFastFileBuff.Lock();
		if(pMember->m_nFastSFilePoint <= nCurFP && nCurFP < pMember->m_nFastEFilePoint)
		{
			UINT32 nSPos = nCurFP - pMember->m_nFastSFilePoint; 
			UINT32 nCpLen = (nCurFP + nBuffLen <= pMember->m_nFastEFilePoint ? nBuffLen : pMember->m_nFastEFilePoint - nCurFP);

			memcpy(pMember->m_pFastFileBuff + nSPos, lpBuff, nCpLen);
		}
		pMember->m_tMutexFastFileBuff.UnLock();		
	}
	m_tMutexMember.UnLock();
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FastNextDBF_ItemBuff(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember)
{	
	INT32 nRtn = 0;
	INT32 nContinue = 1;

	m_tMutex.Lock();
	
	do 
	{
		if(pdts->tDT.tHInfo.nCheckItemFP == 0)// || pdt->tHInfo.nCheckItemEndFP < pdt->tHInfo.nCheckItemFP)
		{
			WriteLogN("next data end..[%s]", pdts->tDT.tHInfo.szName);
			pMember->m_tCurSelTable = NULL;
			nRtn = 1;
			break;
		}

		FastSetFilePointerEx(pMember, ASI_DBF_FILE_HANDLE_READ, pdts->tDT.tHInfo.nCheckItemFP);
		if(pMember->m_pCurItemBuff)	FreeCommonBuff(&(pMember->m_pCurItemBuff));
		
		if(SetER(FastReadDBF(pMember, &(pMember->m_tCurDBFIH), sizeof(DBF_TABLE_ITEM_HEADER))))
		{
			WriteLogN("NextDBF read header fail [empty data]: [%.8x][%I64u]", GetER(), pdts->tDT.tHInfo.nCheckItemFP);			
			nRtn = GetER();
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			break;
		}	

		if(pMember->m_tCurDBFIH.nDelFlag)
		{
			switch(pMember->m_nCurSelDirection)
			{
				case 1:		pdts->tDT.tHInfo.nCheckItemFP = pMember->m_tCurDBFIH.nNextFP;	break;
				case 2:		pdts->tDT.tHInfo.nCheckItemFP = pMember->m_tCurDBFIH.nPreFP;		break;
			}
			continue;
		}

		UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
		if(SetER(FastReadDBF(pMember, pIHLBuff, pdts->m_nItemHLen)))
		{
			WriteLogN("NextDBF read header fail [empty data]: [%.8x]", GetER());
			nRtn = GetER();
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			break;
		}	
		INT32 nSvLen = CopyHLenBuffToMap(pIHLBuff, pMember->m_tCurDBFILenMap, pdts);
		//WriteLogN("NextDBF 2: [%I64u][%I64u]\n", GetFilePointerEx(), m_nNextDataFP);

		if(pMember->m_tCurDBFIH.nCurBlockSize == 0)
		{
			WriteLogN("NextDBF cur block size is zero");
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}		
		//TRACE("NextDBF block size log : cur block size[%d] and sv size[%d]\n", pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);

		if(pMember->m_tCurDBFIH.nCurBlockSize < (UINT32)nSvLen)
		{
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			WriteLogE("NextDBF : detect invalid block size..reserved recovery dbf : [%s][%d/%d]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_BLOCK_SIZE;
			break;
		}

		pMember->m_pCurItemBuff = (PBYTE)MakeCommonBuff(pMember->m_tCurDBFIH.nCurBlockSize);
		if(!pMember->m_pCurItemBuff)
		{
			WriteLogE("NextDBF make malloc fail : [%d][%s]", pMember->m_tCurDBFIH.nCurBlockSize, pdts->tDT.tHInfo.szName);
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		if(SetER(FastReadDBF(pMember, pMember->m_pCurItemBuff, pMember->m_tCurDBFIH.nCurBlockSize)))
		{
			WriteLogE("NextDBF Item fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			nRtn = GetER();
			break;
		}

		if(SetER(ChgTableItemDec(&(pMember->m_tCurDBFIH), &(pMember->m_pCurItemBuff))))
		{
			WriteLogE("NextDBF : change decrypt fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			nRtn = GetER();
			break;
		}

		if(SetER(ApplyKeyToFPInfo(pdts)))
		{

		}

		//WriteLogN("NextDBF 3: [%I64u][%I64u]\n", GetFilePointerEx(), m_nNextDataFP);
		UINT64 nCheckItemFP = 0;
		switch(pMember->m_nCurSelDirection)
		{
			case 1:		
			{
// 				if(pdts->tDT.tHInfo.nCheckItemFP != m_tCurDBFIH.nNextFP && 
// 					pdts->tDT.tHInfo.nCheckItemFP != pdts->tDT.tHInfo.nLastItemFP)
				{
					if(SetFPKeyMap(pMember->m_tCurDBFIH.nNextFP))
					{
						m_tDB.tInfo.nAutoRestore = 1;
						UpdateDBF_DI();
						WriteLogE("NextDBF : detect duplicate fp..reserved recovery dbf : [%s][%I64u]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nNextFP);
					}
					else
					{
						nCheckItemFP = pMember->m_tCurDBFIH.nNextFP;	
					}
				}
				break;
			}
			case 2:	
			{
// 				if(pdts->tDT.tHInfo.nCheckItemFP != m_tCurDBFIH.nPreFP && 
// 					pdts->tDT.tHInfo.nCheckItemFP != pdts->tDT.tHInfo.nFirstItemFP)
				{
					if(SetFPKeyMap(pMember->m_tCurDBFIH.nPreFP))
					{
						m_tDB.tInfo.nAutoRestore = 1;
						UpdateDBF_DI();
						WriteLogE("NextDBF : detect duplicate fp..reserved recovery dbf : [%s][%I64u]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nNextFP);
					}
					else
					{
						nCheckItemFP = pMember->m_tCurDBFIH.nPreFP;	
					}
				}
				break;
			}
		}
		pdts->tDT.tHInfo.nCheckItemFP = nCheckItemFP;
		nContinue = 0;
	} while(nContinue);	

	m_tMutex.UnLock();

	return nRtn;
}
//---------------------------------------------------------------------
