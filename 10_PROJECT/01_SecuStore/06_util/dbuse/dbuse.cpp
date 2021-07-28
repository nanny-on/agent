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
#include "as_odbc_util.h"


int main(int argc, char *argv[])
{
	int rc = 0;
	SQLHANDLE hEnv = NULL;
	SQLHANDLE hDbc = NULL;
	char *pcDbName = NULL;
	char *pcUser = NULL;
	char *pcPswd = NULL;
	char *pcSqlFile = NULL;
	char acLogMsg[MAX_LOGMSG+1] = {0,};
	do{
		if(argc < 4)
		{
			printf("arguments are not enough\n");
			printf("args: [DbName] [SqlPath] [User] [[PassWd]] \n");
			rc = 0;
			break;
		}
		pcDbName = (char *)malloc(MAX_FILE_NAME);
		if(pcDbName == NULL)
		{
			printf("fail to allocate db name memory (%d)\n", errno);
			rc = -1;
			break;
		}
		strncpy(pcDbName, argv[1], MAX_FILE_NAME-1);
		pcDbName[MAX_FILE_NAME-1] = 0;

		pcSqlFile = (char *)malloc(MAX_PATH);
		if(pcSqlFile == NULL)
		{
			printf("fail to allocate sql file memory (%d)\n", errno);
			rc = -2;
			break;
		}
		strncpy(pcSqlFile, argv[2], MAX_PATH-1);
		pcSqlFile[MAX_PATH-1] = 0;

		pcUser = (char *)malloc(MAX_USER_NAME);
		if(pcUser == NULL)
		{
			printf("fail to allocate user name memory (%d)\n", errno);
			rc = -3;
			break;
		}
		strncpy(pcUser, argv[3], MAX_USER_NAME-1);
		pcUser[MAX_USER_NAME-1] = 0;
		
		if(argc >= 5)
		{
			pcPswd = (char *)malloc(MAX_PASSWD);
			if(pcPswd == NULL)
			{
				printf("fail to allocate passwd memory (%d)\n", errno);
				rc = -4;
				break;
			}
			strncpy(pcPswd, argv[4], MAX_PASSWD-1);
			pcPswd[MAX_PASSWD-1] = 0;
		}

		rc = CLIAppInitLocal(pcDbName, pcUser, pcPswd, &hEnv, &hDbc, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, acLogMsg);
		if (rc != 0 || hEnv == NULL || hDbc == NULL)
		{
			printf("fail to connect db (%d) : %s\n", rc, acLogMsg);
			rc = -5;
			break;
		}

		rc = StmtExecDirectFromFile(hDbc, pcSqlFile, acLogMsg);
		if (rc != 0)
		{
			printf("fail to load sql (%d) : %s\n", rc, acLogMsg);
			rc = -6;
			break;
		}

		rc = 0;
	}while(false);

	CLIAppTerm(&hEnv, &hDbc);
	safe_free(pcDbName);
	safe_free(pcSqlFile);
	safe_free(pcUser);
	safe_free(pcPswd);	
	return rc;
} /* main */

