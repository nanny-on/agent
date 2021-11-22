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
#include "com_struct.h"
#include "../WhiteListDef.h"
#include "../../dll_lib/ASIWEngDLLUtil.h"

BOOL g_bCreateFlag = FALSE;
BOOL g_bScanFlag = FALSE;
char g_acWhitePath[MAX_PATH] = {0,};
char g_acScanPath[MAX_PATH] = {0,};

void usage(void)
{
	printf("Usage : white_test -s <scan path> [-c <white list path>] -h\n");
}

INT32 ScanByPathEx(FILE *fpWp, FILE *fpNp, FILE *fpTp, FILE *fpSip, char *acPath, char *acLogMsg)
{
	DWORD dwRetVal = 0;
	INT32 nRetVal = 0;
	CHAR szPathName[MAX_PATH] = {0,};
	CHAR szFileType[32] = {0,};
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	PASI_WENG_WL_EX pWhiteTest = NULL;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if(fpWp == NULL || fpNp == NULL || fpTp == NULL || fpSip == NULL || acPath == NULL || acPath[0] == 0)
	{
		snprintf(acLogMsg, 1023, "[ScanByPathEx] invalid input data");
		return -1;
	}

	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		snprintf(acLogMsg, 1023, "[ScanByPathEx] fail to allocate memory (%d)", errno);
		return -2;
	}

	pWhiteTest = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteTest == NULL)
	{
		snprintf(acLogMsg, 1023, "[ScanByPathEx] fail to allocate memory (%d)", errno);
		safe_free(pWhiteListEx);
		return -3;
	}

	dp = opendir(acPath);
	if (dp == NULL)
	{
		snprintf(acLogMsg, 1023, "[ScanByPathEx] fail to open %s (%d)", acPath, errno);
		safe_free(pWhiteListEx);
		safe_free(pWhiteTest);
		return -4;
	}

	while((dirp = readdir(dp)) != NULL)
	{

		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;
		nRetVal = 0;

		snprintf(szPathName, MAX_PATH-1, "%s/%s", acPath, dirp->d_name);
		if(DT_DIR == dirp->d_type)
		{
			nRetVal = ScanByPathEx(fpWp, fpNp, fpTp, fpSip, szPathName, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 10;
				break;
			}
			continue;
		}
		else if(DT_REG == dirp->d_type)
		{
			memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));
			memset(pWhiteTest, 0, sizeof(ASI_WENG_WL_EX));
			dwRetVal = 0;
			nRetVal = t_ASIWENGDLLUtil->GetWL(szPathName,(PVOID)pWhiteListEx, sizeof(ASI_WENG_WL_EX), &dwRetVal);
			if(nRetVal < 0)
			{
				printf("fail to get white hash %s (%d)\n", szPathName, nRetVal);
				continue;
			}
			if(dwRetVal != AS_INVALID_FILE)
			{
				if(pWhiteListEx->dwMasic == 0x3E)
					strncpy(szFileType, "x86-x64", 31);
				else if(pWhiteListEx->dwMasic == 0x03)
					strncpy(szFileType, "x86", 31);
				else if(pWhiteListEx->dwMasic == 0x08)
					strncpy(szFileType, "MIPS", 31);
				else if(pWhiteListEx->dwMasic == 0x28)
					strncpy(szFileType, "ARM", 31);
				else if(pWhiteListEx->dwMasic == 0x32)
					strncpy(szFileType, "IA-64", 31);
				else if(pWhiteListEx->dwMasic == 0xB7)
					strncpy(szFileType, "AArch64", 31);
				else if(pWhiteListEx->dwMasic == 0xFF)
					strncpy(szFileType, "Scripts", 31);
				else
					snprintf(szFileType, 31, "etc (%x)", pWhiteListEx->dwMasic);
				szFileType[31] = 0;
			}

			if(dwRetVal == AS_WHITE_FILE)
			{
				strncpy(pWhiteTest->acWhiteHash, pWhiteListEx->acWhiteHash, SHA512_BLOCK_SIZE+1);
				pWhiteTest->acWhiteHash[SHA512_BLOCK_SIZE];
				nRetVal = t_ASIWENGDLLUtil->GetWL(NULL,(PVOID)pWhiteTest, sizeof(ASI_WENG_WL_EX), &dwRetVal);
				if(nRetVal < 0 || dwRetVal != AS_WHITE_FILE)
				{
					printf("invalid white hash (pt:%s) (pt:%s) ret:%d\n", szPathName, pWhiteTest->acWhiteHash, nRetVal);
				}
				//				printf("[ScanByPath] (%s) %s is white file : db file (%s : %s)\n", (pWhiteList->dwMasic == 0x020b) ? "x64":"x86", szPathName, pWhiteList->acFileName, pWhiteList->acDbName);
				fprintf(fpWp, "(%s) %s : db file (%s : %s) cat (%d)\n", szFileType, szPathName, pWhiteListEx->acFilePath, pWhiteListEx->acWhiteHash, pWhiteListEx->dwCategory);
				fprintf(fpSip, "(%s) %s : (size : %d) (wt : %d)\n", szFileType, szPathName, pWhiteListEx->dwFileSize, pWhiteListEx->dwWriteTime);
			}
			else if(dwRetVal == AS_ELF_FILE)
			{
				//				printf("[ScanByPath] (%s) %s is normal file\n", (pWhiteList->dwMasic == 0x020b) ? "x64":"x86", szPathName);
				fprintf(fpNp, "(%s) %s\n", szFileType, szPathName);
				fprintf(fpSip, "(%s) %s : (size : %d) (wt : %d)\n", szFileType, szPathName, pWhiteListEx->dwFileSize, pWhiteListEx->dwWriteTime);
			}
			else
			{
				//				printf("[ScanByPath] fail to scan %s : %s\n", szPathName, acErrMsg);
				fprintf(fpTp, "%s\n", szPathName);
			}
		}
	}
	closedir(dp);
	safe_free(pWhiteListEx);
	safe_free(pWhiteTest);
	return nRetVal;
}



