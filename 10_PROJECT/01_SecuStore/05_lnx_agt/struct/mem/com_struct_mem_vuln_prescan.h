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
#ifndef MEM_VULN_PRESCAN_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_VULN_PRESCAN_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_vuln_prescan
{
	_mem_vuln_prescan()
	{
		nID			= 0;
		nVulnID		= 0;
		nVulnScanID	= 0;
		nResult		= 0;
		nErrCode	= 0;
		nRegDate	= 0;
	}

	UINT32		nID;
	UINT32		nVulnID;
	UINT32		nVulnScanID;
	UINT32		nResult;
	UINT32		nErrCode;
	UINT32		nRegDate;

}MEM_VULN_PRESCAN, *PMEM_VULN_PRESCAN;

typedef list<MEM_VULN_PRESCAN>				TListMemVulnPreScan;
typedef TListMemVulnPreScan::iterator		TListMemVulnPreScanItor;

typedef map<UINT32, MEM_VULN_PRESCAN>		TMapMemVulnPreScan;
typedef TMapMemVulnPreScan::iterator		TMapMemVulnPreScanItor;

#endif //MEM_VULN_PRESCAN_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



