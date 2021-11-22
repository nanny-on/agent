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

#ifndef _ELF_INFO_H__
#define _ELF_INFO_H__


#include <elf.h>

//=============================================================================
class CELFInfo
//=============================================================================
{
// Construction
public:
	CELFInfo(char *pcFile = NULL);
	virtual ~CELFInfo();

// Attributes
public:
	INT32	GetFileElfType();
	INT32 	GetMachineType();
	INT32	IsScriptsFile(BOOL &bIsScripts);
	VOID	GetFileElfTypeString(char *pcElfType, INT32 nLength);
	VOID	GetFileElfAbiString(char *pcElfAbi, INT32 nLength);
	VOID	GetFileElfEntryPointString(char *pcEntryPoint, INT32 nLength);
	BOOL	Is64Bit();
	void	Close();
	BOOL	Open(char *pcFilePath, char *pcFileName = NULL);
protected:
	BOOL	IsOpen()	{ return m_nFd != -1; }
	BOOL	IsELF();
	BOOL	IsELF64();
	INT32	ReadElfHdr();
	INT32	ReadElfHdr64();
	

protected:
	INT32		m_nFd;
	INT32		m_nElfClass;
	BOOL		m_bIsScripts;
	Elf32_Ehdr 	m_stElfHeader;
	Elf64_Ehdr	m_stElfHeader64;
	INT32		m_nFileNameLen;
	char		m_acFileName[MAX_FILE_NAME];
};

#endif //_ELF_INFO_H__
