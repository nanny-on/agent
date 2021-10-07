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

#include "stdafx.h"
#include "as_parse.h"
#include "as_time.h"
#include "as_file.h"


int flock(LPCSTR pcPath)
{
	int  fd;
	int  count = 0;

	if(pcPath == NULL || pcPath[0] == 0)
		return -2;

lock_reopen:
	if ( (fd = open(pcPath, O_CREAT | O_WRONLY, ASI_FLOCK_FMODE)) == -1)
	{
		if (errno == EAGAIN)
		{
			if (++count > ASI_FLOCK_RETRY_MAX)
			{
				return -1;
			}
			goto lock_reopen;

		}
		else
		{
			return -2;
		}
	}

relock:
	lseek(fd, 0, SEEK_SET);
	if (lockf(fd, F_TLOCK, 1) == -1)
	{
		if (errno == EAGAIN || errno == EACCES)
		{
			if (++count > ASI_FLOCK_RETRY_MAX)
			{
				close(fd);
				return -3;
			}
			goto relock;

		}
		else
		{
			close(fd);
			return -4;
		}
	}

	return fd;
}


void free_flock(int fd)
{
	int  ret;

refree:
	ret = close(fd);
	if (ret == -1)
	{
		if (errno == EINTR)
		{
			goto refree;
		}
	}
}

void RemoveTailCRLF(char *szString)
{
	int i;

	i = strlen(szString);

	while (i > 0)
	{
		if ((szString[i-1] == 0x0D) || (szString[i-1] == 0x0A))
			szString[i-1] = '\0';
		else
			break;
		i--;
	}
}

char *fgets_query_file(char *szString, int nCount, FILE *fp)
{
	if(szString == NULL || nCount == 0 || fp == NULL)
	{
		return NULL;
	}

	while ( fgets(szString, nCount, fp) )
	{
		if ( szString[0] == '\0' || szString[0] == '#' )
			continue;
		remove_tail_crlf(szString);
		return szString;
	}
	return NULL;
}


char *fgets_data_file(char *szString, int nCount, FILE *fp)
{
	if(szString == NULL || nCount == 0 || fp == NULL)
	{
		return NULL;
	}
	while ( fgets(szString, nCount, fp) )
	{
		remove_tail_crlf(szString);
		trim_white_space(szString);
		if ( szString[0] == '\0' || szString[0] == '#' )
			continue;
		return szString;
	}

	return NULL;
}

char *fgets_data_file_v1(char *szString, int nCount, FILE *fp)
{
	char szLen[8] = {0, };
	char cLenCell = 0;
	int nPos = 0, nReadLen = 0;
	int nReadCnt = 0;
	if(szString == NULL || nCount == 0 || fp == NULL)
	{
		return NULL;
	}

	while(fread(&cLenCell, 1, 1, fp ))
	{
		if(cLenCell == RULE_SEP)
			break;

		if ( cLenCell == '\0' || cLenCell == '#' )
			continue;

		szLen[nPos++] = cLenCell;

		if(nPos > 5)
			return NULL;
	}

	if(nPos)
	{
		nReadLen = atoi(szLen);
	}

	if(nReadLen < 1)
		return NULL;

	memset(szString, 0, nCount);
	nPos = 0;
	while(nReadCnt < nReadLen)
	{
		nReadCnt += (int)fread(szString + nReadCnt, sizeof(CHAR), min(nCount, (nReadLen - nReadCnt)), fp);
		if(nPos++ > 10)
			return NULL;
	}

	if((int)strlen(szString) == nReadLen)
	{
		remove_tail_crlf(szString);

		trim_white_space(szString);

		return szString;
	}

	return NULL;
}

int GetFileSizeWithFd(int nFd, UINT32 *pdwFileLen)
{
	struct stat stStat;
	if(nFd == -1 || pdwFileLen == NULL)
	{
		return -1;
	}
	if(fstat(nFd, &stStat) < 0)
	{
		return -2;
	}

	if (!S_ISREG(stStat.st_mode))
	{
		return -3;
	}
	*pdwFileLen = (UINT32)stStat.st_size;
	return 0;
}

