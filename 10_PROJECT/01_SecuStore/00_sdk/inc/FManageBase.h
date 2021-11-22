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
 */

#ifndef FILE_MANAGE_BASE_H__

#define FILE_MANAGE_BASE_H__

#include <iostream>
#include <string>
#include <pthread.h>
#include <map>

#include "com_define_manage_base.h"


//---------------------------------------------------------------------------
template <class _T>

class CFManageBase
{
public:
	CFManageBase(void);
	virtual ~CFManageBase(void);

public:
	virtual INT32		Clear();

public :
	virtual INT32		AddItem(UINT32 nID, _T& data);
	virtual INT32		AddItem(_T& data);
	virtual INT32		EditItem(UINT32 nID, _T& data);
	virtual INT32		EditItem(_T& data);
	virtual INT32		DeleteItem(UINT32 nID);
	virtual INT32		ClearItem();
	virtual _T*			FindItem(UINT32 nID);
	virtual _T*			FirstItem();
	virtual _T*			LastItem();
	virtual UINT32		FirstID();
	virtual UINT32		LastID();
	virtual INT32		IsExistID(UINT32 nID, list<_T>& tUnitList);

	virtual UINT32 Count();	
	virtual UINT32 GetItem(INT32 nID, TListID &tListID);
	virtual UINT32 GetItemIDList(TListID& tListID);	
	virtual UINT32 GetItemIDListByGKeyListItor(UINT64 nKey, TListID& tListID, UINT32 nStartID, UINT32 nLastID = 0, UINT32 nOnceNum = 0);

public:
	virtual	UINT32		AddKeyID(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyID(UINT32 nKey);
	virtual	UINT32		FindKeyID(UINT32 nKey);
	virtual UINT32		IsExistKeyID(UINT32 nKey);
	virtual _T*			FindKeyItem(UINT32 nKey);
	virtual	UINT32		CountKeyID();
	virtual	UINT32		GetKeyIDList(TListID& tIDList);
	virtual	UINT32		ClearKeyID();
	virtual	UINT32		ConvertKeyID(TListID& tKeyList, TListID& tIDList);
	virtual	UINT32		LastKey();

public:
	virtual	UINT32		AddSKeyID(CString strKey, UINT32 nID);
	virtual	UINT32		DelSKeyID(CString strKey);
	virtual	UINT32		FindSKeyID(CString strKey);
	virtual _T*			FindSKeyID_Item(CString strKey);
	virtual UINT32		GetSKeyID_KeyList(TListCStr& tKeyList);
	virtual	UINT32		ClearSKeyID();
	virtual	UINT32		ConvertSKeyID(TListCStr& tStrKeyList, TListID& tIDList);

public:
	virtual	VOID		AddWPathStr(CString strPath, CString strHash);
	virtual	VOID		DelWPathStr(CString strPath);
	virtual	UINT32		FindWPathStr(CString strPath, CString &strHash);
	virtual	VOID		ClearWPathStr();
	virtual	UINT32		GetWPathStr_FileHashList(TListFileHashInfo& tFileHashList);

public:
	virtual	UINT32		AddKeyIDList(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyIDList(UINT32 nKey);
	virtual	UINT32		DelKeyIDList(UINT32 nKey, UINT32 nID);
	virtual	UINT32		SetKeyIDList(UINT32 nKey, TListID& tIDList);
	virtual	UINT32		GetKeyIDList(UINT32 nKey, TListID& tIDList);
	virtual	UINT32		ClearKeyIDList();

public:
	virtual	UINT32		AddSKeyIDList(CString strKey, UINT32 nID);
	virtual	UINT32		DelSKeyIDList(CString strKey);
	virtual	UINT32		DelSKeyIDList(CString strKey, UINT32 nID);
	virtual	UINT32		SetSKeyIDList(CString strKey, TListID& tIDList);
	virtual	UINT32		GetSKeyIDList(TListCStr& tKeyList);
	virtual	UINT32		GetSKeyIDList(CString strKey, TListID& tIDList);
	virtual	UINT32		IsExistSKeyIDList(CString strKey);
	virtual	UINT32		IsExistSKeyIDList(CString strKey, UINT32 nID);
	virtual	UINT32		ClearSKeyIDList();


public:
	virtual	UINT32		AddKeyIDMap(UINT32 nKey);
	virtual	UINT32		AddKeyIDMap(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyIDMap(UINT32 nKey);
	virtual	UINT32		DelKeyIDMap(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyIDMapByID(UINT32 nID);
	virtual	UINT32		IsExistKeyIDMap(UINT32 nKey, UINT32 nID);
	virtual	UINT32		GetKeyIDMap(UINT32 nKey, TMapID& tIDMap);
	virtual	UINT32		GetKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nLimited);
	virtual UINT32		GetKeyIDMapLast(UINT32 nKey);
	virtual	UINT32		ClearKeyIDMap();

public:
	virtual	UINT32		AddKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID);		//AddKeySubIDMap(
	virtual	UINT32		DelKeyTypeID(UINT32 nKey);									//DelKeySubIDMap(
	virtual	UINT32		DelKeyTypeID(UINT32 nKey, UINT32 nSubKey);					//GetKeySubIDMap(
	virtual	UINT32		GetKeyTypeID(UINT32 nKey, TMapID& tIDMap);
	virtual	UINT32		GetKeyTypeID_TypeList(UINT32 nKey, TListID& tSKeyList);		//GetKeySubIDMapSKey(
	virtual	UINT32		GetKeyTypeID_KeyList(TListID& tKeyList);					//GetKeySubIDMapKey(
	virtual	UINT32		GetKeyTypeID_IDList(UINT32 nKey, TListID& tIDList);			//GetKeySubIDMapID(
	virtual UINT32		FindKeyTypeID_ID(UINT32 nKey, UINT32 nSubKey);				//FindKeySubIDMapID(
	virtual _T*			FindKeyTypeID_Item(UINT32 nKey, UINT32 nSubKey);			//FindKeySubIDMapItem(
	virtual	UINT32		ClearKeyTypeID();											//ClearKeySubIDMap(


public:
	virtual	UINT32		AddPosSKeyID(CString strKey, UINT32 nID, UINT32 nPos = 0);		//AddPosSKeyID(
	virtual	UINT32		DelPosSKeyID(CString strKey, UINT32 nPos);
	virtual	UINT32		FindPosSKeyID_ID(CString strKey, UINT32 nPos);				//FindPosSKeyID(
	virtual	_T*			FindPosSKeyID_Item(CString strKey, UINT32 nPos);			//FindPosStrKeyItem
	virtual	UINT32		GetPosSKeyID_KeyMap(TMapCStrID& tStrKeyIDMap, UINT32 nPos);	//GetPosStrKeyIDMap(
	virtual	UINT32		GetPosSKeyID_KeyList(TListCStr& tStrKeyList, UINT32 nPos);	//GetPosStrKeyList(
	virtual	UINT32		ClearPosSKeyID(UINT32 nPos = 0);								//ClearPosSKeyID(
	virtual	UINT32		ClearPosSKeyID_ALL();											//ClearPosStrKeyID_ALL(


public:
	virtual	UINT32		AddPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeyIDMap(UINT32 nKey, UINT32 nPos);
	virtual	UINT32		DelPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap(UINT32 nKey, TListID& tIDList, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap(TMapIDMap& tKeyIDMap, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap(TMapID& tIDMap, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap_KeyList(TListID& tKeyList, UINT32 nPos);
	virtual	UINT32		GetPosKeyIDMap_KeyMap(TMapID& tKeyMap, UINT32 nPos);
	virtual UINT32		CountPosKeyIDMap(UINT32 nKey, UINT32 nPos);
	virtual	UINT32		ClearPosKeyIDMap(UINT32 nPos = 0);
	virtual	UINT32		ClearPosKeyIDMap_ALL();

public:
	virtual	UINT32		AddPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos = 0);		//AddPosKeySubIDMap(
	virtual	UINT32		DelPosKeyTypeID(UINT32 nKey, UINT32 nPos);									//DelPosKeySubIDMap(
	virtual	UINT32		DelPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);					//DelPosKeySubIDMap(
	virtual	UINT32		GetPosKeyTypeID(UINT32 nKey, TMapID& tSubKeyMap, UINT32 nPos);				//GetPosKeySubIDMap(
	virtual	UINT32		GetPosKeyTypeID(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos);
	virtual	UINT32		GetPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		GetPosKeyTypeID(TMapIDMap& tKeyIDMap, UINT32 nPos);
	virtual	UINT32		CntPosKeyTypeID(UINT32 nKey, UINT32 nPos);									//CountPosKeySubIDMap(
	virtual UINT32		FindPosKeyTypeID_ID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);				//FindPosKeySubIDMapID(
	virtual _T*			FindPosKeyTypeID_Item(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		ClearPosKeyTypeID(UINT32 nPos = 0);												//ClearPosKeySubIDMap(

public:
	virtual	UINT32		AddPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosSKeyTypeID(CString strKey, UINT32 nPos);
	virtual	UINT32		DelPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		GetPosSKeyTypeID(CString strKey, TMapID& tSubKeyMap, UINT32 nPos);
	virtual	UINT32		GetPosSKeyTypeID(CString strKey, TListID& tSubKeyList, UINT32 nPos);
	virtual	UINT32		GetPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		GetPosSKeyTypeID(TMapCStrMapID& tKeyStrMap, UINT32 nPos);
	virtual	UINT32		ClearPosSKeyTypeID(UINT32 nPos = 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	virtual	UINT32		AddPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nPos);
	virtual	UINT32		DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos);

	virtual	UINT32		GetPosKeyTypeIDMap(UINT32 nKey, TMapIDMap& tSubKeyMap, UINT32 nPos);
	virtual	UINT32		GetPosKeyTypeIDMap(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos);
	virtual	UINT32		GetPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, TMapID& tIDMap, UINT32 nPos);
	virtual	UINT32		GetPosKeyTypeIDMap(TMapIDMapMap& tKeySubKeyIDMap, UINT32 nPos);

	virtual	UINT32		CountPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos);
	virtual	UINT32		ClearPosKeyTypeIDMap(UINT32 nPos);
	virtual	UINT32		ClearPosKeyTypeIDMap();


public:
	virtual	UINT32		AddPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeySTypeIDMap(UINT32 nKey, UINT32 nPos);
	virtual	UINT32		DelPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nPos);
	virtual	UINT32		DelPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nID, UINT32 nPos);

	virtual	UINT32		GetPosKeySTypeIDMap(UINT32 nKey, TMapCStrMapID& tSubKeyMap, UINT32 nPos);
	virtual	UINT32		GetPosKeySTypeIDMap(UINT32 nKey, TListCStr& tSubKeyList, UINT32 nPos);
	virtual	UINT32		GetPosKeySTypeIDMap(UINT32 nKey, CString strType, TMapID& tIDMap, UINT32 nPos);
	virtual	UINT32		GetPosKeySTypeIDMap(stlMapKeyCSTypeIDMap& tKeySTypeIDMap, UINT32 nPos);

	virtual	UINT32		CountPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nPos);
	virtual	UINT32		ClearPosKeySTypeIDMap(UINT32 nPos);
	virtual	UINT32		ClearPosKeySTypeIDMap();


