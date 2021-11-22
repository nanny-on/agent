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
#include "asi_sdk_define_system.h"
#include "as_file.h"
#include "as_parse.h"
#include "as_sysinfo.h"


#define F_CPU_INFO							"/proc/cpuinfo"
#define F_BIOS_DATE_INFO					"/sys/class/dmi/id/bios_date"
#define CMD_ARG_TOT_DISKSIZE				"/sbin/fdisk -l"
#define CMD_ARG_NETIFACE_COUNT_HWADDR		"/sbin/ifconfig -a | grep inet | wc -l"
#define CMD_ARG_NETIFACE_COUNT_ETHER		"/sbin/ifconfig -a | grep ether | wc -l"

/* cpu μ½??κ°?λ?λ°??*/
int get_cpu_core_num(void)
{
	int  num = 0;

	num = sysconf(_SC_NPROCESSORS_CONF);
	if (num < 0)
	{
		return -1;
	}

	return num;
}


/* cpu ?€???λ°??*/
int get_cpu_type(char *cpu_type, int len)
{
	FILE  *fp = NULL;
	int   slen;
	char  *cpu_type_str = NULL;
	char  buf[256] = { 0,};

	if ( (fp = fopen(F_CPU_INFO, "r")) == NULL)
	{
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp))
	{
		slen = strlen(buf);
		buf[slen - 1] = '\0';
		if (strstr(buf, "model name") == NULL)
		{
			memset(buf, 0x00, sizeof(buf));
			continue;
		}
		else
		{
			if ( (cpu_type_str = strchr(buf, ':')) == NULL) {
				break;
			}

			cpu_type_str += 2;
			trim_right_white_space(cpu_type_str);
			snprintf(cpu_type, len, "%s", cpu_type_str);
			break;
		}
	}

	fclose(fp);

	if (cpu_type_str == NULL)
	{
		return -2;
	}
	return 0;
}

/* cpu ??„λ?MHz ?¨μ„λ΅?λ°??*/
int get_cpu_speed(void)
{
	FILE  *fp = NULL;
	char  *cpu_spd_str = NULL;
	char  buf[256] = { 0,};

	if ( (fp = fopen(F_CPU_INFO, "r")) == NULL)
	{
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp))
	{
		if (strstr(buf, "cpu MHz") == NULL)
		{
			memset(buf, 0x00, sizeof(buf));
			continue;
		}
		else
		{
			if ( (cpu_spd_str = strchr(buf, ':')) == NULL)
			{
				break;
			}
			cpu_spd_str += 2;
			break;
		}
	}

	fclose(fp);

	if (cpu_spd_str == NULL)
	{
		return -1;
	}
	return atoi(cpu_spd_str);
}


/* bios date */
int get_bios_date(char *pcBiosDate, int nMaxLen)
{
	FILE  *fp = NULL;
	int nRetVal = 0;
	char  buf[256] = { 0,};

	if(pcBiosDate == NULL || nMaxLen < 1)
	{
		return -1;
	}

	do{
		fp = fopen(F_BIOS_DATE_INFO, "r");
		if(fp == NULL)
		{
			nRetVal = -2;
			break;
		}
		fgets(buf, sizeof(buf), fp);
		if(buf[0] == 0)
		{
			nRetVal = -3;
			break;
		}
		trim_right_white_space(buf);
		strncpy(pcBiosDate, buf, nMaxLen-1);
		nRetVal = 0;
	}while(FALSE);

	if(fp != NULL)
		fclose(fp);

	return nRetVal;
}


/* ??¤ν… λ©”λª¨λ¦??©λ‰??MB ?¨μ„λ΅?λ°??*/
int get_total_mem(void)
{
	int nTotalMem = 0;
	struct sysinfo  info;

	if (sysinfo(&info) == -1)
	{
		return -1;
	}

	nTotalMem = (info.totalram) / (KBYTES * KBYTES);

	return nTotalMem;
}


/* μ΄???¤ν??©λ‰??MB ?¨μ„λ΅?λ°??*/
int get_total_disk_size(void)
{
	FILE    *fp_cmd = NULL;
	int     slen;
	int     mb_size;
	char    *pt = NULL;
	char    buf[128] = { 0,};
	char    dummy[12];
	double  disk_size = 0;
	double  total_size = 0;

	if ( (fp_cmd = popen(CMD_ARG_TOT_DISKSIZE, "r")) == NULL)
	{
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp_cmd))
	{
		slen = strlen(buf);
		buf[slen - 1] = '\0';
		if (strncmp(buf, "Disk identifier", 15) == 0)
		{
			memset(buf, 0x00, sizeof(buf));
			continue;
		}

		if (strncmp(buf, "Disk /dev/mapper", 16) == 0)
		{
			memset(buf, 0x00, sizeof(buf));
			continue;
		}

		if (strncmp(buf, "Disk", 4) == 0)
		{
			if ( (pt = strstr(buf, ", ")) == NULL)
			{
				memset(buf, 0x00, sizeof(buf));
				continue;
			}

			pt += 2;

			sscanf(pt, "%lf %s", &disk_size, dummy);
			total_size += disk_size;
		}
	}

	total_size /= (KBYTES * KBYTES);
	mb_size = (int) total_size;

	pclose(fp_cmd);
	return mb_size;
}


