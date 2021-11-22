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

// SecurityUtil.cpp: implementation of the CSecurityUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "SecurityUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

CSecurityUtil::CSecurityUtil()
{	
	m_nEncryptType		= ASI_BUFFER_ENC_TYPE_STRING;
	m_nSessionKey		= 0;
}
//-----------------------------------------------------------------------------------------------

CSecurityUtil::~CSecurityUtil()
{

}	
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::SetMode(UINT32 nEncryptType)
{
	m_nEncryptType	= nEncryptType;

	if(m_nSessionKey)
	{
		switch(m_nEncryptType)
		{
			case ASI_BUFFER_ENC_TYPE_SEED:
			case ASI_BUFFER_ENC_TYPE_TLS1:
			{
				m_tSeedUtil.Set_Key(PBYTE(&m_nSessionKey), sizeof(m_nSessionKey));
				break;
			}
		}
	}

	return;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::SetSessionKey(UINT32 nSKey)
{
	m_nSessionKey	= nSKey;
	if(m_nSessionKey)
	{
		switch(m_nEncryptType)
		{
			case ASI_BUFFER_ENC_TYPE_SEED:
			case ASI_BUFFER_ENC_TYPE_TLS1:
			{
				m_tSeedUtil.Set_Key(PBYTE(&m_nSessionKey), sizeof(m_nSessionKey));
				break;
			}
		}
	}	
	
	return;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::Encrypt(UINT8* lpPlanText, UINT32 nPlanLen, UINT8** lpEncText, UINT32& nEncLen)
{
	if(!lpPlanText || !nPlanLen)	return;
	
	switch(m_nEncryptType)
	{
		case ASI_BUFFER_ENC_TYPE_STRING:
		{
			if(!m_nSessionKey)
				break;
			
			m_tStrEncrypt.ScrembleEncrypt(m_nSessionKey, lpPlanText, nPlanLen);
			
			*lpEncText = lpPlanText;
			nEncLen = nPlanLen;
			break;
		}
		case ASI_BUFFER_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)
			{
				memcpy(*lpEncText, lpPlanText, nPlanLen);
				break;
			}

			m_tSeedUtil.Encrypt_Data(lpPlanText, nPlanLen, lpEncText, nEncLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_TLS1:
		{			
			if(!m_nSessionKey)
			{
				memcpy(*lpEncText, lpPlanText, nPlanLen);
				break;
			}
			
			m_tSeedUtil.Encrypt_Data(lpPlanText, nPlanLen, lpEncText, nEncLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_NONE:
		default:
		{
			*lpEncText = lpPlanText;
			nEncLen = nPlanLen;
			break;
		}
	}
}
//-----------------------------------------------------------------------------------------------

UINT32	CSecurityUtil::Encrypt_Len(UINT32 nPlanLen)
{
	UINT32 nOutLen = 0;

	switch(m_nEncryptType)
	{
		case ASI_BUFFER_ENC_TYPE_STRING:
		{
			nOutLen = nPlanLen;
			break;
		}
		case ASI_BUFFER_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)	nOutLen = nPlanLen;
			else				nOutLen = m_tSeedUtil.Encrypt_Len(nPlanLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_TLS1:
		{
			if(!m_nSessionKey)	nOutLen = nPlanLen;
			else				nOutLen = m_tSeedUtil.Encrypt_Len(nPlanLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_NONE:
		default:
		{
			nOutLen = nPlanLen;
			break;
		}
	}
	return nOutLen;
}
//-----------------------------------------------------------------------------------------------

INT32	CSecurityUtil::Decrypt(UINT8* lpEncText, UINT32 nEncLen, UINT8** lpPlanText, UINT32& nPlanLen)
{
	INT32 nRet = 0;

	if(!lpEncText || !nEncLen)	return -1;
	
	switch(m_nEncryptType)
	{
		case ASI_BUFFER_ENC_TYPE_STRING:
		{
			if(!m_nSessionKey)
				break;

			m_tStrEncrypt.ScrembleDecrypt(m_nSessionKey, lpEncText, nEncLen);
			
			*lpPlanText = lpEncText;
			nPlanLen = nEncLen;
			break;
		}
		case ASI_BUFFER_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)
			{
				*lpPlanText = (BYTE*)malloc(nEncLen + 1);
				if(*lpPlanText != NULL)
				{
					memset(*lpPlanText, 0, nEncLen + 1);
					memcpy(*lpPlanText, lpEncText, nEncLen);
					nPlanLen = nEncLen;
				}
				break;
			}
			
			nRet = m_tSeedUtil.Decrypt_Data(lpEncText, lpPlanText, nPlanLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_TLS1:
		{
			if(!m_nSessionKey)
			{
				*lpPlanText = (BYTE*)malloc(nEncLen + 1);
				if(*lpPlanText != NULL)
				{
					memset(*lpPlanText, 0, nEncLen + 1);
					memcpy(*lpPlanText, lpEncText, nEncLen);
					nPlanLen = nEncLen;
				}
				break;
			}
			
			m_tSeedUtil.Decrypt_Data(lpEncText, lpPlanText, nPlanLen);
			break;
		}
		case ASI_BUFFER_ENC_TYPE_NONE:
		default:
		{
			*lpPlanText = lpEncText;
			nPlanLen = nEncLen;
			break;
		}
	}

	return nRet;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::GetSha256(UINT8* lpSrc, DWORD dwSrcLen, UINT8* lpDst, DWORD dwDstLen)
{
	m_tSha2Util.SHA256Reset();
	m_tSha2Util.SHA256Input((LPCTSTR)lpSrc, dwSrcLen);
	m_tSha2Util.SHA256ResultByte(lpDst);
	return;
}
//-----------------------------------------------------------------------------------------------

INT32	CSecurityUtil::CompareSha256(UINT8* lpSrc, UINT32 nSrcLen, UINT8* lpChkSHA)
{
	INT32 nIndex = 32;

	memset(m_pDstHash, 0, nIndex);
	if(nSrcLen)
		GetSha256(lpSrc, (DWORD)nSrcLen, m_pDstHash, (DWORD)sizeof(m_pDstHash));
	
	while(nIndex)
	{
		nIndex--;
		if(m_pDstHash[nIndex] == lpChkSHA[nIndex])
			continue;
		return 1;
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::ConvertSha256Last(LPSTR lpHashBuffer, DWORD dwMaxLen)
{
	ConvertSha256(m_pDstHash, lpHashBuffer, dwMaxLen);
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::ConvertSha256(UINT8* pHash, LPSTR lpHashBuffer, DWORD dwMaxLen)
{
	CHAR pszCellHash[32] = {0,};
	unsigned int dwValue = 0;

	if(pHash == NULL || lpHashBuffer == NULL || dwMaxLen<65)
		return;

	memset(lpHashBuffer, 0, dwMaxLen);
	for(INT32 i = 0 ; i < 8; i++)
	{
		memcpy(&dwValue, pHash + (i * 4), 4);
		snprintf(pszCellHash, sizeof(pszCellHash)-1, "%08X", dwValue);
		pszCellHash[8] = 0;
		if(dwMaxLen > strlen(lpHashBuffer)+8)
		{
			strncat(lpHashBuffer, pszCellHash, 8);
		}
	}
}
//-----------------------------------------------------------------------------------------------

