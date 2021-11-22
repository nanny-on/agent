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

#include "stdafx.h"
#include "com_struct.h"
#include "SimpleSysCheckUtil.h"

//---------------------------------------------------------------------------

CSimpleSysCheckUtil*		t_SimpleSysCheckUtil = NULL;

//---------------------------------------------------------------------------

CSimpleSysCheckUtil::CSimpleSysCheckUtil()
{
	m_strUtilName = "simple sys check util";
}
//---------------------------------------------------------------------------

CSimpleSysCheckUtil::~CSimpleSysCheckUtil()
{
}
//---------------------------------------------------------------------------

INT32		CSimpleSysCheckUtil::ChkVuln(UINT32 nVulnID, TListID tVulnScanIDList, String strUserSID, INT32 nRepair, INT32 nAutoLock)
{
	INT32 nRtn = 0;
/*
	TListIDItor begin, end;
	begin = tVulnScanIDList.begin();	end = tVulnScanIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VULN_SCAN pdata = t_MMPPVuln->t_ManagePtnVulnScan->FindItem(*begin);
		if(!pdata)	continue;

		nRtn = ChkVuln(nVulnID, pdata->nID, pdata->nCustomID, strUserSID, nRepair, nAutoLock);

		if(nRtn)	break;
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CSimpleSysCheckUtil::ChkVuln(UINT32 nVulnID, UINT32 nVulnScanID, UINT32 nCustomID, String strUserSID, INT32 nRepair, INT32 nAutoLock)
{
	INT32 nRtn = 0;
	switch(nCustomID)
	{
		case SS_PTN_VULN_CUSTOM_ID_WIN_SHARE_FOLDER:		nRtn = ChkVuln_ShareFolder(strUserSID, nRepair, nAutoLock);		break;
		case SS_PTN_VULN_CUSTOM_ID_WIN_USB_RUN:				nRtn = ChkVuln_UsbRun(nRepair, nAutoLock);						break;		
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CSimpleSysCheckUtil::ChkVuln_ScrSaver(String strUserSID, INT32 nRepair, INT32 nAutoLock)
{
	return 0;
/*
	CRegUtil tRegUtil;
	tRegUtil.SetWowDefaultKey();
	tRegUtil.SetRootPath(ConvertLPTSTR(strUserSID));

	CHAR szValue[CHAR_MAX_SIZE] = {0, };
	UINT32 nValue = 0;
	String strRegProtect;
	INT32 nSafe = 0;

	INT32 nRtn = -1;

	do 
	{
		{
			if(nRepair)
				tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveActive", "1");
		}

		{
			if(tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "SCRNSAVE.EXE", szValue))
			{
				if(!nRepair)	break;

				if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "SCRNSAVE.EXE", "scrnsave.scr"))
					break;
			}
			ZeroMemoryExt(szValue);

			if(nAutoLock)
			{
				strRegProtect.Format("HKEY_USERS\\%s\\Control Panel\\Desktop\\SCRNSAVE.EXE", strUserSID);
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);

				strRegProtect = "HKEY_CURRENT_USER\\Control Panel\\Desktop\\SCRNSAVE.EXE";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);
			}
		}

		{
			if(tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveTimeOut", szValue))
			{
				if(!nRepair)	break;

				if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveTimeOut", "300"))
					break;
			}
			else
			{
				if(StrToIntExt(szValue) < 300)
				{
					if(nRepair)
					{
						if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveTimeOut", "300"))
							break;

						nSafe = 1;
					}
				}
				else
				{
					nSafe = 1;
				}				
			}

			if(nAutoLock && nSafe)
			{
				strRegProtect.Format("HKEY_USERS\\%s\\Control Panel\\Desktop\\ScreenSaveTimeOut", strUserSID);
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);

				strRegProtect = "HKEY_CURRENT_USER\\Control Panel\\Desktop\\ScreenSaveTimeOut";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);
			}


			ZeroMemoryExt(szValue);
			nSafe = 0;
		}

		{
			if(tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", szValue))
			{
				if(!nRepair)	break;
				
				if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", "1"))
					break;

				nSafe = 1;
			}
			else
			{
				if(StrToIntExt(szValue) != 1)
				{
					if(nRepair)
					{
						if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", "1"))
							break;

						nSafe = 1;
					}
				}
				else
				{
					nSafe = 1;
				}
			}

			if(nAutoLock && nSafe)
			{
				strRegProtect.Format("HKEY_USERS\\%s\\Control Panel\\Desktop\\ScreenSaverIsSecure", strUserSID);
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);

				strRegProtect = "HKEY_CURRENT_USER\\Control Panel\\Desktop\\ScreenSaverIsSecure";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);
			}

			ZeroMemoryExt(szValue);
			nSafe = 0;
		}

		nRtn = 0;

	} while (FALSE);
	
	return nRtn;
*/
}
//---------------------------------------------------------------------------

