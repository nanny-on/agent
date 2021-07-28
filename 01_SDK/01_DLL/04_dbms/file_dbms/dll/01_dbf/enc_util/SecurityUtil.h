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
 */

// SecurityUtil.h: interface for the CSecurityUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECURITYUTIL_H__863101C1_5B8F_43F3_BA18_D4E6C7706337__INCLUDED_)
#define AFX_SECURITYUTIL_H__863101C1_5B8F_43F3_BA18_D4E6C7706337__INCLUDED_

//-----------------------------------------------------------------------------------------------
#define		SGASC_SECURITY_AREA_TYPE_WINDOWS			0
#define		SGASC_SECURITY_AREA_TYPE_WINDOWS_UNIX		1
//-----------------------------------------------------------------------------------------------
#define		SGASC_SECURITY_ENC_TYPE_NONE				0
#define		SGASC_SECURITY_ENC_TYPE_STRING				1
#define		SGASC_SECURITY_ENC_TYPE_SEED				2
#define		SGASC_SECURITY_ENC_TYPE_TLS1				3
//-----------------------------------------------------------------------------------------------
#include "SeedUtil.h"
#include "SHA2Util.h"
#include "StrEncrypt.h"
//-----------------------------------------------------------------------------------------------

class CSecurityUtil  
{
private:
	UINT32	m_nAreaType;
	UINT32	m_nEncryptType;
	UINT32	m_nSessionKey;

private:
	CStrEncrypt		m_tStrEncrypt;
	CSHA2Util		m_tSha2Util;
	CSeedUtil		m_tSeedUtil;

private:
	BYTE	m_pDstHash[32];

public:
	void	SetMode(UINT32 nAreaType, UINT32 nEncryptType);
	void	SetSessionKey(UINT32 nSKey);

public:
	void	Encrypt(UINT8* lpPlanText, UINT32 nPlanLen, UINT8** lpEncText, UINT32& nEncLen);
	void	Decrypt(UINT8* lpEncText, UINT32 nEncLen, UINT8** lpPlanText, UINT32& nPlanLen);
	UINT32	Encrypt_Len(UINT32 nPlanLen);
	
public:
	void	GetSha256(UINT8* lpSrc, UINT32 nSrcLen, UINT8* lpDst);
	INT32	CompareSha256(UINT8* lpSrc, UINT32 nSrcLen, UINT8* lpChkSHA);
	INT32	CompareSha256(UINT8* lpSrcSHA, UINT8* lpChkSHA);
	void	ConvertSha256(UINT8* pHash, LPTSTR lpHashBuffer);
	void	ConvertSha256Last(LPTSTR lpHashBuffer);

public:
	void	PrintSeedRoundKey();
	void	WriteLogN(char* fmt,...);

public:
	CSecurityUtil();
	virtual ~CSecurityUtil();

};

#endif // !defined(AFX_SECURITYUTIL_H__863101C1_5B8F_43F3_BA18_D4E6C7706337__INCLUDED_)
