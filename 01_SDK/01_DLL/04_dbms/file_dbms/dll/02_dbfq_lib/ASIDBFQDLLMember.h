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

#pragma once

#include "com_header_dbf_user_dll.h"

class CASIDBFQDLLMember
{

public:
	TMapStrID64				m_tDBObjMap;

public:
	DBF_ITEM_RTN			m_tDFIR;
	UINT64					m_nCurFP;
	UINT64					m_nLastID;
	UINT64					m_nLastFp;

public:
	LPTSTR					m_lpTableName;
	CString					m_strCurTableName;

public:
	CASIDBFQDLLMember(void);
	~CASIDBFQDLLMember(void);
};

typedef map<UINT64, CASIDBFQDLLMember*>		TMapASIDBFQDLLMember;
typedef TMapASIDBFQDLLMember::iterator		TMapASIDBFQDLLMemberItor;
