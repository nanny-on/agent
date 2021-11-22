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
 * 
 */

#ifndef MANAGE_ED_BASE_H__

#define MANAGE_ED_BASE_H__

template <class _T>
class CManageEdBase
{
public:
	CManageEdBase(void);
	virtual ~CManageEdBase(void);

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

	virtual UINT32 Count();	
	virtual UINT32 GetItem(INT32 nID, TListID &tListID);
	virtual UINT32 GetItemIDList(TListID& tListID);	

public:
	virtual INT32		SetEditDataMap(UINT32 nID);
	virtual INT32		AddEditDataMap(_T& data);
	virtual INT32		EditEditDataMap(_T& data);
	virtual INT32		DelEditDataMap(UINT32 nEditID);
	virtual _T*			FindEditData(UINT32 nEditID);
	virtual INT32		ClearEditDataMap();
	virtual INT32		GetEditDataIDList(TListID& tEIDList);

public:
	virtual void		SetEditData(UINT32 nID);
	virtual void		SetEditData(_T& data);
	virtual _T*			GetEditData();


public:
	virtual	UINT32		AddKeyID(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyID(UINT32 nKey);
	virtual	UINT32		FindKeyID(UINT32 nKey);
	virtual _T*			FindKeyItem(UINT32 nKey);
	virtual	UINT32		GetKeyIDList(TListID& tIDList);
	virtual	UINT32		ClearKeyID();
	virtual	UINT32		ConvertKeyID(TListID& tKeyList, TListID& tIDList);

public:
	virtual	UINT32		AddStrKeyID(String strKey, UINT32 nID);
	virtual	UINT32		DelStrKeyID(String strKey);
	virtual	UINT32		FindStrKeyID(String strKey);
	virtual _T*			FindStrKeyItem(String strKey);
	virtual UINT32		GetStrKeyList(TListStr& tKeyList);
	virtual	UINT32		ClearStrKeyID();
	virtual	UINT32		ConvertStrKeyID(TListStr& tStrKeyList, TListID& tIDList);

public:
	virtual	UINT32		AddKeyIDList(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyIDList(UINT32 nKey);
	virtual	UINT32		DelKeyIDList(UINT32 nKey, UINT32 nID);
	virtual	UINT32		GetKeyIDList(UINT32 nKey, TListID& tIDList);
	virtual	UINT32		ClearKeyIDList();

public:
	virtual	UINT32		AddKeyIDMap(UINT32 nKey, UINT32 nID);
	virtual	UINT32		DelKeyIDMap(UINT32 nKey);
	virtual	UINT32		DelKeyIDMap(UINT32 nKey, UINT32 nID);
	virtual	UINT32		GetKeyIDMap(UINT32 nKey, TMapID& tIDMap);
	virtual	UINT32		ClearKeyIDMap();

public:
	virtual	UINT32		AddPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeyIDMap(UINT32 nKey, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeyIDMap(UINT32 nKey, TListID& tIDList, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeyIDMap(TMapIDMap& tKeyIDMap, UINT32 nPos = 0);
	virtual	UINT32		ClearPosKeyIDMap(UINT32 nPos = 0);

public:
	virtual	UINT32		AddPosStrKeyID(String strKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosStrKeyID(String strKey, UINT32 nPos = 0);
	virtual	UINT32		FindPosStrKeyID(String strKey, UINT32 nPos = 0);
	virtual	_T*			FindPosStrKeyItem(String strKey, UINT32 nPos = 0);
	virtual	UINT32		GetPosStrKeyIDMap(TMapStrID& tStrKeyIDMap, UINT32 nPos = 0);
	virtual	UINT32		GetPosStrKeyList(TListStr& tStrKeyList, UINT32 nPos = 0);	
	virtual	UINT32		ClearPosStrKeyID(UINT32 nPos = 0);

public:
	virtual	UINT32		AddKeySubIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID);
	virtual	UINT32		DelKeySubIDMap(UINT32 nKey);
	virtual	UINT32		DelKeySubIDMap(UINT32 nKey, UINT32 nSubKey);
	virtual	UINT32		GetKeySubIDMap(UINT32 nKey, TMapID& tIDMap);
	virtual _T*			FindKeySubItem(UINT32 nKey, UINT32 nSubKey);
	virtual	UINT32		ClearKeySubIDMap();

public:
	virtual	UINT32		AddPosKeySubIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeySubIDMap(UINT32 nKey, UINT32 nPos = 0);
	virtual	UINT32		DelPosKeySubIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeySubIDMap(UINT32 nKey, TMapID& tSubKeyMap, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeySubIDMap(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeySubIDMap(UINT32 nKey, UINT32 nSubKey, UINT32 nPos = 0);
	virtual	UINT32		GetPosKeySubIDMap(TMapIDMap& tKeyIDMap, UINT32 nPos = 0);
	virtual	UINT32		ClearPosKeySubIDMap(UINT32 nPos = 0);

public:
	virtual void		PrintDbgInfo();

protected :
	map <UINT32, _T>	m_tMap;
	TMapID				m_tKeyMap;
	TMapStrID			m_tStrKeyMap;
	TMapIDList			m_tKeyListMap;
	TMapIDMap			m_tKeyMapMap;
	TMapIDMap			m_tKeySubIDMap;
	TMapIDMapMap		m_tPosKeyMapMap;
	TMapIDMapMap		m_tPosKeySubMapMap;
	TMapIDMapStrID		m_tPosStrKeyMap;

protected :
	_T					m_tEditData;
	TMapID				m_tEditPkgIDMap;
	map <UINT32, _T>	m_tEditDataMap;
	UINT32				m_nEditID;

	typename map <UINT32, _T>::iterator m_pMapItorFind;
	typename map <UINT32, _T>::iterator	m_pMapItorBegin;
	typename map <UINT32, _T>::iterator	m_pMapItorEnd;
	typename map <UINT32, _T>::reverse_iterator	m_pMapRItorBegin;
	typename map <UINT32, _T>::reverse_iterator	m_pMapRItorEnd;

private:
	pthread_mutex_t 	m_mutex_single;

protected:

	typename map <UINT32, _T>::iterator	g_begin;
	typename map <UINT32, _T>::iterator	g_end;
	UINT32						m_nInitGlobalIter;

public:
	virtual INT32		InitGlobalIter();
	virtual	INT32		FreeGlobalIter();
	virtual _T*			NextGlobalIter();
	virtual INT32		IsNextGlobalIter();
	virtual INT32		IsInitGlobalIter();

public:
	UINT32		m_nRecvNum;
	UINT32		m_nTempClass;
};


#define	TEMPLATE template <class _T>

TEMPLATE	CManageEdBase<_T>::CManageEdBase(void)
{
	m_nTempClass		= 0;
	m_nRecvNum			= 0;
	m_nInitGlobalIter	= 0;
	m_nEditID	= 0;
    pthread_mutex_init(&m_mutex_single, NULL);
}

TEMPLATE	CManageEdBase<_T>::~CManageEdBase(void)
{
	pthread_mutex_destroy(&m_mutex_single);
}

TEMPLATE INT32		CManageEdBase<_T>::AddItem(UINT32 nID, _T& data)
{	
	pthread_mutex_lock (&m_mutex_single);

	m_tMap[nID] = data;

	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE INT32		CManageEdBase<_T>::AddItem(_T& data)
{
	UINT32 nID = ((PDB_ID)&data)->nID;	
	return AddItem(nID, data);
}

TEMPLATE INT32		CManageEdBase<_T>::EditItem(UINT32 nID, _T& data)
{
	pthread_mutex_lock (&m_mutex_single);

	m_pMapItorFind	find = m_tMap.find(nID);
	if (find != m_tMap.end())
	{
		find->second = data;
	}

	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE INT32		CManageEdBase<_T>::EditItem(_T& data)
{
	UINT32 nID = ((PDB_ID)&data)->nID;	
	return EditItem(nID, data);
}
	
TEMPLATE INT32		CManageEdBase<_T>::DeleteItem(UINT32 nID)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	m_pMapItorFind	find = m_tMap.find(nID);
	if (find != m_tMap.end())
	{
		m_tMap.erase(find);
	}
	else
	{
		nRtn = -1;
	}

	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}

TEMPLATE INT32		CManageEdBase<_T>::ClearItem()
{
	pthread_mutex_lock (&m_mutex_single);
	m_tMap.clear();
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE _T*			CManageEdBase<_T>::FindItem(UINT32 nID)
{
	_T* pFind = NULL;

	pthread_mutex_lock (&m_mutex_single);
	m_pMapItorFind	find = m_tMap.find(nID);
	if (find != m_tMap.end())
	{
		pFind = &(find->second);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return pFind;
}

TEMPLATE _T*			CManageEdBase<_T>::FirstItem()
{
	_T* pFind = NULL;

	pthread_mutex_lock (&m_mutex_single);
	m_pMapItorBegin	begin = m_tMap.begin();
	if (begin != m_tMap.end())
	{
		pFind = &(begin->second);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return pFind;
}

TEMPLATE _T*			CManageEdBase<_T>::LastItem()
{
	_T* pFind = NULL;
	pthread_mutex_lock (&m_mutex_single);

	m_pMapItorBegin	begin = m_tMap.begin();
	m_pMapItorEnd end = m_tMap.end();	
	
	if (begin != m_tMap.end())
	{
		pFind = &((--end)->second);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return pFind;
}

TEMPLATE UINT32 CManageEdBase<_T>::Count()
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	nRtn = (UINT32)m_tMap.size();

	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}

TEMPLATE UINT32 CManageEdBase<_T>::GetItem(INT32 nID, TListID &tListID)
{
	UINT32 nRtn = 0;
	m_pMapItorBegin begin;
    m_pMapItorEnd end;

	pthread_mutex_lock (&m_mutex_single);

	begin = m_tMap.begin();	end = m_tMap.end();

	if (nID != -1)
	{
		for (; begin != end; begin++)
		{
			if (begin->second.nID == nID)
			{		
				tListID.push_back(begin->second.nID);
			}			
		}
	}
	else
	{
		for (; begin != end; begin++)
		{
			tListID.push_back(begin->second.nID);
		}
	}
	nRtn = (UINT32)tListID.size();

	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}

TEMPLATE	UINT32	CManageEdBase<_T>::GetItemIDList(TListID& tListID)
{
	UINT32 nRtn = 0;
	m_pMapItorBegin begin;
    m_pMapItorEnd end;

	pthread_mutex_lock (&m_mutex_single);

	begin = m_tMap.begin();	end = m_tMap.end();
	
	for (; begin != end; begin++)
	{			
		tListID.push_back(begin->second.nID);
	}
	nRtn = (UINT32)tListID.size();

	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddKeyID(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);
	m_tKeyMap[nKey] = nID;
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyID(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex_single);
	m_tKeyMap.erase(nKey);
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::FindKeyID(UINT32 nKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex_single);
	TMapIDItor find = m_tKeyMap.find(nKey);
	if(find != m_tKeyMap.end())	nRtn = find->second;
	pthread_mutex_unlock (&m_mutex_single);

	return nRtn;
}

TEMPLATE _T*			CManageEdBase<_T>::FindKeyItem(UINT32 nKey)
{
	UINT32 nID = FindKeyID(nKey);
	return FindItem(nID);
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetKeyIDList(TListID& tIDList)
{
	TMapIDItor begin, end;
    pthread_mutex_lock (&m_mutex_single);
	begin = m_tKeyMap.begin();	end = m_tKeyMap.end();
	for(begin; begin != end; begin++)
	{
		tIDList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return tIDList.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearKeyID()
{
	pthread_mutex_lock (&m_mutex_single);
	m_tKeyMap.clear();
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::ConvertKeyID(TListID& tKeyList, TListID& tIDList)
{
	TListIDItor begin, end;

	pthread_mutex_lock (&m_mutex_single);
	begin = tKeyList.begin();	end = tKeyList.end();
	for(begin; begin != end; begin++)
	{
		TMapIDItor find = m_tKeyMap.find(*begin);
		if(find == m_tKeyMap.end())		continue;

		tIDList.push_back(find->second);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddStrKeyID(String strKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);
	m_tSKeyMap[strKey] = nID;
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelStrKeyID(String strKey)
{
	pthread_mutex_lock (&m_mutex_single);
	m_tSKeyMap.erase(strKey);
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::FindStrKeyID(String strKey)
{
	UINT32 nRtn = 0;

	pthread_mutex_lock (&m_mutex_single);
	TMapStrIDItor find = m_tSKeyMap.find(strKey);
	if(find != m_tSKeyMap.end())	nRtn = find->second;
	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}

TEMPLATE _T*			CManageEdBase<_T>::FindStrKeyItem(String strKey)
{
	UINT32 nID = FindStrKeyID(strKey);
	return FindItem(nID);
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetStrKeyList(TListStr& tKeyList)
{
	TMapStrIDItor begin, end;
    UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	begin = m_tSKeyMap.begin();	end = m_tSKeyMap.end();
	for(begin; begin != end; begin++)
	{
		tKeyList.push_back(begin->first);
	}
	nRtn = tKeyList.size();

	pthread_mutex_unlock (&m_mutex_single);

	return nRtn;
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearSKeyID()
{
	pthread_mutex_lock (&m_mutex_single);
	m_tSKeyMap.clear();
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::ConvertSKeyID(TListStr& tStrKeyList, TListID& tIDList)
{
	TListStrItor begin, end;
    pthread_mutex_lock (&m_mutex_single);
	begin = tStrKeyList.begin();	end = tStrKeyList.end();
	for(begin; begin != end; begin++)
	{
		TMapStrIDItor find = m_tSKeyMap.find(*begin);
		if(find == m_tSKeyMap.end())		continue;

		tIDList.push_back(find->second);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddKeyIDList(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find == m_tKeyListMap.end())
	{
		TListID tIDList;
		m_tKeyListMap[nKey] = tIDList;
		find = m_tKeyListMap.find(nKey);
	}
	find->second.push_back(nID);
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyIDList(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find != m_tKeyListMap.end())
	{
		find->second.clear();
		m_tKeyListMap.erase(nKey);
	}	
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyIDList(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find == m_tKeyListMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	RemoveListID(find->second, nID);

	if(find->second.size() == 0)
	{
		m_tKeyListMap.erase(nKey);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetKeyIDList(UINT32 nKey, TListID& tIDList)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDListItor find = m_tKeyListMap.find(nKey);
	if(find != m_tKeyListMap.end())
	{
		tIDList = find->second;
	}

	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearKeyIDList()
{
	pthread_mutex_lock (&m_mutex_single);

	m_tKeyListMap.clear();

	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddKeyIDMap(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyMapMap.find(nKey);
	if(find == m_tKeyMapMap.end())
	{
		TMapID tIDMap;
		m_tKeyMapMap[nKey] = tIDMap;
		find = m_tKeyMapMap.find(nKey);
	}

	find->second[nID] = 0;

	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyIDMap(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyMapMap.find(nKey);
	if(find != m_tKeyMapMap.end())
	{
		find->second.clear();
		m_tKeyMapMap.erase(nKey);
	}

	pthread_mutex_unlock (&m_mutex_single);

	return 0;
}


TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyIDMap(UINT32 nKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyMapMap.find(nKey);
	if(find != m_tKeyMapMap.end())
	{
		find->second.erase(nID);
	}

	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetKeyIDMap(UINT32 nKey, TMapID& tIDMap)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyMapMap.find(nKey);
	if(find != m_tKeyMapMap.end())
	{
		AppandMapToMap(find->second, tIDMap);
		nRtn = tIDMap.size();
	}

	pthread_mutex_unlock (&m_mutex_single);	
	return nRtn;
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearKeyIDMap()
{
	pthread_mutex_lock (&m_mutex_single);
	m_tKeyMapMap.clear();
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CManageEdBase<_T>::AddPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		TMapIDMap tIDMapMap;
		m_tPosKeyMapMap[nPos] = tIDMapMap;

		find = m_tPosKeyMapMap.find(nPos);
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{		
		TMapID tIDMap;
		find->second[nKey] = tIDMap;
		find_key = find->second.find(nKey);
	}

	find_key->second[nID] = 0;
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelPosKeyIDMap(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(nKey);
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}


TEMPLATE	UINT32		CManageEdBase<_T>::DelPosKeyIDMap(UINT32 nKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	find_key->second.erase(nID);
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyIDMap(UINT32 nKey, TMapID& tIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	AppandMapToMap(find_key->second, tIDMap);
	
	pthread_mutex_unlock (&m_mutex_single);	

	return tIDMap.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyIDMap(UINT32 nKey, TListID& tIDList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	ConvertMapToList(find_key->second, tIDList);
	pthread_mutex_unlock (&m_mutex_single);
	return tIDList.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyIDMap(TMapIDMap& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex_single);
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearPosKeyIDMap(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyMapMap.find(nPos);
	if(find == m_tPosKeyMapMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddPosSKeyID(String strKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		TMapStrID tStrIDMap;
		m_tPosSKeyMap[nPos] = tStrIDMap;

		find = m_tPosSKeyMap.find(nPos);
	}
	
	find->second[strKey] = nID;
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelPosSKeyID(String strKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	find->second.erase(strKey);
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::FindPosSKeyID_ID(String strKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		TMapStrIDItor find_key = find->second.find(strKey);
		if(find_key != find->second.end())
		{
			nRtn = find_key->second;
		}	
	}
	pthread_mutex_unlock (&m_mutex_single);
	return nRtn;
}

TEMPLATE	_T*			CManageEdBase<_T>::FindPosSKeyID_Item(String strKey, UINT32 nPos)
{
	UINT32 nID = FindPosSKeyID_ID(strKey, nPos);
	if(!nID)	return NULL;

	return FindItem(nID);
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosSKeyID_KeyMap(TMapStrID& tKeyIDMap, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		tKeyIDMap = find->second;
	}

	pthread_mutex_unlock (&m_mutex_single);
	
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosSKeyID_KeyList(TListStr& tStrKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find == m_tPosSKeyMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	TMapStrIDItor begin, end;
	begin = find->second.begin();	end = find->second.end();
	for(begin; begin != end; begin++)
	{
		tStrKeyList.push_back(begin->first);
	}
	pthread_mutex_unlock (&m_mutex_single);
	return tStrKeyList.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearPosSKeyID(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapStrIDItor find = m_tPosSKeyMap.find(nPos);
	if(find != m_tPosSKeyMap.end())
	{
		find->second.clear();
	}

	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	UINT32		CManageEdBase<_T>::AddKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find == m_tKeyTypeMap.end())
	{
		TMapID tSubMap;
		m_tKeyTypeMap[nKey] = tSubMap;
		find = m_tKeyTypeMap.find(nKey);
	}

	find->second[nSubKey] = nID;
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyTypeID(UINT32 nKey)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		find->second.clear();
		m_tKeyTypeMap.erase(nKey);
	}
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelKeyTypeID(UINT32 nKey, UINT32 nSubKey)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		find->second.erase(nSubKey);
	}
	pthread_mutex_unlock (&m_mutex_single);		
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetKeyTypeID(UINT32 nKey, TMapID& tIDMap)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find != m_tKeyTypeMap.end())
	{
		AppandMapToMap(find->second, tIDMap);
	}
	pthread_mutex_unlock (&m_mutex_single);		
	return 0;
}

TEMPLATE	_T*			CManageEdBase<_T>::FindKeySubItem(UINT32 nKey, UINT32 nSubKey)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapItor find = m_tKeyTypeMap.find(nKey);
	if(find == m_tKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return NULL;
	}

	TMapIDItor find_sub = find->second.find(nSubKey);
	if(find_sub == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return NULL;
	}
	pthread_mutex_unlock (&m_mutex_single);	

	return FindItem(find_sub->second);
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearKeyTypeID()
{
	m_tKeyTypeMap.clear();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEMPLATE	UINT32		CManageEdBase<_T>::AddPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nID, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);

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
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::DelPosKeyTypeID(UINT32 nKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);

	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	find_key->second.clear();
	find->second.erase(nKey);
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}


TEMPLATE	UINT32		CManageEdBase<_T>::DelPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	find_key->second.erase(nSubKey);
	pthread_mutex_unlock (&m_mutex_single);	
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyTypeID(UINT32 nKey, TMapID& tSubKeyMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	AppandMapToMap(find_key->second, tSubKeyMap);

	pthread_mutex_unlock (&m_mutex_single);	

	return tSubKeyMap.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyTypeID(UINT32 nKey, TListID& tSubKeyList, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	ConvertMapToList(find_key->second, tSubKeyList);
	pthread_mutex_unlock (&m_mutex_single);
	return tSubKeyList.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyTypeID(TMapIDMap& tKeyIDMap, UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	tKeyIDMap = find->second;
	pthread_mutex_unlock (&m_mutex_single);
	return tKeyIDMap.size();
}

TEMPLATE	UINT32		CManageEdBase<_T>::ClearPosKeyTypeID(UINT32 nPos)
{
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);
		return 0;
	}

	find->second.clear();
	pthread_mutex_unlock (&m_mutex_single);
	return 0;
}

TEMPLATE	UINT32		CManageEdBase<_T>::GetPosKeyTypeID(UINT32 nKey, UINT32 nSubKey, UINT32 nPos)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_mutex_single);
	TMapIDMapMapItor find = m_tPosKeyTypeMap.find(nPos);
	if(find == m_tPosKeyTypeMap.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDMapItor find_key = find->second.find(nKey);
	if(find_key == find->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	TMapIDItor find_subkey = find_key->second.find(nSubKey);
	if(find_subkey == find_key->second.end())
	{
		pthread_mutex_unlock (&m_mutex_single);	
		return 0;
	}

	nRtn = find_subkey->second;

	pthread_mutex_unlock (&m_mutex_single);	

	return nRtn;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CManageEdBase<_T>::SetEditDataMap(UINT32 nID)
{
	m_tEditDataMap.clear();
	m_nEditID = 0;

	return m_tEditDataMap.size();
}

TEMPLATE	INT32		CManageEdBase<_T>::AddEditDataMap(_T& data)
{
	data.nEditID = ++m_nEditID;
	m_tEditDataMap[data.nEditID] = data;
	return m_tEditDataMap.size();
}

TEMPLATE	INT32		CManageEdBase<_T>::EditEditDataMap(_T& data)
{
	m_tEditDataMap[data.nEditID] = data;
	return m_tEditDataMap.size();	
}

TEMPLATE	INT32		CManageEdBase<_T>::DelEditDataMap(UINT32 nEditID)
{
	m_tEditDataMap.erase(nEditID);
	return m_tEditDataMap.size();	
}

TEMPLATE	_T*			CManageEdBase<_T>::FindEditData(UINT32 nEditID)
{
	map <unsigned int, _T>::iterator find;

	find = m_tEditDataMap.find(nEditID);
	if(find == m_tEditDataMap.end())	return NULL;

	return &(find->second);
}

TEMPLATE	INT32		CManageEdBase<_T>::ClearEditDataMap()
{
	m_tEditDataMap.clear();
	return 0;
}

TEMPLATE	INT32		CManageEdBase<_T>::GetEditDataIDList(TListID& tEIDList)
{
	map <unsigned int, _T>::iterator begin, end;
	begin = m_tEditDataMap.begin();	end = m_tEditDataMap.end();

	for (; begin != end; begin++)
	{			
		tEIDList.push_back(begin->second.nEditID);
	}
	return tEIDList.size();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	void		CManageEdBase<_T>::SetEditData(UINT32 nID)
{
	_T* pdata = FindItem(nID);
	if(!pdata)	return;

	SetEditData(*pdata);
}

TEMPLATE	void		CManageEdBase<_T>::SetEditData(_T& data)
{
	m_tEditData = data;
	return;
}

TEMPLATE	_T*			CManageEdBase<_T>::GetEditData()
{
	return &(m_tEditData);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEMPLATE	INT32		CManageEdBase<_T>::InitGlobalIter()
{
	g_begin = m_tMap.begin();
	g_end = m_tMap.end();
	m_nInitGlobalIter = 1;
	return 0;
}

TEMPLATE	INT32		CManageEdBase<_T>::FreeGlobalIter()
{
	m_nInitGlobalIter = 0;
	return 0;
}

TEMPLATE	_T*			CManageEdBase<_T>::NextGlobalIter()
{
	if(!m_nInitGlobalIter)	return NULL;

	if(g_begin == g_end)
	{
		FreeGlobalIter();
		return NULL;
	}
	_T* pRtn = &(g_begin->second);
	g_begin++;
	return pRtn;
}

TEMPLATE	INT32		CManageEdBase<_T>::IsNextGlobalIter()
{
	if(!m_nInitGlobalIter)	return 0;

	if(g_begin == g_end)
	{
		FreeGlobalIter();
		return 0;
	}

	return 1;
}

TEMPLATE	INT32		CManageEdBase<_T>::IsInitGlobalIter()
{
	return m_nInitGlobalIter;
}

///////////////////////////////////////////////////////////////////////////////

TEMPLATE	void		CManageEdBase<_T>::PrintDbgInfo()
{
	return;
}

#endif /**/