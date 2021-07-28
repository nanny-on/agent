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
#include "ManageFileDown.h"

//---------------------------------------------------------------------------

CManageFileDown*	t_ManageFileDown = NULL;

//---------------------------------------------------------------------------

CManageFileDown::CManageFileDown()
{
	
}
//---------------------------------------------------------------------------

CManageFileDown::~CManageFileDown()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::GetIDListByType(UINT32 nType, TListID& tIDList)
{
	TMapAFDLInfoItor begin, end;
	begin = m_tMap.begin(); end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nItemType != nType)	continue;

		tIDList.push_back(begin->second.nID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::ClearItemByType(UINT32 nType)
{
	TListID tIDList;

	{
		TMapAFDLInfoItor begin, end;
		begin = m_tMap.begin(); end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.nItemType != nType)	continue;

			tIDList.push_back(begin->second.nID);
		}
	}

	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			DeleteItem(*begin);
			DelKeyIDList(*begin);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::IsExistDnInfo(UINT32 nType, UINT32 nPoID, UINT32 nItemID)
{
	TMapAFDLInfoItor begin, end;
	begin = m_tMap.begin(); end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nItemType != nType)	continue;
		if(begin->second.nPolicyID != nPoID)	continue;
		if(begin->second.nItemID != nItemID)	continue;

		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::ClearItemByEndDp(UINT32 nType, TListID& tNonDelIDList)
{
	TListID tIDList;

	{
		BOOL bDelFlag = TRUE;
		TMapAFDLInfoItor begin, end;
		begin = m_tMap.begin(); end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.nItemType != nType)	continue;

			bDelFlag = TRUE;

			{
				TListIDItor begin_sub, end_sub;
				begin_sub = tNonDelIDList.begin();	end_sub   = tNonDelIDList.end();
				for(; begin_sub != end_sub; begin_sub++)
				{
					if(begin->first != *begin_sub)		continue;

					bDelFlag = FALSE;
					break;
				}
			}

			if(bDelFlag)	tIDList.push_back(begin->second.nID);
		}
	}

	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			WriteLogE("delete file down id (%d)", *begin);
			DeleteItem(*begin);
			DelKeyIDList(*begin);
		}
	}

	return 0;
}

//---------------------------------------------------------------------------

INT32		CManageFileDown::GetDFIDList(TListID64& tIDList)
{
	TMapAFDLInfoItor begin, end;
	begin = m_tMap.begin(); end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nItemType != ASIFDL_DL_FILE_TYPE_DEPLOY)	continue;

		TListID tSchPkgIDList;
		GetKeyIDList(begin->first, tSchPkgIDList);

		//WriteLogN("test_________[%d]:[%d]", begin->first, tSchPkgIDList.size());

		{
			TListIDItor begin_sub, end_sub;
			begin_sub = tSchPkgIDList.begin();	end_sub = tSchPkgIDList.end();
			for(begin_sub; begin_sub != end_sub; begin_sub++)
			{
				UINT64 nInfo = MAKEUINT64(begin->first, *begin_sub);
				tIDList.push_back(nInfo);
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::AddDFInfo(ASI_FDL_INFO& data, TListID& tSchPkgIDList)
{
	AddItem(data);
	{
		TListIDItor begin, end;
		begin = tSchPkgIDList.begin();	end = tSchPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			AddKeyIDList(data.nID, *begin);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::ChkDFInfo(ASI_FDL_INFO& data, TListID& tSchPkgIDList)
{
	INT32 nExistInfo = 0;
	TMapAFDLInfoItor begin, end;
	begin = m_tMap.begin(); end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nItemType != data.nItemType)	continue;
		if(begin->second.nPolicyID != data.nPolicyID)	continue;
		if(begin->second.nItemID != data.nItemID)		continue;

		nExistInfo = 1;
		TListID tCurSchPkgIDList;
		GetKeyIDList(begin->second.nID, tCurSchPkgIDList);

		{
			TListIDItor begin_sub, end_sub;
			begin_sub = tSchPkgIDList.begin();	end_sub = tSchPkgIDList.end();
			for(begin_sub; begin_sub != end_sub; begin_sub++)
			{
				if(IsExistsIDFromList(tCurSchPkgIDList, *begin_sub))	continue;

				AddKeyIDList(begin->second.nID, *begin_sub);	
			}
		}
	}
	return nExistInfo;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::AddEndDpFile(UINT32 nProID, UINT32 nDpLogID)
{
	if(IsExistKeyID(nProID))
		DelKeyID(nProID);

	AddKeyID(nProID, nDpLogID);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageFileDown::GetEndDpFile(UINT32 nProID)
{
	return FindKeyID(nProID);
}
//---------------------------------------------------------------------------
