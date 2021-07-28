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

#include "stdafx.h"
#include "com_struct.h"
#include "ComFuncUtil.h"

CComFuncUtil*		t_ComFuncUtil = NULL;

CComFuncUtil::CComFuncUtil(void)
{
	m_strUtilName = "util com func";
}
//---------------------------------------------------------------------------

CComFuncUtil::~CComFuncUtil(void)
{
}
//---------------------------------------------------------------------------

String		CComFuncUtil::GetNetDrvList()
{
	String strRtn = "";
	return strRtn;
}
//---------------------------------------------------------------------------

INT32		CComFuncUtil::ConvertEnvPathToLocalPath(String strInPath, String& strOutPath)
{
	CFileUtil tFileUtil;
	TListStr tCPList;
	TListStrItor cp_begin, cp_end;
	String strPath;
	char chTok = GetToken(strInPath, ';', ',');
	CTokenString Token(strInPath.c_str(), strInPath.length(), chTok);
	while(Token.IsNextToken())
	{
		strPath = Token.NextToken();

		if(tFileUtil.GetEnvPathToLocalPath(strPath, &tCPList) != 0)
			return -1;

		cp_begin = tCPList.begin();	cp_end = tCPList.end();
		for(cp_begin; cp_begin != cp_end; cp_begin++)
		{
			strOutPath += *cp_begin;
			strOutPath += ";";
		}
		tCPList.clear();
	}

	return 0;
}
//---------------------------------------------------------------------------
