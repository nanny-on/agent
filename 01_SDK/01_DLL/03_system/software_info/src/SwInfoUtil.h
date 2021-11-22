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
 */
#if !defined(AFX_SW_INFO_UTIL_H__FF96934E_ADEF_46DF_BF23_3F785C848B16__INCLUDED_)
#define AFX_SW_INFO_UTIL_H__FF96934E_ADEF_46DF_BF23_3F785C848B16__INCLUDED_

class CSwInfoUtil  
{
public:
	CSwInfoUtil();
	virtual ~CSwInfoUtil();

public:
	INT32	GetSwList(INT32 nProcArchitect, TListSwInfo& tSwInfoList);

private:
	INT32	ParseSwList(char *acBuff, char cSep, PASISI_SW_INFO pSwInfo);
	INT32	IsSwList(TListSwInfo tSwInfoList, PASISI_SW_INFO pSwInfo);
};

#endif // !defined(AFX_SW_INFO_UTIL_H__FF96934E_ADEF_46DF_BF23_3F785C848B16__INCLUDED_)