INT32		CSimpleSysCheckUtil::ChkVuln_ShareFolder(String strUserSID, INT32 nRepair, INT32 nAutoLock)
{
	return 0;
/*
	INT32 nRtn = -1;

	if(!nRepair)	return nRtn;

	NET_API_STATUS	nStatus		= 0;
	PSHARE_INFO_502	pShareInfo	= NULL;	
	DWORD dwEntry = 0, dwTotalEntry = 0, dwResume = 0;
	BOOL bIsFoundShareDir = FALSE;
	TMapStr tCorrectMap;

	do 
	{
		nStatus = NetShareEnum(NULL, 502, (LPBYTE *)&pShareInfo, -1, &dwEntry, &dwTotalEntry, &dwResume);

		if (pShareInfo == NULL)
			continue;

		if (nStatus == NERR_Success || nStatus == ERROR_MORE_DATA)
		{
			PSHARE_INFO_502	pShareItem = pShareInfo;
			for(int i = 0; i < dwEntry; i++) 
			{				
				NetShareDel(NULL, pShareItem->shi502_netname, 0);
				pShareItem++;
			}

			if(pShareInfo != NULL)
				NetApiBufferFree(pShareInfo);
		}

		nRtn = 0;

	}while(FALSE);

	return nRtn;
*/
}
//---------------------------------------------------------------------------

INT32		CSimpleSysCheckUtil::ChkVuln_UsbRun(INT32 nRepair, INT32 nAutoLock)
{
	return 0;
/*
	CRegUtil tRegUtil;
	tRegUtil.SetWowDefaultKey();
	tRegUtil.SetRootPath("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\IniFileMapping\\Autorun.inf");

	CHAR szValue[CHAR_MAX_SIZE] = {0, };
	UINT32 nValue = 0;
	String strRegProtect;
	INT32 nSafe = 0;

	INT32 nRtn = -1;

	do 
	{
		{
			if(tRegUtil.RegReadString(HKEY_LOCAL_MACHINE, "", "@", szValue))
			{
				if(!nRepair)	break;

				if(tRegUtil.RegWriteString(HKEY_LOCAL_MACHINE, "", "@", "@SYS:NoWhere"))
					break;
			}
			else
			{
				String strData = szValue;

				if(strData != "@SYS:NoWhere")
				{
					if(nRepair)
					{
						if(tRegUtil.RegWriteString(HKEY_LOCAL_MACHINE, "", "@", "@SYS:NoWhere"))
							break;

						nSafe = 1;
					}
				}
				else
				{
					nSafe = 1;
				}				
			}

			if(nAutoLock)
			{
				strRegProtect = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\IniFileMapping\\Autorun.inf\\@";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);

				strRegProtect = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\IniFileMapping\\Autorun.inf\\test";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);

				strRegProtect = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\IniFileMapping\\test";
				t_ManageEpsRegInfo->AddRegInfo(ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP, strRegProtect);
			}
		
			ZeroMemoryExt(szValue);
			nSafe = 0;
		}

		nRtn = 0;

	} while (FALSE);

	return nRtn;
*/
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


