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

#ifndef MEM_VULN_RST_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_VULN_RST_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_vuln_patch
{
	_mem_vuln_patch()
	{
		nID				= 0;
	}

	void _set_int(UINT32 nKey, UINT32 nValue)
	{
		tIntMap[nKey] = nValue;
	}

	INT32 _get_int(UINT32 nKey)
	{
		TMapIDItor find = tIntMap.find(nKey);
		if(find == tIntMap.end())	return -1;

		return find->second;
	}

	void _set_str(UINT32 nKey, String strValue)
	{
		tStrMap[nKey] = strValue;
	}

	String _get_str(UINT32 nKey)
	{
		TMapIDStrItor find = tStrMap.find(nKey);
		if(find == tStrMap.end())	return "";

		return find->second;
	}

	UINT32		nID;
	TMapID		tIntMap;
	TMapIDStr	tStrMap;

}MEM_VULN_RST, *PMEM_VULN_RST;

typedef list<MEM_VULN_RST>				TListMemVulnRst;
typedef TListMemVulnRst::iterator		TListMemVulnRstItor;

typedef map<UINT32, MEM_VULN_RST>		TMapMemVulnRst;
typedef TMapMemVulnRst::iterator		TMapMemVulnRstItor;

#endif //MEM_VULN_RST_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



