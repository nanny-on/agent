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
#include "com_struct.h"

INT32 GetFileSize(LPCSTR pcPath, UINT32 *pdwFileLen)
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
	return 0;
}


INT32 CopyFile(LPCSTR pszSrcFile, LPCSTR pszDstFile)
{
	INT32 nRead, nWrite;
	INT32 nSrcFd = -1, nDstFd = -1;
	UINT32 dwFileLen = 0;
	INT32 nRetVal = 0;
	char acBuf[MAX_BUFF] = { 0,};

	if (pszSrcFile == NULL || pszDstFile == NULL)
	{
		return -1;
	}

	nRetVal = GetFileSize(pszSrcFile, &dwFileLen);
	if(nRetVal < 0 || dwFileLen == 0)
	{
		return -2;
	}
	nSrcFd = open(pszSrcFile, O_RDONLY);
	if (nSrcFd == -1)
	{
		return -3;
	}

	nDstFd = open(pszDstFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (nDstFd == -1)
	{
		close(nSrcFd);
		return -4;
	}

	while (1)
	{
		nRead = read(nSrcFd, acBuf, MAX_BUFF);
		if (nRead == 0)
		{
			break;
		}
		else if (nRead < 0)
		{
			nRetVal = -5;
			break;
		}
		nWrite = write(nDstFd, acBuf, nRead);
		if (nWrite != nRead)
		{
			nRetVal = -6;
			break;
		}
		nRetVal = 0;
	}

	if (nSrcFd > 0)
	{
		close(nSrcFd);
	}

	if (nDstFd > 0)
	{
		close(nDstFd);
	}

	return nRetVal;
}

void print_usage(char *cmd)
{
	char szBuf[256] = {0,};
	if(cmd != NULL)
	{
		snprintf(szBuf, 255, "usage : %s src_path dst_path\n", cmd);
		fputs(szBuf, stdout);
	}
}


int main(int argc, char* argv[])
{
	INT32 nRetVal = 0;
	char acSrcPath[MAX_PATH] = {0,};
	char acDstPath[MAX_PATH] = {0,};
	if(argc < 3)
	{
		print_usage(argv[0]);
		exit(1);
	}
	strncpy(acSrcPath, argv[1], MAX_PATH-1);
	acSrcPath[MAX_PATH-1] = 0;
	strncpy(acDstPath, argv[2], MAX_PATH-1);
	acDstPath[MAX_PATH-1] = 0;

	nRetVal = CopyFile(acSrcPath, acDstPath);
	if(nRetVal < 0)
	{
		fprintf(stderr, "fail to copy %s to %s\n", acSrcPath, acDstPath);
		exit(2);
	}

	fprintf(stdout, "success to copy %s to %s\n", acSrcPath, acDstPath);

	return 0;
}


