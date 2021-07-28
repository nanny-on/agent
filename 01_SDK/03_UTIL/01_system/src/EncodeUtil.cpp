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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "EncodeUtil.h"

//---------------------------------------------------------------------------

CEncodeUtil::CEncodeUtil()
{

}
//---------------------------------------------------------------------------

CEncodeUtil::~CEncodeUtil()
{
}
//---------------------------------------------------------------------------

String		CEncodeUtil::EncBase(PBYTE pValue, INT32 nValLen)
{
	String strRtn = "";
	INT32 nOutLen = 0;
	PCHAR pOutBuf = NULL;

	nOutLen = ((nValLen)*4) + 4;
	pOutBuf = (PCHAR)malloc(nOutLen + 1);
	if(pOutBuf != NULL)
	{
		memset(pOutBuf, 0, nOutLen + 1);	
		base32_encode(pValue, nValLen, (PBYTE)pOutBuf, nOutLen);
		strRtn = pOutBuf;
		free(pOutBuf);
	}
	return strRtn;
}
//---------------------------------------------------------------------------

void		CEncodeUtil::DecBase(String strValue, PBYTE pValue, INT32& nValLen)
{
	base32_decode((PBYTE)strValue.c_str(), pValue, nValLen);
}
//---------------------------------------------------------------------------

UINT32		CEncodeUtil::EncBase_U32(UINT32	nValue)
{
	UINT32 nRtn = 0;
	
    ((BYTE*)&nRtn)[0] = nValue >> 16;
    ((BYTE*)&nRtn)[1] = nValue >> 24;
    ((BYTE*)&nRtn)[2] = nValue >> 8;
    ((BYTE*)&nRtn)[3] = nValue;
	
    return nRtn;	
}
//--------------------------------------------------------------------

UINT32		CEncodeUtil::DecBase_U32(UINT32	nValue)
{
	UINT32 nRtn = 0;
	
    ((BYTE*)&nRtn)[0] = nValue >> 24;
    ((BYTE*)&nRtn)[1] = nValue >> 16;
    ((BYTE*)&nRtn)[2] = nValue;
    ((BYTE*)&nRtn)[3] = nValue >> 8;
	
    return nRtn;
}
//--------------------------------------------------------------------