public:
	virtual	UINT32		AddPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosSKeySTypeIDMap(CString strKey, UINT32 nPos);
	virtual	UINT32		DelPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nPos);
	virtual	UINT32		DelPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nID, UINT32 nPos);

	virtual	UINT32		GetPosSKeySTypeIDMap(CString strKey, TMapCStrMapID& tSubKeyMap, UINT32 nPos);
	virtual	UINT32		GetPosSKeySTypeIDMap(CString strKey, TListCStr& tSubKeyList, UINT32 nPos);
	virtual	UINT32		GetPosSKeySTypeIDMap(CString strKey, CString strSubKey, TListID& tIDList, UINT32 nPos);
	virtual	UINT32		GetPosSKeySTypeIDMap(stlMapCSKeyCSTypeIDMap& tKeyStrMap, UINT32 nPos);
	virtual	_T*			FirstPosSKeySTypeIDMap_Item(CString strKey, CString strSubKey, UINT32 nPos);
	virtual	_T*			LastPosSKeySTypeIDMap_Item(CString strKey, CString strSubKey, UINT32 nPos);
	virtual	UINT32		ClearPosSKeySTypeIDMap(UINT32 nPos);
	virtual	UINT32		ClearPosSKeySTypeIDMap();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


public:
	virtual	UINT32		AddKeyList(UINT32 nKey);
	virtual	UINT32		DelKeyList(UINT32 nKey);
	virtual	UINT32		ClearKeyList();
	virtual	UINT32		LastKeyList();
	virtual UINT32		GetKeyListCount();
	virtual	UINT32		GetKeyList(TListID& tKeyList);

public:
	virtual _T&			GetData();
	virtual _T*			GetPData();

public:
	virtual CString		GetHash();

public:
	virtual void		PrintDbgInfo();

protected :
	map <UINT32, _T>		m_tMap;
	TMapID					m_tKeyMap;
	TMapCStrID				m_tSKeyMap;
	TMapCStr				m_tWPathStrMap;

	TMapIDList				m_tKeyListMap;
	TMapCStrList			m_tSKeyListMap;	
	
	TMapIDMap				m_tKeyIDMap;
	TMapIDMap				m_tKeyTypeMap;

	TMapIDMapCStrID			m_tPosSKeyMap;
	TMapIDMapMap			m_tPosKeyTypeMap;
	TMapIDMapCStrMapID		m_tPosSKeyTypeMap;

	TMapIDMapMap			m_tPosKeyIDMap;


	//TMapIDIDStrMapIDItor	m_tPosIDKeyStrSubMapMap;
	
	stlMapPosKeyTypeIDMap		m_tPosKeyTypeIDMap;
	stlMapPosKeyCSTypeIDMap		m_tPosKeySTypeIDMap;
	stlMapPosCSKeyCSTypeIDMap	m_tPosSKeySTypeIDMap;

	TListID				m_tKeyList;
	map<DWORD, _T>		m_tDataMap;

protected:
	list<_T>			m_tTempAddList;
	list<_T>			m_tTempEditList;
	list<UINT32>		m_tTempDelList;
	map <UINT32, _T>	m_tEditMap;

private:
	pthread_mutex_t 	m_mutex;

protected:
	pthread_mutex_t			m_mutex_mgr;

protected:
	typename map <UINT32, _T>::iterator m_pMapItorFind;
	typename map <UINT32, _T>::iterator	m_pMapItorBegin;
	typename map <UINT32, _T>::iterator	m_pMapItorEnd;
	typename map <UINT32, _T>::reverse_iterator	m_pMapRItorBegin;
	typename map <UINT32, _T>::reverse_iterator	m_pMapRItorEnd;

	typename map <DWORD, _T>::iterator	m_pDataMapItorFind;

	typename list <_T>::iterator m_pListItorBegin;
	typename list <_T>::iterator m_pListItorEnd;


	UINT32						m_nInitGlobalIter;

public:
	virtual INT32		InitGlobalIter();
	virtual	INT32		FreeGlobalIter();
	virtual _T*			NextGlobalIter();
	virtual INT32		IsNextGlobalIter();
	virtual INT32		InitGlobalRIter();
	virtual	INT32		FreeGlobalRIter();
	virtual _T*			NextGlobalRIter();
	virtual INT32		IsNextGlobalRIter();
	virtual INT32		IsInitGlobalIter();

protected:
	typename map <UINT32, UINT32>::iterator m_pKeyItorFind;
	typename map <UINT32, UINT32>::iterator	m_pKeyItorBegin;
	typename map <UINT32, UINT32>::iterator	m_pKeyItorEnd;
	typename map <UINT32, UINT32>::reverse_iterator	m_pKeyRItorBegin;
	typename map <UINT32, UINT32>::reverse_iterator	m_pKeyRItorEnd;
	UINT32				m_nInitGlobalKeyIter;