/* ?¤νΈ????Έν„°??΄μ?κ°?λ?λ°??*/
int get_netiface_count(void)
{
	FILE  *fp_cmd = NULL;
	char  buf[8] = { 0,};

	if ( (fp_cmd = popen(CMD_ARG_NETIFACE_COUNT_HWADDR, "r")) == NULL)
	{
		return -1;
	}

	fgets(buf, sizeof(buf), fp_cmd);
	pclose(fp_cmd);

	/* Linux λ°°ν¬????°λ?μ²«λ?μ§?μ»¤λ§¨?λ΅ ?¤νΈ????Έν„°??΄μ¤??
	 * κ°?λ???Έν•μ§€ λª»ν•  ?????Όλ?λ΅??λ²μ§?μ»¤λ§¨?λ΅ ?λ² ??
	 * ??Έν•??	 */
	if (atoi(buf) == 0)
	{
		if ( (fp_cmd = popen(CMD_ARG_NETIFACE_COUNT_ETHER, "r")) == NULL)
		{
			return -2;
		}

		memset(buf, 0x00, sizeof(buf));
		fgets(buf, sizeof(buf), fp_cmd);
		pclose(fp_cmd);
	}

	return atoi(buf);
}


BOOL path_is_separator(const char* path)
{
	if(path == NULL)
		return FALSE;
	return *path == '/';
}

void path_append_separator(char* path, size_t len)
{
	if(path != NULL)
		strncat(path, "/", len);
}


char* ac_path_join_array(char* pDest, int nLen, int nNumParts, const char** ppParts)
{
    const char* pPart;
    int i = 0;
    int nIdx = 0;
    int nPartLen = 0;
	if(pDest == NULL || nLen < 1 || ppParts == NULL)
		return NULL;

    pDest[0] = '\0';
	pPart = ppParts[0];
	while(pPart != NULL && i < nNumParts)
	{
        nPartLen = (int)strlen(pPart);
        if(nPartLen > nLen)
            return NULL;

        /* If previous part was not finished by separator, add it*/
        if(i != 0 && !path_is_separator(pDest + nIdx - 1))
		{
        	path_append_separator(pDest, nLen);

            nLen -= PATH_SEP_LENGTH;
            nIdx += PATH_SEP_LENGTH;
        }

        strncat(pDest + nIdx, pPart, nLen);

        nIdx += nPartLen;
        nLen -= nPartLen;

        pPart = ppParts[++i];
    }

	return pDest;
}


char* as_path_join(char** ppPath, ...)
{
	va_list va;
	int i = 0;
	int nCount = 0;
	const char* acParts[PATH_MAX_PARTS] = {0,};
	const char* pPart = NULL;
	if(ppPath == NULL)
		return NULL;

	va_start(va, ppPath);
	do {
		pPart = va_arg(va, const char*);
		acParts[i++] = pPart;

		if(pPart) {
			nCount += (int)strlen(pPart) + PATH_SEP_LENGTH;
		}

		if(i == PATH_MAX_PARTS)
			return NULL;
	} while(pPart != NULL);
	va_end(va);

	if(nCount < 1)
		return NULL;
	
	*ppPath = (char *)malloc(nCount+1);
	if(*ppPath == NULL)
		return NULL;
	memset(*ppPath, 0, nCount+1);
	
	return ac_path_join_array(*ppPath, nCount + 1, i, acParts);
}

int as_linux_sysfs_readstr(const char *pRoot, const char *pName, const char *pObject, char *acStr, int nMaxLen)
{
	char *pPath = NULL;
	int fd = 0;
	int ret = 0;
	int nReadLen = 0;

	if(pRoot == NULL || pName == NULL || acStr == NULL || nMaxLen < 1)
		return -1;

	do{
		if(as_path_join(&pPath, pRoot, pName, pObject, NULL) == NULL)
		{
			ret = -2;
			break;
		}

		if(pPath == NULL)
		{
			ret = -3;
			break;
		}

		fd = open(pPath, O_RDONLY);
		if(fd == -1)
		{
			ret = -4;
			break;
		}
		nReadLen = (int)read(fd, acStr, nMaxLen - 1);
		if(nReadLen > nMaxLen - 1)
			nReadLen = nMaxLen - 1;
		acStr[nReadLen] = '\0';
		close(fd);
		ret = 0;
	}while(FALSE);

	safe_free(pPath);
	return ret;
}

