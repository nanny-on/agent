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

// SHA2Util.h: interface for the CSHA2Util class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHA2UTIL_H__883ED082_6019_46FA_81F7_5CCE6EB1D4A6__INCLUDED_)
#define AFX_SHA2UTIL_H__883ED082_6019_46FA_81F7_5CCE6EB1D4A6__INCLUDED_

#include "SHA256.h"

class CSHA2Util  
{

public:
	SHA256Context	tSha256Context;

public:		
	void	SHA256Reset();
	void	SHA256Input(LPCTSTR, unsigned);
	int		SHA256Result(LPSTR lpBuffer, DWORD dwLegth);
	int		SHA256ResultByte(PBYTE lpHash);

private:
	void	SHA256ProcessMessageBlock();
	void	SHA256PadMessage();

public:
	CSHA2Util();
	virtual ~CSHA2Util();

};

#endif // !defined(AFX_SHA2UTIL_H__883ED082_6019_46FA_81F7_5CCE6EB1D4A6__INCLUDED_)
