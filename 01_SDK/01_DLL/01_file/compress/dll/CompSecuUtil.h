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

#ifndef _COMP_SECU_UTIL_H__

#define _COMP_SECU_UTIL_H__

#include "CompMemToken.h"
//#include "fileutil.h"

class CCompSecuUtil  
{
private:
	NotifyStateType			m_fNotifyState;
	PVOID					lpCallBackHandle;
	INT32					m_nInitEngine;

	ASI_COMP_NOTIFY_INFO	m_tACNI;

private:
	INT32		InitEngine();
	INT32		SetNotifyState(PVOID lParam, PVOID NotifyState);
	String		GetSHA2(PBYTE pBuff, INT32 nLen);

private:
	INT32		CompBuff(PVOID pInBuff, INT32 nLen, PVOID* lpOutBuff, INT32 &nOutLen);
	INT32		DecompBuff(PVOID pInBuff, INT32 nLen, PVOID* lpOutBuff, INT32 &nOutLen);

private:
	INT32		WriteHdr(ASI_COMP_SECU_FILE_HDR& tACSFH, INT32 nTarFd);
	INT32		ReadHdr(ASI_COMP_SECU_FILE_HDR& tACSFH, INT32 nSrcFd);
	INT32		WriteLHdr(ASI_COMP_SECU_LOOP_HDR& tACSLH, INT32 nTarFd);
	INT32		ReadLHdr(ASI_COMP_SECU_LOOP_HDR& tACSLH, INT32 nSrcFd);
	INT32		WriteBody(INT32 nTarFd, PBYTE pBuff, UINT32 nLen);
	INT32		ReadBody(INT32 nSrcFd, PBYTE pBuff, UINT32 nLen);

private:
	INT32		CompBuffToFile(INT32 nFd, PVOID pOrBuf, INT32 nOrLen);
	INT32		DecompBuffToFile(INT32 nFd, PVOID pCompBuff, INT32 nCompLen);

	INT32		CompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pEncKey);
	INT32		DecompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pDecKey);

	INT32		CompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pEncKey, UINT32 nTokenSize);
	INT32		DecompFileToFileToken(INT32 nSrcFd, INT32 nTarFd, PBYTE pDecKey);

public:
	INT32		CompFile(PASI_COMP_SECU pACS, UINT32 nSize);
	INT32		DecompFile(PASI_COMP_SECU pACS, UINT32 nSize, char *acLogMsg = NULL);
	

public:	
	CCompSecuUtil();
	virtual ~CCompSecuUtil();

};

#endif /*_COMP_SECU_UTIL_H__*/