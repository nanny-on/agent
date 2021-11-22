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
//---------------------------------------------------------------------------

#ifndef SimpleVulnPreScanUtilH
#define SimpleVulnPreScanUtilH
//---------------------------------------------------------------------------

class CSimpleVulnPreScanUtil
{

private:
	String		m_strUtilName;

private:
	INT32		ChkVulnPreScan_VCProcess();
	INT32		ChkVulnPreScan_VCPatch();
	INT32		ChkVulnPreScan_WinUpdate(UINT32 nVulnID, UINT32 nVulnScanID);
	INT32		ChkVulnPreScan_ScrSaver(String strUserSID, INT32 nRepair);	
	INT32		ChkVulnPreScan_EOSSystem();
	INT32		ChkVulnPreScan_WinPwUsed();

public:
	INT32		ChkVulnPreScan(UINT32 nVulnID, TListID tVulnScanIDList, String strUserSID, INT32 nRepair);
	INT32		ChkVulnPreScan(UINT32 nVulnID, UINT32 nVulnScanID, UINT32 nCustomID, String strUserSID, INT32 nRepair);

public:
	INT32		CheckWindowsPatch(UINT32 nWorkID);
	INT32		GetWMI_GetAntiVirusProduct(UINT32& nInstall, UINT32& nEnable, UINT32& nUpdate);
	
public:
	CSimpleVulnPreScanUtil();
    ~CSimpleVulnPreScanUtil();
};

extern CSimpleVulnPreScanUtil*		t_SimpleVulnPreScanUtil;


#endif
