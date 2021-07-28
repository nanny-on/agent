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

// CompUtil.cpp: implementation of the CCompUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "CompUtil.h"
#include "FileUtil.h"
#include "FileToken.h"

CCompUtil::CCompUtil()
{
	f_NotifyState		= 0;
	lpCallBackHandle	= 0;
	m_nInitEngine		= InitEngine();
}
//-----------------------------------------------------------------------------------

CCompUtil::~CCompUtil()
{

}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::InitEngine()
{
	if (lzo_init() != LZO_E_OK)
    {
        return 0;
    }
	return 1;
}
//-----------------------------------------------------------------------------------

INT32 CCompUtil::CompressEngine(PVOID lpInput, INT32 nOrLen, PVOID lpOutput, INT32 &nChLen)
{
	lzo_uint nOutLen = nChLen;
	INT32 nRtn;
	nRtn = lzo1x_1_compress((lzo_bytep)lpInput, nOrLen, (lzo_bytep)lpOutput, (lzo_uintp)&nOutLen, wrkmem);
	nChLen = nOutLen;
    if (nRtn != LZO_E_OK)
		return -1;
    
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressEngine(PVOID lpInput, INT32 nChLen, PVOID lpOutput, INT32 nOrLen)
{		
	INT32 nNewLen = 0;
	INT32 nRtn = lzo1x_decompress((lzo_bytep)lpInput,nChLen,(lzo_bytep)lpOutput,(lzo_uintp)&nNewLen,NULL);
    if (nRtn != LZO_E_OK)
		return -1;
	
	if(nOrLen != nNewLen)
	{	
        return -2;
    }

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::SetNotifyState(PVOID lParam, PVOID NotifyState)
{
	lpCallBackHandle	= lParam;
	f_NotifyState		= (NotifyStateType)NotifyState;
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressDir(PASI_COMP_INFO paci)
{
	CFileUtil	tFileUtil;
	String 	strSaveTemp;
	INT32	nRtn = 0;
	INT32	nIndex = 0;
	TListGatherFileInfo tNamesList;
	if(paci == NULL || m_nInitEngine == 0)
		return -1;

	InitEngine();
	
	strSaveTemp = paci->pszCompOutName;
	strSaveTemp += ".temp";

	if(tFileUtil.FileExists(strSaveTemp.c_str()) == TRUE)
		unlink(strSaveTemp.c_str());
	

	if(tFileUtil.GatherFileNameList(paci->pszCompPath, "", paci->pszPassExtension, tNamesList, paci->nSubDirComp) == 0)
	{
		return -2;
	}

	sgc_notify_info._add_info("", "", (UINT32)tNamesList.size(), 0, lpCallBackHandle);

	{
		
		CompMemToken tCompHeadToken(1000);
		tCompHeadToken.TokenAdd_16(ASI_COMP_VER_MAJOR);
		tCompHeadToken.TokenAdd_16(ASI_COMP_VER_MINOR);
		tCompHeadToken.TokenAdd_32(GetCurrentDateTimeInt());
		tCompHeadToken.TokenAdd_32(nIndex);
		if(WriteFileComp(strSaveTemp.c_str(), tCompHeadToken))
		{
			nRtn = -11;
			goto ERROR_OP;
		}
	}

	{
		CompMemToken tCompToken(100000000);			
		TListGatherFileInfoItor begin, end;
		begin = tNamesList.begin();	end = tNamesList.end();
		while(begin != end)
		{
			if(CompressFile( &(*begin), tCompToken) == 0)
				nIndex++;	
			
			sgc_notify_info._add_info((*begin).strAddPath.c_str(), (*begin).strFileName.c_str(), nIndex);
			if(f_NotifyState)	
				f_NotifyState(&sgc_notify_info);
			
			if(WriteFileComp(strSaveTemp.c_str(), tCompToken))
			{
				nRtn = -12;
				goto ERROR_OP;
			}
			tCompToken.Clear();
			begin++;
		}
	}

	{
		
		CompMemToken tCompHeadToken(1000);
		tCompHeadToken.TokenAdd_16(ASI_COMP_VER_MAJOR);
		tCompHeadToken.TokenAdd_16(ASI_COMP_VER_MINOR);
		tCompHeadToken.TokenAdd_32(GetCurrentDateTimeInt());
		tCompHeadToken.TokenAdd_32(nIndex);
		if(WriteFileComp(strSaveTemp.c_str(), tCompHeadToken, 0))
		{
			nRtn = -13;
			goto ERROR_OP;
		}
	}	
	if(tFileUtil.FileExists(paci->pszCompOutName) == TRUE)
		unlink(paci->pszCompOutName);
	MoveFileEx(strSaveTemp.c_str(), paci->pszCompOutName);
	return 0;

ERROR_OP:
	if(tFileUtil.FileExists(strSaveTemp.c_str()) == TRUE)
		unlink(strSaveTemp.c_str());
	return nRtn;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressDir(PASI_DECOMP_INFO padi)
{
	if(m_nInitEngine == 0 || padi == NULL)
		return -1;

	InitEngine();

	if(AnalysisCompFileAndDecomp(padi->pszDecompFileName, padi->pszDecompTargetDir))
	{
		return -2;
	}
	
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressFile(PFF_GATHER_FILE_INFO pGatherFileInfo, CompMemToken &tSaveToken)
{
	PVOID	lpBuf = NULL;
	PVOID	lpOutBuf = NULL;
	INT32	nLen, nOutLen;
	CFileUtil	tFileUtil;
	String strFileName;

	if(pGatherFileInfo == NULL)
		return -1;
	if(pGatherFileInfo->strPrePath.length() != 0)
	{
		strFileName = pGatherFileInfo->strPrePath;
	}
	if(pGatherFileInfo->strAddPath.length() != 0)
	{
		if(strFileName.length() != 0)
			tFileUtil.AddLasCHAR(strFileName);
		strFileName += pGatherFileInfo->strAddPath;
	}
	if(pGatherFileInfo->strFileName.length() != 0)
	{
		if(strFileName.length() != 0)
			tFileUtil.AddLasCHAR(strFileName);
		strFileName += pGatherFileInfo->strFileName;
	}
	
	if(ReadFileBuf(strFileName.c_str(), &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -2;
	}

	if(nLen < 1)
	{
		safe_free(lpBuf);
		return -3;
	}

	nOutLen = 2*nLen;
	if (nOutLen < 1024)
		nOutLen += 1024;
	nOutLen += (nOutLen*0.3);	
	
	lpOutBuf = malloc(nOutLen + 1);
	if(lpOutBuf == NULL)
	{
		safe_free(lpBuf);
		return -4;
	}
	memset(lpOutBuf, 0, nOutLen + 1);

	if(CompressEngine(lpBuf, nLen, lpOutBuf, nOutLen))
	{
		safe_free(lpBuf);
		safe_free(lpOutBuf);
		return -5;
	}

	tSaveToken.TokenAdd_String(pGatherFileInfo->strAddPath);
	tSaveToken.TokenAdd_String(pGatherFileInfo->strFileName);
	tSaveToken.TokenAdd_32(nLen);
	tSaveToken.TokenAdd_32(nOutLen);
	tSaveToken.TokenAdd_PVOID(lpOutBuf, nOutLen);

	safe_free(lpBuf);
	safe_free(lpOutBuf);

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressFileAndSave(LPCSTR lpFileName, LPCSTR lpSaveDir, LPCSTR lpSaveName)
{
	PVOID lpBuf = NULL;
	PVOID lpOutBuf = NULL;
	INT32 nOutLen = 0;
	INT32 nLen = 0;
	String strSaveName;
	CFileUtil tFileUtil;
	CHAR	acCompFileName[MAX_HBUFF] = {0, };

	CompMemToken	tSaveToken(100000000);

	if(lpFileName == NULL || lpSaveDir == NULL || lpSaveName == NULL)
		return -1;

	if(ReadFileBuf(lpFileName, &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -2;
	}
	if(nLen < 1)
	{
		safe_free(lpBuf);
		return -3;
	}

	nOutLen = 2*nLen;
	if (nOutLen < 1024)
		nOutLen += 1024;

	nOutLen += (nOutLen*0.3);
	
	lpOutBuf = malloc(nOutLen + 1);
	if(lpOutBuf == NULL)
	{
		safe_free(lpBuf);
		return -4;
	}
	memset(lpOutBuf, 0, nOutLen + 1);

	if(CompressEngine(lpBuf, nLen, lpOutBuf, nOutLen))
	{
		safe_free(lpBuf);
		safe_free(lpOutBuf);
		return -5;
	}
	
	strSaveName = lpSaveDir;

	if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE)	
		tFileUtil.ForceDirectory(strSaveName.c_str());
	
	tFileUtil.AddLasCHAR(strSaveName);	
	strSaveName += lpSaveName;

	if(tFileUtil.FileExists(strSaveName.c_str()) == TRUE)	
		unlink(strSaveName.c_str());	

	if(get_basename((char *)lpFileName, (char *)acCompFileName, MAX_HBUFF-1) == NULL)
	{
		safe_free(lpBuf);
		safe_free(lpOutBuf);
		return -6;
	}
	acCompFileName[MAX_HBUFF-1] = 0;

	CompMemToken tCompHeadToken(1);

	tSaveToken.TokenAdd_String(acCompFileName);
	tSaveToken.TokenAdd_32(nLen);
	tSaveToken.TokenAdd_32(nOutLen);
	tSaveToken.TokenAdd_PVOID(lpOutBuf, nOutLen);

	safe_free(lpBuf);
	safe_free(lpOutBuf);

	if(WriteFileComp(strSaveName.c_str(), tCompHeadToken, tSaveToken))
		return -7;

	return 0;

}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressBufferAndSave(LPCSTR lpName, BYTE *lpSrcBuffer, INT32 nBufferLen, LPCSTR lpSaveDir, LPCSTR lpSaveFile)
{
	CFileUtil	tFileUtil;	
	PVOID lpOutBuf = NULL;
	PVOID lpBuf = NULL;
	INT32 nLen = 0;
	INT32 nOutLen = 0;
	String strSaveName;

	if(lpName == NULL || lpSrcBuffer == NULL || nBufferLen < 1 || lpSaveDir == NULL || lpSaveFile == NULL)
		return -1;

	lpBuf = lpSrcBuffer;
	nLen = nBufferLen;
	nOutLen = 2*nLen;
	if (nOutLen < 1024)
		nOutLen += 1024;

	nOutLen += (nOutLen*0.3);	
	
	lpOutBuf = malloc(nOutLen + 1);
	if(lpOutBuf == NULL)
		return -2;
	
	memset(lpOutBuf, 0, nOutLen + 1);

	if(CompressEngine(lpBuf, nLen, lpOutBuf, nOutLen))
	{
		safe_free(lpOutBuf);
		return -3;
	}
	
	CompMemToken tCompHeadToken(1);
	CompMemToken tSaveToken(100000000);
	
	tSaveToken.TokenAdd_String(lpName);
	tSaveToken.TokenAdd_32(nLen);
	tSaveToken.TokenAdd_32(nOutLen);
	tSaveToken.TokenAdd_PVOID(lpOutBuf, nOutLen);

	safe_free(lpOutBuf);

	strSaveName = lpSaveDir;
	if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE)	
		tFileUtil.ForceDirectory(strSaveName.c_str());
	tFileUtil.AddLasCHAR(strSaveName);
	strSaveName += lpSaveFile;
	if(tFileUtil.FileExists(strSaveName.c_str()) == TRUE)	
		unlink(strSaveName.c_str());

	if(WriteFileComp(strSaveName.c_str(), tCompHeadToken, tSaveToken))
		return -4;

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressFileAndBuffer(LPCSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen)
{
	PVOID lpBuf = NULL;
	PVOID lpOrBuf = NULL;
	INT32 nLen = 0;	
	INT32 nNewLen = 0;
	ASI_COMP_FILE_INFO stCompFileInfo;
	
	if(lpFileName == NULL || lpOutBuffer == NULL)
		return -1;

	if(ReadFileBuf(lpFileName, &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -2;
	}

	if(nLen < 1)
	{
		safe_free(lpBuf);
		return -2;
	}

	CompMemToken tComToken(lpBuf, nLen);

	if( tComToken.TokenDel_String(stCompFileInfo.header.strFileName) < 0)
	{
		safe_free(lpBuf);
		return -32;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nOrFileSize) )
	{
		safe_free(lpBuf);
		return -33;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nComFileSize) )
	{
		safe_free(lpBuf);
		return -34;
	}
	if(stCompFileInfo.header.nComFileSize == 0)
	{
		safe_free(lpBuf);
		return -35;
	}
	
	stCompFileInfo.body.data = malloc(sizeof(BYTE) * (stCompFileInfo.header.nComFileSize + 1));
	if(stCompFileInfo.body.data == NULL)
	{
		safe_free(lpBuf);
		return -36;
	}
	memset(stCompFileInfo.body.data, 0, stCompFileInfo.header.nComFileSize + 1);
	
	if( tComToken.TokenDel_PVOID(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize) < 0)
	{
		safe_free(lpBuf);
		return -37;
	}
	safe_free(lpBuf);
	
	nNewLen = stCompFileInfo.header.nOrFileSize;
	if(nNewLen < 1)
	{
		return -38;
	}
	
	lpOrBuf = malloc(sizeof(BYTE) * (nNewLen + 1));
	if(lpOrBuf == NULL)
	{
		return -39;
	}
	memset(lpOrBuf, 0, nNewLen + 1);
	
	if(DecompressEngine(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize, lpOrBuf, nNewLen))
	{
		safe_free(lpOrBuf);
		return -40;
	}

	*lpOutBuffer = (BYTE*)lpOrBuf;
	nOutBuffeLen = stCompFileInfo.header.nOrFileSize;

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCSTR lpRelativeFilePath, LPCSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{	
	INT32	nOutLen = 0;
	PVOID	lpOutBuf = NULL;
	CHAR	acCompFileName[MAX_HBUFF] = {0, };

	if(lpInBuffer == NULL || nLen < 1 || lpOutBuffer == NULL || lpAbsoluteFileName == NULL || lpRelativeFilePath == NULL)
		return -1;
	
	nOutLen = 2*nLen;
	if (nOutLen < 1024)
		nOutLen += 1024;

	nOutLen += (nOutLen*0.3);

	lpOutBuf = malloc(nOutLen + 1);
	if(lpOutBuf == NULL)
		return -2;
	memset(lpOutBuf, 0, nOutLen + 1);
	if(nUsedComp)
	{
		if(CompressEngine(lpInBuffer, nLen, lpOutBuf, nOutLen))
		{
			safe_free(lpOutBuf);
			return -3;
		}
	}
	else
	{
		memcpy(lpOutBuf, lpInBuffer, nLen);
		nOutLen = nLen;
	}

	CompMemToken tSaveToken(100000000);
	
	if(get_basename((char *)lpAbsoluteFileName, (char *)acCompFileName, MAX_HBUFF-1) == NULL)
	{
		safe_free(lpOutBuf);
		return -4;
	}
	acCompFileName[MAX_HBUFF-1] = 0;

	tSaveToken.TokenAdd_String(lpRelativeFilePath);
	tSaveToken.TokenAdd_String(acCompFileName);
	tSaveToken.TokenAdd_32(nLen);
	tSaveToken.TokenAdd_32(nOutLen);
	tSaveToken.TokenAdd_PVOID(lpOutBuf, nOutLen);	
	safe_free(lpOutBuf);

	nOutBufferLen = tSaveToken.GetLength();
	if(nOutBufferLen == 0)
	{
		return -5;
	}
	*lpOutBuffer = (BYTE*)malloc(nOutBufferLen + 1);
	if(*lpOutBuffer == NULL)
	{
		return -6;
	}
	memset(*lpOutBuffer, 0, nOutBufferLen + 1);
	memcpy(*lpOutBuffer, tSaveToken.GetData(), nOutBufferLen);	

	return 0;	
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::CompressFileAndBuffer(LPCSTR lpRelativeFilePath, LPCSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{	
	INT32 nLen = 0;
	INT32 nOutLen = 0;
	PVOID lpBuf = NULL;
	PVOID lpOutBuf = NULL;
	CHAR	acCompFileName[MAX_HBUFF] = {0, };

	if(lpRelativeFilePath == NULL || lpAbsoluteFileName == NULL || lpOutBuffer == NULL)
		return -1;
	
	if(ReadFileBuf(lpAbsoluteFileName, &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -2;
	}
	if(nLen < 1)
	{
		safe_free(lpBuf);
		return -3;
	}
	nOutLen = 2*nLen;
	if (nOutLen < 1024)
		nOutLen += 1024;
	nOutLen += (nOutLen*0.3);	
	lpOutBuf = malloc(nOutLen + 1);
	if(lpOutBuf == NULL)
	{
		safe_free(lpBuf);
		return -4;
	}
	memset(lpOutBuf, 0, nOutLen + 1);

	if(nUsedComp)
	{
		if(CompressEngine(lpBuf, nLen, lpOutBuf, nOutLen))
		{
			safe_free(lpBuf);
			safe_free(lpOutBuf);
			return -5;
		}
	}
	else
	{
		memcpy(lpOutBuf, lpBuf, nLen);
		nOutLen = nLen;
	}
	safe_free(lpBuf);

	CompMemToken tSaveToken(100000000);

	if(get_basename((char *)lpAbsoluteFileName, (char *)acCompFileName, MAX_HBUFF-1) == NULL)
	{
		safe_free(lpOutBuf);
		return -6;
	}
	acCompFileName[MAX_HBUFF-1] = 0;

	tSaveToken.TokenAdd_String(lpRelativeFilePath);
	tSaveToken.TokenAdd_String(acCompFileName);
	tSaveToken.TokenAdd_32(nLen);
	tSaveToken.TokenAdd_32(nOutLen);
	tSaveToken.TokenAdd_PVOID(lpOutBuf, nOutLen);	
	safe_free(lpOutBuf);

	nOutBufferLen = tSaveToken.GetLength();
	if(nOutBufferLen < 1)
	{
		return -7;
	}
	*lpOutBuffer = (BYTE*)malloc(nOutBufferLen + 1);
	if(*lpOutBuffer == NULL)
	{
		return -8;
	}
	memset(*lpOutBuffer, 0, nOutBufferLen + 1);
	memcpy(*lpOutBuffer, tSaveToken.GetData(), nOutBufferLen);	
		
	return 0;		
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCSTR lpSaveDir, LPSTR lpSaveFile, INT32 nSaveFileLen, LPCSTR lpSaveFileAddOn, UINT32 nUsedComp)
{
	INT32 nNewLen = 0;
	PVOID lpOrBuf = NULL;
	String strSaveName;
	CFileUtil	tFileUtil;
	ASI_COMP_FILE_INFO stCompFileInfo;
	
	if(lpSrcBuffer == NULL || nSrcBuffLen < 1 || lpSaveDir == NULL || lpSaveFile == NULL || nSaveFileLen < 1)
		return -1;

	CompMemToken tComToken(lpSrcBuffer, nSrcBuffLen);
	
	if( tComToken.TokenDel_String(stCompFileInfo.header.strSubPath) < 0)
	{
		return -31;
	}
	if( tComToken.TokenDel_String(stCompFileInfo.header.strFileName) < 0)
	{
		return -32;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nOrFileSize) )
	{
		return -33;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nComFileSize) )
	{
		return -34;
	}

	if(stCompFileInfo.header.nOrFileSize < 1 || stCompFileInfo.header.nComFileSize)
	{
		return -35;
	}
	
	stCompFileInfo.body.data = malloc(stCompFileInfo.header.nComFileSize + 1);
	if(stCompFileInfo.body.data == NULL)
	{
		return -2;
	}
	
	memset(stCompFileInfo.body.data, 0, stCompFileInfo.header.nComFileSize + 1);
	
	if( tComToken.TokenDel_PVOID(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize) < 0)
	{
		return -36;
	}
	
	nNewLen = stCompFileInfo.header.nOrFileSize;	
	lpOrBuf = malloc(nNewLen + 1);
	if(lpOrBuf == NULL)
	{
		return -3;
	}
	memset(lpOrBuf, 0, nNewLen + 1);
	
	if(nUsedComp)
	{
		if(DecompressEngine(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize, lpOrBuf, nNewLen))
		{		
			safe_free(lpOrBuf);
			return -4;
		}
	}
	else
	{
		memcpy(lpOrBuf, stCompFileInfo.body.data, nNewLen);
	}

	
	strSaveName = lpSaveDir;
	if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE)	
		tFileUtil.ForceDirectory(strSaveName.c_str());

	if(lpSaveFile && strlen(lpSaveFile))
	{
		tFileUtil.AddLasCHAR(strSaveName);		
		strSaveName += lpSaveFile;
	}
	else			
	{
		if(stCompFileInfo.header.strSubPath.length() != 0)
		{
			tFileUtil.AddLasCHAR(strSaveName); 
			strSaveName += stCompFileInfo.header.strSubPath;
		}
		if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE) 
			tFileUtil.ForceDirectory(strSaveName.c_str());
		if(stCompFileInfo.header.strFileName.length() != 0)
		{
			tFileUtil.AddLasCHAR(strSaveName); 
			strSaveName += stCompFileInfo.header.strFileName;
		}
		
		if(lpSaveFileAddOn && strlen(lpSaveFileAddOn))
		{
			CTokenString Token(strSaveName.c_str(), strSaveName.length(), '.', 1);
			String strExtension = ".";
			strExtension += Token.NextToken();

			strSaveName = Token.RemainValue();
			strSaveName += lpSaveFileAddOn;
			strSaveName += strExtension;
		}
		
		if(lpSaveFile)
			strncpy(lpSaveFile, strSaveName.c_str(), nSaveFileLen-1);
	}

	if(tFileUtil.FileExists(strSaveName.c_str()) == TRUE)	
		unlink(strSaveName.c_str());

	if(WriteFile_SG(strSaveName.c_str(), lpOrBuf, nNewLen))
	{
		safe_free(lpOrBuf);
		return -5;
	}
	
	safe_free(lpOrBuf);
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::ReadFileBuf(LPCSTR lpFileName, PVOID* lpBuf, INT32& nLen)
{
	DWORD dwFileSize = 0;
	INT32 nFd = -1;
	BYTE* pTempBuffer = NULL;
	DWORD dwTotalReadSize = 0;
	BYTE pszBuff[CHAR_MAX_SIZE + 1] = {0, };
	INT32 nReadSize = 0;
	DWORD dwOnceReadSize = 0;

	if(lpFileName == NULL || lpBuf == NULL)
		return -1;

	if(get_file_size(lpFileName, &dwFileSize) != 0)
		return -2;

	if(dwFileSize == 0)
		return -3;

	nFd = open(lpFileName, O_RDONLY | O_NOFOLLOW);
	if(nFd == -1)
		return -4;

	*lpBuf = malloc(dwFileSize + 1);
	if(*lpBuf == NULL)
		return -5;
	
	memset(*lpBuf, 0, dwFileSize + 1);

	pTempBuffer = (BYTE*)*lpBuf;
    while(dwTotalReadSize < dwFileSize)
    {
		dwOnceReadSize = dwFileSize - dwTotalReadSize;
		if(dwOnceReadSize > CHAR_MAX_SIZE)
			dwOnceReadSize = CHAR_MAX_SIZE; 
		nReadSize = (INT32)read(nFd, pszBuff, dwOnceReadSize);
        if(nReadSize == -1)		
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				Sleep(10);
				continue;
			}
			else
			{
				close(nFd);
				safe_free(*lpBuf);
				return -6;
			}
        }
		if(nReadSize > 0)
		{
			memcpy(&pTempBuffer[dwTotalReadSize], pszBuff, nReadSize);
			dwTotalReadSize += (DWORD)nReadSize;
		}
    }

    close(nFd);
	nLen = (INT32)dwTotalReadSize;
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::ReadFileBufW(wchar_t * pwFileName, PVOID* lpBuf, INT32& nLen)
{
	StringW strFileNameW;
	String strFileName;
	if(pwFileName == NULL || lpBuf == NULL)
		return -1;
	strFileNameW = pwFileName;

	strFileName = ConvertAnsiString(strFileNameW);
	if(strFileName.length() == 0)
		return -2;
	return ReadFileBuf(strFileName.c_str(), lpBuf, nLen);
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::GetFileSizeEx(LPCSTR lpFileName)
{
	DWORD dwFileSize = 0;
	if(lpFileName == NULL)
		return -1;
	if(get_file_size(lpFileName, &dwFileSize) != 0)
		return -2;
	
	return (INT32)dwFileSize;
}
//-----------------------------------------------------------------------------------

INT32 CCompUtil::WriteFileComp(LPCSTR lpSaveName, CompMemToken &tHeadToken, CompMemToken &tBodyToken)
{
	INT32 nFd = -1;

	if(lpSaveName == NULL)
		return -1;

	if(is_file(lpSaveName) == 0)	
		unlink(lpSaveName);

	nFd = open(lpSaveName, O_CREAT | O_RDWR, 0666);
	if(nFd == -1)
		return -3;

	if(tHeadToken.GetLength())
		WriteFile_SG(nFd, tHeadToken.GetData(), tHeadToken.GetLength());

	WriteFile_SG(nFd, tBodyToken.GetData(), tBodyToken.GetLength());

	close(nFd);

	return 0;	
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::WriteFileComp(LPCSTR lpSaveName, CompMemToken& tToken, UINT64 nMovePos)
{
	INT32 nFd = -1;
	INT32 nRetVal = 0;

	if(lpSaveName == NULL)
		return -1;

	nFd = open(lpSaveName, O_CREAT | O_RDWR);
	if(nFd == -1)
	{
		return -2;
	}
	do{	
		if(nMovePos != 0xFFFFFFFFFFFFFFFF)
		{
			if(lseek(nFd, (off_t)nMovePos, SEEK_SET) == -1)
			{
				nRetVal = -2;
				break;
			}
		}
		else
		{
			if(lseek(nFd, 0, SEEK_END) == -1)
			{
				nRetVal = -3;
				break;
			}
		}

		nRetVal = WriteFile_SG(nFd, tToken.GetData(), tToken.GetLength());
		if(nRetVal != 0)
		{
			nRetVal = -4;
			break;
		}
		nRetVal = 0;
	}while(FALSE);		
	close(nFd);
	return nRetVal;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::WriteFile_SG(INT32 fd, PVOID lpBuf, DWORD dwLen)
{
	DWORD dwWritedByte = 0;
	INT32 nWriteByte = 0;
	DWORD dwTotalByte = dwLen;
	char *pData = (char *)lpBuf;

	if(fd == -1 || lpBuf == NULL || dwLen == 0)
		return -1;

	while(dwTotalByte > dwWritedByte)
	{
		nWriteByte = write(fd, &pData[dwWritedByte], dwTotalByte - dwWritedByte);
		if(nWriteByte == -1)
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
		dwWritedByte += (DWORD)nWriteByte;
	}
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::WriteFile_SG(LPCSTR lpSaveName, PVOID lpBuf, DWORD dwLen)
{
	INT32 fd = -1;
	INT32 nRetVal = 0;

	if(lpSaveName == NULL || lpBuf == NULL || dwLen == 0)
		return -1;
	
	if(is_file(lpSaveName) == 0)	
		unlink(lpSaveName);
	
	fd = open(lpSaveName, O_CREAT | O_RDWR, 0666);
	if(fd == -1)
		return -2;
	
	nRetVal = WriteFile_SG(fd, lpBuf, dwLen);
	
	close(fd);
	return nRetVal;
}

INT32		CCompUtil::AnalysisCompFile(LPCSTR lpFileName, TListASICompFileInfo &tASICompFileInfoList)
{
	PVOID lpBuf = NULL;
	INT32 nLen = 0;	
	ASI_COMP_FILE_VER stSgcFileVer;
	INT32 nFileNum = 0;
	PASI_COMP_FILE_INFO pCompFileInfo = NULL;
	if(lpFileName == NULL)
		return -1;
	
	if(ReadFileBuf(lpFileName, &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -2;
	}
	if(lpBuf == NULL || nLen < 1)
	{
		safe_free(lpBuf);
		return -3;
	}

	CompMemToken tComToken(lpBuf, nLen);

	memset(&stSgcFileVer, 0, sizeof(stSgcFileVer));
	if( !tComToken.TokenDel_16(stSgcFileVer.nMajorVer))
	{
		safe_free(lpBuf);
		return -10;
	}
	if( !tComToken.TokenDel_16(stSgcFileVer.nMinorVer))
	{
		safe_free(lpBuf);
		return -11;
	}
	if( !tComToken.TokenDel_32(stSgcFileVer.nCreateDateTime))
	{
		safe_free(lpBuf);
		return -12;
	}
	if( !tComToken.TokenDel_32(stSgcFileVer.nFileNum))
	{
		safe_free(lpBuf);
		return -13;
	}
	
	if(stSgcFileVer.nMajorVer != ASI_COMP_VER_MAJOR || stSgcFileVer.nMinorVer != ASI_COMP_VER_MINOR)
	{
		safe_free(lpBuf);
		return -20;
	}
	
	nFileNum = stSgcFileVer.nFileNum;
	while(nFileNum)
	{
		pCompFileInfo = new ASI_COMP_FILE_INFO;
		if(pCompFileInfo == NULL)
		{
			safe_free(lpBuf);
			return -31;
		}
		
		if( tComToken.TokenDel_String(pCompFileInfo->header.strSubPath) < 0)
		{
			safe_free(lpBuf);
			SAFE_DELETE(pCompFileInfo);
			return -32;
		}
		if( tComToken.TokenDel_String(pCompFileInfo->header.strFileName) < 0)
		{
			safe_free(lpBuf);
			SAFE_DELETE(pCompFileInfo);
			return -33;
		}
		if( !tComToken.TokenDel_32(pCompFileInfo->header.nOrFileSize) )
		{
			safe_free(lpBuf);
			SAFE_DELETE(pCompFileInfo);
			return -34;
		}
		if( !tComToken.TokenDel_32(pCompFileInfo->header.nComFileSize) )
		{
			safe_free(lpBuf);
			SAFE_DELETE(pCompFileInfo);
			return -35;
		}

		if(pCompFileInfo->header.nComFileSize == 0)
			pCompFileInfo->body.data = NULL;
		else
		{
			pCompFileInfo->body.data = malloc(pCompFileInfo->header.nComFileSize + 1);
			if(pCompFileInfo->body.data == NULL)
			{
				safe_free(lpBuf);
				SAFE_DELETE(pCompFileInfo);
				return -36;
			}
			memset(pCompFileInfo->body.data, 0, pCompFileInfo->header.nComFileSize + 1);
			
			if( tComToken.TokenDel_PVOID(pCompFileInfo->body.data, pCompFileInfo->header.nComFileSize) < 0)
			{
				safe_free(lpBuf);
				SAFE_DELETE(pCompFileInfo);
				return -37;
			}
		}		
		tASICompFileInfoList.push_back(pCompFileInfo);
		nFileNum--;
	}
	safe_free(lpBuf);
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::AnalysisCompFileAndDecomp(LPCSTR lpFileName, LPCSTR lpTargetDir)
{
	INT32 nIndex = 0;
	INT32 nFileNum = 0;
	ASI_COMP_FILE_VER stCompFileVer;
	ASI_COMP_FILE_INFO stFileInfo;

	if(lpFileName == NULL || lpTargetDir == NULL)
		return -1;

	FileToken tComToken(lpFileName);
	
	if( !tComToken.TokenDel_16(stCompFileVer.nMajorVer))
	{
		return -10;
	}
	if( !tComToken.TokenDel_16(stCompFileVer.nMinorVer))
	{
		return -11;
	}
	if( !tComToken.TokenDel_32(stCompFileVer.nCreateDateTime))
	{
		return -12;
	}
	if( !tComToken.TokenDel_32(stCompFileVer.nFileNum))
	{
		return -13;
	}
	
	if(stCompFileVer.nMajorVer != ASI_COMP_VER_MAJOR || stCompFileVer.nMinorVer != ASI_COMP_VER_MINOR)
	{
		return -20;
	}
	
	sgc_notify_info._add_info("", "", stCompFileVer.nFileNum, 0, lpCallBackHandle);

	nFileNum = stCompFileVer.nFileNum;
	while(nFileNum)
	{
		if( tComToken.TokenDel_String(stFileInfo.header.strSubPath) < 0)
		{
			return -31;
		}
		if( tComToken.TokenDel_String(stFileInfo.header.strFileName) < 0)
		{
			return -32;
		}
		if( !tComToken.TokenDel_32(stFileInfo.header.nOrFileSize) )
		{
			return -33;
		}
		if( !tComToken.TokenDel_32(stFileInfo.header.nComFileSize) )
		{
			return -34;
		}

		if(stFileInfo.header.nComFileSize != 0)
		{
			stFileInfo.body.data = malloc(stFileInfo.header.nComFileSize + 1);
			if(stFileInfo.body.data == NULL)
			{
				return -35;
			}
			memset(stFileInfo.body.data, 0, stFileInfo.header.nComFileSize + 1);
			
			if( tComToken.TokenDel_PVOID(stFileInfo.body.data, stFileInfo.header.nComFileSize) < 0)
			{
				safe_free(stFileInfo.body.data);
				return -36;
			}

			if(DecompressFile(&stFileInfo, lpTargetDir))
			{
				safe_free(stFileInfo.body.data);
				return -4;
			}
			nIndex++;
			sgc_notify_info._add_info(stFileInfo.header.strSubPath.c_str(), stFileInfo.header.strFileName.c_str(), nIndex);
			if(f_NotifyState)	
				f_NotifyState(&sgc_notify_info);
			safe_free(stFileInfo.body.data);
		}		
		nFileNum--;
	}
	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressFile(PASI_COMP_FILE_INFO pCompFileInfo, LPCSTR pDefaultTargetPath)
{
	INT32 nNewLen = 0;
	PVOID lpOrBuf = NULL;
	CFileUtil tFileUtil;
	String strSaveName;
	INT32 fd = -1;
	
	if(pCompFileInfo == NULL || pDefaultTargetPath == NULL)
		return -1;

	nNewLen = pCompFileInfo->header.nOrFileSize;
	if(nNewLen < 1)
		return -2;

	lpOrBuf = malloc(nNewLen + 1);
	if(lpOrBuf == NULL)
		return -3;
	memset(lpOrBuf, 0, nNewLen + 1);
	
	if(DecompressEngine(pCompFileInfo->body.data, pCompFileInfo->header.nComFileSize, lpOrBuf, pCompFileInfo->header.nOrFileSize))
	{
		safe_free(lpOrBuf);
		return -4;
	}

	
	strSaveName = pDefaultTargetPath;
	if(pCompFileInfo->header.strSubPath.length() != 0)
	{
		tFileUtil.AddLasCHAR(strSaveName);
		strSaveName += pCompFileInfo->header.strSubPath;
	}

	if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE)	
		tFileUtil.ForceDirectory(strSaveName.c_str());

	if(pCompFileInfo->header.strFileName.length() == 0)

	{
		safe_free(lpOrBuf);
		return -5;
	}
	tFileUtil.AddLasCHAR(strSaveName);
	strSaveName += pCompFileInfo->header.strFileName;

	if(tFileUtil.FileExists(strSaveName.c_str()) == TRUE)
		unlink(strSaveName.c_str());

	fd = open(strSaveName.c_str(), O_CREAT | O_RDWR, 0666);
	if(fd == -1)
	{
		safe_free(lpOrBuf);
		return -6;
	}

	if(WriteFile_SG(fd, lpOrBuf, pCompFileInfo->header.nOrFileSize) != 0)
	{
		close(fd);
		safe_free(lpOrBuf);
		return -7;
	}

	close(fd);
	safe_free(lpOrBuf);

	return 0;
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressFileAndSave(LPCSTR lpFileName, LPCSTR lpSaveDir, LPCSTR lpSaveName)
{
	PVOID lpBuf = NULL;
	PVOID lpOrBuf = NULL;
	INT32 nLen = 0;
	INT32 nNewLen = 0;
	INT32 fd = -1;
	CFileUtil tFileUtil;
	String strSaveName;
	ASI_COMP_FILE_INFO stCompFileInfo;

	if(lpFileName == NULL || lpSaveDir == NULL || lpSaveName == NULL)
		return -1;

	if(ReadFileBuf(lpFileName, &lpBuf, nLen))
	{
		safe_free(lpBuf);
		return -1;
	}

	CompMemToken tComToken(lpBuf, nLen);

	if( tComToken.TokenDel_String(stCompFileInfo.header.strFileName) < 0)
	{
		safe_free(lpBuf);
		return -32;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nOrFileSize) )
	{
		safe_free(lpBuf);
		return -33;
	}
	if( !tComToken.TokenDel_32(stCompFileInfo.header.nComFileSize) )
	{
		safe_free(lpBuf);
		return -34;
	}

	if(stCompFileInfo.header.nComFileSize == 0 || stCompFileInfo.header.nOrFileSize == 0)
	{
		safe_free(lpBuf);
		return -35;
	}
	
	stCompFileInfo.body.data = malloc(stCompFileInfo.header.nComFileSize + 1);
	if(stCompFileInfo.body.data == NULL)
	{
		safe_free(lpBuf);
		return -2;
	}
	memset(stCompFileInfo.body.data, 0, stCompFileInfo.header.nComFileSize + 1);
	
	if( tComToken.TokenDel_PVOID(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize) < 0)
	{
		safe_free(lpBuf);
		return -36;
	}

	safe_free(lpBuf);
	
	nNewLen = stCompFileInfo.header.nOrFileSize;
	
	lpOrBuf = malloc(nNewLen + 1);
	if(lpOrBuf == NULL)
		return -3;
	
	memset(lpOrBuf, 0, nNewLen + 1);
		
	if(DecompressEngine(stCompFileInfo.body.data, stCompFileInfo.header.nComFileSize, lpOrBuf, nNewLen))
	{
		safe_free(lpOrBuf);
		return -4;
	}

	strSaveName = lpSaveDir;
	if(tFileUtil.DirectoryExists(strSaveName.c_str()) == FALSE)	
		tFileUtil.ForceDirectory(strSaveName.c_str());
	
	tFileUtil.AddLasCHAR(strSaveName);	
	strSaveName += lpSaveName;

	if(tFileUtil.FileExists(strSaveName.c_str()) == TRUE)
		unlink(strSaveName.c_str());

	fd = open(strSaveName.c_str(), O_CREAT | O_RDWR, 0666);
	if(fd == -1)
	{
		usleep(1000);
		fd = open(strSaveName.c_str(), O_CREAT | O_RDWR, 0666);
		if(fd == -1)
		{
			safe_free(lpOrBuf);
			return -5;
		}
	}

	if(WriteFile_SG(fd, lpOrBuf, nNewLen) != 0)
	{
		close(fd);
		safe_free(lpOrBuf);
		return -6;
	}
	close(fd);
	safe_free(lpOrBuf);
	return 0;
}

INT32		CCompUtil::CompressFileAndSaveW(wchar_t *wcFileName, LPCSTR szSaveDir, LPCSTR szSaveName)
{
	String strFileName;
	StringW strFileNameW = wcFileName;
	strFileName = ConvertAnsiString(strFileNameW);
	return CompressFileAndSave(strFileName.c_str(), szSaveDir, szSaveName);
}
//-----------------------------------------------------------------------------------

INT32		CCompUtil::DecompressFileAndSaveW(LPCSTR szFileName, wchar_t *wzSaveDir, wchar_t * wzSaveName)
{
	String strSaveDir;
	String strSaveName;
	StringW strSaveDirW = wzSaveDir;
	StringW strSaveNameW = wzSaveName;
		
	strSaveDir = ConvertAnsiString(strSaveDirW);
	strSaveName = ConvertAnsiString(strSaveNameW);

	return DecompressFileAndSave(szFileName, strSaveDir.c_str(), strSaveName.c_str());
}

