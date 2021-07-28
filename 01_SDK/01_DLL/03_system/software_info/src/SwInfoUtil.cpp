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
#include "stdafx.h"
#include "com_struct.h"
#include "SwInfoUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSwInfoUtil::CSwInfoUtil()
{	
}

CSwInfoUtil::~CSwInfoUtil()
{	
}


INT32 CSwInfoUtil::ParseSwList(char *acBuff, char cSep, PASISI_SW_INFO pSwInfo)
{
	char szToken[CHAR_MAX_SIZE] = {0,};
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char *pcList = NULL;

	if(acBuff == NULL || acBuff[0] == 0 || cSep == 0 || pSwInfo == NULL)
		return -1;

	memset(pSwInfo, 0, sizeof(ASISI_SW_INFO));

	pcList = acBuff;

	if ((pcList = copy_token(szToken, pcList, cSep)) == NULL)
	{
		return -2;
	}

	if(szToken[0] != 0)
	{
		strncpy(pSwInfo->szName, szToken, MAX_PATH-1);
		strncpy(pSwInfo->szChkKey, szToken, MAX_PATH-1);
	}

	if ((pcList = copy_token(szToken, pcList, cSep)) == NULL)
	{
		return -3;
	}

	if(szToken[0] != 0)
		strncpy(pSwInfo->szVersion, szToken, MAX_PATH-1);

	copy_token(szToken, pcList, cSep);

	if(szToken[0] != 0)
		strncpy(pSwInfo->szPublisher, szToken, MAX_PATH-1);

	return 0;
}

INT32	CSwInfoUtil::IsSwList(TListSwInfo tSwInfoList, PASISI_SW_INFO pSwInfo)
{
	TListSwInfoItor begin, end;

	if(pSwInfo == NULL)
		return 0;

	if(tSwInfoList.size() < 1)
		return 0;

	begin = tSwInfoList.begin();	end = tSwInfoList.end();
	for(begin; begin != end; begin++)
	{
		if(!_stricmp(begin->szName, pSwInfo->szName) && !_stricmp(begin->szVersion, pSwInfo->szVersion))
		{
			return 1;
		}
	}
	return 0;
}


INT32	CSwInfoUtil::GetSwList(INT32 nProcArchitect, TListSwInfo& tSwInfoList)
{	
	FILE* pPipe = NULL;
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acBuff[CHAR_MAX_SIZE] = {0,};
	INT32 nRetVal = 0;
	PASISI_SW_INFO pSwInfo = NULL;

	GetCurrentDateTime(2, acTimeBuf);

	nRetVal = is_file("/usr/bin/dpkg-query");
	if(nRetVal != 0)
		return -1;

	pSwInfo = (PASISI_SW_INFO)malloc(sizeof(ASISI_SW_INFO));
	if(pSwInfo == NULL)
	{
		return -2;
	}

	pPipe = popen("/usr/bin/dpkg-query -W -f='${Source};${Version};${Homepage}\n'", "r");
	if(pPipe == NULL)
	{
		safe_free(pSwInfo);
		return -3;
	}

	while (fgets(acBuff, CHAR_MAX_SIZE-1, pPipe))
	{
		nRetVal = ParseSwList(acBuff, ';', pSwInfo);
		if(nRetVal != 0)
			continue;

		if(pSwInfo->szName[0] == 0)
			continue;

		nRetVal = IsSwList(tSwInfoList, pSwInfo);
		if(nRetVal == 1)
			continue;

		pSwInfo->nProcArchitect = nProcArchitect;
		strncpy(pSwInfo->szInsDate, acTimeBuf, MAX_PATH-1);
		tSwInfoList.push_back(*pSwInfo);
	}
	pclose(pPipe);
	safe_free(pSwInfo);

	return 0;
}
