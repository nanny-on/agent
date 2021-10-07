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

//---------------------------------------------------------------------------
#include "stdafx.h"
#include "com_struct.h"
#include "MainDlg.h"
#include "NannyAgent.h"

#define   MAKE_DAEMON

#ifdef MAKE_DAEMON
	int make_daemon(void);
#endif


void SignalTermHandler(int nSig)
{
	if(nSig == SIGTERM && t_MainDlg)
	{
		t_MainDlg->CloseMain();
		Sleep(500);
		t_MainDlg->PostMessage(WM_QUIT, 0, 0);
	}
}

#ifdef MAKE_DAEMON
int make_daemon(void)
{
	int ret;

	if (getuid() != 0 || geteuid() != 0)
	{
		fprintf(stdout, "invalid user permission. uid [%d] euid [%d]. \n\n", getuid(), geteuid());
		return -1;
	}

	ret = check_proc_exist_by_name(NANNY_AGENT_IDENT, getpid());
	if (ret == 1)
	{
		fprintf(stdout, "nanny agent daemon is already running.\n\n");
		return -2;
	}

	daemon_init();

	return 0;
}
#endif


void print_usage(char *cmd)
{
	char szBuf[256] = {0,};
	if(cmd != NULL)
	{
		snprintf(szBuf, 255, "usage : %s [-v -h]\n", cmd);
		fputs(szBuf, stdout);
	}
}

void print_version()
{
	char szBuf[256] = {0,};
	snprintf(szBuf, 255, "\n nanny agent vesion : %s\n\n", NANNY_AGENT_VERSION);
	fputs(szBuf, stdout);
}

int set_opt(int argc, char *argv[])
{
	int c;
	extern char *optarg;
	int nRetVal = FALSE;
	while ((c = getopt(argc, argv, ":vh")) != EOF)
	{
		switch ((char)c)
		{
			case 'v':
				print_version();
				nRetVal = TRUE;
				break;
			case 'h':
				print_usage(argv[0]);
				nRetVal = TRUE;
				break;
			default:
				nRetVal = FALSE;
				break;
		}
	}
	return nRetVal;
}



int main(int argc, char* argv[])
{
	int nRetVal = 0;
	struct sigaction sigact;
	if(argc > 1)
	{
		if(set_opt(argc, argv) == TRUE)
		{
			exit(1);
		}
	}

#ifdef MAKE_DAEMON
	nRetVal = make_daemon();
	if (nRetVal != 0)
	{
		fprintf(stdout, "fail to make daemon (%d).\n", nRetVal);
		exit(2);
	}
#endif
	// Setting Signal Handler
	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_handler = SignalTermHandler;
	sigfillset(&sigact.sa_mask);
	
	sigaction(SIGTERM, &sigact, NULL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);

	// Starting Application Class
	t_MainDlg = new CMainDlg();
	if(t_MainDlg == NULL)
	{
		exit(3);
	}
	t_MainDlg->Run();
	SAFE_DELETE(t_MainDlg);
	exit(0);
}
