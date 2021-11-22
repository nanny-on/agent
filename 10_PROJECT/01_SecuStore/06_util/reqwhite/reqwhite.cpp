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


void print_usage()
{
	fprintf(stderr, "\nUsage: reqwhite [options]\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -p deny_file_full_path (ex : /bin/ping)\n");
	fprintf(stderr, "  -l req_level			(0:default, 1:medium, 2:high, 3:urgency)\n");
	fprintf(stderr, "  -d expiration date   (0:unlimited, 1~31 days)\n");
	fprintf(stderr, "  -h help\n\n");
}

int set_opt(int argc, char *argv[], char *pcPath, int& nLevel, int& nDay)
{
	int c;
	extern char *optarg;
	int nRetVal = 0;
	if(pcPath == NULL)
	{
		return 0;
	}

	while ((c = getopt(argc, argv, "p:l:d:h")) != EOF)
	{
		switch ((char)c)
		{
		case 'p':
			if(optarg != NULL)
			{
				strncpy(pcPath, optarg, MAX_PATH-1);
				pcPath[MAX_PATH-1] = 0;
				nRetVal = 1;
			}
			break;
		case 'l':
			if(optarg != NULL)
				nLevel = atoi(optarg);
			break;
		case 'd':
			if(optarg != NULL)
				nDay = atoi(optarg);
			break;
		case 'h':
			nRetVal = 0;
			break;
		}
	}
	return nRetVal;
}

int valid_check(char *pcPath, int &nLevel, int &nDay)
{
	if(pcPath == NULL)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}
	if(pcPath[0] != '/')
	{
		fprintf(stderr, "invalid file path %s\n", pcPath);
		return -2;
	}
	if(is_file(pcPath) != REG_FILE)
	{
		fprintf(stderr, "fail to find %s (%d)\n", pcPath, errno);
		return -3;
	}

	if(nDay < 0 || nDay > 31)
	{
		fprintf(stderr, "invalid expiration date (%d)\n", nDay);
		return -4;
	}
	if(nLevel == 0)
		nLevel = SS_PO_IN_PTN_SP_REQ_LEVEL_NONE;
	else if(nLevel == 1)
		nLevel = SS_PO_IN_PTN_SP_REQ_LEVEL_NORMAL;
	else if(nLevel == 2)
		nLevel = SS_PO_IN_PTN_SP_REQ_LEVEL_HIGH;
	else if(nLevel == 3)
		nLevel = SS_PO_IN_PTN_SP_REQ_LEVEL_URGENCY;
	else
	{
		fprintf(stderr, "invalid request level (%d)\n", nLevel);
		return -5;
	}

	return 0;
}


int set_po_in_req(PPIPE_PO_IN_REQ pPoInReq, char *pcPath, int nLevel, int nDay)
{
	if(pPoInReq == NULL || pcPath == NULL)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}

	pPoInReq->nCmdId = CMD_PIPE_PO_IN_REQ;
	pPoInReq->nReqLevel = nLevel;
	pPoInReq->nReqDay = nDay;
	strncpy(pPoInReq->acReqFullPath, pcPath, MAX_PATH-1);
	pPoInReq->acReqFullPath[MAX_PATH-1] = 0;
	if(get_dirname(pPoInReq->acReqFullPath, pPoInReq->acReqPath, MAX_PATH-1) == NULL)
	{
		fprintf(stderr, "fail to get dir name (%s)", pPoInReq->acReqFullPath);
		return -2;
	}
	pPoInReq->acReqPath[MAX_PATH-1] = 0;

	if(get_basename(pPoInReq->acReqFullPath, pPoInReq->acReqFile, MAX_FILE_NAME-1) == NULL)
	{
		fprintf(stderr, "fail to get base name (%s)", pPoInReq->acReqFullPath);
		return -3;
	}
	pPoInReq->acReqFile[MAX_FILE_NAME-1] = 0;

	return 0;
}


int main(int argc, char* argv[])
{
	int nLevel = 0;
	int nDay = 0;
	int nRetVal = 0;
	INT32 nFd = -1;
	PPIPE_PO_IN_REQ pPoInReq = NULL;
	int nSize = sizeof(PIPE_PO_IN_REQ);
	char acDirPath[MAX_PATH] = {0,};
	char acFiFoPath[MAX_PATH] = {0,};

	snprintf(acFiFoPath, MAX_PATH-1, "%s/%s", NANNY_INSTALL_DIR, FIFO_FILE_PATH);
	nRetVal = set_opt(argc, argv, acDirPath, nLevel, nDay);
	if(nRetVal != 1)
	{
		print_usage();
		return 1;
	}
	nRetVal = valid_check(acDirPath, nLevel, nDay);
	if(nRetVal < 0)
	{
		return 2;
	}

	do{
		pPoInReq = (PPIPE_PO_IN_REQ)malloc(nSize);
		if(pPoInReq == NULL)
		{
			fprintf(stderr, "fail to allocate memory (%d) (%d)\n", nSize, errno);
			nRetVal = 3;
		}
		memset(pPoInReq, 0, sizeof(nSize));

		nFd=open(acFiFoPath, O_WRONLY);
		if(nFd == -1)
		{
			fprintf(stderr, "fail to open fifo %s (%d)", acFiFoPath, errno);
			nRetVal = 4;
			break;
		}

		nRetVal = set_po_in_req(pPoInReq, acDirPath, nLevel, nDay);
		if(nRetVal < 0)
		{
			nRetVal = 5;
			break;
		}

		if(write(nFd, (PVOID)pPoInReq, nSize) == -1)
		{
			fprintf(stderr, "fail to write data (%d)", errno);
			nRetVal = 6;
			break;
		}
		printf("\nsuccess to send request message (%s)\n", acDirPath);
		nRetVal = 0;
	}while(FALSE);

	if(nFd != -1)
		close(nFd);
	safe_free(pPoInReq);
	return nRetVal;
}
