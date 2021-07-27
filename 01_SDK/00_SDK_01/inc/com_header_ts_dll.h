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



#ifndef     COM_HEADER_TS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_TS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
typedef struct _asi_ts_init
{
	_asi_ts_init()
	{
		nRemainLog = 1;
		ZeroMemory(szLogFilePath, CHAR_MAX_SIZE);
		ZeroMemory(szLogFileName, CHAR_MAX_SIZE);
	}
	UINT32	nRemainLog;
	CHAR	szLogFilePath[CHAR_MAX_SIZE];
	CHAR	szLogFileName[CHAR_MAX_SIZE];
}ASI_TS_INIT, *PASI_TS_INIT;

//------------------------------------------------------------------------------

typedef struct _asi_ts_info
{
	_asi_ts_info()
	{
		ZeroMemory(szTaskName, CHAR_MAX_SIZE);
		ZeroMemory(szProgram, CHAR_MAX_SIZE);
		ZeroMemory(szProParam, CHAR_MAX_SIZE);
		ZeroMemory(szAcntName, CHAR_MAX_SIZE);
		ZeroMemory(szAcntPassword, CHAR_MAX_SIZE);

		nStartTime	= 0;
		nEndTime	= 0;
		nChkPeriod	= 0;
		nMultipleInst = 2;
	}
	CHAR	szTaskName[CHAR_MAX_SIZE];
	CHAR	szProgram[CHAR_MAX_SIZE];
	CHAR	szProParam[CHAR_MAX_SIZE];
	CHAR	szAcntName[CHAR_MAX_SIZE];
	CHAR	szAcntPassword[CHAR_MAX_SIZE];
	UINT32	nStartTime;
	UINT32	nEndTime;
	UINT32	nChkPeriod;
	UINT32	nMultipleInst;

}ASI_TS_INFO, *PASI_TS_INFO;
//--------------------------------------------------------------------------

#define ASI_TS_CHECK_PREIOD_TYPE_ONCE				0
#define ASI_TS_CHECK_PREIOD_TYPE_DAILY				1
#define ASI_TS_CHECK_PREIOD_TYPE_WEEKLY				2
#define ASI_TS_CHECK_PREIOD_TYPE_MONTHLY			3
#define ASI_TS_CHECK_PREIOD_TYPE_SYSTEM_BOOT		4
#define ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN			5

typedef enum _ASI_TS_MULTIPLE_INST_TYPE
{
	ASI_TS_MULTIPLE_INST_TYPE_PARALLEL = 0,
	ASI_TS_MULTIPLE_INST_TYPE_QUEUE,
	ASI_TS_MULTIPLE_INST_TYPE_IGNORE_NEW,
	ASI_TS_MULTIPLE_INST_TYPE_STOP_EXISTING,
}ASI_TS_MULTIPLE_INST_TYPE;

typedef INT32 (*CallBack_TaskInfo)(PVOID lParam, LPCTSTR);

typedef	INT32 (*ASITS_InitType)(PVOID* lpObject, PASI_TS_INIT psc_ts_init, UINT32 nSize);
typedef	INT32 (*ASITS_FreeType)(PVOID lpObject);
//--------------------------------------------------------------------------

typedef INT32 (*ASITS_AddTaskInfoType)(PVOID lpObject, PASI_TS_INFO pasi_ts_info, UINT32 nSize);
typedef INT32 (*ASITS_DelTaskInfoType)(PVOID lpObject, LPCTSTR lpTaskName);
typedef INT32 (*ASITS_IsExistTaskInfoType)(PVOID lpObject, LPCTSTR lpTaskName, PUINT32 pnExistMode);
typedef INT32 (*ASITS_GetTaskExecuteProcIDType)(PVOID lpObject, LPCTSTR lpszTaskTitle, PUINT32 pnProcID);
typedef	INT32 (*ASITS_GetTaskListType)(PVOID lpObject, LPCTSTR lpszTaskName, PVOID lParam, PVOID fnCallBack);
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


#endif      //COM_HEADER_TS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
