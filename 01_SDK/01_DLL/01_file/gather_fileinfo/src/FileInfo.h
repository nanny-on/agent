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

#ifndef _FILE_INFO_H__
#define _FILE_INFO_H__


extern "C"{

INT32 ASIFI_GetFileDescription(char *pcFileName, char *pcFileDescr, INT32 nLength);
INT32 ASIFI_GetFileVersion(char *pcFileName, char *pcFileVersion, INT32 nLength);
INT32 ASIFI_GetFileSpecialValue(char *pcFileName, char *pcFileKey, char *pcFileValue, INT32 nLength);
INT32 ASIFI_GetFileLanguage(char *pcFileName, INT32& nLanguageType);
INT32 ASIFI_GetFileElfMagic(char *pcFileName, INT32& nElfMagic);
INT32	ASIFI_GetFileElfType(char *pcFilePath, INT32& nElfType, char *pcFileName = NULL);

}

#endif /*_FILE_INFO_H__*/