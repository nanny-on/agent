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

#if !defined(AFX_ASIDBFMEMBER_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_)
#define AFX_ASIDBFMEMBER_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_

class CASIDBFMember
{
public:
	UINT32						m_nErrRtn;

public:
	UINT64						m_nLastInsID;
	
public:
	PDBF_TABLE_SEARCH			m_tCurSelTable;
	INT32						m_nCurSelDirection;
	PBYTE						m_pCurItemBuff;
	TMapID64					m_tCurDBFILenMap;	
	DBF_TABLE_ITEM_HEADER		m_tCurDBFIH;
	DBF_ITEM_RTN				m_tCurDBFIR;
	DBF_ITEM_RTN				m_tCurDBFIRArray[ASI_DBF_MAX_COLUMN_NUMBER];

public:
	PDBF_TABLE_SEARCH			m_tFastCurTable;
	TMapTItemSetValue			m_tFastCurTableItemMap;
	UINT32						m_nFastItemLen;

	CMutexExt					m_tMutexFastFileBuff;
	UINT32						m_nFastFileMode;
	PBYTE						m_pFastFileBuff;
	UINT32						m_nFastFileBuffLen;
	UINT64						m_nFastSFilePoint;
	UINT64						m_nFastEFilePoint;
	UINT64						m_nFastCFilePoint;
	UINT32						m_nFastFileReadNum;

public:
	TMapTColumnSetValueList		m_tTColumSetValueListMap;
	TMapTItemSetValueMap		m_tTItemSetValueMapMap;

public:
	UINT32						m_nChkOpTickID;
	UINT32						m_nChkOpTickCntIdx;
	UINT64						m_tChkOpTickCntMap[100];

public:
	CString						m_strSelTName;
	UINT32						m_nSelColNum;
	INT32						m_tSelColIDArray[ASI_DBF_MAX_COLUMN_NUMBER];
	UINT32						m_nSelContinue;
	PUINT64						m_tSelItemArray;
	UINT64						m_nSelItemArraySize;

// private:
// 	CThreadSelect*				m_tThreadSelect;
// 	INT32						StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime = 10/*sec*/);

public:
	TMapID64					m_tFPKeyMap;

public:
	INT32						SetER(INT32 nRtn);

// public:
// 	UINT32						m_nASyncUsedMode;
// 	CThreadSelect*				GetThreadSel(PVOID pOwner);
// 	INT32						StopThread();

public:
	INT32						ClearMember();
	INT32						ClearFastFileBuff();

public:
	CASIDBFMember();
	virtual ~CASIDBFMember();
};

typedef map<UINT64, CASIDBFMember*>		TMapASIDBFMember;
typedef TMapASIDBFMember::iterator		TMapASIDBFMemberItor;

#endif // !defined(AFX_ASIDBFMEMBER_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_)
