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

// CompUtil.h: interface for the CCompUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPUTIL_H__03323207_0A00_4F02_BAE6_927CA4F15D2E__INCLUDED_)
#define AFX_COMPUTIL_H__03323207_0A00_4F02_BAE6_927CA4F15D2E__INCLUDED_

#include "MiniLzo.h"
#include "CompMemToken.h"
//#include "fileutil.h"

#define HEAP_ALLOC(var,size) lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]
static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);

class CCompUtil  
{
private:
	NotifyStateType		f_NotifyState;
	PVOID				lpCallBackHandle;
	INT32				m_nInitEngine;

	ASI_COMP_NOTIFY_INFO		sgc_notify_info;
private:
	INT32		InitEngine();
	INT32		CompressEngine(PVOID lpInput, INT32 nOrLen, PVOID lpOutput, INT32& nChLen);
	INT32		DecompressEngine(PVOID lpInput, INT32 nChLen, PVOID lpOutput, INT32 nOrLen);

private:
	INT32		AnalysisCompFile(LPCSTR lpFileName, TListASICompFileInfo& tASICompFileInfoList);
	INT32		AnalysisCompFileAndDecomp(LPCSTR lpFileName, LPCSTR lpTargetDir);
	INT32		CompressFile(PFF_GATHER_FILE_INFO pgather_file_info, CompMemToken& tSaveToken);
	INT32		GetFileSizeEx(LPCSTR lpFileName);
	INT32		ReadFileBuf(LPCSTR lpFileName, PVOID* lpBuf, INT32& nLen);
	INT32		WriteFileComp(LPCSTR lpSaveName, CompMemToken& tHeadToken, CompMemToken& tBodyToken);
	INT32		WriteFileComp(LPCSTR lpSaveName, CompMemToken& tToken, UINT64 nMovePos = 0xFFFFFFFFFFFFFFFF);
	INT32		WriteFile_SG(INT32 nFd, PVOID lpBuf, DWORD dwLen);
	INT32		WriteFile_SG(LPCSTR lpSaveName, PVOID lpBuf, DWORD dwLen);

	INT32		ReadFileBufW(wchar_t *wzFileName, PVOID* lpBuf, INT32& nLen);

private:
	INT32		DecompressFile(PASI_COMP_FILE_INFO pac_file_info, LPCSTR lpDefaultTargetPath);

public:
	INT32		SetNotifyState(PVOID lParam, PVOID NotifyState);
	INT32		CompressDir(PASI_COMP_INFO paci);
	INT32		DecompressDir(PASI_DECOMP_INFO padi);

	INT32		CompressFileAndSave(LPCSTR lpFileName, LPCSTR lpSaveDir, LPCSTR lpSaveName);
	INT32		DecompressFileAndSave(LPCSTR lpFileName, LPCSTR lpSaveDir, LPCSTR lpSaveName);

	INT32		CompressFileAndSaveW(wchar_t *wcFileName, LPCSTR szSaveDir, LPCSTR szSaveName);
	INT32		DecompressFileAndSaveW(LPCSTR szFileName, wchar_t *wcSaveDir, wchar_t *wcSaveName);

	INT32		CompressBufferAndSave(LPCSTR lpName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCSTR lpSaveDir, LPCSTR lpSavedFile);
	INT32		DecompressFileAndBuffer(LPCSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen);

	INT32		CompressBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCSTR lpRelativeFilePath, LPCSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
	INT32		CompressFileAndBuffer(LPCSTR lpRelativeFilePath, LPCSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
	INT32		DecompressBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCSTR lpSaveDir, LPSTR lpSaveFile, INT32 nSaveFileLen, LPCSTR lpSaveFileAddOn, UINT32 nUsedComp);

public:	
	CCompUtil();
	virtual ~CCompUtil();

};

#endif // !defined(AFX_COMPUTIL_H__03323207_0A00_4F02_BAE6_927CA4F15D2E__INCLUDED_)
