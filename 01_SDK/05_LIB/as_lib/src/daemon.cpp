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
#include "as_daemon.h"


void daemon_init(void)
{
	int i = 0;
	pid_t  pid;
	if ( (pid = fork()) == -1)
	{
		fprintf(stdout, "fail to fork (%d).\n", errno);
		exit(1);
	}

	/* parent terminate */
	if (pid != 0)
	{
		exit(0);
	}

	/* become session leader */
	setsid();

	signal(SIGHUP, SIG_IGN);

	if ( (pid = fork()) == -1)
	{
		fprintf(stdout, "fail to fork (%d).\n", errno);
		exit(3);
	}

	/* 1st child terminate */
	if (pid != 0)
	{
		exit(0);
	}

	chdir("/");
	umask(0);

	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);
	for (i = 3; i < ASI_DAEMON_MAX_FD; i++) 
	{
		close(i);
	}
}
