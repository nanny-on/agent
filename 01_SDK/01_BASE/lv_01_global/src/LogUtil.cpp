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


#include "stdafx.h"
#include "com_struct.h"
#include "LogUtil.h"


CMutexExt	g_MutexLog;
char		g_acLogFilePath[MAX_PATH] = {0, };
char		g_acLogFileName[MAX_PATH] = {0, };
UINT32		g_nRemainLog = 1;
UINT32		g_nFileLogRetention = 5;
UINT32		g_nLogSKey = 0;

void	WriteLogN(char* fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	g_MutexLog.Lock();
	do{
		if(!g_nRemainLog)
			break;
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		if(g_acLogFileName[0] == 0)
			strncpy(g_acLogFileName, "/nanny_agt_sys_log_", MAX_PATH-1);
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, g_acLogFileName, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, g_acLogFileName, g_nFileLogRetention);
		}
	
		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}
			
		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[Info]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}


void	WriteLogD(char* fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	g_MutexLog.Lock();
	do{
		if(!g_nRemainLog)
			break;
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		if(g_acLogFileName[0] == 0)
			strncpy(g_acLogFileName, "/nanny_agt_sys_log_", MAX_PATH-1);
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, g_acLogFileName, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, g_acLogFileName, g_nFileLogRetention);
		}
	
		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}
			
		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[Dbg]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void WritePerfTest1Log(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acLogFile[MAX_FILE_NAME] = "/nanny_perf_test1_log_";
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	g_MutexLog.Lock();
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, acLogFile, g_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[TC1]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void WritePerfTest2Log(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acLogFile[MAX_FILE_NAME] = "/nanny_perf_test2_log_";
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	g_MutexLog.Lock();
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, acLogFile, g_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[TC2]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void WritePerfTest3Log(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acLogFile[MAX_FILE_NAME] = "/nanny_perf_test3_log_";
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	g_MutexLog.Lock();
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, acLogFile, g_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[TC3]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void WritePerfTest4Log(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acLogFile[MAX_FILE_NAME] = "/nanny_perf_test4_log_";
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	g_MutexLog.Lock();
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, acLogFile, g_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[TC4]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void WritePerfTest5Log(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acLogFile[MAX_FILE_NAME] = "/nanny_perf_test5_log_";
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };
	g_MutexLog.Lock();
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, acLogFile, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, acLogFile, g_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t[TC5]\t%s\n", acTimeBuf, acLogBuf);
		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}
//-------------------------------------------------------------------------

void	WriteLogE(char* fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char	acSaveFile[MAX_PATH] = {0, };
	char	acTimeBuf[MAX_TIME_STR] = {0, };
	char	acLogBuf[CHAR_MAX_SIZE] = {0, };

	g_MutexLog.Lock();
	
	do{
		if(!g_nRemainLog)
			break;

		if(g_acLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH) != 0)
				break;
			snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}
		if(g_acLogFileName[0] == 0)
			strncpy(g_acLogFileName, "/nanny_agt_sys_log_", MAX_PATH-1);

		GetCurrentDateTime(0, acTimeBuf);

		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, g_acLogFileName, acTimeBuf);

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(g_acLogFilePath, g_acLogFileName, g_nFileLogRetention);
		}

		fp =fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);

		fprintf(fp, "%s\t[Error]\t%s\n", acTimeBuf, acLogBuf);

		fclose(fp);
	}while(FALSE);
	g_MutexLog.UnLock();
}

void	Encrypt_Log(UINT32	key, PBYTE SrcData,	INT32 SrcLen)
{	
	INT32 count = SrcLen / 4,
		remainCount = SrcLen % 4;
	INT32 i ;
	BYTE t ;
	UINT32 data ;
	UINT32 *p    = (UINT32 *)SrcData ;
	BYTE *p2   = (BYTE *)&key ;
	for( i = 0; i < count; i++ )
	{
		data = (p[i] ^ key) ;
		p[i] = ((data >> 24L) & 0x000000FF)
			| ((data << 8L)  & 0xFF000000)
			| ((data << 8L)  & 0x00FF0000)
			| ((data << 8L)  & 0x0000FF00);
	}
	for( i = 0; i < count; i++ )
	{
		data = (p[i] ^ key) ;
		p[i] = ((data >> 16L) & 0x000000FF)
			| ((data >> 16L) & 0x0000FF00)
			| ((data << 16L) & 0x00FF0000)
			| ((data << 16L) & 0xFF000000);
	}
	SrcData += 4*count ;

	for( i = 0; i < remainCount/2; i++ )
	{
		t = SrcData[i] ;

		SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
		SrcData[remainCount-i-1] = t ^ p2[i] ;
	}
	if( remainCount%2 != 0 )
	{
		SrcData[i] = SrcData[i] ^ p2[i] ;
	}
	return;
}
//--------------------------------------------------------------------

