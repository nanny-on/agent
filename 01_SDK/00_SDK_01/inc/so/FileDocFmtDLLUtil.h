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


#if !defined(AFX_ASIFDFDLLUTIL_H__BD7746C2_F7BE_48A8_B748_D8FB1E47D06D__INCLUDED_)
#define AFX_ASIFDFDLLUTIL_H__BD7746C2_F7BE_48A8_B748_D8FB1E47D06D__INCLUDED_

//------------------------------------------------------
#include "com_header_file_doc_fmt_dll.h"
//------------------------------------------------------

class CASIFDFDLLUtil
{
private:
	HINSTANCE				m_hDll;

public:
	INT32					LoadLibraryExt(LPCTSTR lpDLLPath);
	INT32					FreeLibraryExt();

public:
	INT32					ASIDFF_SetDFFmtInit(PASI_DFILE_FMT_INIT pADFFI);
	INT32					ASIDFF_GetDFFmtInfo(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);
	
public:
	CASIFDFDLLUtil();
	virtual ~CASIFDFDLLUtil();

};

extern CASIFDFDLLUtil*	t_ASIFDFDLLUtil;

#endif // !defined(AFX_ASIFDFDLLUTILL_H__BD7746C2_F7BE_48A8_B748_D8FB1E47D06D__INCLUDED_)
