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
	m_nAreaType			= SGASC_SECURITY_AREA_TYPE_WINDOWS;
	m_nEncryptType		= SGASC_SECURITY_ENC_TYPE_STRING;
	m_nSessionKey		= 0;
}
//-----------------------------------------------------------------------------------------------

CSecurityUtil::~CSecurityUtil()
{

}	
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::SetMode(UINT32 nAreaType, UINT32 nEncryptType)
{
	m_nAreaType		= nAreaType;
	m_nEncryptType	= nEncryptType;

	if(m_nSessionKey)
	{
		switch(m_nEncryptType)
		{
			case SGASC_SECURITY_ENC_TYPE_SEED:
			case SGASC_SECURITY_ENC_TYPE_TLS1:
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
	//m_nSessionKey	= htonl(nSKey);

	if(m_nSessionKey)
	{
		switch(m_nEncryptType)
		{
			case SGASC_SECURITY_ENC_TYPE_SEED:
			case SGASC_SECURITY_ENC_TYPE_TLS1:
			{
				m_tSeedUtil.Set_Key(PBYTE(&m_nSessionKey), sizeof(m_nSessionKey));

				//PrintSeedRoundKey();
				break;
			}
		}
	}	
	
	return;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::PrintSeedRoundKey()
{
	CString strRKey;

	PDWORD pdwRKey = m_tSeedUtil.Get_RoundKey();
	for(INT32 i = 0; i < 32; i++)
	{
		DWORD dwKey = pdwRKey[i];
		if(!dwKey)		continue;	
		
		CString strTmp;
		strTmp.Format("%.8X", dwKey);
		strRKey += strTmp;
	}
	
	WriteLogN("%s", (LPCTSTR)strRKey);
	return;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::Encrypt(UINT8* lpPlanText, UINT32 nPlanLen, UINT8** lpEncText, UINT32& nEncLen)
{
	if(!lpPlanText || !nPlanLen)	return;
	
	switch(m_nEncryptType)
	{
		case SGASC_SECURITY_ENC_TYPE_STRING:
		{
			if(!m_nSessionKey)	break;
			
			if(m_nAreaType == SGASC_SECURITY_AREA_TYPE_WINDOWS)
				m_tStrEncrypt.ScrembleEncrypt_Win(m_nSessionKey, lpPlanText, nPlanLen);
			else	
				m_tStrEncrypt.ScrembleEncrypt(m_nSessionKey, lpPlanText, nPlanLen);
			
			*lpEncText = lpPlanText;
			nEncLen = nPlanLen;
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)
			{
				memcpy(*lpEncText, lpPlanText, nPlanLen);
				break;
			}

			m_tSeedUtil.Encrypt_Data(lpPlanText, nPlanLen, lpEncText, nEncLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_TLS1:
		{			
			if(!m_nSessionKey)
			{
				memcpy(*lpEncText, lpPlanText, nPlanLen);
				break;
			}
			
			m_tSeedUtil.Encrypt_Data(lpPlanText, nPlanLen, lpEncText, nEncLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_NONE:
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
		case SGASC_SECURITY_ENC_TYPE_STRING:
		{
			nOutLen = nPlanLen;
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)	nOutLen = nPlanLen;
			else				nOutLen = m_tSeedUtil.Encrypt_Len(nPlanLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_TLS1:
		{
			if(!m_nSessionKey)	nOutLen = nPlanLen;
			else				nOutLen = m_tSeedUtil.Encrypt_Len(nPlanLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_NONE:
		default:
		{
			nOutLen = nPlanLen;
			break;
		}
	}
	return nOutLen;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::Decrypt(UINT8* lpEncText, UINT32 nEncLen, UINT8** lpPlanText, UINT32& nPlanLen)
{
	if(!lpEncText || !nEncLen)	return;
	
	switch(m_nEncryptType)
	{
		case SGASC_SECURITY_ENC_TYPE_STRING:
		{
			if(!m_nSessionKey)		break;

			if(m_nAreaType == SGASC_SECURITY_AREA_TYPE_WINDOWS)
				m_tStrEncrypt.ScrembleDecrypt_Win(m_nSessionKey, lpEncText, nEncLen);
			else	
				m_tStrEncrypt.ScrembleDecrypt(m_nSessionKey, lpEncText, nEncLen);
			
			*lpPlanText = lpEncText;
			nPlanLen = nEncLen;
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_SEED:
		{
			if(!m_nSessionKey)
			{
				*lpPlanText = (BYTE*)malloc(nEncLen + 1);
				memset(*lpPlanText, 0, nEncLen + 1);
				memcpy(*lpPlanText, lpEncText, nEncLen);
				nPlanLen = nEncLen;
				break;
			}
			
			m_tSeedUtil.Decrypt_Data(lpEncText, lpPlanText, nPlanLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_TLS1:
		{
			if(!m_nSessionKey)
			{
				*lpPlanText = (BYTE*)malloc(nEncLen + 1);
				memset(*lpPlanText, 0, nEncLen + 1);
				memcpy(*lpPlanText, lpEncText, nEncLen);
				nPlanLen = nEncLen;
				break;
			}
			
			m_tSeedUtil.Decrypt_Data(lpEncText, lpPlanText, nPlanLen);
			break;
		}
		case SGASC_SECURITY_ENC_TYPE_NONE:
		default:
		{
			*lpPlanText = lpEncText;
			nPlanLen = nEncLen;
			break;
		}
	}
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::GetSha256(UINT8* lpSrc, UINT32 nSrcLen, UINT8* lpDst)
{
	m_tSha2Util.SHA256Reset();
	m_tSha2Util.SHA256Input((LPCTSTR)lpSrc, nSrcLen);
	m_tSha2Util.SHA256ResultByte(lpDst);
	return;
}
//-----------------------------------------------------------------------------------------------

INT32	CSecurityUtil::CompareSha256(UINT8* lpSrc, UINT32 nSrcLen, UINT8* lpChkSHA)
{
	memset(m_pDstHash, 0, 32);
		
	if(nSrcLen)	GetSha256(lpSrc, nSrcLen, m_pDstHash);
	
	INT32 nIndex = 32;
	while(nIndex)
	{
		nIndex--;
		if(m_pDstHash[nIndex] == lpChkSHA[nIndex])	continue;
		return 1;
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------

INT32	CSecurityUtil::CompareSha256(UINT8* lpSrcSHA, UINT8* lpChkSHA)
{
	INT32 nIndex = 32;
	while(nIndex)
	{
		nIndex--;
		if(lpSrcSHA[nIndex] == lpChkSHA[nIndex])	continue;
		return 1;
	}
	return 0;
}

void	CSecurityUtil::ConvertSha256Last(LPTSTR lpHashBuffer)
{
	ConvertSha256(m_pDstHash, lpHashBuffer);
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::ConvertSha256(UINT8* pHash, LPTSTR lpHashBuffer)
{
	CHAR pszCellHash[255];

	for(INT32 i = 0 ; i < 8; i++)
	{
		DWORD dwValue;
		memcpy(&dwValue, pHash + (i * 4), 4);
		sprintf(pszCellHash, "%08X", dwValue);
		strcat(lpHashBuffer, pszCellHash);
	}
	return;
}
//-----------------------------------------------------------------------------------------------

void	CSecurityUtil::WriteLogN(char* fmt,...)
{
	FILE *fp;
	va_list args;
	char	lpSaveFile[TCHAR_MAX_SIZE] = {0, };
	
	snprintf(lpSaveFile, TCHAR_MAX_SIZE-1, "./log/sock_security_util.txt");
	
	if((fp =fopen(lpSaveFile, "a")) !=NULL)
	{
		char lpLogBuf[TCHAR_MAX_SIZE] = {0, };
		char lpBuf[TCHAR_MAX_SIZE] = {0, };
		
		va_start(args,fmt);
		vsnprintf(lpBuf, TCHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		
		snprintf(lpLogBuf, TCHAR_MAX_SIZE-1, "%s\r\n", lpBuf);
		
		fprintf(fp, lpLogBuf);
		fclose(fp);
	}
}
//-----------------------------------------------------------------------------------------------

