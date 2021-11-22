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

#pragma once

typedef struct _mem_site_vuln_repair_dll
{
	_mem_site_vuln_repair_dll()
	{
		nID				= 0;
		nUsedFlag		= 0;
		nRegDate		= 0;
		nUsedMode		= 0;
		nAdminID		= 0;
		nExtOption		= 0;

		nOsType			= 0;
		nOsID			= 0;
		nOsPa			= 0;

		nSVulnID		= 0;
		nSVulnSubID		= 0;
		nRepairOrder	= 0;

		nRepairType		= 0;
		nCustomID		= 0;

		nRepairOpType	= 0;
		nValueType		= 0;

		nVulnResult		= 0;
		nErrorCode		= 0;
	} 

	UINT32			nID ;
	UINT32			nUsedFlag;
	UINT32			nRegDate;
	UINT32			nUsedMode;
	UINT32			nAdminID;
	UINT32			nExtOption;

	String			strName;
	String			strDescr;

	UINT32			nOsType;		// ASI_SYSTEM_TYPE_XXXX
	UINT64			nOsID;			// ASI_SYSTEM_ID_XXXX	
	UINT32			nOsPa;			// ASI_SYSTEM_ARCH_XXXX	

	UINT32			nSVulnID;
	UINT32			nSVulnSubID;
	UINT32			nRepairOrder;

	UINT32			nRepairType;	// SS_SITE_VULN_TYPE_XXXX
	UINT32			nCustomID;		// SS_SITE_VULN_CUSTOM_ID_XXXX

	String			strRepairPath;		
	UINT32			nValueType;		// SS_SITE_VULN_VALUE_TYPE_XXXX	
	UINT32			nRepairOpType;	// SS_SITE_VULN_OP_TYPE_XXXX
	String			strRepairValue;

	UINT32			nVulnResult;
	UINT32			nErrorCode;

}MEM_SITE_VULN_REPAIR_DLL, *PMEM_SITE_VULN_REPAIR_DLL;

typedef list<MEM_SITE_VULN_REPAIR_DLL>			TListMemSiteVulnRepairDll;
typedef TListMemSiteVulnRepairDll::iterator		TListMemSiteVulnRepairDllItor;

typedef map<UINT32, MEM_SITE_VULN_REPAIR_DLL>	TMapMemSiteVulnRepairDll;
typedef TMapMemSiteVulnRepairDll::iterator		TMapMemSiteVulnRepairDllItor;

