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


#ifndef LogicCtrlVulnH
#define LogicCtrlVulnH
//---------------------------------------------------------------------------

class CLogicCtrlVuln	: public CLogicBase
{        
public:
	INT32		AnalyzePkt_FromLink_Ext();
	INT32		AnalyzePkt_FromLink_Ext_Scan();


public:
	void		SendPkt_Scan(UINT32 nID, UINT32 nRst, TListMemVulnRst* tVulnPatchList);

public:
	INT32		ScanPtnVuln(PDB_PTN_VULN pdata);
	INT32		ScanPtnVulnCustom(PDB_PTN_VULN_SCAN pdata);

public:
	INT32		ScanWindowsPatch();
	INT32		CheckWindowsPatch(UINT32 nWorkID);

public:
	INT32		ScanHwpUpdate();

public:
	INT32		ScanPDFUpdate();

public:
	INT32		ScanEditProg();
	INT32		ScanEditProg_Global(UINT32 nID, TListMemVulnRst& tNotiList);
	INT32		ScanEditProg_Site(UINT32 nID, TListMemVulnRst& tNotiList);

public:
	INT32		ScanNonConfirmProg();
	INT32		ScanNonConfirmProg_Global(String strSwName, String strSwVersion, TListMemVulnRst& tNotiList);
	INT32		ScanNonConfirmProg_Site(UINT32 nID, TListMemVulnRst& tNotiList);

public:
	INT32		ScanSecuUSBProg();
	INT32		ScanSecuUSBProg_ChkReg(String strRegKey);
	INT32		ScanSecuUSBProg_ChkProc(String strProcess);
	INT32		ScanSecuUSBProg_ChkFile(String strFilePath);

public:
	INT32		MakeSkipWindowsPatch(TMapIDMap& tSkipIDMap);
	INT32		MakeSkipWindowsPatchUnit(TMapIDMap& tSkipIDMap, String strInfo);

	INT32		IsSkipWindowsPatch(TMapIDMap& tSkipIDMap, PMEM_VULN_RST pdata);

public:
	CLogicCtrlVuln();
    ~CLogicCtrlVuln();
};

extern CLogicCtrlVuln*		t_LogicCtrlVuln;

#endif
