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


#ifndef MatchFilterUtilH
#define MatchFilterUtilH
//---------------------------------------------------------------------------

class CMatchFilterUtil
{

private:
	INT32		IsMatchFilter(String& strTarget, String& strSpec, UINT32 nChkType, UINT32& nMatchFind, CHAR chTok = CHAR_TOKEN_F);

public:
	INT32		IsMatchFilter(DB_FILE_INFO& tSrcDFI, DB_FILE_INFO& tTarDFI, UINT32 nFltChkType, CHAR chTok = CHAR_TOKEN_F);

public:
	INT32		IsMatchFilter(PDB_SITE_FILE& pdata, DB_FILE_INFO& tSrcDFI, DB_FILE_INFO& tTarDFI, UINT32 nFltChkType, CHAR chTok = CHAR_TOKEN_F);
	INT32		IsMatchFilter(PDB_SITE_FILE& pdata, DB_FILE_INFO& tTarDFI, UINT32 nFltChkType, CHAR chTok = CHAR_TOKEN_F);

public:
	CMatchFilterUtil();
    ~CMatchFilterUtil();
};

extern CMatchFilterUtil*		t_MatchFilterUtil;


#endif
