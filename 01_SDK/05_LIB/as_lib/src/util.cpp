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
#include "as_string.h"


pid_t gettid(void)
{
	return syscall(SYS_gettid);
}


int copy_file(const char *src_file, const char *dst_file)
{
	int nread, nwrite;
	int nRetVal = 0;
	int src_fd = -1, dst_fd = -1;
	char buf[MAX_BUFF] = {0,};
	struct stat f_stat;

	if (src_file == NULL || dst_file == NULL)
	{
		return -1;
	}

	if ( (src_fd = open(src_file, O_RDONLY)) == -1)
	{
		return -2;
	}

	if ( (dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
	{
		close(src_fd);
		return -3;
	}

	if (fstat(src_fd, &f_stat) == -1)
	{
		nRetVal = -4;
		goto func_ret;
	}

	nRetVal = 0;
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
				nRetVal = -5;
				break;
			}
			if ( (nwrite = write(dst_fd, buf, nread)) != nread)
			{
				nRetVal = -6;
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

	if (nRetVal < 0)
	{
		unlink(dst_file);
	}
	return nRetVal;
}


int make_dir(const char *path)
{
	char path_tmp[MAX_PATH] = { 0,};
	char dir_tmp[MAX_PATH] = "/";
	char *dir_name = NULL;
	char *last;
	struct stat	 f_stat;

	snprintf(path_tmp, MAX_PATH-1, "%s", path);
	path_tmp[MAX_PATH-1] = 0;
	if (stat(path_tmp, &f_stat) == -1)
	{
		if (errno == ENOENT)
		{
			dir_name = strtok_r(path_tmp, "/", &last);
			strlcat(dir_tmp, dir_name, sizeof(dir_tmp));

			if (mkdir(dir_tmp, 0755) != 0) {
				if (errno != EEXIST) {
					return -1;
				}
			}

			while ((dir_name = strtok_r(NULL, "/", &last)) != NULL)
			{
				strlcat(dir_tmp, (char *) "/", MAX_PATH-1);
				strlcat(dir_tmp, dir_name, MAX_PATH-1);

				if (mkdir(dir_tmp, 0755) != 0)
				{
					if (errno != EEXIST)
					{
						return -2;
					}
				}
			}
		}
		else
		{
			return -3;
		}
	}

	return 0;
}


DWORD GetTickCount()
{
	struct timeb ft;
	DWORD dwCount = 0;
	ftime(&ft);
	dwCount = (DWORD)(ft.time*1000 + ft.millitm);

	return dwCount;
}

void OutputDebugString(LPCSTR pString)
{
	if(pString != NULL)
		printf("%s\n", pString);
}

DWORD GetLastError()
{
	return (DWORD)errno;
}