void as_linux_sysfs_fixstr(char* p)
{
	if(p == NULL)
		return;

	while(*p)
	{
		if(*p == '\n')
			*p = ' ';
		++p;
	}
}


int get_system_name(char *acSystemName, int nMaxLen)
{
	char acVendor[MAX_QQBUFF] = {0,};
	char acModel[MAX_QQBUFF] = {0,};
	int ret = 0;;

	if(acSystemName == NULL || nMaxLen < 1)
		return -1;
	
	do{
		ret = as_linux_sysfs_readstr(SYS_DMI_PATH, "sys_vendor", NULL, acVendor, MAX_QQBUFF-1);
		if(ret != 0)
		{
			break;
		}
		as_linux_sysfs_fixstr(acVendor);

		as_linux_sysfs_readstr(SYS_DMI_PATH, "product_name", NULL, acModel, 128);
		if(ret != 0)
		{
			break;
		}
		as_linux_sysfs_fixstr(acModel);

		snprintf(acSystemName, nMaxLen-1, "%s %s", acVendor, acModel);
		ret = 0;
	}while(FALSE);
	return ret;
}


int read_redhat_release(char *acOsName, int nMaxLen, UINT64 *pddwOsID, UINT32 *pdwOsType)
{
	FILE* fp = NULL;
	char acFirst[MAX_QHBUFF] = {0,};
	int nLen = 0;;
	int i, j;
	UINT64 ddwOsID = 0;

	if(acOsName == NULL || nMaxLen < 1 || pddwOsID == NULL || pdwOsType == NULL)
		return -1;
	if(is_file("/etc/oracle-release") == REG_FILE)
	{
		fp = fopen("/etc/oracle-release", "r");
		ddwOsID = ASI_LINUX_SYSTEM_ID_ORACLE;
	}
	else if(is_file("/etc/centos-release") == REG_FILE)
	{
		fp = fopen("/etc/centos-release", "r");
		ddwOsID = ASI_LINUX_SYSTEM_ID_CENTOS;
	}
	else
	{
		fp = fopen("/etc/redhat-release", "r");
		ddwOsID = ASI_LINUX_SYSTEM_ID_REDHAT;
	}
	if(fp == NULL)
		return -2;

	fgets(acFirst, MAX_QHBUFF-1, fp);
	nLen = (int)strlen(acFirst);

	fclose(fp);

	if(nLen < 1)
		return -3;

	for(i = 0, j = 0; i < nLen - 2 && j < 64; ++i, ++j)
	{
		if(acFirst[i] == 'r' && (strncmp(acFirst + i, "release ", 8) == 0))
		{
			i += 8;
		}

		if(acFirst[i] == '(')
			break;
		
		if(j >= nMaxLen-1)
			break;
		acOsName[j] = acFirst[i];
	}

	acOsName[j] = '\0';
	*pddwOsID = ddwOsID;
	*pdwOsType = ASI_SYSTEM_TYPE_SERVER;
	return 0;
}

int read_suse_release(char *acOsName, int nMaxLen, UINT64 *pddwOsID, UINT32 *pdwOsType)
{
	FILE* fp = NULL;
	char acFirst[MAX_QHBUFF] = {0,};
	int nLen = 0;
	char* pEnd = NULL;

	if(acOsName == NULL || nMaxLen < 1 || pddwOsID == NULL || pdwOsType == NULL)
		return -1;

	fp = fopen("/etc/SuSE-release", "r");
	if(fp == NULL)
		return -2;

	fgets(acFirst, MAX_QHBUFF-1, fp);
	nLen = (int)strlen(acFirst);
	fclose(fp);

	if(nLen < 1 || nLen > MAX_QHBUFF - 1)
		return -3;

	pEnd = strchr(acFirst, '(');
	if(!pEnd || pEnd == acFirst)
	{
		pEnd = acFirst + nLen;
	}
	*--pEnd = '\0';

	strncpy(acOsName, acFirst, nMaxLen-1);
	*pddwOsID = ASI_LINUX_SYSTEM_ID_SUSE;
	*pdwOsType = ASI_SYSTEM_TYPE_SERVER;
	return 0;
}


