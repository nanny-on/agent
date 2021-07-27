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

#ifndef _ASI_DBF_DLL_MEMBER_H__

#define _ASI_DBF_DLL_MEMBER_H__

#include "com_struct_dbf_header_user.h"

class CASIDBFDLLMember
{

public:
	CString				m_strTbName;

public:
	DBF_ITEM_RTN			m_tDFIR;
	DBF_TABLE_COLUMN_RTN	m_tDFTCR;
	UINT64					m_nCurFP;

public:
	CASIDBFDLLMember(void);
	~CASIDBFDLLMember(void);
};

typedef map<UINT64, CASIDBFDLLMember*>		TMapASIDBFDLLMember;
typedef TMapASIDBFDLLMember::iterator		TMapASIDBFDLLMemberItor;

#endif /*_ASI_DBF_DLL_MEMBER_H__*/