INT32 CreateDbByPath(char *acWhitePath, char *acDbPath, char *acDbName, BOOL bCreateDb, char *acLogMsg)
{
	INT32 nRetVal = 0;
	CHAR szPathName[MAX_PATH] = {0,};
	INT32 nCount = 0, i=0;
	char acHash[SHA512_BLOCK_SIZE+1] = {0,};
	BOOL bDeleteTmpFile = TRUE;
	DWORD dwFileType = 0;
	PASI_WENG_WL_EX pWhiteTest = NULL;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	if(acWhitePath == NULL || acWhitePath[0] == 0 || acDbPath == NULL || acDbPath[0] == 0)
	{
		snprintf(acLogMsg, 1023, "[CreateDbByPath] invalid input data");
		return -1;
	}

	pWhiteTest = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteTest == NULL)
	{
		snprintf(acLogMsg, 1023, "[CreateDbByPath] fail to allocate memory (%d)", errno);
		return -2;
	}

	dp = opendir(acWhitePath);
	if (dp == NULL)
	{
		snprintf(acLogMsg, 1023, "[CreateDbByPath] fail to open %s (%d)", acWhitePath, errno);
		return -3;
	}


	while((dirp = readdir(dp)) != NULL)
	{
		nRetVal = 0;
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;

		snprintf(szPathName, MAX_PATH-1, "%s/%s", acWhitePath, dirp->d_name);
		if(DT_DIR == dirp->d_type)
		{
			nRetVal = CreateDbByPath(szPathName, acDbPath, acDbName, FALSE, acLogMsg);
			if(nRetVal < 0)
			{
				break;
			}
			continue;
		}
		else if(DT_REG == dirp->d_type)
		{
			memset(pWhiteTest, 0, sizeof(ASI_WENG_WL_EX));
			nRetVal = t_ASIWENGDLLUtil->AddWL(szPathName, pWhiteTest, sizeof(ASI_WENG_WL_EX));
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] add : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}
			strncpy(pWhiteTest->acDbName, "test db", 16);

			nRetVal = t_ASIWENGDLLUtil->ModWL(pWhiteTest, sizeof(ASI_WENG_WL_EX));
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] mod : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}

			nRetVal = t_ASIWENGDLLUtil->DelWL(szPathName, pWhiteTest, sizeof(ASI_WENG_WL_EX));
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] del : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}

			nRetVal = t_ASIWENGDLLUtil->DelWL(szPathName, pWhiteTest, sizeof(ASI_WENG_WL_EX));
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] del : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}

			nRetVal = t_ASIWENGDLLUtil->AddWL(NULL, pWhiteTest, sizeof(ASI_WENG_WL_EX));
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] add : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}
			strncpy(acHash, pWhiteTest->acWhiteHash, SHA512_BLOCK_SIZE+1);
			acHash[SHA512_BLOCK_SIZE] = 0;
			memset(pWhiteTest, 0, sizeof(ASI_WENG_WL_EX));
			memcpy(pWhiteTest->acWhiteHash, acHash, SHA512_BLOCK_SIZE);
			nRetVal = t_ASIWENGDLLUtil->GetWL(NULL, pWhiteTest, sizeof(ASI_WENG_WL_EX), &dwFileType);
			if(nRetVal < 0)
			{
				printf("[CreateDbByPath] get : %s %d : %s\n", szPathName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				continue;
			}
			printf("%s type %d\n", szPathName, dwFileType);
		}
	}

	closedir(dp);
	Sleep(10);
	safe_free(pWhiteTest);

	if(bCreateDb == TRUE)
	{
		nRetVal = t_ASIWENGDLLUtil->GetWLCnt(NULL, 0, &nCount);
		if(nRetVal < 0)
		{
			printf("fail to get white list count : %s\n", acLogMsg);
			return nRetVal;
		}
		printf("get white list count : %d\n", nCount);

		if(nCount > 0)
		{
			pWhiteTest = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX)*nCount);
			if(pWhiteTest != NULL)
			{
				memset(pWhiteTest, 0, sizeof(ASI_WENG_WL_EX)*nCount);
				nRetVal = t_ASIWENGDLLUtil->GetAllWL(NULL, 0, nCount, pWhiteTest, sizeof(ASI_WENG_WL_EX));
				if(nRetVal < 0)
				{
					printf("fail to get white all list : %s\n", acLogMsg);
				}
				free(pWhiteTest);
			}
		}
		nRetVal = t_ASIWENGDLLUtil->MakeDB(acDbPath, acDbName);
		t_ASIWENGDLLUtil->ClearFile();
		if(nRetVal < 0)
		{
			printf("[CreateDbByPath] fail to create db %s %s (%d) : %s\n", acDbPath, acDbName, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
		}
	}

	return nRetVal;
}

