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

#ifndef DB_PTN_PATCH_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_PATCH_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE


typedef struct _db_ptn_patch_lan
{
	String		strName;
	String		strDescr;

}DB_PTN_PATCH_LAN, *PDB_PTN_PATCH_LAN, DPPL, *PDPPL;

typedef struct _db_ptn_patch
{
	_db_ptn_patch()
	{
		nID					= 0;
		nUsedMode			= 1;
		nRevID				= 0;
		nType				= 0;

		nRisk				= 0;
		nReBoot				= 0;
		nRollBack			= 0;
		nApplyOrder			= 0;

		nVulnKBID			= 0;

		nReplace			= 0;

		nProcArch			= 0;

		nSrcType			= 0;
		nPtnCrDate			= 0;
		nPtnModDate			= 0;
	}

	UINT32		nID;
	UINT32		nUsedMode;
	UINT32		nRevID;
	UINT32		nType;	

	UINT32		nAnnounce;
	DPPL		tLanKor;
	DPPL		tLanEng;

	INT32  		nRisk;
	INT32		nReBoot;
	UINT32		nRollBack;
	INT32		nApplyOrder;

	String		strVulnName;
	String		strVulnPublisher;
	String		strVulnGuid;
	UINT32		nVulnKBID;

	String		strRefer;
	UINT32		nReplace;

	String		strTarComp;
	String		strTarFamily;
	String		strTarProduct;
	UINT32		nProcArch;

	UINT32		nSrcType;	
	UINT32		nPtnCrDate;
	UINT32		nPtnModDate;

}DB_PTN_PATCH, *PDB_PTN_PATCH;

typedef list<DB_PTN_PATCH>				TListDBPtnPatch;
typedef TListDBPtnPatch::iterator		TListDBPtnPatchItor;

typedef map<UINT32, DB_PTN_PATCH>		TMapDBPtnPatch;
typedef TMapDBPtnPatch::iterator		TMapDBPtnPatchItor;

#endif /*DB_PTN_PATCH_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



