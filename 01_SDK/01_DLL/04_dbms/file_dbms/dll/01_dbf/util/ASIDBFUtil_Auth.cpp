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
#include "ASIDBFUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::LoginDBF(CString strID, CString strPw)
{
	switch(m_tDB.tAuth.nAuthChkType)
	{
		case ASI_DBF_AUTH_INFO_TYPE_NONE:	m_nAuthResult = 1;		break;
		case ASI_DBF_AUTH_INFO_TYPE_SHA2:
		{
			if(strID.IsEmpty() || strPw.IsEmpty())
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			}
			
			UINT8	pIDBuff[32] = {0, };
			UINT8	pPwBuff[32] = {0, };

			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strID, strID.GetLength(), pIDBuff);
			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strPw, strPw.GetLength(), pPwBuff);

			if(m_tSecurityUtil.CompareSha256(pIDBuff, (m_tDB.tAuth.szUserID)))
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			}
			
			if(m_tSecurityUtil.CompareSha256(pPwBuff, (m_tDB.tAuth.szUserPW)))
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			}

			m_nAuthResult = 1;
			break;
		}
		default:
		{
			return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
		}
	}
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::ChangeAuthInfoDBF(CString strOldID, CString strOldPw, CString strNewID, CString strNewPw, UINT32 nChkNewType)
{
	switch(m_tDB.tAuth.nAuthChkType)
	{
		case ASI_DBF_AUTH_INFO_TYPE_NONE:		break;
		case ASI_DBF_AUTH_INFO_TYPE_SHA2:
		{
			UINT8	pIDBuff[32] = {0, };
			UINT8	pPwBuff[32] = {0, };

			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strOldID, strOldID.GetLength(), pIDBuff);
			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strOldPw, strOldPw.GetLength(), pPwBuff);

			if(m_tSecurityUtil.CompareSha256(pIDBuff, (m_tDB.tAuth.szUserID)))
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			}

			if(m_tSecurityUtil.CompareSha256(pPwBuff, (m_tDB.tAuth.szUserPW)))
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			}
			break;
		}
		default:
		{
			return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;
			break;
		}
	}

	switch(nChkNewType)
	{
		case ASI_DBF_AUTH_INFO_TYPE_NONE:
		{
			if(strNewID.IsEmpty() == FALSE || strNewPw.IsEmpty() == FALSE)
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
			}

			DBF_DATABASE tDB = m_tDB;
			memset(tDB.tAuth.szUserID, 0, ASI_DBF_AUTH_USER_ID_MAX_LEN);
			memset(tDB.tAuth.szUserPW, 0, ASI_DBF_AUTH_USER_PW_MAX_LEN);
			tDB.tAuth.nAuthChkType = nChkNewType;


			if(SetER(WriteDBF_DI(&tDB)))
			{
				WriteLogE("write dbf database header fail : [%.8x]", GetER());
				return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
			}

			if(SetER(ReadDBF_DI(&m_tDB)))
			{
				WriteLogE("read dbf database header fail : [%.8x]", GetER());
				return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
			}
			break;
		}
		case ASI_DBF_AUTH_INFO_TYPE_SHA2:
		{
			if(strNewID.IsEmpty() || strNewPw.IsEmpty())
			{
				return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
			}

			DBF_DATABASE tDB = m_tDB;
			memset(tDB.tAuth.szUserID, 0, ASI_DBF_AUTH_USER_ID_MAX_LEN);
			memset(tDB.tAuth.szUserPW, 0, ASI_DBF_AUTH_USER_PW_MAX_LEN);

			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strNewID, strNewID.GetLength(), tDB.tAuth.szUserID);
			m_tSecurityUtil.GetSha256((PUINT8)(LPCTSTR)strNewPw, strNewPw.GetLength(), tDB.tAuth.szUserPW);
			tDB.tAuth.nAuthChkType = nChkNewType;

			if(SetER(WriteDBF_DI(&tDB)))
			{
				WriteLogE("write dbf database header fail : [%.8x]", GetER());
				return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
			}

			if(SetER(ReadDBF_DI(&m_tDB)))
			{
				WriteLogE("read dbf database header fail : [%.8x]", GetER());
				return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
			}
			break;
		}
		default:
		{
			WriteLogE("not define auth info check type : [%d]", nChkNewType);
			return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_INFO_TYPE;
		}
	}
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::CopyAuthInfoDBF(PDBF_DATABASE_AUTH pDDA)
{
	DBF_DATABASE tDB = m_tDB;
	memcpy(&(tDB.tAuth), pDDA, sizeof(tDB.tAuth));

	if(SetER(WriteDBF_DI(&tDB)))
	{
		WriteLogE("copy write dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
	}

	if(SetER(ReadDBF_DI(&m_tDB)))
	{
		WriteLogE("copy read dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
	}
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::IsInvalidAuthRight()
{
	return (m_nAuthResult ? 0 : 1);
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::SetTItemEncTypeDBF(UINT32 nDefEncType)
{
	if(IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

	DBF_DATABASE tDB = m_tDB;
	tDB.tAuth.nDefEncType	= nDefEncType;

	if(SetER(WriteDBF_DI(&tDB)))
	{
		WriteLogE("write dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
	}

	if(SetER(ReadDBF_DI(&m_tDB)))
	{
		WriteLogE("read dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
	}
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::SetTItemCompTypeDBF(UINT32 nDefCompType)
{
	if(IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

	DBF_DATABASE tDB = m_tDB;
	tDB.tAuth.nDefCompType	= nDefCompType;

	if(SetER(WriteDBF_DI(&tDB)))
	{
		WriteLogE("write dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
	}

	if(SetER(ReadDBF_DI(&m_tDB)))
	{
		WriteLogE("read dbf database header fail : [%.8x]", GetER());
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
	}
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::GetTItemEncTypeDBF(PUINT32 pnDefEncType)
{
	if(IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

	*pnDefEncType = m_tDB.tAuth.nDefEncType; 
	return 0;
}
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::GetTItemCompTypeDBF(PUINT32 pnDefCompType)
{
	if(IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

	*pnDefCompType = m_tDB.tAuth.nDefCompType;
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

UINT32			CASIDBFUtil::GetSessionKey()
{
	UINT16	lpSeedKey[16] = {0xFA, 0x12, 0x36, 0x79, 0xEA, 0x1B, 0x9C, 0x1F, 0x9A, 0x46, 0x93, 0x28, 0x59, 0x37, 0x26, 0x75};

	UINT32	nKey = 0;

	nKey += lpSeedKey[0];
	nKey += lpSeedKey[1];
	nKey += lpSeedKey[0];
	nKey *= lpSeedKey[4];
	nKey += lpSeedKey[2];
	nKey += lpSeedKey[4];
	nKey *= lpSeedKey[0];
	nKey += lpSeedKey[6];
	nKey *= lpSeedKey[6];
	nKey += lpSeedKey[6];

	return nKey;
}
//---------------------------------------------------------------------

INT32			CASIDBFUtil::ChgTableItemEnc(PDBF_TABLE_ITEM_HEADER pdtih, PBYTE lpBuff, PBYTE* lpEncBuff)
{
	INT32 nRtn = 0;
	switch(m_tDB.tAuth.nDefEncType)	
	{
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_NONE:
		{
			pdtih->nEncType = m_tDB.tAuth.nDefEncType;
			break;
		}
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_SEED128:
		{
			UINT32 nEncLen = 0;

			m_tSecurityUtil.SetSessionKey(GetSessionKey());
			m_tSecurityUtil.SetMode(SGASC_SECURITY_AREA_TYPE_WINDOWS_UNIX, SGASC_SECURITY_ENC_TYPE_SEED);
			m_tSecurityUtil.Encrypt(lpBuff, pdtih->nCurBlockSize, lpEncBuff, nEncLen);

			pdtih->nCurBlockSize = nEncLen;
			pdtih->nEncType = m_tDB.tAuth.nDefEncType;
			break;
		}
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_BASE32:
		{
			UINT32 nEncLen = 0;

			m_tSecurityUtil.SetSessionKey(GetSessionKey());
			m_tSecurityUtil.SetMode(SGASC_SECURITY_AREA_TYPE_WINDOWS_UNIX, SGASC_SECURITY_ENC_TYPE_STRING);
			m_tSecurityUtil.Encrypt(lpBuff, pdtih->nCurBlockSize, lpEncBuff, nEncLen);

			pdtih->nCurBlockSize = nEncLen;
			pdtih->nEncType = m_tDB.tAuth.nDefEncType;
			break;
		}
		default:
		{
			WriteLogE("invalid table item encrypt type : [%d]", m_tDB.tAuth.nDefEncType);
			nRtn = -1;
			break;
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------

INT32			CASIDBFUtil::ChgTableItemDec(PDBF_TABLE_ITEM_HEADER pdtih, PBYTE* lpBuff)
{
	INT32 nRtn = 0;
	switch(pdtih->nEncType)	
	{
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_NONE:
		{
			break;
		}
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_SEED128:
		{
			PUINT8 lpDecBuff = NULL;
			UINT32 nDecLen = 0;

			m_tSecurityUtil.SetSessionKey(GetSessionKey());
			m_tSecurityUtil.SetMode(SGASC_SECURITY_AREA_TYPE_WINDOWS_UNIX, SGASC_SECURITY_ENC_TYPE_SEED);
			m_tSecurityUtil.Decrypt(*lpBuff, pdtih->nCurBlockSize, &lpDecBuff, nDecLen);
			pdtih->nCurBlockSize = nDecLen;

			FreeCommonBuff(lpBuff);
			*lpBuff = lpDecBuff;
			break;
		}
		case ASI_DBF_TABLE_ITEM_ENC_TYPE_BASE32:
		{
			PUINT8 lpDecBuff = NULL;
			UINT32 nDecLen = 0;

			m_tSecurityUtil.SetSessionKey(GetSessionKey());
			m_tSecurityUtil.SetMode(SGASC_SECURITY_AREA_TYPE_WINDOWS_UNIX, SGASC_SECURITY_ENC_TYPE_STRING);
			m_tSecurityUtil.Decrypt(*lpBuff, pdtih->nCurBlockSize, &lpDecBuff, nDecLen);
			pdtih->nCurBlockSize = nDecLen;

			FreeCommonBuff(lpBuff);
			*lpBuff = lpDecBuff;
			break;
		}
		default:
		{
			WriteLogE("invalid table item encrypt type : [%d]", pdtih->nEncType);
			nRtn = -1;
			break;
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------




