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


#ifndef ASI_SDK_LV01_GLB_FN_LOG_H_15BE18C4_D543_413E_96F8_61C24AD45985
#define	ASI_SDK_LV01_GLB_FN_LOG_H_15BE18C4_D543_413E_96F8_61C24AD45985

#define ASI_SDK_LOG_KEY_TYPE_SSA20_PKG			1541428297

void	WriteLogN(char* fmt,...);
void	WriteLogD(char* fmt,...);
void	WriteLogE(char* fmt,...);
void    WriteLogN_Size(UINT32 nSize, LPCTSTR lpFileName, char* fmt,...);
void	SetLogFileInfo(LPSTR lpLogPath, LPSTR lpLogName, INT32 nRmLog = 1, UINT32 nFileLogRetention = 5);

void	Encrypt_Log(UINT32	key, PBYTE SrcData,	INT32 SrcLen);
void	Decript_Log(UINT32	key, PBYTE SrcData,	INT32 SrcLen);
void	WriteLog_Enc(LPTSTR lpLog, FILE* fp);
void	SetLogSKey(UINT32 nSKey);

void	WritePerfTest1Log(char *fmt,...);
void	WritePerfTest2Log(char *fmt,...);
void	WritePerfTest3Log(char *fmt,...);
void	WritePerfTest4Log(char *fmt,...);
void	WritePerfTest5Log(char *fmt,...);
#endif