public:
	virtual INT32		InitGlobalKeyIter();
	virtual	INT32		FreeGlobalKeyIter();
	virtual UINT32		NextGlobalKeyIter();
	virtual INT32		IsNextGlobalKeyIter();
	virtual INT32		IsInitGlobalKeyIter();

private:
	TMapMgrBaseGKey		m_tMgrBaseGKeyMap;

public:	
	virtual INT32		InitGlobalKeyListIter(UINT64 nKey);
	virtual	INT32		FreeGlobalKeyListIter(UINT64 nKey);
	virtual UINT32		NextGlobalKeyListIter(UINT64 nKey);
	virtual INT32		IsNextGlobalKeyListIter(UINT64 nKey);
	virtual INT32		IsInitGlobalKeyListIter(UINT64 nKey);
	virtual INT32		IsNextFreeGlobalKeyListIter(UINT64 nKey);

public:
	INT32				SetSyncDBMSType(UINT32 nType);
	INT32				IsSyncDBMS();

public:
	TMapID				m_tTempIDMap;
	INT32				AddTempID(UINT32 nID, UINT32 nValue);
	INT32				DelTempID(UINT32 nID);
	INT32				ClearTempID();
	INT32				GetTempIDList(TListID& tIDList);
	INT32				SetTempIDList(TListID& tIDList);
	INT32				SetTempIDList(TMapID& tIDMap);

public:
	virtual INT32		AddEditData(_T& data);
	virtual INT32		EditEditData(_T& data);
	virtual INT32		DelEditData(UINT32 nID);
	virtual _T*			FindEditData(UINT32 nID);
	virtual INT32		ClearEditData();
	virtual INT32		GetEditDataIDList(TListID& tIDList);

private:
	_T					m_tEditData;

public:
	virtual	INT32		SetEditData(_T& data);
	virtual _T*			GetEditData();


public:
	UINT32		m_nRecvNum;
	UINT32		m_nTempClass;
	CString		m_strFullHash;
	CString		m_strHashValue;	

public:
	UINT32		m_nDBSyncType;
	UINT32		m_nDBSyncChkType;

public:
	INT32		m_nAutoClear;
};


#define	TEMPLATE template <class _T>

TEMPLATE	CFManageBase<_T>::CFManageBase(void)
{
	m_nTempClass			= 0;
	m_nRecvNum				= 0;

	m_nInitGlobalIter		= 0;
	m_nInitGlobalKeyIter	= 0;

	m_nAutoClear			= 1;
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&m_mutex_mgr, NULL);
}

TEMPLATE	CFManageBase<_T>::~CFManageBase(void)
{
	if(m_nAutoClear)
	{
		Clear();
	}
	pthread_mutex_destroy(&m_mutex);
	pthread_mutex_destroy(&m_mutex_mgr);	
}

TEMPLATE INT32		CFManageBase<_T>::Clear()
{
	ClearItem();

	ClearSKeyID();
	ClearSKeyIDList();

	ClearKeyID();
	ClearKeyIDList();
	ClearKeyIDMap();

	ClearKeyList();
	ClearKeyTypeID();

	ClearPosKeyIDMap_ALL();
	ClearPosSKeyID_ALL();

	ClearPosKeyTypeID();
	ClearPosSKeyTypeID();

	ClearPosKeyTypeIDMap();
	ClearPosKeySTypeIDMap();
	ClearPosSKeySTypeIDMap();

	return 0;
}