void		Decrypt_Log(UINT32	key, PBYTE SrcData,	INT32 SrcLen)
{
	INT32	count = SrcLen / 4,	remainCount = SrcLen % 4;
	INT32	i ;
	BYTE	t ;
	UINT32 data ;
	UINT32 *p    = (UINT32 *)SrcData ;
	BYTE *p2   = (BYTE *)&key ;
	for( i = 0; i < count; i++ )
	{
		data = p[i] ;
		p[i] = ((data << 16L) & 0xFF000000)
			| ((data << 16L) & 0x00FF0000)
			| ((data >> 16L) & 0x0000FF00)
			| ((data >> 16L) & 0x000000FF);
		p[i] = (p[i] ^ key) ;
	}
	for( i = 0; i < count; i++ )
	{
		data = p[i] ;
		p[i] = ((data << 24L) & 0xFF000000)
			| ((data >> 8L)  & 0x00FF0000)
			| ((data >> 8L)  & 0x0000FF00)
			| ((data >> 8L)  & 0x000000FF);
		p[i] = (p[i] ^ key) ;
	}
	SrcData += 4*count ;

	for( i = 0; i < remainCount/2; i++ )
	{
		t = SrcData[i] ;

		SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
		SrcData[remainCount-i-1] = t ^ p2[i] ;
	}
	if( remainCount%2 != 0 )
	{
		SrcData[i] = SrcData[i] ^ p2[i] ;
	}
	return;
}
//-------------------------------------------------------------------- 

void		WriteLog_Enc(LPTSTR lpLog, FILE* fp)
{
	BYTE lpEncBuff[CHAR_MAX_SIZE + 2] = {0, };
	INT16 nEncLen = strlen(lpLog);
	if(nEncLen <= 0)
		return;

	Encrypt_Log(g_nLogSKey, (PBYTE)lpLog, nEncLen);	

	memcpy(lpEncBuff, &nEncLen, sizeof(INT16));
	memcpy(lpEncBuff + 2, lpLog, nEncLen);
	
	{
		UINT32 nWEndLen = 0;
		UINT32 nWToLen = nEncLen + 2;

		while(nWEndLen != nWToLen)
		{
			INT32 nWOneLen = fwrite(lpEncBuff + nWEndLen, sizeof(BYTE), nWToLen-nWEndLen, fp);
			if(nWOneLen < 0)	break;

			nWEndLen += nWOneLen;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------------

void	SetLogSKey(UINT32 nSKey)
{
	g_nLogSKey = nSKey;
	return;
}
//-------------------------------------------------------------------------------------------

void    WriteLogN_Size(UINT32 nSize, LPCTSTR lpFileName, char* fmt,...)
{
/*
	if(!g_nRemainLog)	return;
*/
	
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };

	GetCurrentDateTime(0, acTimeBuf);


	if(g_acLogFilePath[0] == 0)
	{
		char acRootPath[MAX_PATH] = {0, };
		if(get_nanny_agent_root(acRootPath, MAX_PATH) != 0)
			return;
		snprintf(g_acLogFilePath, MAX_PATH-1, "%s/nanny/log", acRootPath);
	}

	snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", g_acLogFilePath, lpFileName, acTimeBuf);

	if(GetFileSizeExt(acSaveFile) > (ASI_MEGABYTE * nSize))
	{
		unlink(acSaveFile);
	}

	g_MutexLog.Lock();
	if((fp =fopen(acSaveFile, "a")) !=NULL)
	{
		char acLogBuf[CHAR_MAX_SIZE] = {0, };
		GetCurrentDateTime(1, acTimeBuf);

		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);

		fprintf(fp, "%s\t[Info]\t%s\n", acTimeBuf, acLogBuf);

		fclose(fp);
	}
	g_MutexLog.UnLock();
}
//-------------------------------------------------------------------------

void	SetLogFileInfo(LPSTR lpLogPath, LPSTR lpLogName, INT32 nRmLog, UINT32 nFileLogRetention)
{
	g_nRemainLog = nRmLog;
	g_nFileLogRetention = nFileLogRetention;

	memset(g_acLogFilePath, 0, MAX_PATH);
	memset(g_acLogFileName, 0, MAX_PATH);
	if(lpLogPath != NULL && lpLogPath[0] != 0)
		strncpy(g_acLogFilePath, lpLogPath, MAX_PATH-1);
	if(lpLogName != NULL && lpLogName[0] != 0)
		strncpy(g_acLogFileName, lpLogName, MAX_PATH-1);
}

//-------------------------------------------------------------------------
