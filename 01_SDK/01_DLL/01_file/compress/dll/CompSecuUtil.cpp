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

// CompSecuUtil.cpp: implementation of the CCompSecuUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "as_sha256.h"
#include "as_seedx.h"
#include "as_file.h"
#include "as_seed.h"
#include "CompUtil.h"
#include "CompSecuUtil.h"
#include "FileUtil.h"
#include "CompMemToken.h"
#include "FileToken.h"


CCompSecuUtil::CCompSecuUtil()
{
	m_fNotifyState		= 0;
	lpCallBackHandle	= NULL;
	m_nInitEngine		= InitEngine();
}
//-----------------------------------------------------------------------------------

CCompSecuUtil::~CCompSecuUtil()
{

}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::InitEngine()
{
	if (lzo_init() != LZO_E_OK)
    {
        return 0;
    }
	return 1;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::SetNotifyState(PVOID lParam, PVOID NotifyState)
{
	lpCallBackHandle	= lParam;
	m_fNotifyState		= (NotifyStateType)NotifyState;
	return 0;
}
//-----------------------------------------------------------------------------------

String		CCompSecuUtil::GetSHA2(PBYTE pBuff, INT32 nLen)
{
	String strHash;
	CHAR szHash[CHAR_MAX_SIZE] = {0, };
	if(pBuff == NULL || nLen < 1)
		return "";

	if(get_sha256_hash((char *)pBuff, nLen, szHash, CHAR_MAX_SIZE) != 0)
		return "";

	strHash = szHash;
	return strHash;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::WriteHdr(ASI_COMP_SECU_FILE_HDR& tACSFH, INT32 nTarFd)
{
	if(nTarFd == -1)
		return -1;
	
	FileToken tFileToken(nTarFd);

	if(tFileToken.TokenAdd_32(tACSFH.nCompVer) == FALSE)
		return -2;
	if(tFileToken.TokenAdd_32(tACSFH.nEncType) == FALSE)
		return -3;
	if(tFileToken.TokenAdd_Char(tACSFH.acEncHash) == FALSE)
		return -4;
	if(tFileToken.TokenAdd_Char(tACSFH.acRelativePath) == FALSE)
		return -5;
	if(tFileToken.TokenAdd_Char(tACSFH.acFileName) == FALSE)
		return -6;
	if(tFileToken.TokenAdd_64(tACSFH.nFileSize) == FALSE)
		return -7;
	if(tFileToken.TokenAdd_32(tACSFH.nFileMode) == FALSE)
		return -8;
	if(tFileToken.TokenAdd_32(tACSFH.nTokenSize) == FALSE)
		return -9;

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::ReadHdr(ASI_COMP_SECU_FILE_HDR& tACSFH, INT32 nSrcFd)
{
	INT32 nRetVal = 0;
	if(nSrcFd == -1)
		return -1;

	FileToken tFileToken(nSrcFd);

	if(!tFileToken.TokenDel_32(tACSFH.nCompVer))
		return -2;
	if(!tFileToken.TokenDel_32(tACSFH.nEncType))
		return -3;

	if(tACSFH.nCompVer != 0)
		return -4;
	if(tACSFH.nEncType > 1)
		return -5;

	nRetVal = tFileToken.TokenDel_Char(tACSFH.acEncHash, MAX_QBUFF);
	if( nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	nRetVal = tFileToken.TokenDel_Char(tACSFH.acRelativePath, MAX_QBUFF);
	if( nRetVal < 0)
	{
		nRetVal -= 20;
		return nRetVal;
	}


	nRetVal = tFileToken.TokenDel_Char(tACSFH.acFileName, MAX_QBUFF);
	if( nRetVal < 0)
	{
		nRetVal -= 30;
		return nRetVal;
	}

	if(!tFileToken.TokenDel_64(tACSFH.nFileSize))
		return -6;

	if(!tFileToken.TokenDel_32(tACSFH.nFileMode))
		return -7;

	if(!tFileToken.TokenDel_32(tACSFH.nTokenSize))
		return -8;

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::WriteLHdr(ASI_COMP_SECU_LOOP_HDR& tACSLH, INT32 nTarFd)
{
	if(nTarFd == -1)
		return -1;

	FileToken tFileToken(nTarFd);

	tFileToken.TokenAdd_32(tACSLH.nSrcSize);
	tFileToken.TokenAdd_32(tACSLH.nTarSize);

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::ReadLHdr(ASI_COMP_SECU_LOOP_HDR& tACSLH, INT32 nSrcFile)
{
	FileToken tFileToken(nSrcFile);

	tFileToken.TokenDel_32(tACSLH.nSrcSize);
	tFileToken.TokenDel_32(tACSLH.nTarSize);

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::WriteBody(INT32 nTarFd, PBYTE pBuff, UINT32 dwLen)
{
	INT32 nOnceWritedSize = 0;
	DWORD dwOnceSize = 0;
	UINT32 dwWritedSize = 0;
	UINT32 dwTryNum = 0;

	if(nTarFd == -1 || pBuff == NULL || dwLen == 0)
		return -1;

	dwTryNum = dwLen/CHAR_MAX_SIZE + 10;

	while((dwWritedSize < dwLen) && dwTryNum)
	{
		dwOnceSize = dwLen - dwWritedSize;
		if(dwOnceSize > CHAR_MAX_SIZE)
			dwOnceSize = CHAR_MAX_SIZE;
		nOnceWritedSize = write(nTarFd, &pBuff[dwWritedSize], dwOnceSize);
		if(nOnceWritedSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -2;
			}
		}
		dwWritedSize += (UINT32)nOnceWritedSize;
		if(nOnceWritedSize != 0)
			dwTryNum -= 1;
	}
	
	if(!dwTryNum)
		return -3;

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::ReadBody(INT32 nSrcFd, PBYTE pBuff, UINT32 dwLen)
{
	INT32 nOnceReadSize = 0;
	DWORD dwOnceSize = 0;
	UINT32 dwReadSize = 0;
	UINT32 dwTryNum = 0;

	if(nSrcFd == -1 || pBuff == NULL || dwLen == 0)
		return -1;

	memset(pBuff, 0, dwLen);

	dwTryNum = dwLen/CHAR_MAX_SIZE + 10;

	while((dwReadSize < dwLen) && dwTryNum)
	{
		dwOnceSize = dwLen - dwReadSize;
		if(dwOnceSize > CHAR_MAX_SIZE)
			dwOnceSize = CHAR_MAX_SIZE;
			
		nOnceReadSize = read(nSrcFd, &pBuff[dwReadSize], dwOnceSize);
		if(nOnceReadSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -2;
			}
		}
		dwReadSize += (UINT32)nOnceReadSize;
		if(nOnceReadSize != 0)
			dwTryNum -= 1;
	}	

	if(!dwTryNum)
		return -3;

	return 0;
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::CompFile(PASI_COMP_SECU pACS, UINT32 nSize)
{
	INT32 nSrcFd = -1;
	INT32 nTarFd = -1;
	INT32 nRetVal = 0;
	DWORD dwSize = 0;
	DWORD dwMode = 0666;
	CHAR acCompFileName[MAX_HBUFF] = {0, };
	ASI_COMP_SECU tACS;
	ASI_COMP_SECU_FILE_HDR tACSFH;
	if(pACS == NULL)
		return -1;
	
	memcpy(&tACS, pACS, sizeof(tACS));
	memset(&tACSFH, 0, sizeof(tACSFH));
	
	nSrcFd = open(tACS.acSrcFile, O_RDWR);
	if(nSrcFd == -1)
		return -2;
	
	nTarFd = open(tACS.acTarFile, O_CREAT | O_RDWR, dwMode);
	if(nTarFd == -1)
	{
		close(nSrcFd);
		return -3;
	}

	do{
		if(get_basename(tACS.acSrcFile, acCompFileName, MAX_HBUFF-1) == NULL)
		{
			nRetVal = -4;
			break;
		}
		acCompFileName[MAX_HBUFF-1] = 0;
		if(get_file_size(tACS.acSrcFile, &dwSize, &dwMode) != 0)
		{
			nRetVal = -5;
			break;
		}
/*
		if(dwSize == 0)
		{
			nRetVal = -6;
			break;
		}
*/
		if(dwMode == 0)
		{
			dwMode = 0666;
		}
		tACSFH.nCompVer			= 0;
		tACSFH.nEncType			= tACS.pSeedKey[0] ? 1 : 0;
		strncpy(tACSFH.acEncHash, GetSHA2(tACS.pSeedKey, 16).c_str(), MAX_QBUFF-1);
		tACSFH.acEncHash[MAX_QBUFF-1] = 0;
		tACSFH.nFileSize		= dwSize;	
		tACSFH.nTokenSize		= ASI_COMP_SECU_TOKEN_SIZE;
		strncpy(tACSFH.acFileName, acCompFileName, MAX_QBUFF-1);
		tACSFH.nFileSize		= dwSize;	
		tACSFH.nFileMode		= dwMode;

		nRetVal = WriteHdr(tACSFH, nTarFd);
		if(nRetVal != 0)
		{
			nRetVal -= 100;
			break;
		}
		if(dwSize > 0)
		{
			nRetVal = CompFileToFile(nSrcFd, nTarFd, tACS.pSeedKey);
			if(nRetVal != 0)
			{
				nRetVal -= 200;
				break;
			}
		}
		nRetVal = 0;
	}while(FALSE);
	
	close(nSrcFd);
	close(nTarFd);

	if(nRetVal != 0)
	{
		if(is_file(tACS.acTarFile) == 0)
			unlink(tACS.acTarFile);
	}

	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::DecompFile(PASI_COMP_SECU pACS, UINT32 nSize, char *acLogMsg)
{
	ASI_COMP_SECU tACS;
	ASI_COMP_SECU_FILE_HDR tACSFH;
	CFileUtil tFileUtil;
	INT32 nSrcFd = -1;
	INT32 nTarFd = -1;
	INT32 nRetVal = 0;
	DWORD dwLoopNum = 0;
	DWORD dwRemainNum = 0;
	String strSvFile = "";
	char acCompHash[MAX_QBUFF+1] = {0,};
	if(pACS == NULL)
		return -1;

	memcpy(&tACS, pACS, sizeof(tACS));
	memset(&tACSFH, 0, sizeof(tACSFH));

	if(tACS.acTarPath[0] == 0)
	{
		return -2;
	}

	nSrcFd = open(tACS.acSrcFile, O_RDWR);
	if(nSrcFd == -1)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", tACS.acSrcFile, errno);
		return -3;
	}

	nRetVal = ReadHdr(tACSFH, nSrcFd);
	if(nRetVal != 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to read hdr %s (%d)", tACS.acSrcFile, nRetVal);
		close(nSrcFd);
		nRetVal -= 100;
		return nRetVal;
	}

	if(tACSFH.nTokenSize == 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid token size %s", tACS.acSrcFile);
		close(nSrcFd);
		return -4;
	}
	strncpy(acCompHash, GetSHA2(tACS.pSeedKey, 16).c_str(), MAX_QBUFF);
	acCompHash[MAX_QBUFF] = 0;
	if(_stricmp(tACSFH.acEncHash, acCompHash))
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid hash enc:%s comp:%s", tACSFH.acEncHash, acCompHash);
		close(nSrcFd);
		return -5;
	}

	if(tFileUtil.DirectoryExists(tACS.acTarPath) == FALSE)
		tFileUtil.ForceDirectory(tACS.acTarPath);

	if(tACS.acTarFile[0])
	{
		strSvFile = SPrintf("%s/%s", tACS.acTarPath, tACS.acTarFile);
	}
	else
	{
		strSvFile = SPrintf("%s/%s", tACS.acTarPath, tACSFH.acFileName);
	}
	if(tACSFH.nFileMode == 0)
	{
		tACSFH.nFileMode = 0666;
	}

	nTarFd = open(strSvFile.c_str(), O_RDWR | O_CREAT, tACSFH.nFileMode);
	if(nTarFd == -1)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", strSvFile.c_str(), errno);
		close(nSrcFd);
		return -6;
	}
	
	if(tACSFH.nFileSize > 0)
	{
		dwLoopNum = (DWORD)(tACSFH.nFileSize / tACSFH.nTokenSize);
		dwRemainNum = (DWORD)(tACSFH.nFileSize % tACSFH.nTokenSize);
		if(dwRemainNum)
			dwLoopNum += 1;

		while(dwLoopNum--)
		{
			nRetVal = DecompFileToFileToken(nSrcFd, nTarFd, tACS.pSeedKey);
			if(nRetVal != 0)
			{
				nRetVal -= 200;
				break;
			}
		}
	}	
	close(nSrcFd);
	close(nTarFd);
	if(nRetVal != 0)
	{
		if(tFileUtil.FileExists(strSvFile.c_str()) == TRUE)
			unlink(strSvFile.c_str());
	}
	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::CompBuff(PVOID pInBuff, INT32 nLen, PVOID* ppOutBuff, INT32 &nOutLen)
{
	INT32 nRtn = LZO_E_OK;

	PVOID pOutBuff = NULL;

	if(pInBuff == NULL || nLen < 1 || ppOutBuff == NULL)
		return -1;

	if(*ppOutBuff != NULL)
		safe_free(*ppOutBuff);

	nOutLen = (nLen * 2);
	if (nOutLen < 1024)
		nOutLen += 1024;
	pOutBuff = malloc(nOutLen);
	if(pOutBuff == NULL)
		return -2;
	memset(pOutBuff, 0, nOutLen);

	nRtn = lzo1x_1_compress((lzo_bytep)pInBuff, nLen, (lzo_bytep)pOutBuff, (lzo_uintp)(&nOutLen), wrkmem);
	if (nRtn != LZO_E_OK)
	{
		safe_free(pOutBuff);
		return -41;
	}
	*ppOutBuff = pOutBuff;
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::DecompBuff(PVOID pInBuff, INT32 nLen, PVOID* lpOutBuff, INT32 &nOutLen)
{
	INT32 nNewLen = 0;
	INT32 nRtn = LZO_E_OK; 

	if(pInBuff == NULL || nLen < 1 || lpOutBuff == NULL)
		return -1;

	if(*lpOutBuff == NULL)
	{
		nNewLen = (nLen*10);
		*lpOutBuff = malloc(nNewLen + 1);
		if(*lpOutBuff == NULL)
			return -2;
		memset(*lpOutBuff, 0, nNewLen + 1);
	}
	else
	{
		memset(*lpOutBuff, 0, nOutLen);
	}

	nRtn = lzo1x_decompress((lzo_bytep)pInBuff, nLen, (lzo_bytep)*lpOutBuff, (lzo_uintp)&nOutLen, NULL);
	if (nRtn != LZO_E_OK)
	{
		safe_free(*lpOutBuff);
		return -42;
	}

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::CompBuffToFile(INT32 nFd, PVOID pOrBuf, INT32 nOrLen)
{
	PBYTE pComBuff = NULL;
	INT32 nCompLen = 0;
	INT32 nRetVal = 0;

	if(nFd == -1 || pOrBuf == NULL || nOrLen < 1)	
	{
		return -1;
	}
	
	nRetVal = CompBuff(pOrBuf, nOrLen, (PVOID*)&pComBuff, nCompLen);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	nRetVal = WriteBody(nFd, pComBuff, nCompLen);
	safe_free(pComBuff);

	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::DecompBuffToFile(INT32 nFd, PVOID pCompBuff, INT32 nCompLen)
{
	PBYTE pOrBuff = NULL;
	INT32 nOrLen = 0;
	INT32 nRetVal = 0;

	if(nFd == -1 || pCompBuff == NULL || nCompLen < 1)	
	{
		return -1;
	}

	nRetVal = DecompBuff(pCompBuff, nCompLen, (PVOID*)&pOrBuff, nOrLen);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	nRetVal = WriteBody(nFd, pOrBuff, nOrLen);
	safe_free(pOrBuff);

	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::CompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pEncKey)
{
	INT32 nRetVal = 0;
	DWORD dwLoopCnt = 0;
	DWORD dwRemainSize = 0;
	DWORD dwSize = 0;
	
	if(nSrcFd == -1 || nTarFd == -1 || pEncKey == NULL)
	{
		return -1;
	}
	
	nRetVal = GetFileSizeWithFd(nSrcFd, &dwSize);	
	if(nRetVal != 0)
	{
		return -2;
	}

	if(dwSize == 0)
		return 0;

	dwLoopCnt = dwSize / ASI_COMP_SECU_TOKEN_SIZE;
	dwRemainSize = dwSize % ASI_COMP_SECU_TOKEN_SIZE;

	while(dwLoopCnt--)
	{
		nRetVal = CompFileToFile(nSrcFd, nTarFd, pEncKey, ASI_COMP_SECU_TOKEN_SIZE);
		if(nRetVal != 0)
		{
			nRetVal -= 10;
			return nRetVal;
		}
	}

	if(dwRemainSize)
	{
		nRetVal = CompFileToFile(nSrcFd, nTarFd, pEncKey, dwRemainSize);
		if(nRetVal != 0)
		{
			nRetVal -= 20;
			return nRetVal;
		}
	}	
	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::DecompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pDecKey)
{
	INT32 nRetVal = 0;
	DWORD dwLoopNum = 0;
	DWORD dwRemainNum = 0;
	ASI_COMP_SECU_FILE_HDR tACSFH;

	if(nSrcFd == -1 || nTarFd == -1 || pDecKey == NULL)
	{
		return -1;
	}
	
	memset(&tACSFH, 0, sizeof(tACSFH));
	
	nRetVal = ReadHdr(tACSFH, nSrcFd);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}
	

	dwLoopNum = (DWORD)(tACSFH.nFileSize / tACSFH.nTokenSize);
	dwRemainNum = (DWORD)(tACSFH.nFileSize % tACSFH.nTokenSize);
	if(dwRemainNum)
		dwLoopNum += 1;

	while(dwLoopNum--)
	{
		nRetVal = DecompFileToFileToken(nSrcFd, nTarFd, pDecKey);
		if(nRetVal != 0)
		{
			nRetVal -= 20;
			return nRetVal;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompSecuUtil::CompFileToFile(INT32 nSrcFd, INT32 nTarFd, PBYTE pEncKey, UINT32 nTokenSize)
{	
	PBYTE pSaveBuf = NULL;
	INT32 nSaveLen = 0;
	PBYTE pSrcBuf = NULL;
	INT32 nRetVal = 0;
	PBYTE pEncBuf = NULL;
	INT32 nEncLen = 0;
	ASI_COMP_SECU_LOOP_HDR tACSLH;

	if(nSrcFd == -1 || nTarFd == -1 || nTokenSize == 0)
	{
		return -1;
	}

	pSrcBuf = (PBYTE)malloc(nTokenSize + 1);
	if(pSrcBuf == NULL)
		return -2;
	
	memset(pSrcBuf, 0, nTokenSize + 1);

	nRetVal = ReadBody(nSrcFd, pSrcBuf, nTokenSize);
	if(nRetVal != 0)	
	{
		safe_free(pSrcBuf);
		nRetVal -= 10;
		return nRetVal;
	}

	if(pEncKey && pEncKey[0])
	{
		seed_init((char *)pEncKey, ASI_COMP_S_SEED_KEY_LEN);
		nRetVal = seed_encrypt_data((char *)pSrcBuf, nTokenSize, (char **)&pEncBuf, &nEncLen);
		if(nRetVal != 0)
		{
			safe_free(pEncBuf);
			safe_free(pSrcBuf);
			nRetVal -= 20;
			return nRetVal;
		}
		safe_free(pSrcBuf);	

		nRetVal = CompBuff((PVOID)pEncBuf, nEncLen, (PVOID*)&pSaveBuf, nSaveLen);
		safe_free(pEncBuf);
		if(nRetVal != 0)
		{
			safe_free(pSaveBuf);
			nRetVal -= 30;
			return nRetVal;
		}
	}
	else
	{
		nRetVal = CompBuff((PVOID)pSrcBuf, nTokenSize, (PVOID*)&pSaveBuf, nSaveLen);
		safe_free(pSrcBuf);
		if(nRetVal != 0)
		{
			safe_free(pSaveBuf);
			nRetVal -= 40;
			return nRetVal;
		}
	}
	tACSLH.nSrcSize = nTokenSize;
	tACSLH.nTarSize	= nSaveLen;

	nRetVal = WriteLHdr(tACSLH, nTarFd);
	if(nRetVal != 0)
	{
		safe_free(pSaveBuf);
		nRetVal -= 50;
		return nRetVal;
	}
	nRetVal = WriteBody(nTarFd, pSaveBuf, nSaveLen);
	if(nRetVal != 0)
	{
		safe_free(pSaveBuf);
		nRetVal -= 60;
		return nRetVal;
	}
	safe_free(pSaveBuf);
	return 0;
}
//-----------------------------------------------------------------------------------

INT32 CCompSecuUtil::DecompFileToFileToken(INT32 nSrcFd, INT32 nTarFd, PBYTE pDecKey)
{
	ASI_COMP_SECU_LOOP_HDR tACSLH;
	PBYTE pSaveBuf = NULL;
	PBYTE pCompBuf = NULL; 
	PBYTE pDecompBuf = NULL; 
	INT32 nSaveLen = 0;
	INT32 nOrEncSize = 0;
	INT32 nRetVal = 0;
	INT32 nDecompLen = 0;

	if(nSrcFd == -1 || nTarFd == -1)
	{
		return -1;
	}

	memset(&tACSLH, 0, sizeof(tACSLH));
	nRetVal = ReadLHdr(tACSLH, nSrcFd);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	if(tACSLH.nTarSize < 1)
	{
		return -2;
	}
	
	pCompBuf = (PBYTE)malloc(tACSLH.nTarSize + 1);
	if(pCompBuf == NULL)
	{
		return -3;
	}
	memset(pCompBuf, 0, tACSLH.nTarSize + 1);
	
	nRetVal = ReadBody(nSrcFd, pCompBuf, tACSLH.nTarSize);
	if(nRetVal != 0)
	{
		safe_free(pCompBuf);
		nRetVal -= 20;
		return nRetVal;
	}
	if(pDecKey && pDecKey[0])
	{		
		nOrEncSize = tACSLH.nSrcSize + 30;
		pDecompBuf = (PBYTE)malloc(nOrEncSize);
		if(pDecompBuf == NULL)
		{
			safe_free(pCompBuf);
			return -4;
		}
		
		memset(pDecompBuf, 0, nOrEncSize);

		nRetVal = DecompBuff((PVOID)pCompBuf, tACSLH.nTarSize, (PVOID*)&pDecompBuf, nDecompLen);
		safe_free(pCompBuf);
		if(nRetVal != 0)
		{
			safe_free(pDecompBuf);
			nRetVal -= 30;
			return nRetVal;
		}
		
		seed_init((char *)pDecKey, ASI_COMP_S_SEED_KEY_LEN);
		nRetVal = seed_decrypt_data((char *)pDecompBuf, (char **)&pSaveBuf, &nSaveLen);
		if(nRetVal != 0)
		{
			safe_free(pDecompBuf);
			safe_free(pSaveBuf);
			nRetVal -= 40;
			return nRetVal;
		}
		safe_free(pDecompBuf);
	}
	else
	{
		nRetVal = DecompBuff(pCompBuf, tACSLH.nTarSize, (PVOID*)&pSaveBuf, nSaveLen);
		safe_free(pCompBuf);
		if(nRetVal != 0)
		{
			safe_free(pSaveBuf);
			nRetVal -= 50;
			return nRetVal;
		}
	}
	nRetVal = WriteBody(nTarFd, pSaveBuf, nSaveLen);
	if(nRetVal != 0)
	{
		safe_free(pSaveBuf);
		nRetVal -= 60;
		return nRetVal;
	}
	safe_free(pSaveBuf);
	return 0;
}
//-----------------------------------------------------------------------------------