TEMPLATE INT32		CFManageBase<_T>::AddItem(UINT32 nID, _T& data)
{	
	pthread_mutex_lock (&m_mutex);

	m_tMap[nID] = data;

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE INT32		CFManageBase<_T>::AddItem(_T& data)
{
	UINT32 nID = ((PDB_ID)&data)->nID;	
	return AddItem(nID, data);
}

TEMPLATE INT32		CFManageBase<_T>::EditItem(UINT32 nID, _T& data)
{
	pthread_mutex_lock (&m_mutex);
	m_pMapItorFind = m_tMap.find(nID);
	if (m_pMapItorFind != m_tMap.end())
	{
		m_pMapItorFind->second = data;
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE INT32		CFManageBase<_T>::EditItem(_T& data)
{
	UINT32 nID = ((PDB_ID)&data)->nID;	
	return EditItem(nID, data);
}
	
TEMPLATE INT32		CFManageBase<_T>::DeleteItem(UINT32 nID)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	m_pMapItorFind = m_tMap.find(nID);
	if (m_pMapItorFind != m_tMap.end())
	{
		m_tMap.erase(m_pMapItorFind);
	}
	else
	{
		nRtn = -1;
	}

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE INT32		CFManageBase<_T>::ClearItem()
{
	pthread_mutex_lock (&m_mutex);
	m_tMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE _T*			CFManageBase<_T>::FindItem(UINT32 nID)
{
	_T* pFind = NULL;
	pthread_mutex_lock (&m_mutex);
	m_pMapItorFind = m_tMap.find(nID);
	if (m_pMapItorFind != m_tMap.end())
	{
		pFind = &(m_pMapItorFind->second);
	}
	pthread_mutex_unlock (&m_mutex);
	return pFind;
}

TEMPLATE _T*			CFManageBase<_T>::FirstItem()
{
	_T* pFind = NULL;

	pthread_mutex_lock (&m_mutex);
	m_pMapItorBegin = m_tMap.begin();
	if (m_pMapItorBegin != m_tMap.end())
	{
		pFind = &(m_pMapItorBegin->second);
	}
	pthread_mutex_unlock (&m_mutex);
	return pFind;
}

TEMPLATE _T*			CFManageBase<_T>::LastItem()
{
	_T* pFind = NULL;
	pthread_mutex_lock (&m_mutex);

	m_pMapItorBegin = m_tMap.begin();
	m_pMapItorEnd = m_tMap.end();	
	if (m_pMapItorBegin != m_tMap.end())
	{
		pFind = &((--m_pMapItorEnd)->second);
	}
	pthread_mutex_unlock (&m_mutex);
	return pFind;
}

TEMPLATE UINT32			CFManageBase<_T>::FirstID()
{
	UINT32 nID = 0;
	pthread_mutex_lock (&m_mutex);

	m_pMapItorBegin = m_tMap.begin();
	m_pMapItorEnd = m_tMap.end();	
	if (m_pMapItorBegin != m_pMapItorEnd)
	{
		nID = m_pMapItorBegin->first;
	}
	pthread_mutex_unlock (&m_mutex);
	return nID;
}

TEMPLATE UINT32			CFManageBase<_T>::LastID()
{
	UINT32 nID = 0;
	pthread_mutex_lock (&m_mutex);
	m_pMapRItorBegin = m_tMap.rbegin();
	m_pMapRItorEnd = m_tMap.rend();	
	if (m_pMapRItorBegin != m_pMapRItorEnd)
	{
		nID = m_pMapRItorBegin->first;
	}
	pthread_mutex_unlock (&m_mutex);
	return nID;
}

TEMPLATE INT32	CFManageBase<_T>::IsExistID(UINT32 nID, list<_T>& tUnitList)
{
	m_pListItorBegin = tUnitList.begin();
	m_pListItorEnd = tUnitList.end();
	for(m_pListItorBegin; m_pListItorBegin != m_pListItorEnd; m_pListItorBegin++)
	{
		if(m_pListItorBegin->nID == nID)
			return 1;
	}
	return 0;
}


TEMPLATE UINT32 CFManageBase<_T>::Count()
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	nRtn = (UINT32)m_tMap.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}



TEMPLATE UINT32 CFManageBase<_T>::GetItem(INT32 nID, TListID &tListID)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	m_pMapItorBegin = m_tMap.begin();
	m_pMapItorEnd = m_tMap.end();

	if (nID != -1)
	{
		for (; m_pMapItorBegin != m_pMapItorEnd; m_pMapItorBegin++)
		{
			if (m_pMapItorBegin->second.nID == nID)
			{		
				tListID.push_back(m_pMapItorBegin->second.nID);
			}			
		}
	}
	else
	{
		for (; m_pMapItorBegin != m_pMapItorEnd; m_pMapItorBegin++)
		{
			tListID.push_back(m_pMapItorBegin->second.nID);
		}
	}
	nRtn = (UINT32)tListID.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	UINT32	CFManageBase<_T>::GetItemIDList(TListID& tListID)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	m_pMapItorBegin = m_tMap.begin();
	m_pMapItorEnd = m_tMap.end();
	for (; m_pMapItorBegin != m_pMapItorEnd; m_pMapItorBegin++)
	{			
		tListID.push_back(m_pMapItorBegin->second.nID);
	}
	nRtn = (UINT32)tListID.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	UINT32 CFManageBase<_T>::GetItemIDListByGKeyListItor(UINT64 nKey, TListID& tListID, UINT32 nStartID, UINT32 nLastID, UINT32 nOnceNum)
{
	if(!IsInitGlobalKeyListIter(nKey))
	{
		InitGlobalKeyListIter(nKey);
	}

	while(IsNextGlobalKeyListIter(nKey) && nOnceNum)
	{
		UINT32 nID = NextGlobalKeyListIter(nKey);
		
		if(nID <= nStartID)			continue;		
		if(FindItem(nID) == NULL)	continue;

		tListID.push_back(nID);
		nOnceNum -= 1;

		if(nLastID && nID == nLastID)
		{
			FreeGlobalKeyListIter(nKey);
			break;
		}
	}
	return tListID.size();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddKeyID(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyMap[nKey] = nID;
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyID(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyMap.erase(nKey);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::FindKeyID(UINT32 nKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex);
	TMapIDItor find = m_tKeyMap.find(nKey);
	if(find != m_tKeyMap.end())	nRtn = find->second;
	pthread_mutex_unlock (&m_mutex);

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::IsExistKeyID(UINT32 nKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex);
	TMapIDItor find = m_tKeyMap.find(nKey);
	if(find != m_tKeyMap.end())	nRtn = 1;
	pthread_mutex_unlock (&m_mutex);

	return nRtn;
}

TEMPLATE _T*			CFManageBase<_T>::FindKeyItem(UINT32 nKey)
{
	UINT32 nID = FindKeyID(nKey);
	return FindItem(nID);
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyIDList(TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDItor begin, end;
	begin = m_tKeyMap.begin();	end = m_tKeyMap.end();
	for(begin; begin != end; begin++)
	{
		tIDList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex);

	return tIDList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::CountKeyID()
{
	return m_tKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearKeyID()
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ConvertKeyID(TListID& tKeyList, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TListIDItor begin, end;
	begin = tKeyList.begin();	end = tKeyList.end();
	for(begin; begin != end; begin++)
	{
		TMapIDItor find = m_tKeyMap.find(*begin);
		if(find == m_tKeyMap.end())		continue;

		tIDList.push_back(find->second);
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::LastKey()
{
	UINT32 nKey = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDRItor	begin = m_tKeyMap.rbegin();
	TMapIDRItor	end = m_tKeyMap.rend();	

	if (begin != end)
	{
		nKey = begin->first;
	}
	pthread_mutex_unlock (&m_mutex);
	return nKey;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddSKeyID(CString strKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	m_tSKeyMap[strKey] = nID;
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelSKeyID(CString strKey)
{
	pthread_mutex_lock (&m_mutex);
	m_tSKeyMap.erase(strKey);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::FindSKeyID(CString strKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex);
	TMapCStrIDItor find = m_tSKeyMap.find(strKey);
	if(find != m_tSKeyMap.end())	nRtn = find->second;
	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE _T*			CFManageBase<_T>::FindSKeyID_Item(CString strKey)
{
	UINT32 nID = FindSKeyID(strKey);
	return FindItem(nID);
}

TEMPLATE	UINT32		CFManageBase<_T>::GetSKeyID_KeyList(TListCStr& tKeyList)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapCStrIDItor begin, end;
	begin = m_tSKeyMap.begin();	end = m_tSKeyMap.end();
	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(begin->first);
	}

	nRtn = tKeyList.size();
	pthread_mutex_unlock (&m_mutex);

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearSKeyID()
{
	pthread_mutex_lock (&m_mutex);
	m_tSKeyMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ConvertSKeyID(TListCStr& tStrKeyList, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);
	TListCStrItor begin, end;
	begin = tStrKeyList.begin();	end = tStrKeyList.end();
	for(begin; begin != end; begin++)
	{
		TMapCStrIDItor find = m_tSKeyMap.find(*begin);
		if(find == m_tSKeyMap.end())		continue;

		tIDList.push_back(find->second);
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}


TEMPLATE	VOID		CFManageBase<_T>::AddWPathStr(CString strPath, CString strHash)
{
	pthread_mutex_lock (&m_mutex);
	m_tWPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&m_mutex);
}

TEMPLATE	VOID		CFManageBase<_T>::DelWPathStr(CString strPath)
{
	pthread_mutex_lock (&m_mutex);
	m_tWPathStrMap.erase(strPath);
	pthread_mutex_unlock (&m_mutex);
}

TEMPLATE	UINT32		CFManageBase<_T>::FindWPathStr(CString strPath, CString &strHash)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex);
	TMapCStrItor find = m_tWPathStrMap.find(strPath);
	if(find != m_tWPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	VOID		CFManageBase<_T>::ClearWPathStr()
{
	pthread_mutex_lock (&m_mutex);
	m_tWPathStrMap.clear();
	pthread_mutex_unlock (&m_mutex);
}

TEMPLATE	UINT32		CFManageBase<_T>::GetWPathStr_FileHashList(TListFileHashInfo& tFileHashList)
{
	UINT32 nCount = 0;
	ASI_FILE_HASH stFileHash;
	TMapCStrItor begin, end;
	pthread_mutex_lock (&m_mutex);

	begin = m_tWPathStrMap.begin();	end = m_tWPathStrMap.end();
	for(begin; begin != end; begin++)
	{
		memset(&stFileHash, 0, sizeof(ASI_FILE_HASH));
		strncpy(stFileHash.acFullPath, (char *)(LPCSTR)begin->first, MAX_PATH-1);
		stFileHash.acFullPath[MAX_PATH-1] = 0;
		strncpy(stFileHash.acWhiteHash, (char *)(LPCSTR)begin->second, SHA512_BLOCK_SIZE);
		stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		tFileHashList.push_back(stFileHash);
	}

	nCount = tFileHashList.size();
	pthread_mutex_unlock (&m_mutex);

	return nCount;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddKeyIDList(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find == m_tKeyListMap.end())
	{
		TListID tIDList;
		m_tKeyListMap[nKey] = tIDList;
		find = m_tKeyListMap.find(nKey);
	}

	find->second.push_back(nID);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyIDList(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find != m_tKeyListMap.end())
	{
		find->second.clear();
		m_tKeyListMap.erase(nKey);
	}	
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyIDList(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find == m_tKeyListMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	RemoveListID(find->second, nID);

	if(find->second.size() == 0)
	{
		m_tKeyListMap.erase(nKey);
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::SetKeyIDList(UINT32 nKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find == m_tKeyListMap.end())
	{
		m_tKeyListMap[nKey] = tIDList;
		find = m_tKeyListMap.find(nKey);
	}

	find->second = tIDList;

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyIDList(UINT32 nKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find != m_tKeyListMap.end())
	{
		tIDList = find->second;
	}

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearKeyIDList()
{
	pthread_mutex_lock (&m_mutex);

	m_tKeyListMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddSKeyIDList(CString strKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find == m_tSKeyListMap.end())
	{
		TListID tIDList;
		m_tSKeyListMap[strKey] = tIDList;
		find = m_tSKeyListMap.find(strKey);
	}

	find->second.push_back(nID);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelSKeyIDList(CString strKey)
{
	pthread_mutex_lock (&m_mutex);
	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find != m_tSKeyListMap.end())
	{
		find->second.clear();
		m_tSKeyListMap.erase(strKey);
	}	
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelSKeyIDList(CString strKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);
	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find == m_tSKeyListMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	RemoveListID(find->second, nID);

	if(find->second.size() == 0)
	{
		m_tSKeyListMap.erase(strKey);
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::SetSKeyIDList(CString strKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find == m_tSKeyListMap.end())
	{
		m_tSKeyListMap[strKey] = tIDList;
		find = m_tSKeyListMap.find(strKey);
	}

	find->second = tIDList;

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetSKeyIDList(TListCStr& tKeyList)
{
	pthread_mutex_lock (&m_mutex);

	TMapCStrListItor begin, end;
	begin = m_tSKeyListMap.begin();		end = m_tSKeyListMap.end();
	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(begin->first);
	}

	pthread_mutex_unlock (&m_mutex);

	return tKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetSKeyIDList(CString strKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find != m_tSKeyListMap.end())
	{
		tIDList = find->second;
	}

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::IsExistSKeyIDList(CString strKey)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find != m_tSKeyListMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::IsExistSKeyIDList(CString strKey, UINT32 nID)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapCStrListItor find = m_tSKeyListMap.find(strKey);
	if(find != m_tSKeyListMap.end())
	{
		if(IsExistsIDFromList(find->second, nID))
			nRtn = 1;
	}
	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearSKeyIDList()
{
	pthread_mutex_lock (&m_mutex);

	m_tSKeyListMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddKeyIDMap(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find == m_tKeyIDMap.end())
	{
		TMapID tIDMap;
		m_tKeyIDMap[nKey] = tIDMap;		
	}

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::AddKeyIDMap(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find == m_tKeyIDMap.end())
	{
		TMapID tIDMap;
		m_tKeyIDMap[nKey] = tIDMap;
		find = m_tKeyIDMap.find(nKey);
	}

	find->second[nID] = 0;

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyIDMap(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		find->second.clear();
		m_tKeyIDMap.erase(nKey);
	}

	pthread_mutex_unlock (&m_mutex);

	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelKeyIDMap(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		find->second.erase(nID);
	}

	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyIDMapByID(UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor begin, end;
	begin = m_tKeyIDMap.begin();	end = m_tKeyIDMap.end();
	for(begin; begin != end; begin++)
	{
		begin->second.erase(nID);
	}
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::IsExistKeyIDMap(UINT32 nKey, UINT32 nID)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		TMapIDItor find_key = find->second.find(nID);
		if(find_key != find->second.end())
			nRtn = 1;
	}

	pthread_mutex_unlock (&m_mutex);	
	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyIDMap(UINT32 nKey, TMapID& tIDMap)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		AppandMapToMap(find->second, tIDMap);
		nRtn = tIDMap.size();
	}

	pthread_mutex_unlock (&m_mutex);	
	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nLimited)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		TMapIDItor begin, end;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end && nLimited--; begin++)
		{
			tIDMap[begin->first] = begin->second;
		}
		nRtn = tIDMap.size();
	}

	pthread_mutex_unlock (&m_mutex);	
	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyIDMapLast(UINT32 nKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex);
	TMapIDMapItor find = m_tKeyIDMap.find(nKey);
	if(find != m_tKeyIDMap.end())
	{
		TMapIDRItor rbegin;
		rbegin = find->second.rbegin();
		if(rbegin != find->second.rend())
		{
			nRtn = rbegin->first;
		}
	}
	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearKeyIDMap()
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyIDMap.clear();
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CFManageBase<_T>::AddPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		TMapIDMap tIDMapMap;
		m_tPosKeyIDMap[nPos] = tIDMapMap;

		find = m_tPosKeyIDMap.find(nPos);
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{		
		TMapID tIDMap;
		find->second[nKey] = tIDMap;
		find_key = find->second.find(nKey);
	}

	find_key->second[nID] = 0;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyIDMap(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(nKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(nID);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToMap(find_key->second, tIDMap);
	
	pthread_mutex_unlock (&m_mutex);	

	return tIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap(UINT32 nKey, TListID& tIDList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	ConvertMapToList(find_key->second, tIDList);
	pthread_mutex_unlock (&m_mutex);
	return tIDList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap(TMapIDMap& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap(TMapID& tIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		TMapIDItor begin_sub, end_sub;
		begin_sub = begin->second.begin();	end_sub = begin->second.end();
		for(begin_sub; begin_sub != end_sub; begin_sub++)
		{
			tIDMap[begin_sub->first] = 0;
		}
	}
	pthread_mutex_unlock (&m_mutex);
	return tIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap_KeyList(TListID& tKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex);
	return tKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyIDMap_KeyMap(TMapID& tKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		tKeyMap[begin->first] = begin->second.size();
	}
	pthread_mutex_unlock (&m_mutex);
	return tKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::CountPosKeyIDMap(UINT32 nKey, UINT32 nPos)
{
	UINT32 nCnt = 0;
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	nCnt = find_key->second.size();

	pthread_mutex_unlock (&m_mutex);
	return nCnt;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeyIDMap(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyIDMap.find(nPos);
	if(find == m_tPosKeyIDMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeyIDMap_ALL()
{
	pthread_mutex_lock (&m_mutex);
	m_tPosKeyIDMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddPosSKeyID(CString strKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		TMapCStrID tStrIDMap;
		m_tPosSKeyMap[nPos] = tStrIDMap;

		find = m_tPosSKeyMap.find(nPos);
	}
	
	find->second[strKey] = nID;
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeyID(CString strKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.erase(strKey);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::FindPosSKeyID_ID(CString strKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		TMapCStrIDItor find_key = find->second.find(strKey);
		if(find_key != find->second.end())
		{
			nRtn = find_key->second;
		}	
	}
	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}

TEMPLATE	_T*			CFManageBase<_T>::FindPosSKeyID_Item(CString strKey, UINT32 nPos)
{
	UINT32 nID = FindPosSKeyID_ID(strKey, nPos);
	if(!nID)	return NULL;

	return FindItem(nID);
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyID_KeyMap(TMapCStrID& tKeyIDMap, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		tKeyIDMap = find->second;
	}

	pthread_mutex_unlock (&m_mutex);
	
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyID_KeyList(TListCStr& tStrKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapCStrIDItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		tStrKeyList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex);
	return tStrKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosSKeyID(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		find->second.clear();
	}

	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosSKeyID_ALL()
{
	pthread_mutex_lock (&m_mutex);
	m_tPosSKeyMap.clear();
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find == m_tKeyTypeMap.end())
	{
		TMapID tSubMap;
		m_tKeyTypeMap[nKey] = tSubMap;
		find = m_tKeyTypeMap.find(nKey);
	}

	find->second[nSubKey] = nID;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyTypeID(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		find->second.clear();
		m_tKeyTypeMap.erase(nKey);
	}
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyTypeID(UINT32 nKey, UINT32 nSubKey)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		find->second.erase(nSubKey);
	}
	pthread_mutex_unlock (&m_mutex);		
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyTypeID(UINT32 nKey, TMapID& tIDMap)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		AppandMapToMap(find->second, tIDMap);
	}
	pthread_mutex_unlock (&m_mutex);		
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyTypeID_KeyList(TListID& tKeyList)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor begin, end;
	begin = m_tKeyTypeMap.begin();	end = m_tKeyTypeMap.end();
	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex);		
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyTypeID_TypeList(UINT32 nKey, TListID& tSKeyList)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		TMapIDItor begin, end;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end; begin++)
		{
			tSKeyList.push_back(begin->first);
		}
	}
	pthread_mutex_unlock (&m_mutex);		
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyTypeID_IDList(UINT32 nKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		TMapIDItor begin, end;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end; begin++)
		{
			tIDList.push_back(begin->second);
		}
	}
	pthread_mutex_unlock (&m_mutex);		
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::FindKeyTypeID_ID(UINT32 nKey, UINT32 nSubKey)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find == m_tKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDItor find_sub = find->second.find(nSubKey);
	if(find_sub == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}
	pthread_mutex_unlock (&m_mutex);	

	return find_sub->second;
}

TEMPLATE	_T*			CFManageBase<_T>::FindKeyTypeID_Item(UINT32 nKey, UINT32 nSubKey)
{
	UINT32 nID = FindKeyTypeID_ID(nKey, nSubKey);
	if(!nID)	return NULL;
	return FindItem(nID);
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearKeyTypeID()
{
	m_tKeyTypeMap.clear();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CFManageBase<_T>::AddPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		TMapIDMap tIDMapMap;
		m_tPosKeyTypeMap[nPos] = tIDMapMap;

		find = m_tPosKeyTypeMap.find(nPos);
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{		
		TMapID tIDMap;
		find->second[nKey] = tIDMap;
		find_key = find->second.find(nKey);
	}

	find_key->second[nSubKey] = nID;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyTypeID(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(nKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(nSubKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeID(UINT32 nKey, TMapID& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToMap(find_key->second, tSubKeyMap);

	pthread_mutex_unlock (&m_mutex);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeID(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	ConvertMapToList(find_key->second, tSubKeyList);
	pthread_mutex_unlock (&m_mutex);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeID(TMapIDMap& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::CntPosKeyTypeID(UINT32 nKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_key->second.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	 UINT32		CFManageBase<_T>::FindPosKeyTypeID_ID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDItor find_key_sub = find_key->second.find(nSubKey);
	if(find_key_sub == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_key_sub->second;
	pthread_mutex_unlock (&m_mutex);	
	return nRtn;
}

TEMPLATE	_T*			CFManageBase<_T>::FindPosKeyTypeID_Item(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	return FindItem(FindPosKeyTypeID_ID(nKey, nSubKey, nPos));
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeyTypeID(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDItor find_subkey = find_key->second.find(nSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_subkey->second;

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CFManageBase<_T>::AddPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		TMapCStrMapID tIDMapMap;
		m_tPosSKeyTypeMap[nPos] = tIDMapMap;

		find = m_tPosSKeyTypeMap.find(nPos);
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{		
		TMapID tIDMap;
		find->second[strKey] = tIDMap;
		find_key = find->second.find(strKey);
	}

	find_key->second[nSubKey] = nID;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeyTypeID(CString strKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(strKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(nSubKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyTypeID(CString strKey, TMapID& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToMap(find_key->second, tSubKeyMap);

	pthread_mutex_unlock (&m_mutex);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyTypeID(CString strKey, TListID& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	ConvertMapToList(find_key->second, tSubKeyList);
	pthread_mutex_unlock (&m_mutex);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyTypeID(TMapCStrMapID& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosSKeyTypeID(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeyTypeID(CString strKey, UINT32 nSubKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapIDMapCStrMapIDItor find = m_tPosSKeyTypeMap.find(nPos);
	if(find == m_tPosSKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_key = find->second.find(strKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDItor find_subkey = find_key->second.find(nSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_subkey->second;

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		TMapIDMapMap tIDMapMapMap;
		m_tPosKeyTypeIDMap.tMap[nPos] = tIDMapMapMap;

		find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{		
		TMapIDMap tIDMapMap;
		find->second[nKey] = tIDMapMap;
		find_key = find->second.find(nKey);
	}

	TMapIDMapItor find_sub_key = find_key->second.find(nSubKey);
	if(find_sub_key == find_key->second.end())
	{		
		TMapID tIDMap;
		find_key->second[nSubKey] = tIDMap;
		find_sub_key = find_key->second.find(nSubKey);
	}

	find_sub_key->second[nID] = 0;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(nKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(nSubKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_sub_key = find_key->second.find(nSubKey);
	if(find_sub_key == find_key->second.end())
	{		
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}


	find_sub_key->second.erase(nID);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeIDMap(UINT32 nKey, TMapIDMap& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	tSubKeyMap = find_key->second;

	pthread_mutex_unlock (&m_mutex);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeIDMap(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	{
		TMapIDMapItor begin, end;
		begin = find_key->second.begin();	end = find_key->second.end();
		for(begin; begin != end; begin++)
		{
			tSubKeyList.push_back(begin->first);
		}
	}

	pthread_mutex_unlock (&m_mutex);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, TMapID& tIDMap, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_subkey = find_key->second.find(nSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToMap(find_subkey->second, tIDMap);
	nRtn = tIDMap.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeyTypeIDMap(TMapIDMapMap& tKeySubKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeySubKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeySubKeyIDMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::CountPosKeyTypeIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapIDMapItor find_subkey = find_key->second.find(nSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_subkey->second.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeyTypeIDMap(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyTypeIDMapItor find = m_tPosKeyTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeyTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeyTypeIDMap()
{
	pthread_mutex_lock (&m_mutex);
	
	m_tPosKeyTypeIDMap.tMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CFManageBase<_T>::AddPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		stlMapKeyCSTypeIDMap tKeySTypeIDMap;
		m_tPosKeySTypeIDMap.tMap[nPos] = tKeySTypeIDMap;

		find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{		
		TMapCStrMapID tIDMapMap;
		find->second.tMap[nKey] = tIDMapMap;
		find_key = find->second.tMap.find(nKey);
	}

	TMapCStrMapIDItor find_sub_key = find_key->second.find(strType);
	if(find_sub_key == find_key->second.end())
	{		
		TMapID tIDMap;
		find_key->second[strType] = tIDMap;
		find_sub_key = find_key->second.find(strType);
	}

	find_sub_key->second[nID] = 0;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeySTypeIDMap(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.tMap.erase(nKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(strType);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_sub_key = find_key->second.find(strType);
	if(find_sub_key == find_key->second.end())
	{		
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}


	find_sub_key->second.erase(nID);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeySTypeIDMap(UINT32 nKey, TMapCStrMapID& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	tSubKeyMap = find_key->second;

	pthread_mutex_unlock (&m_mutex);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeySTypeIDMap(UINT32 nKey, TListCStr& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	{
		TMapCStrMapIDItor begin, end;
		begin = find_key->second.begin();	end = find_key->second.end();
		for(begin; begin != end; begin++)
		{
			tSubKeyList.push_back(begin->first);
		}
	}

	pthread_mutex_unlock (&m_mutex);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeySTypeIDMap(UINT32 nKey, CString strType, TMapID& tIDMap, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_subkey = find_key->second.find(strType);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToMap(find_subkey->second, tIDMap);
	nRtn = tIDMap.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosKeySTypeIDMap(stlMapKeyCSTypeIDMap& tKeySTypeIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeySTypeIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeySTypeIDMap.tMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::CountPosKeySTypeIDMap(UINT32 nKey, CString strType, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapKeyCSTypeIDMapItor find_key = find->second.tMap.find(nKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_subkey = find_key->second.find(strType);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	nRtn = find_subkey->second.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeySTypeIDMap(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosKeyCSTypeIDMapItor find = m_tPosKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.tMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosKeySTypeIDMap()
{
	pthread_mutex_lock (&m_mutex);
	
	m_tPosKeySTypeIDMap.tMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CFManageBase<_T>::AddPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		stlMapCSKeyCSTypeIDMap tIDMapMap;
		m_tPosSKeySTypeIDMap.tMap[nPos] = tIDMapMap;

		find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{		
		TMapCStrMapID tStrMapIDMap;
		find->second.tMap[strKey] = tStrMapIDMap;
		find_key = find->second.tMap.find(strKey);
	}

	TMapCStrMapIDItor find_sub_key = find_key->second.find(strSubKey);
	if(find_sub_key == find_key->second.end())
	{		
		TMapID tIDMap;
		find_key->second[strSubKey] = tIDMap;
		find_sub_key = find_key->second.find(strSubKey);
	}

	find_sub_key->second[nID] = 0;
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeySTypeIDMap(CString strKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);

	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.clear();
	find->second.tMap.erase(strKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	find_key->second.erase(strSubKey);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}


TEMPLATE	UINT32		CFManageBase<_T>::DelPosSKeySTypeIDMap(CString strKey, CString strSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_sub_key = find_key->second.find(strSubKey);
	if(find_sub_key == find_key->second.end())
	{		
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}


	find_sub_key->second.erase(nID);
	pthread_mutex_unlock (&m_mutex);	
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeySTypeIDMap(CString strKey, TMapCStrMapID& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	tSubKeyMap = find_key->second;

	pthread_mutex_unlock (&m_mutex);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeySTypeIDMap(CString strKey, TListCStr& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	{
		TMapCStrMapIDItor begin, end;
		begin = find_key->second.begin();	end = find_key->second.end();
		for(begin; begin != end; begin++)
		{
			tSubKeyList.push_back(begin->first);
		}
	}
	
	pthread_mutex_unlock (&m_mutex);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeySTypeIDMap(CString strKey, CString strSubKey, TListID& tIDList, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_subkey = find_key->second.find(strSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	AppandMapToList(find_subkey->second, tIDList);
	nRtn = tIDList.size();

	pthread_mutex_unlock (&m_mutex);	

	return nRtn;
}

TEMPLATE	UINT32		CFManageBase<_T>::GetPosSKeySTypeIDMap(stlMapCSKeyCSTypeIDMap& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex);
	return tKeyIDMap.tMap.size();
}

TEMPLATE	_T*		CFManageBase<_T>::FirstPosSKeySTypeIDMap_Item(CString strKey, CString strSubKey, UINT32 nPos)
{
	_T* pFind = NULL;
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_subkey = find_key->second.find(strSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	{
		TMapIDItor find_item = find_subkey->second.begin();
		if(find_item != find_subkey->second.end())
		{
			pFind = FindItem(find_item->first);
		}
	}

	pthread_mutex_unlock (&m_mutex);	

	return pFind;
}

TEMPLATE	_T*		CFManageBase<_T>::LastPosSKeySTypeIDMap_Item(CString strKey, CString strSubKey, UINT32 nPos)
{
	_T* pFind = NULL;
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCSKeyCSTypeIDMapItor find_key = find->second.tMap.find(strKey);
	if(find_key == find->second.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	TMapCStrMapIDItor find_subkey = find_key->second.find(strSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex);	
		return 0;
	}

	{
		TMapIDRItor find_item = find_subkey->second.rbegin();
		if(find_item != find_subkey->second.rend())
		{
			pFind = FindItem(find_item->first);
		}
	}

	pthread_mutex_unlock (&m_mutex);	

	return pFind;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosSKeySTypeIDMap()
{
	pthread_mutex_lock (&m_mutex);

	m_tPosSKeySTypeIDMap.tMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::ClearPosSKeySTypeIDMap(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex);
	TMapPosCSKeyCSTypeIDMapItor find = m_tPosSKeySTypeIDMap.tMap.find(nPos);
	if(find == m_tPosSKeySTypeIDMap.tMap.end())
	{
		pthread_mutex_unlock (&m_mutex);
		return 0;
	}

	find->second.tMap.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TEMPLATE	UINT32		CFManageBase<_T>::AddKeyList(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyList.push_back(nKey);
	pthread_mutex_unlock (&m_mutex);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	UINT32		CFManageBase<_T>::DelKeyList(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex);
	TListIDItor begin, end;
	begin = m_tKeyList.begin();	end = m_tKeyList.end();
	for(begin; begin != end; begin++)
	{
		if(*begin == nKey)
		{
			m_tKeyList.erase(begin);
			break;
		}
	}
	pthread_mutex_unlock (&m_mutex);
	return 0;
}

//-----------------------------------------------------------------------------

TEMPLATE	UINT32		CFManageBase<_T>::ClearKeyList()
{
	pthread_mutex_lock (&m_mutex);
	m_tKeyList.clear();
	pthread_mutex_unlock (&m_mutex);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	UINT32		CFManageBase<_T>::LastKeyList()
{
	UINT32 nKey = 0;
	pthread_mutex_lock (&m_mutex);

	TListIDRItor	begin = m_tKeyList.rbegin();
	TListIDRItor	end = m_tKeyList.rend();	

	if (begin != end)
	{
		nKey = *begin;
	}
	pthread_mutex_unlock (&m_mutex);
	return nKey;
}
//-----------------------------------------------------------------------------

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyListCount()
{
	UINT32 nCnt = 0;

	pthread_mutex_lock (&m_mutex);
	nCnt = m_tKeyList.size();
	pthread_mutex_unlock (&m_mutex);

	return nCnt;
}
//-----------------------------------------------------------------------------

TEMPLATE	UINT32		CFManageBase<_T>::GetKeyList(TListID& tKeyList)
{
	pthread_mutex_lock (&m_mutex);

	TListIDItor	begin = m_tKeyList.begin();
	TListIDItor	end = m_tKeyList.end();	

	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(*begin);
	}
	pthread_mutex_unlock (&m_mutex);
	return tKeyList.size();
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CFManageBase<_T>::InitGlobalIter()
{
	m_pMapItorBegin = m_tMap.begin();
	m_pMapItorEnd = m_tMap.end();
	m_nInitGlobalIter = 1;
	return 0;
}

TEMPLATE	INT32		CFManageBase<_T>::FreeGlobalIter()
{
	m_nInitGlobalIter = 0;
	return 0;
}

TEMPLATE	_T*			CFManageBase<_T>::NextGlobalIter()
{
	if(!m_nInitGlobalIter)	return NULL;

	if(m_pMapItorBegin == m_pMapItorEnd)
	{
		FreeGlobalIter();
		return NULL;
	}
	_T* pRtn = &(m_pMapItorBegin->second);
	m_pMapItorBegin++;
	return pRtn;
}

TEMPLATE	INT32		CFManageBase<_T>::IsNextGlobalIter()
{
	if(!m_nInitGlobalIter)	return 0;

	if(m_pMapItorBegin == m_pMapItorEnd)
	{
		FreeGlobalIter();
		return 0;
	}

	return 1;
}

TEMPLATE	INT32		CFManageBase<_T>::InitGlobalRIter()
{
	m_pMapRItorBegin = m_tMap.rbegin();
	m_pMapRItorEnd = m_tMap.rend();
	m_nInitGlobalIter = 1;
	return 0;
}

TEMPLATE	INT32		CFManageBase<_T>::FreeGlobalRIter()
{
	m_nInitGlobalIter = 0;
	return 0;
}

TEMPLATE	_T*			CFManageBase<_T>::NextGlobalRIter()
{
	if(!m_nInitGlobalIter)	return NULL;

	if(m_pMapRItorBegin == m_pMapRItorEnd)
	{
		FreeGlobalRIter();
		return NULL;
	}
	_T* pRtn = &(m_pMapRItorBegin->second);
	m_pMapRItorBegin++;
	return pRtn;
}

TEMPLATE	INT32		CFManageBase<_T>::IsNextGlobalRIter()
{
	if(!m_nInitGlobalIter)	return 0;

	if(m_pMapRItorBegin == m_pMapRItorEnd)
	{
		FreeGlobalRIter();
		return 0;
	}

	return 1;
}

TEMPLATE	INT32		CFManageBase<_T>::IsInitGlobalIter()
{
	return m_nInitGlobalIter;
}

///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CFManageBase<_T>::InitGlobalKeyIter()
{
	m_pKeyItorBegin = m_tKeyMap.begin();
	m_pKeyItorEnd = m_tKeyMap.end();
	m_nInitGlobalKeyIter = 1;
	return 0;
}

TEMPLATE	INT32		CFManageBase<_T>::FreeGlobalKeyIter()
{
	m_nInitGlobalKeyIter = 0;
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::NextGlobalKeyIter()
{
	if(!m_nInitGlobalKeyIter)	return NULL;

	if(m_pKeyItorBegin == m_pKeyItorEnd)
	{
		FreeGlobalKeyIter();
		return NULL;
	}
	UINT32 nRtn = (m_pKeyItorBegin->first);
	m_pKeyItorBegin++;
	return nRtn;
}

TEMPLATE	INT32		CFManageBase<_T>::IsNextGlobalKeyIter()
{
	if(!m_nInitGlobalKeyIter)	return 0;

	if(m_pKeyItorBegin == m_pKeyItorEnd)
	{
		FreeGlobalKeyIter();
		return 0;
	}

	return 1;
}

TEMPLATE	INT32		CFManageBase<_T>::IsInitGlobalKeyIter()
{
	return m_nInitGlobalKeyIter;
}

///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CFManageBase<_T>::InitGlobalKeyListIter(UINT64 nKey)
{
	MGR_BASE_GKEY tMBG;
	tMBG.tItorBegin	= m_tKeyList.begin();
	tMBG.tItorEnd	= m_tKeyList.end();
	tMBG.nInitMode	= 1;
	tMBG.nKey		= nKey;

	m_tMgrBaseGKeyMap[nKey] = tMBG;
	return 0;
}

TEMPLATE	INT32		CFManageBase<_T>::FreeGlobalKeyListIter(UINT64 nKey)
{
	m_tMgrBaseGKeyMap.erase(nKey);
	return 0;
}

TEMPLATE	UINT32		CFManageBase<_T>::NextGlobalKeyListIter(UINT64 nKey)
{
	TMapMgrBaseGKeyItor find = m_tMgrBaseGKeyMap.find(nKey);
	if(find == m_tMgrBaseGKeyMap.end())		return 0;

	if(!find->second.nInitMode)		return 0;

	if(find->second.tItorBegin == find->second.tItorEnd)
	{
		FreeGlobalKeyListIter(nKey);
		return 0;
	}
	UINT32 nRtn = *(find->second.tItorBegin);
	find->second.tItorBegin++;
	return nRtn;
}

TEMPLATE	INT32		CFManageBase<_T>::IsNextGlobalKeyListIter(UINT64 nKey)
{
	TMapMgrBaseGKeyItor find = m_tMgrBaseGKeyMap.find(nKey);
	if(find == m_tMgrBaseGKeyMap.end())		return 0;

	if(!find->second.nInitMode)		return 0;

	if(find->second.tItorBegin == find->second.tItorEnd)
	{
		FreeGlobalKeyListIter(nKey);
		return 0;
	}

	return 1;
}

TEMPLATE	INT32		CFManageBase<_T>::IsInitGlobalKeyListIter(UINT64 nKey)
{
	TMapMgrBaseGKeyItor find = m_tMgrBaseGKeyMap.find(nKey);
	if(find == m_tMgrBaseGKeyMap.end())		return 0;

	return find->second.nInitMode;
}

TEMPLATE	INT32		CFManageBase<_T>::IsNextFreeGlobalKeyListIter(UINT64 nKey)
{
	INT32 nRtn = IsNextGlobalKeyListIter(nKey);
	if(!nRtn)	FreeGlobalKeyListIter(nKey);

	return nRtn;
}

///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CFManageBase<_T>::SetSyncDBMSType(UINT32 nType)
{
	m_nDBSyncChkType = nType;
	return m_nDBSyncChkType;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32		CFManageBase<_T>::IsSyncDBMS()
{
	return (m_nDBSyncChkType & m_nDBSyncType);
}
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32				CFManageBase<_T>::AddTempID(UINT32 nID, UINT32 nValue)
{
	pthread_mutex_lock (&m_mutex);

	m_tTempIDMap[nID] = nValue;
	pthread_mutex_unlock (&m_mutex);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32				CFManageBase<_T>::DelTempID(UINT32 nID)
{
	pthread_mutex_lock (&m_mutex);

	m_tTempIDMap.erase(nID);

	pthread_mutex_unlock (&m_mutex);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32				CFManageBase<_T>::ClearTempID()
{
	pthread_mutex_lock (&m_mutex);

	m_tTempIDMap.clear();

	pthread_mutex_unlock (&m_mutex);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32				CFManageBase<_T>::GetTempIDList(TListID& tIDList)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);

	map <unsigned int, unsigned int>::iterator begin, end;
	begin = m_tTempIDMap.begin();	end = m_tTempIDMap.end();

	for (; begin != end; begin++)
	{			
		tIDList.push_back(begin->first);
	}
	nRtn = (UINT32)tIDList.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32				CFManageBase<_T>::SetTempIDList(TListID& tIDList)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	m_tTempIDMap.clear();

	list <unsigned int>::iterator begin, end;
	begin = tIDList.begin();	end = tIDList.end();

	for (; begin != end; begin++)
	{			
		m_tTempIDMap[*begin] = 0;
	}
	nRtn = (UINT32)m_tTempIDMap.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}
//-----------------------------------------------------------------------------

TEMPLATE	INT32				CFManageBase<_T>::SetTempIDList(TMapID& tIDMap)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex);
	m_tTempIDMap.clear();

	map <unsigned int, unsigned int>::iterator begin, end;
	begin = tIDMap.begin();	end = tIDMap.end();

	for (; begin != end; begin++)
	{			
		m_tTempIDMap[begin->first] = 0;
	}
	nRtn = (UINT32)m_tTempIDMap.size();

	pthread_mutex_unlock (&m_mutex);
	return nRtn;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	INT32		CFManageBase<_T>::AddEditData(_T& data)
{
	m_tEditMap[data.nID] = data;
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	 INT32		CFManageBase<_T>::EditEditData(_T& data)
{
	m_tEditMap[data.nID] = data;
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	 INT32		CFManageBase<_T>::DelEditData(UINT32 nID)
{
	m_tEditMap.erase(nID);
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	 _T*		CFManageBase<_T>::FindEditData(UINT32 nID)
{
	_T* pFind = NULL;

	m_pMapItorFind = m_tEditMap.find(nID);
	if (m_pMapItorFind != m_tEditMap.end())
	{
		pFind = &(m_pMapItorFind->second);
	}
	return pFind;
}
//-----------------------------------------------------------------------------

TEMPLATE	 INT32		CFManageBase<_T>::ClearEditData()
{
	m_tEditMap.clear();
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	 INT32		CFManageBase<_T>::GetEditDataIDList(TListID& tIDList)
{
	m_pMapItorBegin = m_tEditMap.begin();	m_pMapItorEnd = m_tEditMap.end();
	for(m_pMapItorBegin; m_pMapItorBegin != m_pMapItorEnd; m_pMapItorBegin++)
	{
		tIDList.push_back(m_pMapItorBegin->first);
	}
	return tIDList.size();
}
//-----------------------------------------------------------------------------

TEMPLATE	 INT32		CFManageBase<_T>::SetEditData(_T& data)
{
	m_tEditData = data;
	return 0;
}
//-----------------------------------------------------------------------------

TEMPLATE	 _T*		CFManageBase<_T>::GetEditData()
{
	return &m_tEditData;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	void		CFManageBase<_T>::PrintDbgInfo()
{
	return;
}
//-----------------------------------------------------------------------------

TEMPLATE CString			CFManageBase<_T>::GetHash()
{
	return m_strFullHash;
}
//-----------------------------------------------------------------------------

TEMPLATE _T&			CFManageBase<_T>::GetData()
{
	_T* pData = NULL;

	pthread_mutex_lock (&m_mutex);
	DWORD dwThreadID = gettid();

	m_pDataMapItorFind = m_tDataMap.find(dwThreadID);
	if(m_pDataMapItorFind == m_tDataMap.end())
	{
		_T tTemplate;
		m_tDataMap[dwThreadID] = tTemplate;
		m_pDataMapItorFind = m_tDataMap.find(dwThreadID);
	}

	pData = &(m_pDataMapItorFind->second);
	pthread_mutex_unlock (&m_mutex);

	return (*pData);
}
//-----------------------------------------------------------------------------

TEMPLATE _T*			CFManageBase<_T>::GetPData()
{
	_T* pData = NULL;

	pthread_mutex_lock (&m_mutex);
	DWORD dwThreadID = gettid();
	m_pDataMapItorFind = m_tDataMap.find(dwThreadID);
	if(m_pDataMapItorFind == m_tDataMap.end())
	{
		_T tTemplate;
		m_tDataMap[dwThreadID] = tTemplate;
		m_pDataMapItorFind = m_tDataMap.find(dwThreadID);
	}

	pData = &(m_pDataMapItorFind->second);
	pthread_mutex_unlock (&m_mutex);

	return pData;
}
//-----------------------------------------------------------------------------

#endif /*FILE_MANAGE_BASE_H__*/
