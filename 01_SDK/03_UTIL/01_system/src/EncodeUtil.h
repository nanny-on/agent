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

#ifndef _ENCODE_UTIL_H__
#define _ENCODE_UTIL_H__
//---------------------------------------------------------------------------

class CEncodeUtil
{

public:
	String		EncBase(PBYTE pValue, INT32 nValLen);
	void		DecBase(String strValue, PBYTE pValue, INT32& nValLen);
	UINT32		EncBase_U32(UINT32	nValue);
	UINT32		DecBase_U32(UINT32	nValue);

public:
	CEncodeUtil();
    ~CEncodeUtil();
};

#endif /*_ENCODE_UTIL_H__*/