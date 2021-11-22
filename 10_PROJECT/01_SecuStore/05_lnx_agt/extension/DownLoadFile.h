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
 * 
 */

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
 * 
 */


#ifndef _DOWNLOAD_FILE_H__
#define _DOWNLOAD_FILE_H__

#include "DownLoadUtil.h"

INT32 AddDLInfo(PASI_FDL_INFO pafi);
INT32 DelDLInfo(UINT32 nItemType, UINT32 nItemID);
INT32 FindDLInfo(UINT32 nID, PASI_FDL_INFO pafi);
INT32 RunDL();
INT32 StopDL();
INT32 IsRunDL();
INT32 SetDLInit(PASI_FDL_INIT pafdli);
INT32 SetDLSvrType(UINT32 nDLSvrType);
INT32 SetDLSvrInfo(UINT32 nItemType, LPCTSTR szSvrInfo);
INT32 SetHomePath(LPCTSTR lpHomePathList, UINT32 nLen);
INT32 SetDLLogPath(char *pLogPath, char *pLogFile, INT32 nRemainLog, UINT32 nFileLogRetention);
	
#endif /*_DOWNLOAD_FILE_H__*/