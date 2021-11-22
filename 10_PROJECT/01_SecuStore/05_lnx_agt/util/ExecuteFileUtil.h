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

#pragma once

class CExecuteFileUtil : public CUtilBase
{
public:
	CExecuteFileUtil(void);
	~CExecuteFileUtil(void);

private:
	CProcUtil	m_tProcUtil;

private:
	INT32		ChkFileHash(String strFileName, String strFileHash);
	INT32		ChkFileCertInfo(String strFileName);

public:
	INT32		ExecuteFileByUser(ASI_TS_INFO& tATIO, ASI_TS_MGR_PARAM& tATMP, UINT32 nWaitTime = -1);

private:
	INT32		ExecuteFile_Udt();
	INT32		ExecuteFile_DpFile();
	INT32		ExecuteFile_WPtnGBL(INT32& nChgWPtn);
	INT32		ExecuteFile_WPtnGWL(INT32& nChgWPtn);
	INT32		ExecuteFile_WPtnGBO(INT32& nChgWPtn);
	INT32		ExecuteFile_WPtnGWO(INT32& nChgWPtn);
	INT32		ExecuteFile_WPtnGBC(INT32& nChgWPtn);
	INT32		ExecuteFile_WPtnGWC(INT32& nChgWPtn);

	INT32		ExecuteFile_PPtnASP(INT32& nChgWPtn);
	INT32		ExecuteFile_PPtnWSUS(INT32& nChgWPtn);
	INT32		ExecuteFile_PPtnPatch();
	INT32		ExecuteFile_PPtnDeploy();
	INT32		ExecuteFile_PPtnPatchUtil();

	INT32		ExecuteFile_VPtnASV(INT32& nChgWPtn);
	INT32		ExecuteFile_VPtnWSUS(INT32& nChgWPtn);
	INT32		ExecuteFile_VPtnPatch();
	INT32		ExecuteFile_VPtnDeploy();

public:
	INT32		ExecuteFile(LPVOID lParam);
};

extern CExecuteFileUtil*		t_ExecuteFileUtil;