int ProcessArgs(int argc, char* argv[])
{
	int nRetVal = 0;

	if(argc < 2)
	{
		usage();
		return 0;
	}
	if(!_stricmp(argv[1], "-c"))
	{
		if(argc >= 3)
		{   
			nRetVal = 1;
			g_bCreateFlag = TRUE;
			strncpy(g_acWhitePath, argv[2], MAX_PATH-1);
			g_acWhitePath[MAX_PATH-1] = 0;
		}
	}
	else if(strcmp(argv[1], "-s") == 0)
	{
		if(argc >= 3)
		{
			nRetVal = 1;
			g_bScanFlag = TRUE;
			strncpy(g_acScanPath, argv[2], MAX_PATH-1);
			g_acScanPath[MAX_PATH-1] = 0;
		}
	}
	if(nRetVal == 0)
		usage();
	return nRetVal;
}


int main(int argc, CHAR* argv[])
{
	int nRetVal = 0;
	char acPath[MAX_PATH] = {0,};
	char acDllPath[MAX_PATH] = {0,};
	char acDbPath[MAX_PATH] = {0,};
	char acDbFile[MAX_PATH] = {0,};
	char acOutputPath[MAX_PATH] = {0,};
	char acWhiteElfFile[MAX_PATH] = {0,};
	char acNormalElfFile[MAX_PATH] = {0,};
	char acNotElfFile[MAX_PATH] = {0,};
	char acScanInfoFile[MAX_PATH] = {0,};
	char acLogMsg[1024] = {0,};
	INT32 i, nCount = 0;
	FILE *fpSip = NULL;
	FILE *fpWp = NULL;
	FILE *fpNp = NULL;
	FILE *fpTp = NULL;
	FILE *fpLog = NULL;

	PWHITE_PARSE_DATA pRuleData = NULL;
	PASI_WENG_WL_EX pWhiteList = NULL;
	PWHITE_FILE_HDR pFileHdr = NULL;
	if(ProcessArgs(argc, argv) == 0)
	{
		return 0;
	}


	if(realpath(".", acPath) == NULL)
	{
		printf("fail to get real path (%d)\n", errno);
		return -2;
	}

	snprintf(acDbPath, MAX_PATH-1, "%s/wdb", acPath);
	if(is_file(acDbPath) < 0)
	{
		if(CreateDirectory(acDbPath, FALSE) == FALSE)
		{
			printf("fail to create path (%s) (%d)\n", acDbPath, errno);
			return -3;
		}
	}
	snprintf(acOutputPath, MAX_PATH-1, "%s/out", acPath);
	if(is_file(acOutputPath) < 0)
	{
		if(CreateDirectory(acOutputPath, NULL) == FALSE)
		{
			printf("fail to create path (%s) (%d)\n", acOutputPath, errno);
			return -4;
		}
	}

	snprintf(acWhiteElfFile, MAX_PATH-1, "%s/white_elf.txt", acOutputPath);
	snprintf(acNormalElfFile, MAX_PATH-1, "%s/normal_elf.txt", acOutputPath);
	snprintf(acNotElfFile, MAX_PATH-1, "%s/not_elf.txt", acOutputPath);
	snprintf(acScanInfoFile, MAX_PATH-1, "%s/scan_info.txt", acOutputPath);
	
	do{
		t_ASIWENGDLLUtil			= new CASIWEngDLLUtil();
		if(t_ASIWENGDLLUtil == NULL)
		{
			printf("fail to alllocate memory (%d)\n", errno);
			nRetVal = -5;
			break;
		}

		snprintf(acDllPath, MAX_PATH-1, "%s/asi_weng.so", acPath);
		nRetVal = t_ASIWENGDLLUtil->LoadLibraryExt(acDllPath);
		if(nRetVal != 0)
		{
			printf("fail to load library : [%s][%d]\n", acDllPath, nRetVal);
			nRetVal = -6;
			break;
		}

		t_ASIWENGDLLUtil->Init();
		t_ASIWENGDLLUtil->InitDB();

		if(g_bCreateFlag == TRUE)
		{
			if(g_acWhitePath[0] == 0)
			{
				printf("invalid white list path\n");
				nRetVal = -7;
				break;
			}
			nRetVal = CreateDbByPath(g_acWhitePath, acDbPath, "as_patten", TRUE, acLogMsg);
			if(nRetVal < 0)
			{
				printf("fail to create db : %s (%d)\n", acLogMsg, nRetVal);
				nRetVal -= 10;
				break;
			}
			Sleep(100);
		}

		if(g_bScanFlag == TRUE)
		{
			if(g_acScanPath[0] == 0)
			{
				printf("invalid scan path\n");
				nRetVal = -7;
				break;
			}

			fpWp = fopen(acWhiteElfFile, "wt");
			fpNp = fopen(acNormalElfFile, "wt");
			fpTp = fopen(acNotElfFile, "wt");
			fpSip = fopen(acScanInfoFile, "wt");
			if(fpWp == NULL || fpNp == NULL || fpTp == NULL || fpSip == NULL)
			{
				printf("fail to open %s %s %s %s (%d)\n", acWhiteElfFile, acNormalElfFile, acNotElfFile, acScanInfoFile, errno);
				nRetVal = -8;
				break;
			}
			//if(g_we_load_db(acDbPath, acLogMsg) == FALSE)
/*
			pRuleData = (PWHITE_PARSE_DATA)malloc(sizeof(WHITE_PARSE_DATA));
			if(pRuleData != NULL)
			{
				memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));
				pRuleData->dwParseFlag = VC_PARSE_FILE_PATH;
				snprintf(pRuleData->acPathRule, PATH_LENGTH-1, "E:\\util\\util\\*");
//				pRuleData->dwCategory = VC_GLOBAL_WHITE;
				//pRuleData->dwMasic = 0x010B;
			}
			*/
			nRetVal = t_ASIWENGDLLUtil->LoadDB(acDbPath);
			if(nRetVal < 0)
			{
				printf("fail to load we : %s %d : %s\n", acDbPath, nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				nRetVal -= 20;
				break;
			}
			nCount = 0;
			nRetVal = t_ASIWENGDLLUtil->GetWLCnt(NULL, 0, &nCount);
			if(nRetVal < 0)
			{
				printf("fail to get cnt: %d : %s\n", nRetVal, t_ASIWENGDLLUtil->m_szErrMsg);
				nRetVal -= 30;
				break;
			}
			printf("get cnt: %d\n", nCount);
			//modify test
			/*
			memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));
			dwCount = 0;
			pRuleData->dwParseFlag = VC_PARSE_FILE_PATH;
			snprintf(pRuleData->acPathRule, PATH_LENGTH-1, "E:\\util\\util\\IceSword\\*");
			if(g_we_get_white_list_count(NULL, 0, &dwCount, acLogMsg) == FALSE)
			{
				printf("fail to get white list count : %s\n", acLogMsg);
				break;
			}
			if(dwCount != 0)
			{
				pWhiteList = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX)*dwCount);
				if(pWhiteList == NULL)
				{
					printf("fail to allocate memory %d\n", errno);
					break;
				}
				memset(pWhiteList, 0, sizeof(ASI_WENG_WL_EX)*dwCount);
				if(g_we_get_all_white_list(NULL, 0, dwCount, pWhiteList, sizeof(ASI_WENG_WL_EX), acLogMsg) == FALSE)
				{
					printf("fail to get white all list : %s\n", acLogMsg);
					break;
				}

				for(i=0; i<dwCount; i++)
				{
					printf("%d %d\n", i, pWhiteList[i].dwCategory);
				}
				if(g_we_del_white_list(NULL, &pWhiteList[2], sizeof(ASI_WENG_WL_EX), acLogMsg) == FALSE)
				{
					printf("fail to del list : %s\n", acLogMsg);
					break;
				}
				memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));
				dwCount = 0;
				pRuleData->dwParseFlag = VC_PARSE_FILE_PATH;
				snprintf(pRuleData->acPathRule, PATH_LENGTH-1, "E:\\util\\util\\IceSword\\*.exe");

				if(g_we_make_wdb("E:\\svnroot\\vc80d\\sdk\\wecorex\\bin\\debug\\wdb\\test.wdb", VC_GLOBAL_WHITE, NULL,0, &dwCount, acLogMsg) == FALSE)
				{
					printf("fail to make db : %s\n", acLogMsg);
					break;
				}
			}
			*/
			
			nRetVal = ScanByPathEx(fpWp, fpNp, fpTp, fpSip, g_acScanPath, acLogMsg);
			if(nRetVal < 0)
			{
				printf("fail to scan path : %s (%d)\n", acLogMsg, nRetVal);
				nRetVal -= 30;
				break;
			}
/*
			dwCount = 0;
			if(g_we_get_wdb_count(acDbPath, &dwCount, acLogMsg) == FALSE)
			{
				printf("fail to get wdb count : %s\n", acLogMsg);
				break;
			}

			if(dwCount > 0)
			{
				pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR)*dwCount);
				if(pFileHdr == NULL)
				{
					printf("fail to allocate memory %d\n", errno);
					break;
				}
				memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR)*dwCount);
				if(g_we_get_all_wdb_hdr(acDbPath, dwCount, pFileHdr, sizeof(WHITE_FILE_HDR), acLogMsg) == FALSE)
				{
					printf("fail to get wdb file header : %s\n", acLogMsg);
					break;
				}
				if(g_we_get_wdb_hdr("E:\\svnroot\\vc80d\\sdk\\wecorex\\bin\\debug\\wdb\\test.wdb", &pFileHdr[1], sizeof(WHITE_FILE_HDR), acLogMsg) == FALSE)
				{
					printf("fail to get wdb file header : %s\n", acLogMsg);
					break;
				}
			}
*/
		}
	}while(FALSE);

	safe_free(pRuleData);
	safe_free(pWhiteList);
	safe_free(pFileHdr);

	t_ASIWENGDLLUtil->UnInitDB();
	t_ASIWENGDLLUtil->Free();

	SAFE_DELETE(t_ASIWENGDLLUtil);

	if(fpWp != NULL)
		fclose(fpWp);
	if(fpNp != NULL)
		fclose(fpNp);
	if(fpTp != NULL)
		fclose(fpTp);
	if(fpSip != NULL)
		fclose(fpSip);
	if(fpLog != NULL)
		fclose(fpLog);
	return 0;
}