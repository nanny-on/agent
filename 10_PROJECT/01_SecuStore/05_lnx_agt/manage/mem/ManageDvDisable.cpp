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
#include "ManageDvDisable.h"

//---------------------------------------------------------------------------

CManageDvDisable*	t_ManageDvDisable = NULL;

//---------------------------------------------------------------------------

CManageDvDisable::CManageDvDisable()
{
	
}
//---------------------------------------------------------------------------

CManageDvDisable::~CManageDvDisable()
{
	
}
//---------------------------------------------------------------------------
UINT32		CManageDvDisable::GetMaxID()
{
	TMapMemDvDisableRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	if(begin == end)	return 1;

	return (begin->first + 1);
}
//---------------------------------------------------------------------------
void		CManageDvDisable::DevicesEnable()
{
/*
	TMapMemDvDisableItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(t_ASIDVIDLLUtil->StateChange(begin->second.strDvInstanceID, 1) != 0)
			WriteLogE("fail to enable device : %s", begin->second.strDvInstanceID);
	}
*/
}
//---------------------------------------------------------------------------
void		CManageDvDisable::CurUsbEnable()
{
/*
	TListDeviceInfo tDviList;
	t_ASIDVIDLLUtil->GetDeviceInfo(tDviList);
	if(tDviList.size() > 0)
	{
		TListDeviceInfoItor begin, end;
		begin = tDviList.begin();	end = tDviList.end();
		for(begin; begin != end; begin++)
		{
			String strClassName = "", strDvInstanceID = "", strDvPaInstanceID = "";
			strClassName.Format("%s", begin->strClassName);
			strDvInstanceID.Format("%s", begin->strDeviceInstanceID);
			strDvPaInstanceID.Format("%s", begin->strPaDeviceInstanceID);
			strClassName.MakeLower();	strDvInstanceID.MakeLower();	strDvPaInstanceID.MakeLower();

			if((strDvInstanceID.Left(4).Compare("usb\\") == 0 || strDvPaInstanceID.Left(4).Compare("usb\\") == 0) && (t_ManagePoDvDefault->FindDvPolID(begin->strDeviceInstanceID, begin->strPaDeviceInstanceID) == 0))
			{
				t_ASIDVIDLLUtil->StateChange(begin->strDeviceInstanceID, 1);
			}
		}
	}
*/
}