int get_file_size(LPCSTR pcPath, UINT32 *pdwFileLen, UINT32 *pdwFileMode)
{
	struct stat stStat;
	if(pcPath == NULL || pdwFileLen == NULL)
	{
		return -1;
	}
	if(stat(pcPath, &stStat) < 0)
	{
		return -2;
	}

	if (!S_ISREG(stStat.st_mode))
	{
		return -3;
	}
	*pdwFileLen = (UINT32)stStat.st_size;
	if(pdwFileMode != NULL)
	{
		*pdwFileMode = (UINT32)(stStat.st_mode & 0777);
	}
	return 0;
}


int is_file(LPCSTR pcPath)
{
	struct stat stStat;
	if(pcPath == NULL || pcPath[0] == 0)
		return -2;

	if(stat(pcPath, &stStat) < 0)
	{
		return -3;
	}
	if (S_ISREG(stStat.st_mode))
		return REG_FILE;
	else if (S_ISDIR(stStat.st_mode))
		return DIR_FILE;
	else if (S_ISFIFO(stStat.st_mode))
		return FIFO_FILE;
	else if(S_ISSOCK(stStat.st_mode))
		return SOCK_FILE;

	return -1;
}


BOOL ForceDirectories(LPCSTR pcDir, char *acLogMsg)
{
	char pszFullPathDir[MAX_PATH] = {0,};
	char pszDirTmp[MAX_PATH] = {0,};
	char pszMakeDir[MAX_PATH] = {0,};
	char *pszDirName = NULL;
	struct stat	stDirStat;

	if(pcDir == NULL || pcDir[0] == 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[ForceDirectories] invalid input data");
		return FALSE;
	}
	
	snprintf(pszFullPathDir, MAX_PATH-1, "%s", pcDir);
	
	if (stat(pszFullPathDir, &stDirStat) == -1)
	{
		if (errno == ENOENT)
		{
			pszDirName = strtok(pszFullPathDir, "/");
			if(pszDirName == NULL)
			{
				if(acLogMsg != NULL)
					snprintf(acLogMsg, MAX_LOGMSG, "[ForceDirectories] invalid path (%s) (%d)", pszFullPathDir, errno);
				return FALSE;
			}
			snprintf(pszMakeDir,MAX_PATH-1, "/%s", pszDirName);
			pszMakeDir[MAX_PATH-1] = 0;
			strncpy(pszDirTmp, pszMakeDir, MAX_PATH-1);
			pszDirTmp[MAX_PATH-1] = 0;
			
			if (mkdir(pszMakeDir, 0755) != 0)
			{
				if (errno != EEXIST)
				{
					if(acLogMsg != NULL)
						snprintf(acLogMsg, MAX_LOGMSG, "[ForceDirectories] fail to make directory (%s) (%d)", pszMakeDir, errno);
					return FALSE;
				}
			}
			while ((pszDirName = strtok(NULL, "/")) != NULL)
			{
				snprintf(pszMakeDir, MAX_PATH-1, "%s/%s", pszDirTmp, pszDirName);
				pszMakeDir[MAX_PATH-1] = 0;
				strncpy(pszDirTmp, pszMakeDir, MAX_PATH-1);
				pszDirTmp[MAX_PATH-1] = 0;

				if (mkdir(pszMakeDir, 0755) != 0)
				{
					if (errno != EEXIST)
					{
						if(acLogMsg != NULL)
							snprintf(acLogMsg, MAX_LOGMSG, "[ForceDirectories] fail to make directory (%s) (%d)", pszMakeDir, errno);
						return FALSE;
					}
				}
			}
		}
		else
		{
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "[ForceDirectories] invalid file path (%s) (%d)", pszFullPathDir, errno);
			return FALSE;
		}
	}

	return TRUE;

}

BOOL DirectoryExists(LPCSTR pcDirectory)
{
	struct stat sb;
	if(pcDirectory == NULL || pcDirectory[0] == 0)
	{
		return FALSE;
	}
	
	if(stat(pcDirectory, &sb) == -1)
		return FALSE;

	return S_ISDIR(sb.st_mode);
}


BOOL CreateDirectory(LPCSTR lpPathName, void* lpParam)
{
	if(DirectoryExists(lpPathName) == TRUE)
		return TRUE;
	
	if(mkdir(lpPathName, 0755) == 0)
		return TRUE;
	
	return FALSE;
}