int read_lsb_release_file(UINT64 *pddwOsID)
{
	FILE* fp = NULL;
	char acFirst[MAX_QHBUFF] = {0,};
	int nLen = 0;
	char* pEnd = NULL;

	if(pddwOsID == NULL)
		return -1;

	fp = fopen("/etc/lsb-release", "r");
	if(fp == NULL)
		return -2;

	fgets(acFirst, MAX_QHBUFF-1, fp);
	nLen = (int)strlen(acFirst);
	fclose(fp);

	if(nLen < 1 || nLen > MAX_QHBUFF - 1)
		return -3;

	if(acFirst[nLen-1] == '\n')
		acFirst[nLen-1] = '\0';

	pEnd = strchr(acFirst, '=');
	if(pEnd == NULL || pEnd == acFirst)
	{
		return -4;
	}
	pEnd++;
	if(pEnd[0] == 0)
	{
		return -5;
	}
	if(_stricmp(pEnd, LINUX_GOOROOM_NAME))
	{
		return -6;
	}
	*pddwOsID = ASI_LINUX_SYSTEM_ID_GOOROOM;
	return 0;
}

int read_lsb_release(char *acOsName, int nMaxLen, UINT64 *pddwOsID, UINT32 *pdwOsType)
{
	FILE* pPipe = NULL;
	char acFirst[MAX_QHBUFF] = {0,};
	char *pDescr, *pEnd;
	int i, nLen, nTail;

	int ret = 0;

	if(acOsName == NULL || nMaxLen < 1 || pddwOsID == NULL || pdwOsType == NULL)
		return -1;

	/* pipe() or fork() failed, failure! */
	do{
		pPipe = popen("/usr/bin/lsb_release -d -s", "r");
		if(pPipe == NULL)
		{
			ret = -2;
			break;
		}

		fgets(acFirst, MAX_QHBUFF-1, pPipe);
		nLen = strlen(acFirst);
		pclose(pPipe);
		if(nLen < 1)
		{
			ret = -3;
			break;
		}

		/*Success, ignore first quotes*/
		pDescr = acFirst;
		if(*pDescr == '"')
			++pDescr;

		nTail = nLen - 1;

		/*Remove trailing quotes and CRs*/
		while(pDescr[nTail] == '"' || pDescr[nTail] == '\n')
		{
			pDescr[nTail] = '\0';
			--nTail;
		}

		/* Cut out architecture label, or at least CR */
		pEnd = strchr(pDescr, '(');
		if(!pEnd || pEnd == pDescr)
		{
			pEnd = pDescr + nLen;
		}
		*--pEnd = '\0';

		nLen = (int)strlen(pDescr);
		if(nLen < 1)
		{
			ret = -4;
			break;
		}

		strncpy(acOsName, pDescr, nMaxLen-1);
		for(i=0; i<nLen; i++)
		{
			if(pDescr[i] == ' ')
			{
				pDescr[i] = '\0';
				break;
			}
		}
		if(!_stricmp(pDescr, LINUX_UBUNTU_NAME))
		{
			*pddwOsID = ASI_LINUX_SYSTEM_ID_UBUNTU;
			ret = 0;
		}
		else if(!_stricmp(pDescr, LINUX_TMAXOS_NAME))
		{
			*pddwOsID = ASI_LINUX_SYSTEM_ID_TMAXOS;
			ret = 0;
		}
		else if(!_stricmp(pDescr, LINUX_HAMONIKR_NAME))
		{
			*pddwOsID = ASI_LINUX_SYSTEM_ID_HAMONIKR;
			ret = 0;
		}
		else
		{
			ret = 5;
		}
	}while(FALSE);

	if(ret != 0)
	{
		if(read_lsb_release_file(pddwOsID) != 0)
			*pddwOsID = ASI_LINUX_SYSTEM_ID_DEBIAN;
	}
	*pdwOsType = ASI_SYSTEM_TYPE_WORKSTATION;

	return 0;
}


void get_os_info(char *acOsName, int nMaxLen, UINT64 *pddwOsID, UINT32 *pdwOsType)
{
	if(acOsName == NULL || nMaxLen < 1 || pddwOsID == NULL || pdwOsType == NULL)
		return;
	if(read_redhat_release(acOsName, nMaxLen, pddwOsID, pdwOsType) == 0)
		return;
	if(read_suse_release(acOsName, nMaxLen, pddwOsID, pdwOsType) == 0)
	   return;
	if(read_lsb_release(acOsName, nMaxLen, pddwOsID, pdwOsType) == 0)
		return;
	strncpy(acOsName, "Unknown Linux", nMaxLen-1);
	*pddwOsID = ASI_LINUX_SYSTEM_ID_UNKNOWN;
	*pdwOsType = ASI_SYSTEM_TYPE_SERVER;
}


