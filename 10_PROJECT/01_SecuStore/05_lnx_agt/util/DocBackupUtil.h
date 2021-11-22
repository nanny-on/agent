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

#pragma once

class CDocBackupUtil : public CUtilBase
{
public:
	CDocBackupUtil(void);
	~CDocBackupUtil(void);

private:
	CFileUtil	m_tFileUtil;
	CMutexExt	m_tBkMutex;
	CProcUtil	m_tProcUtil;

private:
	TMapStr		m_tSkipNameMap;
	TMapStr		m_tSkipProcMap;

private:
	TMapStrID	m_tDirSizeMap;

public:
	INT32		InitEnv();

public:
	INT32		RemoveBackupFile(PDB_LOG_DOC pdld);
	INT32		BackupFile(PDB_LOG_DOC pdld, UINT32 nDelMethod = 0, UINT32 nDelCnt = 7, UINT32 nLimitSize = 0, UINT32 nLimitDelCnt = 0, UINT32 nPoFaUsedMode=1);
	INT32		RecoveryFile(PDB_LOG_DOC pdld);

public:
	INT32		BackupFile(PDB_LOG_RS_BK pdata);
	INT32		RecoveryFile(PDB_LOG_RS_BK pdata);
	INT32		RemoveBackupFile(PDB_LOG_RS_BK pdata);

public:
	INT32		RemoveBackupFilesByNonExistLog(UINT32& nContinue);

private:
	INT32		SetBkSeedKey(PBYTE pSeedKey, UINT32 nMode = 0);
};

extern CDocBackupUtil*		t_DocBackupUtil;