BOOL CopyFile(LPCSTR pszSrcFile, LPCSTR pszDstFile, BOOL bFailIfExists)
{
	int          nread, nwrite;
	int          error_flag = 0;
	int          src_fd = -1, dst_fd = -1;
	char         buf[4096] = { 0,};
	struct stat  f_stat;

	if (pszSrcFile == NULL || pszDstFile == NULL)
	{
		return FALSE;
	}

	if(bFailIfExists == TRUE && is_file(pszDstFile) == 0)
	{
		return FALSE;
	}

	if ( (src_fd = open(pszSrcFile, O_RDONLY)) == -1)
	{
		return FALSE;
	}

	if ( (dst_fd = open(pszDstFile, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
	{
		close(src_fd);
		return FALSE;
	}

	if (fstat(src_fd, &f_stat) == -1)
	{
		error_flag = 1;
		goto func_ret;
	}

	if (f_stat.st_size > 0)
	{

		while (1)
		{

			nread = read(src_fd, buf, sizeof(buf));

			if (nread == 0)
			{
				break;
			}
			else if (nread < 0)
			{
				error_flag = 1;
				break;
			}

			if ( (nwrite = write(dst_fd, buf, nread)) != nread)
			{
				error_flag = 1;
				break;
			}
		}
	}

func_ret:

	if (src_fd >= 0)
	{
		close(src_fd);
	}

	if (dst_fd >= 0)
	{
		close(dst_fd);
	}

	if (error_flag == 1)
	{
		unlink(pszDstFile);
		return FALSE;
	}
	return TRUE;
}

void MoveFileEx(LPCSTR pszSrcFile, LPCSTR pszDstFile)
{
	char acTempFile[MAX_PATH] = {0,};
	if(pszSrcFile != NULL && pszDstFile != NULL)
	{
		if(CopyFile(pszSrcFile, pszDstFile, FALSE))
		{
			unlink(pszSrcFile);
		}
		else
		{
			snprintf(acTempFile, MAX_PATH-1, "%s.tmp", pszDstFile);
			acTempFile[MAX_PATH-1] = 0;
			if(CopyFile(pszSrcFile, acTempFile, FALSE))
			{
				unlink(pszSrcFile);
			}
		}
	}
}


HMODULE LoadLibrary(LPCTSTR lpFileName)
{
	HANDLE hLib = NULL;

	if(lpFileName == NULL)
		return NULL;

	hLib = dlopen(lpFileName, RTLD_NOW);
	if(hLib == NULL)
	{
		hLib = dlopen(lpFileName, RTLD_LAZY);
	}
	return hLib;
}

void* GetProcAddress(HMODULE hModule, LPCTSTR lpProcName)
{
	if(hModule == NULL || lpProcName == NULL)
		return NULL;
	return dlsym(hModule, lpProcName);
}

void FreeLibrary(HMODULE hModule)
{
	if(hModule != NULL)
	{
		dlclose(hModule);
	}
}

void	ClearOldLogFile(char *acLogPath, char *acLogFile, UINT32 m_nFileLogRetention)
{
/*
	UINT32 i=0;
	char acDeleteFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	if(acLogPath == NULL || acLogFile == NULL || m_nFileLogRetention == 0)
	{
		return;
	}
	for(i=m_nFileLogRetention; i<m_nFileLogRetention+3; i++)
	{
		GetDateTimeByIndex(i, acTimeBuf);
		snprintf(acDeleteFile, MAX_PATH-1, "%s%s%s.txt", acLogPath, acLogFile, acTimeBuf);
		if(is_file(acDeleteFile) == 0)
		{
			unlink(acDeleteFile);
		}
	}
*/
}

BOOL is_mounted_ecryptfs()
{
	FILE *fp = NULL;
	struct mntent *pPart = NULL;
	BOOL bIsMounted = FALSE;

	fp = setmntent("/proc/mounts", "r");
	if(fp == NULL)
		return FALSE;

	while ((pPart = getmntent(fp)) != NULL)
	{
		if ((pPart->mnt_type != NULL) && (!_stricmp(pPart->mnt_type, "ecryptfs")))
		{
			bIsMounted = TRUE;
			break;
		}
	}

	endmntent(fp);

	return bIsMounted;
}


