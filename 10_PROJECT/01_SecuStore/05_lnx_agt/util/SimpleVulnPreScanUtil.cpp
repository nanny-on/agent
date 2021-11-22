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
#include "SimpleVulnPreScanUtil.h"

//---------------------------------------------------------------------------

CSimpleVulnPreScanUtil*		t_SimpleVulnPreScanUtil = NULL;

//---------------------------------------------------------------------------

CSimpleVulnPreScanUtil::CSimpleVulnPreScanUtil()
{
	m_strUtilName = "simple vuln pre scan util";
}
//---------------------------------------------------------------------------

CSimpleVulnPreScanUtil::~CSimpleVulnPreScanUtil()
{
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan(UINT32 nVulnID, TListID tVulnScanIDList, String strUserSID, INT32 nRepair)
{
	INT32 nRtn = 0;
/*
	TListIDItor begin, end;
	begin = tVulnScanIDList.begin();	end = tVulnScanIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VULN_SCAN pdata = t_MMPPVuln->t_ManagePtnVulnScan->FindItem(*begin);
		if(!pdata)	continue;

		nRtn = ChkVulnPreScan(nVulnID, pdata->nID, pdata->nCustomID, strUserSID, nRepair);

		if(nRtn)	break;
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan(UINT32 nVulnID, UINT32 nVulnScanID, UINT32 nCustomID, String strUserSID, INT32 nRepair)
{
	INT32 nRtn = 0;
/*
	switch(nCustomID)
	{
		case SS_PTN_VULN_CUSTOM_ID_VC_PROCESS:				nRtn = ChkVulnPreScan_VCProcess();							break;
		case SS_PTN_VULN_CUSTOM_ID_VC_PATCH:				nRtn = ChkVulnPreScan_VCPatch();							break;
		case SS_PTN_VULN_CUSTOM_ID_WIN_UPDATE:				nRtn = ChkVulnPreScan_WinUpdate(nVulnID, nVulnScanID);		break;
		case SS_PTN_VULN_CUSTOM_ID_WIN_SCREEN_SAFE:			nRtn = ChkVulnPreScan_ScrSaver(strUserSID, nRepair);		break;
		case SS_PTN_VULN_CUSTOM_ID_EOS_SYSTEM:				nRtn = ChkVulnPreScan_EOSSystem();							break;
		case SS_PTN_VULN_CUSTOM_ID_WIN_PW_USED:				nRtn = ChkVulnPreScan_WinPwUsed();							break;
	}


	MEM_VULN_PRESCAN tMVPS;
	{
		PMEM_VULN_PRESCAN pMVPS = t_ManageVulnPreScan->FindKeyTypeID_Item(nVulnID, nVulnScanID);
		if(pMVPS)		tMVPS	= *pMVPS;
		else
		{
			tMVPS.nVulnID		= nVulnID;
			tMVPS.nVulnScanID	= nVulnScanID;
		}
		tMVPS.nRegDate		= GetCurrentDateTimeInt();		
		tMVPS.nResult		= nRtn;

		t_ManageVulnPreScan->ApplyVulnPreScan(tMVPS);
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_VCProcess()
{
/*
	UINT32 nInstall = 0;
	UINT32 nEnable = 0;
	UINT32 nUpdate = 0;

	GetWMI_GetAntiVirusProduct(nInstall, nEnable, nUpdate);

	if(nInstall)	return SS_PTN_VULN_SCAN_RESULT_SAFE;
*/
	return SS_PTN_VULN_SCAN_RESULT_DANGER;
		
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_VCPatch()
{
/*
	UINT32 nInstall = 0;
	UINT32 nEnable = 0;
	UINT32 nUpdate = 0;

	GetWMI_GetAntiVirusProduct(nInstall, nEnable, nUpdate);

	if(nUpdate)	return SS_PTN_VULN_SCAN_RESULT_SAFE;
*/
	return SS_PTN_VULN_SCAN_RESULT_DANGER;
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::GetWMI_GetAntiVirusProduct(UINT32& nInstall, UINT32& nEnable, UINT32& nUpdate)
{
	return 0;
/*
	INT32 nRtn = -1;
	String strBuffer;
	HRESULT hr = S_FALSE;
	IWbemLocator *pLocator					= NULL;
	IWbemServices *pServices				= NULL;
	IWbemClassObject *pObject				= NULL;
	IEnumWbemClassObject* pEnumerator		= NULL;

	do 
	{
		hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
		if (FAILED(hr) && hr != RPC_E_TOO_LATE)
		{
			break;
		}

		if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLocator)))
		{
			break;
		}

		if (FAILED(pLocator->ConnectServer(_bstr_t(L"ROOT\\SECURITYCENTER2"), NULL, NULL, 0, NULL, 0, 0, &pServices)))
		{
			break;
		}

		if (FAILED(CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
		{
			break;
		}

		BSTR bstrQuery = ::SysAlloString(L"SELECT * FROM AntiVirusProduct");
		hr = pServices->ExecQuery(bstr_t("WQL"), bstrQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
		BSTR_RELEASE(bstrQuery);
		if (FAILED(hr) || pEnumerator == NULL)
		{
			break;
		}

		VARIANT vtProp;
		ULONG uReturn	= 0;
		
		USES_CONVERSION;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		while (pEnumerator)
		{
			hr = pEnumerator->Next(WBEM_INFINITE, 1, &pObject, &uReturn);
			if(0 == uReturn)	break;

			VariantInit(&vtProp);
			hr = pObject->Get(L"Productstate", 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr))
			{
				nInstall = TRUE;

				unsigned long ulProductStatus = 0;
				ulProductStatus = vtProp.ulVal;
				
				if((ulProductStatus & 0x00001000) == 0x00001000)
				{
					nEnable = TRUE;
					if((ulProductStatus & 0x00000010) == 0x00000010)
					{
						nUpdate = FALSE;
					}
					else
					{
						nUpdate = TRUE;
					}
				}
			}
			VariantClear(&vtProp);
		}	

		nRtn = 0;
	} while (FALSE);


	COM_RELEASE(pServices);
	COM_RELEASE(pLocator);
	COM_RELEASE(pEnumerator);
	COM_RELEASE(pObject);

	return nRtn;
*/		
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_WinUpdate(UINT32 nVulnID, UINT32 nVulnScanID)
{
	INT32 nRtn = SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
/*
	INT32 nErrRtn = ERR_SUCCESS;

	do 
	{
		INT32 nVulnOnLineScan = t_LogicMgrPtnVuln->IsVulnWsusLive();

		ASI_PMS_WORK tAPW;
		tAPW.nID = t_ManagePMSWork->GetLastID();
		sprintf_ext(MAX_PATH, tAPW.szWorkName, "scan_vuln_new_patch_list_%u", tAPW.nID);
		tAPW.nScanType = (nVulnOnLineScan ? ASI_PMS_WORK_SCAN_TYPE_ONLINE : ASI_PMS_WORK_SCAN_TYPE_OFFLINE);
		tAPW.hNotifyHwnd = t_EnvInfoOp->GetMainHandle();
		tAPW.nNotifyID = WM_GLOBAL_THREAD_VULN_PMS_WORK_PRE_NEW;
		
		tAPW.nPolicyID = nVulnID;
		tAPW.nItemID = nVulnScanID;

		if(!nVulnOnLineScan)
		{
			String strWScanPtnPath = t_LogicMgrPtnVuln->GetLastVulnWsusFName();
			if(strWScanPtnPath.empty())
			{
				WriteLogN("[%s] vuln patch scan fail...current mode offline and not find wua ptn file : [%s][%d][%x]", m_strUtilName, tAPW.szWorkName, tAPW.nScanType, tAPW.nFindType);
				nRtn = SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
				nErrRtn = ERR_SYS_FILE_NOT_FIND;

				break;
				//#ifdef _DEBUG
				//strWScanPtnPath = "d:\\wsusscn2_20200212.cab";
				//#endif
			}
			tAPW.nFindType += ASI_PMS_WORK_FIND_TYPE_NEW_OS_VULN_WUA;
			strcpy_ext(tAPW.szPtnPath, strWScanPtnPath, MAX_PATH); 
		}
		else
		{
			tAPW.nFindType += ASI_PMS_WORK_FIND_TYPE_NEW_OS_VULN_WUA;
		}

		WriteLogN("[%s] vuln pms pre scan start : [%s][%s][%x]", m_strUtilName, tAPW.szWorkName, (tAPW.nScanType == ASI_PMS_WORK_SCAN_TYPE_ONLINE ? "online" : "offline"), tAPW.nFindType);
		t_ASIPMSVulnUtil->ASIPMS_ScanPatch(&tAPW);
		t_ManagePMSWork->AddItem(tAPW);

		nRtn = SS_PTN_VULN_SCAN_RESULT_SCANNING;

	} while (FALSE);
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::CheckWindowsPatch(UINT32 nWorkID)
{
/*
	INT32 nSendPkt = 1;	
	TListMemVulnRst tCurList, tNotiList;
	TMapID tExIDMap, tDMIDMap;
	MEM_VULN_PRESCAN tMVPS;

	ASI_PMS_WORK data_work;
	PASI_PMS_WORK pdata_work = t_ManagePMSWork->FindItem(nWorkID);
	if(!pdata_work)
	{
		WriteLogE("[%s] not find pms work info : [%d]", m_strUtilName, nWorkID);
		return -1;
	}
	else
	{
		data_work = *pdata_work;
	}	

	{
		PMEM_VULN_PRESCAN pMVPS = t_ManageVulnPreScan->FindKeyTypeID_Item(pdata_work->nPolicyID, pdata_work->nItemID);
		if(pMVPS)		tMVPS	= *pMVPS;
		else
		{
			tMVPS.nVulnID		= pdata_work->nPolicyID;
			tMVPS.nVulnScanID	= pdata_work->nItemID;
		}
		tMVPS.nRegDate		= GetCurrentDateTimeInt();		
	}

	do 
	{
		{			
			t_ASIPMSVulnUtil->ASIPMS_GetLastError(nWorkID, &tMVPS.nErrCode);

			if(tMVPS.nErrCode)
			{
				WriteLogE("[%s] occur error when pms po scan unit info : [%d][%x]", m_strUtilName, pdata_work->nItemID, tMVPS.nErrCode);
				break;
			}
		}

		{			
			if(pdata_work->nFindType & (ASI_PMS_WORK_FIND_TYPE_NEW_OS_VULN_WUA))
			{
				UINT32 nFindType = ASI_PMS_WORK_FIND_TYPE_NEW_OS_VULN_WUA;
				UINT32 nFindCnt = 0;
				t_ASIPMSVulnUtil->ASIPMS_GetFindCnt(nWorkID, nFindType, &nFindCnt);

				WriteLogN("[%s] FindCnt Origin : [%d]", m_strUtilName, nFindCnt);

				while(nFindCnt)
				{
					nFindCnt -= 1;
					ASI_PMS_FIND_ITEM tAPFI;
					t_ASIPMSVulnUtil->ASIPMS_GetFindRst(nWorkID, nFindType, nFindCnt, &tAPFI);

					if(tAPFI.szServerity == NULL || tAPFI.szServerity[0] == NULL)
					{
						WriteLogN("[%s] FindCnt Origin Skip by non check serverity : [%s]", m_strUtilName, tAPFI.szVulnName);
						continue;
					}

					MEM_VULN_RST tMVP;	
					tMVP._set_str(0, tAPFI.szVulnName);
					tMVP._set_str(1, tAPFI.szServerity);
					tMVP._set_int(0, tAPFI.nKBID);

					PDB_PTN_PATCH pdata_patch = t_MMPPVuln->t_ManagePtnPatch->FindPosSKeyID_Item(tAPFI.szGUID, POS_KEY_STR_POSITION_PTN_PATCH_GUID_MAP);
					if(pdata_patch)
					{	
						tMVP._set_int(1, pdata_patch->nType);
						tMVP._set_int(2, pdata_patch->nID);
					}

					tCurList.push_back(tMVP);			
				}
			}
		}

	} while (FALSE);

	{
		t_ASIPMSVulnUtil->ASIPMS_ClearWork(nWorkID);
		t_ManagePMSWork->DeleteItem(nWorkID);
	}

	do 
	{
		if(tMVPS.nErrCode)
		{
			tMVPS.nResult = SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
			break;
		}

		{	
			TMapIDMap tSkipIDMap;
			t_LogicCtrlVuln->MakeSkipWindowsPatch(tSkipIDMap);

			INT32 nCurIdx = 1;
			TListMemVulnRstItor begin, end;
			begin = tCurList.begin();	end = tCurList.end();
			for(begin; begin != end; ++begin)
			{			
				PMEM_VULN_RST pdata = &(*begin);

				if((pdata->_get_int(1) & pdata_work->nScanType)	== 0)	
				{
					WriteLogN("[%s] find result skip by non match scan type : [%s]:[%x/%x]", m_strUtilName, pdata->_get_str(0), pdata->_get_int(1), pdata_work->nScanType);
					continue;
				}

				if(t_LogicCtrlVuln->IsSkipWindowsPatch(tSkipIDMap, pdata))			
				{
					WriteLogN("[%s] find result skip by admin skip item : [%s]", m_strUtilName, pdata->_get_str(0));
					continue;
				}

				tNotiList.push_back(*begin);

				WriteLogN("[%s] FindItem : [%d]---[%d][%s]", m_strUtilName, nCurIdx++, pdata->_get_int(0), pdata->_get_str(0));
			}
		}

		if(tNotiList.size())	tMVPS.nResult = SS_PTN_VULN_SCAN_RESULT_DANGER;
		else					tMVPS.nResult = SS_PTN_VULN_SCAN_RESULT_SAFE;

	} while (FALSE);	

	t_ManageVulnPreScan->ApplyVulnPreScan(tMVPS);
*/
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_ScrSaver(String strUserSID, INT32 nRepair)
{
	INT32 nRtn = SS_PTN_VULN_SCAN_RESULT_DANGER;
/*
	CRegUtil tRegUtil;
	tRegUtil.SetWowDefaultKey();
	tRegUtil.SetRootPath(ConvertLPTSTR(strUserSID));

	CHAR szValue[TCHAR_MAX_SIZE] = {0, };
	UINT32 nValue = 0;
	String strRegProtect;

	do 
	{
		{
			tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveActive", szValue);
			
			if(strcmp(szValue, "1"))
			{
				if(nRepair)
					tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaveActive", "1");
				else
					break;
			}
			ZeroMemoryExt(szValue);
		}

		{
			if(tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "SCRNSAVE.EXE", szValue))
			{
				if(!nRepair)	break;

				if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "SCRNSAVE.EXE", "scrnsave.scr"))
					break;
			}
			ZeroMemoryExt(szValue);
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
					}
					else
						break;
				}				
			}

			ZeroMemoryExt(szValue);
		}

		{
			if(tRegUtil.RegReadString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", szValue))
			{
				if(!nRepair)	break;
				
				if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", "1"))
					break;
			}
			else
			{
				if(StrToIntExt(szValue) != 1)
				{
					if(nRepair)
					{
						if(tRegUtil.RegWriteString(HKEY_USERS, "\\Control Panel\\Desktop", "ScreenSaverIsSecure", "1"))
							break;
					}
					else
						break;
				}
			}

			ZeroMemoryExt(szValue);
		}

		nRtn = SS_PTN_VULN_SCAN_RESULT_SAFE;

	} while (FALSE);
*/	
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_EOSSystem()
{
/*
	CSystemInfo tSysInfo;
	if(tSysInfo.GetOsMajorVer() >= 10)	return SS_PTN_VULN_SCAN_RESULT_SAFE;
*/
	return SS_PTN_VULN_SCAN_RESULT_DANGER;
}
//---------------------------------------------------------------------------

INT32		CSimpleVulnPreScanUtil::ChkVulnPreScan_WinPwUsed()
{
	return 0;
/*
	String strTemp;

	INT32 nRtn = SS_PTN_VULN_SCAN_RESULT_DANGER;

	INT32 nLogonRtn = 0, nRet = 0, nRetCode = 0;
	DWORD dwError = 0;
	String	strCurUserName = t_ManageWinSession.GetLogonUserName();
	String strCurDomainName = t_ManageWinSession.GetLogonDomainName();
	String strUserPw = "";

	HANDLE pHandle = NULL;
	{

		do 
		{
			nLogonRtn = LogonUser(strCurUserName, NULL, strUserPw, LOGON32_LOGON_NETWORK,LOGON32_PROVIDER_DEFAULT,&pHandle);
			dwError = GetLastError();

			if(nLogonRtn == 0)
			{
				if(dwError == 1327)		nRtn = SS_PTN_VULN_SCAN_RESULT_SAFE;
				else
				{
					nLogonRtn = LogonUser(strCurUserName, NULL, strUserPw, LOGON32_LOGON_INTERACTIVE,LOGON32_PROVIDER_DEFAULT,&pHandle);
					if(nLogonRtn==0)
					{
						nRtn = SS_PTN_VULN_SCAN_RESULT_SAFE;
					}
				}
			}
		} while (FALSE);

		SAFE_CLOSE(pHandle);
	}
	return nRtn;
*/
}
//---------------------------------------------------------------------------


