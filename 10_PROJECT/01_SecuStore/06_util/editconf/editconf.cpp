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
#include "sqlite3.h"
#include <iostream>
#include <map>
#include <dlfcn.h>


using namespace std;

typedef map<UINT32, string> ID2STRMAP;

const char* root_key_nm[] = {"Setting db connection configurations", 
							"Setting nanny agent configurations",
							NULL};

const HKEY root_key[] = {(HKEY)(-1), (HKEY)(-1), HKEY_LOCAL_MACHINE, (HKEY)(-1), NULL};

const char* local_subkey_dnm[] = {
	        "Connection Env.", "DB Env.", "Local Env.", "Path Env.", NULL};

const char* local_subkey_nm[] = {
	    "conenv", "dbenv","localenv", "pathenv", NULL};

void PrintMenuTitle(const char* root = "", const char* keyname = "")
{
    system("clear");
	printf("=======================================\n");
    printf("Edit Nanny Agent for Linux Configuration.\n");
	printf("Build Date - %s\n", __DATE__);
    printf("=======================================\n\n");
	if(root && strlen(root)) 
	{
		printf("[%s]", root);
		if(keyname && strlen(keyname))
			printf(" >> %s", keyname); 
		printf("\n");
	}

	printf("\n");
}

BOOL GetNannyAgentRoot(String& strPath)
{
	HKEY hSubKey = NULL;
	char szPath[MAX_PATH] = {0,};
	DWORD dwDisp = 0;
	char acLogMsg[MAX_LOGMSG+1] = {0,};
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_LOCAL_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp, acLogMsg) != 0)
	{
		printf("fail to create key : %s\n", acLogMsg);
		return FALSE;
	}
	DWORD dwType = 0;
	DWORD cbData = MAX_PATH - 1;
	if(RegQueryValueEx(hSubKey, "root_path", 0, &dwType, (PBYTE)szPath, MAX_PATH-1, &cbData) != 0)
	{
		printf("fail to get value key (%d)\n", errno);
		RegCloseKey(hSubKey);
		return FALSE;
	}
	strPath = szPath;
	RegCloseKey(hSubKey);
	return TRUE;
}

int SetNannyAgentRoot(String strPath)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	char acLogMsg[MAX_LOGMSG+1] = {0,};
	if(strPath.length() == 0)
	{
		printf("invalid root path\n");
		return 13;
	}
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_LOCAL_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp, acLogMsg) != 0)
	{
		printf("fail to create key : %s\n", acLogMsg);
		return 14;
	}
	if(RegSetValueEx(hSubKey,  "root_path", 0, REG_SZ, (PBYTE)strPath.c_str(), strPath.length()+1) != 0)
	{
		printf("fail to set value key : %s\n", acLogMsg);
		RegCloseKey(hSubKey);
		return 15;
	}
	RegCloseKey(hSubKey);
	return 0;
}

int SetDBAccount(char* pszDBName, char* pszID)
{
	HKEY hSubKey = NULL;
   	DWORD dwDisp = 0;
	int nRetVal = 0;
	int nLengh = 0;
	char acLogMsg[MAX_LOGMSG+1] = {0,};

	if(pszDBName == NULL || pszID == NULL)
	{
		printf("invalid input data\n");
		return 13;
	}

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_DB_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp, acLogMsg) != 0)
	{
		printf("fail to create key : %s\n", acLogMsg);
		return 14;
	}

	do{
		nLengh = (int)strlen(pszDBName);
		if(nLengh < 1)
		{
			printf("invalid db name (%d)\n", nLengh);
			nRetVal = 15;
			break;
		}
		if(RegSetValueEx(hSubKey, "main_db_name", 0, REG_SZ, (PBYTE)pszDBName, nLengh+1, acLogMsg) != 0)
		{
			printf("fail to set value main_db_name : %s\n", acLogMsg);
			nRetVal = 16;
			break;
		}

		nLengh = (int)strlen(pszID);
		if(nLengh < 1)
		{
			printf("invalid db id (%d)\n", nLengh);
			nRetVal = 17;
			break;
		}

		if(RegSetValueEx(hSubKey,  "main_db_account_id", 0, REG_SZ, (PBYTE)pszID, nLengh+1, acLogMsg) != 0)
		{
			printf("fail to set value main_db_account_id : %s\n", acLogMsg);
			nRetVal = 18;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	RegCloseKey(hSubKey);
	return nRetVal;
}

BOOL GetDBInfo(DBMS_ACCOUNT_INFO& dai)
{
	DWORD dwDisp = 0;
	BOOL bRetVal = TRUE;
	DWORD dwType = 0;
	CHAR szTemp[MAX_PATH] = {0,};
	DWORD cbData = 0;
	HKEY hSubKey = NULL;

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_DB_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp) != ERROR_SUCCESS)
	{
		printf("fail to open conenv key (%d)\n", errno);
		return FALSE;
	}


	do{
		cbData = MAX_PATH-1;
		if(RegQueryValueEx(hSubKey, "main_db_name", 0, &dwType, (PBYTE)szTemp, MAX_PATH-1, &cbData) != ERROR_SUCCESS)
		{
			printf("fail to query main db name (%d)\n", errno);
			bRetVal = FALSE;
			break;
		}
		dai.strDB = szTemp;

		cbData = MAX_PATH-1;
		if(RegQueryValueEx(hSubKey, "main_db_account_id", 0, &dwType, (LPBYTE)szTemp, MAX_PATH-1, &cbData) != ERROR_SUCCESS)
		{
			printf("fail to query main db account id (%d)\n", errno);
			bRetVal = FALSE;
			break;
		}
		dai.strID = szTemp;
		bRetVal = TRUE;
	}while(FALSE);
	
	RegCloseKey(hSubKey);

	return bRetVal;
}

