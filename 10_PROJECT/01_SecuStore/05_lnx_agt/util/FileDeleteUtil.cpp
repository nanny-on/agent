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

#include "stdafx.h"
#include "com_struct.h"
#include "FileDeleteUtil.h"

CFileDeleteUtil* t_FileDeleteUtil = NULL;

CFileDeleteUtil::CFileDeleteUtil(void)
{
	m_strUtilName		= "util file delete";
	m_nDelMethod		= 0;
	t_WipeUtil = new CSecureDeleteUtil();
}
//---------------------------------------------------------------------------

CFileDeleteUtil::~CFileDeleteUtil(void)
{
	m_nDelMethod		= 0;
	SAFE_DELETE(t_WipeUtil);
}
//---------------------------------------------------------------------------

void		CFileDeleteUtil::SetDeleteMethod(UINT32 nMethod)
{
	m_nDelMethod = nMethod;
}

INT32 CFileDeleteUtil::ZeroFile(LPCSTR szPath, DWORD dwOverwriteCount)
{
	INT32 nFd = -1;
	DWORD i = 0;
	char acBuffer[SECURE_DEL_BUF_SIZE] = {0,};
	ssize_t ddwWrittenCount = 0;
	ssize_t ddwCount = 0;
	ssize_t ddwRetVal = 0;
	ssize_t ddwFileSize = 0;
	struct stat fStat;
	mode_t nMode = 0;
	
	if(szPath == NULL || dwOverwriteCount == 0)
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return -1;
	}
	
	if (stat(szPath, &fStat) == -1)
	{
		WriteLogE("[%s] fail to get stat %s (%d)", m_strUtilName.c_str(), szPath, errno);
		return -2;
	}

	ddwFileSize = (ssize_t)fStat.st_size;
	if(ddwFileSize < 1)
	{
		return 0;
	}

	nFd = open(szPath, O_WRONLY);
	if ((nFd == -1) && (errno == EACCES))
	{
		nMode = fStat.st_mode;
		if(!(nMode & S_IWUSR))
			nMode = nMode | S_IWUSR;
		if(!(nMode & S_IWGRP))
			nMode = nMode | S_IWGRP;
		if(!(nMode & S_IWOTH))
			nMode = nMode | S_IWOTH;
		if (chmod(szPath, nMode) == -1)
		{
			WriteLogE("[%s] fail to chmode %s (%d)", m_strUtilName.c_str(), szPath, errno);
			return -4;
		}
		nFd = open(szPath, O_WRONLY);
	}

	if (nFd == -1)
	{
		WriteLogE("[%s] fail to open %s (%d)", m_strUtilName.c_str(), szPath, errno);
		return -5;
	}

	for(i=0; i<dwOverwriteCount; i++)
	{
		while (ddwWrittenCount < ddwFileSize)
		{
			ddwCount = ddwFileSize - ddwWrittenCount;
			if (ddwCount > SECURE_DEL_BUF_SIZE)
			{
				ddwCount = SECURE_DEL_BUF_SIZE;
			}

			ddwRetVal = write(nFd, acBuffer, ddwCount);
			if (ddwRetVal != ddwCount)
			{
				if ((ddwRetVal == -1) && (errno == EINTR))
				{
					Sleep(10);
					continue;
				}
				else
				{
					WriteLogE("[%s] fail to write %s (%d)", m_strUtilName.c_str(), szPath, errno);
					close(nFd);
					return -6;
				}
			}
			ddwWrittenCount += ddwRetVal;
		}
		fdatasync(nFd);
		Sleep(10);
	}
	close(nFd);
	return 0;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

INT32 CFileDeleteUtil::SecureDeleteFile(LPCSTR szPath, UINT32 nDelMethod, DWORD dwOverwriteCount, BOOL bReadOnlyDelete, BOOL bWait, UINT32 nLimitMByteByte, UINT32 nLimitDelCnt)
{
	CFileUtil fu;
	DWORD dwFileSize = 0;
	INT32 nRetVal = 0;
	if (!szPath || !szPath[0])
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return -1;
	}

	if(!dwOverwriteCount)
	{
		if(unlink(szPath) == -1)
		{
			WriteLogN("[%s] fail to delete %s (%d) and delete file after restart", m_strUtilName.c_str(), szPath, errno);
			if(t_LogicMgrPoFaDelFileAfterBoot)
				t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(szPath);
		}
		return 0;
	}

	if (!(nLimitMByteByte == 0 || nLimitDelCnt == 0))
	{
		dwFileSize = fu.GetFileSizeExt(szPath);
		if (dwFileSize >= nLimitMByteByte *ASI_MEGABYTE)
		{
			WriteLogN("secure del file size over and change del cnt. [%s][%d=>%d]", szPath, dwOverwriteCount, nLimitDelCnt);
			if(!nLimitDelCnt)
			{
				if(unlink(szPath) == -1)
				{
					WriteLogN("[%s] fail to delete %s (%d) and delete file after restart", m_strUtilName.c_str(), szPath, errno);
					if(t_LogicMgrPoFaDelFileAfterBoot)
						t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(szPath);
				}
				return 0;
			}
			dwOverwriteCount = nLimitDelCnt;
		}
	}

	if (fu.FileExists(szPath) == FALSE)
	{
		return 0;
	}

	if (nDelMethod == 0)
	{
		nRetVal = ZeroFile(szPath, dwOverwriteCount);
		if(nRetVal == 0)
		{
			WriteLogN("[%s] success to write to %s by %lu times zero fill", m_strUtilName.c_str(), szPath, dwOverwriteCount);
		}
		else
		{
			WriteLogE("[%s] fail to write to %s by %lu times zero fill (%d)", m_strUtilName.c_str(), szPath, dwOverwriteCount, nRetVal);
		}
		if(unlink(szPath) == -1)
		{
			WriteLogN("[%s] fail to delete %s (%d) and delete file after restart", m_strUtilName.c_str(), szPath, errno);
			if(t_LogicMgrPoFaDelFileAfterBoot)
				t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(szPath);
		}

	}
	else if (nDelMethod == 1)
	{
		if(t_WipeUtil != NULL)
		{
			nRetVal = t_WipeUtil->WipeFilesA((char *)szPath, (INT32)dwOverwriteCount);
			if(nRetVal == 0)
			{
				WriteLogN("[%s] success to write to %s by %lu times fast wipe", m_strUtilName.c_str(), szPath, dwOverwriteCount);
			}
			else
			{
				WriteLogE("[%s] fail to write to %s by %lu times fast wipe (%d)", m_strUtilName.c_str(), szPath, dwOverwriteCount, nRetVal);
			}
		}
	}
	else
	{
		if(unlink(szPath) == -1)
		{
			WriteLogN("[%s] fail to delete %s (%d) and delete file after restart", m_strUtilName.c_str(), szPath, errno);
			if(t_LogicMgrPoFaDelFileAfterBoot)
				t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(szPath);
		}
	}
	return 0;
}