//---------------------------------------------------------------------------------------
typedef struct _mem_site_vuln_scan_dll
{
	_mem_site_vuln_scan_dll()
	{
		nID				= 0;
		nUsedFlag		= 0;
		nRegDate		= 0;
		nUsedMode		= 0;
		nAdminID		= 0;
		nExtOption		= 0;

		nOsType			= 0;
		nOsID			= 0;
		nOsPa			= 0;	

		nSVulnID		= 0;
		nSVulnSubID		= 0;
		nScanOrder		= 0;

		nScanType		= 0;
		nCustomID		= 0;
		
		nValueType		= 0;
		nCompType		= 0;
		nDecisionType	= 0;

		nVulnResult		= 0;
		nErrorCode		= 0;
	}

	_mem_site_vuln_scan_dll& operator=(const _mem_site_vuln_scan_dll *pRhs)
	{
		if (pRhs == NULL)
			return *this;

		return operator=(*pRhs);
	}

	_mem_site_vuln_scan_dll& operator=(const _mem_site_vuln_scan_dll &rhs)
	{
		if (this != &rhs)
		{
			this->nID			= rhs.nID;
			this->nUsedFlag		= rhs.nUsedFlag;
			this->nRegDate		= rhs.nRegDate;
			this->nUsedMode		= rhs.nUsedMode;
			this->nAdminID		= rhs.nAdminID;
			this->nExtOption	= rhs.nExtOption;
			this->strName		= rhs.strName;
			this->strDescr		= rhs.strDescr;
			this->nOsType		= rhs.nOsType;
			this->nOsID			= rhs.nOsID;
			this->nOsPa			= rhs.nOsPa;
			this->nSVulnID		= rhs.nSVulnID;
			this->nSVulnSubID	= rhs.nSVulnSubID;
			this->nScanOrder	= rhs.nScanOrder;
			this->nScanType		= rhs.nScanType;
			this->nCustomID		= rhs.nCustomID;
			this->strScanPath	= rhs.strScanPath;
			this->nValueType	= rhs.nValueType;
			this->nDecisionType	= rhs.nDecisionType;
			this->nCompType		= rhs.nCompType;
			this->strCompValue	= rhs.strCompValue;
		}
		return *this;
	}

	UINT32			nID ;
	UINT32			nUsedFlag;
	UINT32			nRegDate;
	UINT32			nUsedMode;
	UINT32			nAdminID;
	UINT32			nExtOption;

	String			strName;
	String			strDescr;

	UINT32			nOsType;			// ASI_SYSTEM_TYPE_XXXX
	UINT64			nOsID;			// ASI_SYSTEM_ID_XXXX	
	UINT32			nOsPa;			// ASI_SYSTEM_ARCH_XXXX	

	UINT32			nSVulnID;
	UINT32			nSVulnSubID;
	UINT32			nScanOrder;
	
	UINT32			nScanType;		// SS_SITE_VULN_TYPE_XXXX
	UINT32			nCustomID;		// SS_SITE_VULN_CUSTOM_ID_XXXX

	String			strScanPath;		
	UINT32			nValueType;		// SS_SITE_VULN_VALUE_TYPE_XXXX
	UINT32			nDecisionType;		// SS_SITE_VULN_DECISION_CRITERIA_XXXX
	UINT32			nCompType;		// SS_SITE_VULN_COMPARE_TYPE_XXXX
	String			strCompValue;	

	INT32			nVulnResult;
	INT32			nErrorCode;

}MEM_SITE_VULN_SCAN_DLL, *PMEM_SITE_VULN_SCAN_DLL;

typedef list<MEM_SITE_VULN_SCAN_DLL>			TListMemSiteVulnScanDll;
typedef TListMemSiteVulnScanDll::iterator		TListMemSiteVulnScanDllItor;

typedef map<UINT32, MEM_SITE_VULN_SCAN_DLL>		TMapMemSiteVulnScanDll;
typedef TMapMemSiteVulnScanDll::iterator		TMapMemSiteVulnScanDllItor;
//---------------------------------------------------------------------------------------

typedef struct _mem_vuln_callback_param_dll
{
	_mem_vuln_callback_param_dll()
	{
		nContext		= 0;
		pFuncCallBack	= NULL;
		hWndCallBack	= NULL;
		nCallBackMsg	= 0;
	} 

	UINT64			nContext;
	LPVOID			pFuncCallBack;
	HWND			hWndCallBack;
	UINT32			nCallBackMsg;

}MEM_VULN_CALLBACK_PARAM_DLL, *PMEM_VULN_CALLBACK_PARAM_DLL;
//---------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
typedef	INT32 (*ASISYSCHECK_InitializeType)();
typedef	INT32 (*ASISYSCHECK_UnIntializeType)();
typedef	INT32 (*ASISYSCHECK_ScanType)(MEM_SITE_VULN_SCAN_DLL*);
typedef	INT32 (*ASISYSCHECK_ScanListType)(TListMemSiteVulnScanDll*, MEM_VULN_CALLBACK_PARAM_DLL*);
typedef	INT32 (*ASISYSCHECK_RepairType)(MEM_SITE_VULN_REPAIR_DLL*);
typedef	INT32 (*ASISYSCHECK_RepairListType)(TListMemSiteVulnRepairDll*, MEM_VULN_CALLBACK_PARAM_DLL*);
//////////////////////////////////////////////////////////////////////////