int SetLogonServer(char* pcLgnSvrIp, char* pcLgnSvrPort)
{
	HKEY hSubKey = NULL;
   	DWORD dwDisp = 0;
	int nRetVal = 0;
	int nPort = 0;
	UINT32 dwIpAddr = INADDR_NONE;
	String strLgnSvrInfo;
	char acLogMsg[MAX_LOGMSG+1] = {0,};

	if(pcLgnSvrIp == NULL || pcLgnSvrPort == NULL)
	{
		printf("fail to set logon server info : invalid input data\n", acLogMsg);
		return 13;
	}
		
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_CON_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp, acLogMsg) != 0)
	{
		printf("fail to create key : %s\n", acLogMsg);
		return 14;
	}

	do{
		dwIpAddr = ConvertHost(pcLgnSvrIp);
		if(dwIpAddr == (UINT32)INADDR_NONE)
		{
			printf("invalid ip address : %s\n", pcLgnSvrIp);
			nRetVal = 15;
			break;
		}

		if(is_valid_num(pcLgnSvrPort) != 0)
		{
			printf("invalid port : %s\n", pcLgnSvrPort);
			nRetVal = 16;
			break;
		}
		nPort = atoi(pcLgnSvrPort);
		if(nPort < 1 || nPort > 65535)
		{
			printf("invalid port range : %d\n", nPort);
			nRetVal = 17;
			break;
		}
			
		strLgnSvrInfo = SPrintf("%s:%s;", pcLgnSvrIp, pcLgnSvrPort);

		if(RegSetValueEx(hSubKey,  "lgn_svr_list", 0, REG_SZ, (PBYTE)strLgnSvrInfo.c_str(), strLgnSvrInfo.length()+1, acLogMsg) != 0)
		{
			printf("fail to set value lgn_svr_list : %s\n", acLogMsg);
			nRetVal = 18;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	RegCloseKey(hSubKey);
	return nRetVal;
}

INT32 GetDeletePasswdMode()
{
	INT32 rc = 0;
	INT32 nRetVal = 0;
	INT32 nCols = 0;
	sqlite3 *pConn = NULL;
	sqlite3_stmt *pStatement = NULL;
	INT32 nUsedFlag = 0;
	INT32 nUsedMode = 0;
	String strRootPath;
	char acDbPath[MAX_PATH] = {0,};
	DBMS_ACCOUNT_INFO stDbInfo;
	char acSqlQuery[256] = "select used_flag, used_mode from po_host_rm_info;";

	if(GetDBInfo(stDbInfo) == FALSE)
	{
		printf("fail to get db info\n");
		return 13;
	}

	if(GetNannyAgentRoot(strRootPath) == FALSE)
	{
		printf("fail to get nanny agent root\n");
		return 14;
	}
	
	snprintf(acDbPath, MAX_PATH-1, "%s/%s/db/%s.db", (char *)strRootPath.c_str(), NANNY_AGENT_DIR, (char *)stDbInfo.strDB.c_str());

	rc = sqlite3_open_v2(acDbPath, &pConn, SQLITE_OPEN_READONLY, NULL);
	if (rc != 0 || pConn == NULL)
	{
		printf("fail to open sqlite3 (%s) (%d)\n", acDbPath, errno);
		return 15;
	}

	rc = sqlite3_prepare_v2(pConn, acSqlQuery, -1, &pStatement, NULL);
	if (rc != SQLITE_OK || pStatement == NULL)
	{
		printf("fail to prepare sqlite3 (%s) (%d)\n", acSqlQuery, errno);
		sqlite3_close(pConn);
		return 16;
	}

	nRetVal = 0;
	while(sqlite3_step (pStatement) == SQLITE_ROW)
	{
		nCols = sqlite3_column_count(pStatement);
		if(nCols != 2)
		{
			nRetVal = 17;
			printf("invalid column count (%d)\n", nCols);
			break;
		}

		rc = sqlite3_column_type(pStatement, 0);
		if(rc != SQLITE_INTEGER)
		{
			nRetVal = 18;
			printf("invalid column type (%d)\n", rc);
			break;
		}

		nUsedFlag = sqlite3_column_int(pStatement, 0);
		rc = sqlite3_column_type(pStatement, 1);
		if(rc != SQLITE_INTEGER)
		{
			printf("invalid column type (%d)\n", rc);
			nRetVal = 19;
			break;
		}

		nUsedMode = sqlite3_column_int(pStatement, 1);
		if(nUsedFlag == 1 && nUsedMode == 1)
		{
			nRetVal = 1;
			break;
		}
	}while(FALSE);

	if(pStatement != NULL)
		sqlite3_finalize(pStatement);
	if(pConn != NULL)
		sqlite3_close(pConn);

	return nRetVal;

}


INT32 CheckDeletePasswd(char* pDeletePasswd)
{
	INT32 rc, nRetVal = 0;
	INT32 nCols = 0;
	sqlite3 *pConn = NULL;
	sqlite3_stmt *pStatement = NULL;
	INT32 nUsedFlag = 0;
	INT32 nUsedMode = 0;
	String strRootPath;
	char acDbPath[MAX_PATH] = {0,};
	DBMS_ACCOUNT_INFO stDbInfo;
	char acSqlQuery[256] = "select used_flag, used_mode, rm_pw from po_host_rm_info;";
	char acPasswd[MAX_PASSWD] = {0,};

	if(pDeletePasswd == NULL || pDeletePasswd[0] == 0)
	{
		printf("invalid input data\n");
		return 13;
	}

	if(GetDBInfo(stDbInfo) == FALSE)
	{
		printf("fail to get db info\n");
		return 14;
	}

	if(GetNannyAgentRoot(strRootPath) == FALSE)
	{
		printf("fail to get nanny agent root\n");
		return 15;
	}

	snprintf(acDbPath, MAX_PATH-1, "%s/%s/db/%s.db", (char *)strRootPath.c_str(), NANNY_AGENT_DIR, (char *)stDbInfo.strDB.c_str());

	rc = sqlite3_open_v2(acDbPath, &pConn, SQLITE_OPEN_READONLY, NULL);
	if (rc != 0 || pConn == NULL)
	{
		printf("fail to open sqlite3 (%s) (%d)\n", acDbPath, errno);
		return 15;
	}

	rc = sqlite3_prepare_v2(pConn, acSqlQuery, -1, &pStatement, NULL);
	if (rc != SQLITE_OK || pStatement == NULL)
	{
		printf("fail to prepare sqlite3 (%s) (%d)\n", acSqlQuery, errno);
		sqlite3_close(pConn);
		return 16;
	}

	nRetVal = 22;
	while(sqlite3_step (pStatement) == SQLITE_ROW)
	{
		nCols = sqlite3_column_count(pStatement);
		if(nCols != 3)
		{
			nRetVal = 17;
			printf("invalid column count (%d)\n", nCols);
			break;
		}

		rc = sqlite3_column_type(pStatement, 0);
		if(rc != SQLITE_INTEGER)
		{
			nRetVal = 18;
			printf("invalid column type (%d)\n", rc);
			break;
		}

		nUsedFlag = sqlite3_column_int(pStatement, 0);
		rc = sqlite3_column_type(pStatement, 1);
		if(rc != SQLITE_INTEGER)
		{
			printf("invalid column type (%d)\n", rc);
			nRetVal = 19;
			break;
		}

		nUsedMode = sqlite3_column_int(pStatement, 1);
		rc = sqlite3_column_type(pStatement, 2);
		if(rc != SQLITE_TEXT)
		{
			printf("invalid column type (%d)\n", rc);
			nRetVal = 20;
			break;
		}

		nRetVal = 1;
		if(nUsedFlag == 1 && nUsedMode == 1)
		{
			strncpy(acPasswd, (char*)sqlite3_column_text(pStatement, 2), MAX_PASSWD-1);
			if(acPasswd[0] != 0 && !_stricmp(acPasswd, pDeletePasswd))
			{
				nRetVal = 0;
				break;
			}
		}
	}while(FALSE);

	if(pStatement != NULL)
		sqlite3_finalize(pStatement);
	if(pConn != NULL)
		sqlite3_close(pConn);

	return nRetVal;

}

//-----------------------------------------------------------------------------------------------------------
int ProcessArgs(int argc, char* argv[])
{
	int nRetVal = 101;
	BOOL bValidArgs = FALSE;

	if(strcmp(argv[1], "-rp") == 0) // root path setting
	{
		if(argc >= 3)
		{   
			bValidArgs = TRUE;
			nRetVal = SetNannyAgentRoot(argv[2]);
		}
	}
	else if(strcmp(argv[1], "-db") == 0) // database setting
	{
		if(argc >= 4)
		{
			bValidArgs = TRUE;
			nRetVal = SetDBAccount(argv[2], argv[3]); // dbname, id
		}
	}
	else if(strcmp(argv[1], "-li") == 0) // logon server setting
	{
		if(argc >= 4)
		{   
			bValidArgs = TRUE;
			nRetVal = SetLogonServer(argv[2], argv[3]); // ip, port
		}
	}
	else if(strcmp(argv[1], "-gm") == 0) // get passwd mode
	{
		if(argc >= 2)
		{   
			bValidArgs = TRUE;
			nRetVal = GetDeletePasswdMode();
			if(nRetVal == 0)
				printf("disable delete passwd mode\n");
			else if(nRetVal == 1)
				printf("enable delete passwd mode\n");
		}
	}
	else if(strcmp(argv[1], "-cp") == 0) // check passwd
	{
		if(argc >= 3)
		{   
			bValidArgs = TRUE;
			nRetVal = CheckDeletePasswd(argv[2]);
			if(nRetVal == 0)
				printf("match delete passwd\n");
			else if(nRetVal == 1)
				printf("mismatch delete passwd\n");
		}
	}
	if(bValidArgs == FALSE)
	{
		printf("usage1: -rp [agent root path]\n");
		printf("usage2: -db [database name] [admin id]\n");
		printf("usage3: -li [logon server ip] [logon server port]\n");
		printf("usage4: -gm\n");
		printf("usage5: -cp [delete passwd]\n");
		return 102;
	}

	return nRetVal;
}

int main(int argc, char* argv[])
{
	if(argc >= 2)
	{
		return ProcessArgs(argc, argv);
	}

	HKEY hSubKey = NULL;
	INT32 nIndex = 0;
	INT32 nRetVal = 0;

	char c = 'c';
	string strCmd;
	while(c != 'q')
	{
		PrintMenuTitle();
		INT32 i = 0;
		printf("---------------------------------------\n");
		while(root_key_nm[i] && i < 2)
		{
			printf("%d. %s\n", i+1, root_key_nm[i]);
			i++;
		}
		printf("\np. previous\n");
		printf("q. quit\n");
		printf("---------------------------------------\n\n");
		printf("select number: ");
		getline(cin, strCmd);
		c = strCmd[0];
		while( c >= '1' && c <= '9')
		{
			switch(c)
			{
				case '1': // db settings
					{
						String strDBName;
						String strID;
						printf("\n DB Name   : ");
						getline(cin, strDBName);

						printf(" DB Lgn ID : ");
						getline(cin, strID);


						nRetVal = SetDBAccount((char *)strDBName.c_str(), (char *)strID.c_str());
						if(nRetVal == 0)
							printf("\n Success to set db connection configurations\n\n");
						else
							printf("\n Fail to set db connection configurations (%d)\n\n", nRetVal);
						printf("Press 'Enter' to continue ...\n");
                        getline(cin, strCmd);
						c = 'p';
					}
					break;
				case '2': // HKEY_LOCAL_MACHINE
				{
					i = 0;
					PrintMenuTitle(root_key_nm[c-'1'], "");
					printf("---------------------------------------\n");
        			while(local_subkey_dnm[i])
			        {
			            printf("%d. %s\n", i+1, local_subkey_dnm[i]);
			            i++;
			        }
					printf("\np. previous\nq. quit\n");
					printf("---------------------------------------\n");
					printf("\nselect number: ");

					getline(cin, strCmd);
					if(strCmd[0] == 'p')
					{
						c = 'p';
						continue;
					}
					if(strCmd[0] == 'q')
						return 0;

					INT32 iSel = -1;
					sscanf(strCmd.c_str(), "%d", &iSel);
					while(iSel >= 1 && iSel <= i)
					{
						CHAR szSubKeyName[CHAR_MAX_SIZE] = {0,};
						snprintf(szSubKeyName, CHAR_MAX_SIZE-1, "%s/%s", STR_REG_DEFAULT_SVC_PATH, local_subkey_nm[iSel-1]);
						printf("szSubKeyName: %s\n", szSubKeyName);
						DWORD dwDisp = 0;
						if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKeyName, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp) == ERROR_SUCCESS)
						{
							nIndex = 0;
							PrintMenuTitle(root_key_nm[c-'1'], local_subkey_dnm[iSel-1]);
							CHAR szKeyValueName[CHAR_MAX_SIZE] = {0,};
							ID2STRMAP valuenames;
							printf("---------------------------------------\n");
							while(RegEnumKey(hSubKey, nIndex, szKeyValueName, CHAR_MAX_SIZE-1) != ERROR_NO_MORE_ITEMS)
							{
								if(!szKeyValueName[0])
									break;
								printf("%d. %s\n", nIndex+1, szKeyValueName);
								valuenames[nIndex] = szKeyValueName;
								szKeyValueName[0] = '\0';
								nIndex++;
							}
							RegCloseKey(hSubKey);
							if(nIndex)
								printf("\n");
							printf("p. previous\nq. quit\n");
							printf("---------------------------------------\n");
							printf("\nselect number: ");

							INT32 iKey = -1;
							getline(cin, strCmd);
		                    if(strCmd[0] == 'p')
							{
								iSel = -1;
		                        continue;
							}
		                    if(strCmd[0] == 'q')
		                        return 0;

							sscanf(strCmd.c_str(), "%d", &iKey); 
							if(iKey >= 1 && iKey <= nIndex)
							{
								DWORD dwDisp = 0;
								if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKeyName, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp) == ERROR_SUCCESS)
								{
									DWORD dwType = 0;
									BYTE data[MAX_PATH] = {0,};
									DWORD cbData = MAX_PATH-1;
									PrintMenuTitle(root_key_nm[c-'1'], local_subkey_dnm[iSel-1]);
									const char* valuename = valuenames[iKey-1].c_str();
									if(RegQueryValueEx(hSubKey, valuename, 0, &dwType, data, MAX_PATH-1, &cbData) == ERROR_SUCCESS)
									{
										printf("name: %s\t\tvalue:", valuename);
										switch(dwType)
										{
											case REG_SZ: printf("%s", (char*)data); break;
											case REG_BINARY: 
												 {
													for(int i = 0; i < cbData; i++)
													{
													 	printf("%02X", data[i]); if(i != cbData - 1) printf(" ");
													}
												 }
												 break;
											case REG_DWORD: printf("%lu", *(DWORD*)data); break;
											default : printf("(%d). length: %d", dwType, cbData); break;
										}
										printf("\n");
										memset(data, 0, sizeof(data));
										printf("\nnew value: ");
										string strValue;
										getline(cin, strValue);

										switch(dwType)
										{
											case REG_SZ:
												if(strValue.length() > 0)
													RegSetValueEx(
															hSubKey, valuename, 
															0, dwType, (LPBYTE)strValue.c_str(), strValue.length()+1); 
												break;
											case REG_DWORD:
												{
													if(strValue[0] >= '0' && strValue[0] <= '9')
													{
														DWORD dwValue = 0;
														sscanf(strValue.c_str(), "%d", &dwValue);
														RegSetValueEx(
																hSubKey, valuename, 
																0, dwType, (LPBYTE)&dwValue, sizeof(DWORD));
													}
												}
												break;
											default:
												printf("not implemented for this data type now!\n");

										}

									}
									else
										printf("cannot query value: %s\n", szKeyValueName);

									RegCloseKey(hSubKey);
								}
								else
									printf("failed to open key: %s\n", szSubKeyName);

								printf("\nENTER to continue ...");
								getline(cin, strCmd);
							}
						}
					}
				}
				break;
			default:
				c = 'p';
				break;
			}
		}
	}

	return 0;
}
