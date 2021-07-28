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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "asi_glb_w32_handle.h"
#include "asi_glb_w32_file.h"
#include "ASIDBFUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::OpenDBF(CString strDBFileName, CString strUserID, CString strUserPw, UINT32 nAccessMode, UINT32 nAutoLogin)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	UINT32 nFileShareMode = FILE_SHARE_READ;
	INT32 nRetVal = 0;
	CloseDBF();

	if(nAccessMode == (GENERIC_READ))
		nFileShareMode += FILE_SHARE_WRITE;
	hFile = CreateFile((LPCTSTR)strDBFileName, nAccessMode, nFileShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		INT32 nLErr = GetLastError();
		WriteLogE("open dbf database fail : [%d][%x][%x]", nLErr, nAccessMode, nFileShareMode);
		CloseDBF();

		if(nLErr == ERROR_SHARING_VIOLATION)	
			return ASI_DBF_ERROR_TYPE_OPEN_DB_FAIL_USED_AN_PROC;

		return ASI_DBF_ERROR_TYPE_OPEN_FILE_FAIL;
	}
	SetFileHandle(hFile, ASI_DBF_FILE_HANDLE_ALL);

	WriteLogN("dbf file open success : ac[%x]", nAccessMode);

	m_strDBFName = strDBFileName;
	m_nDBFLastFP = 0;

	DWORD dwSize = 0;
	nRetVal = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwSize);
	if(nRetVal != 0 || dwSize < sizeof(DBF_DATABASE))
	{
		WriteLogE("invalid file size (%d) (%d)", nRetVal, dwSize);
		CloseDBF();
		return ASI_DBF_ERROR_TYPE_V2_NOT_DBF_DATABASE_HEADER;	
	}

	nRetVal = ReadDBF_DI(&m_tDB);
	if(nRetVal != 0)
	{
		SetER(nRetVal);
		WriteLogE("read dbf database header fail : (%d) [%.8x]", nRetVal, GetER());
		CloseDBF();
		return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_READ_FAIL;
	}

	if(m_tDB.tVer.nVersion != ASI_DBF_MODULE_VERSION)
	{
		WriteLogE("invalid dbf file module version : [%d:%d]", m_tDB.tVer.nVersion, ASI_DBF_MODULE_VERSION);
		CloseDBF();
		return ASI_DBF_ERROR_TYPE_V2_INVALID_DBF_VERSION;
	}

	WriteLogN("dbf file open success step 1 : ac[%x]", nAccessMode);

	{	
		if(!nAutoLogin && LoginDBF(strUserID, strUserPw))
		{
			CloseDBF();
			return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_LOGIN_INFO;	
		}
		m_nAuthResult = 1;
	}

	WriteLogN("dbf file open success step 2 : ac[%x]", nAccessMode);

	{
		if(SetER(ReadDBF_TableList()))
		{
			WriteLogE("ReadDBF Table List Fail [%.8x]", GetER());
			CloseDBF();
			return GetER();
		}

		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
		m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	}

	if(m_tDB.tInfo.nAutoRestore)
	{
		WriteLogN("dbf open success and auto recovery start...");
		return RecoveryDBF(1);
	}


	WriteLogN("dbf open success : ac[%x]", nAccessMode);

	return 0;	
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CreateDBF(CString strDBFileName, CString strUserID, CString strUserPw)
{
	CloseDBF();

	CFileUtil tFileUtil;
	if(tFileUtil.FileExists(strDBFileName))
	{
		WriteLogE("already exist dbf file : [%s]", (char *)(LPCSTR)strDBFileName);
		return ASI_DBF_ERROR_TYPE_V2_ALREADY_EXIST_DBF_FILE;	
	}
	
	HANDLE hFile = CreateFile(strDBFileName, (GENERIC_READ|GENERIC_WRITE), 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ASI_DBF_ERROR_TYPE_OPEN_FILE_FAIL;
	SetFileHandle(hFile, ASI_DBF_FILE_HANDLE_ALL);

	m_strDBFName = strDBFileName;
	m_nDBFLastFP = 0;

	DWORD dwHigh, dwLow;
	dwLow = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwHigh);
	if(MAKEUINT64(dwLow, dwHigh) > 0)
	{
		WriteLogE("already exist dbf file : [%s]", (char *)(LPCSTR)strDBFileName);
		return ASI_DBF_ERROR_TYPE_V2_ALREADY_EXIST_DBF_FILE;	
	}

	{	
		memset(&m_tDB, 0, sizeof(DBF_DATABASE));
		m_tDB.tVer.nVersion = ASI_DBF_MODULE_VERSION;	
		
		if(SetER(WriteDBF_DI(&m_tDB)))
		{
			WriteLogE("write dbf database header fail : [%.8x]", GetER());
			CloseDBF();
			return ASI_DBF_ERROR_TYPE_V2_DBF_DATABASE_WRITE_FAIL;
		}
		m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	}

	if(strUserID.IsEmpty() == FALSE && strUserPw.IsEmpty() == FALSE)
	{
		ChangeAuthInfoDBF("", "", strUserID, strUserPw, ASI_DBF_AUTH_INFO_TYPE_SHA2);
	}
	CloseDBF();
	return 0;	
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CloseDBF()
{
	TMapPVOIDItor begin, end;
	begin = m_tFHMap.begin();	end = m_tFHMap.end();
	for(begin; begin != end; begin++)
	{
		CloseHandle((HANDLE)(begin->second));
	}
	m_tFHMap.clear();
	ClearMember();

	m_nAuthResult = 0;

	return 0;
}
//---------------------------------------b------------------------------

INT32		CASIDBFUtil::RemoveDBF(UINT32 nRenameMode)
{
	CloseDBF();	
	if(nRenameMode)
	{
		CString strDBFDBName;
		strDBFDBName.Format("%s_%u_%u", m_strDBFName, GetCurrentDateTimeInt(), GetTickCount());
		MoveFileEx((LPCSTR)m_strDBFName, (LPCSTR)strDBFDBName);
	}
	else
	{
		unlink((char *)(LPCSTR)m_strDBFName);	
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::RecoveryDBF(UINT32 nRenameMode)
{	
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_READ))		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;
	
	UINT64 nCurFSZ = 0, nNewFSZ = 0;
	{
		DWORD dwHigh, dwLow;
		dwLow = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwHigh);
		nCurFSZ	= MAKEUINT64(dwLow, dwHigh);
	}
	
	CASIDBFUtil tASIDBFUtil;
	CString strTempDBFName = m_strDBFName + ".temp";
	if(SetER(tASIDBFUtil.CreateDBF(strTempDBFName, "", "")))
	{
		WriteLogE("create dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_CREATE_DBF_FAIL;
	}
	
	if(SetER(tASIDBFUtil.OpenDBF(strTempDBFName, "", "", (GENERIC_READ|GENERIC_WRITE), 1)))
	{
		WriteLogE("open dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_CREATE_DBF_FAIL;
	}
	
	if(SetER(tASIDBFUtil.CopyAuthInfoDBF(&(m_tDB.tAuth))))
	{
		WriteLogE("set auth info dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	if(SetER(tASIDBFUtil.SetTItemEncTypeDBF(m_tDB.tAuth.nDefEncType)))
	{
		WriteLogE("set auth info (te) dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	if(SetER(tASIDBFUtil.SetTItemCompTypeDBF(m_tDB.tAuth.nDefCompType)))
	{
		WriteLogE("set auth info (tc) dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	{
		TListDBFTableSearchItor begin, end;
		begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->tDT.tHInfo.nDelFlag)		continue;

			DBF_TABLE_SEARCH tDTS = *begin;
			{
				tDTS.tDT.tHInfo.nLastID			= 0;
				tDTS.tDT.tHInfo.nItemCnt		= 0;
				tDTS.tDT.tHInfo.nItemTCnt		= 0;
				tDTS.tDT.tHInfo.nHeadTableFP	= 0;
				tDTS.tDT.tHInfo.nTailTableFP	= 0;
				tDTS.tDT.tHInfo.nFirstItemFP	= 0;
				tDTS.tDT.tHInfo.nCheckItemFP	= 0;
				tDTS.tDT.tHInfo.nLastItemFP		= 0;
			}		
			tASIDBFUtil.CreateDBF_Table(&tDTS);
		}
	}

	{
		CASIDBFMember* pMember = GetMember();
		pMember->m_strSelTName = "";
		
		TListDBFTableSearchItor begin, end;
		begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->tDT.tHInfo.nDelFlag)		continue;

			PDBF_TABLE_SEARCH pdts_new = tASIDBFUtil.FindDBF_Table(begin->tDT.tHInfo.szName);
			if(!pdts_new)	continue;

			WriteLogN("recovery database table [%s][%u][%u] start", begin->tDT.tHInfo.szName, begin->tDT.tHInfo.nItemCnt, pdts_new->tDT.tHInfo.nLastID);
			UINT32 nSTime = GetCurrentDateTimeInt();
			
			//INT32 nChkIdx = 0;			
			FirstDBF_Item(&(*begin));
			while(NextDBF_Item() == 0)
			{
				//WriteLogN("debug recovery check index : [%d]", nChkIdx++);
				tASIDBFUtil.InsertDBF_TItem(pdts_new, &(pMember->m_tCurDBFIH), pMember->m_tCurDBFILenMap, pMember->m_pCurItemBuff);
			}
			FreeCommonBuff(&(pMember->m_pCurItemBuff));

			WriteLogN("recovery database table [%s][%d] end : opt : %u(sec)", begin->tDT.tHInfo.szName, pdts_new->tDT.tHInfo.nLastID, GetCurrentDateTimeInt() - nSTime);
		}
	}

	{
		tASIDBFUtil.CloseDBF();
		RemoveDBF(nRenameMode);

		MoveFileEx((LPCSTR)strTempDBFName, (LPCSTR)m_strDBFName);
		OpenDBF(m_strDBFName, "", "", (GENERIC_READ|GENERIC_WRITE), 1);		
	}

	{
		DWORD dwHigh, dwLow;
		dwLow = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwHigh);
		nNewFSZ	= MAKEUINT64(dwLow, dwHigh);
	}

	WriteLogN("recovery dbf [%I64u]->[%I64u] : [%I64d]", nCurFSZ, nNewFSZ, (nCurFSZ-nNewFSZ));
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::BackupDBF(CString strBkFileName)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_READ))		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	UINT64 nCurFSZ = 0, nNewFSZ = 0;
	{
		DWORD dwHigh, dwLow;
		dwLow = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwHigh);
		nCurFSZ	= MAKEUINT64(dwLow, dwHigh);
	}

	CASIDBFUtil tASIDBFUtil;
	CString strTempDBFName = strBkFileName;
	if(SetER(tASIDBFUtil.CreateDBF(strTempDBFName, "", "")))
	{
		WriteLogE("create dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_CREATE_DBF_FAIL;
	}

	if(SetER(tASIDBFUtil.OpenDBF(strTempDBFName, "", "", (GENERIC_READ|GENERIC_WRITE), 1)))
	{
		WriteLogE("open dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_CREATE_DBF_FAIL;
	}

	if(SetER(tASIDBFUtil.CopyAuthInfoDBF(&(m_tDB.tAuth))))
	{
		WriteLogE("set auth info dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	if(SetER(tASIDBFUtil.SetTItemEncTypeDBF(m_tDB.tAuth.nDefEncType)))
	{
		WriteLogE("set auth info (te) dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	if(SetER(tASIDBFUtil.SetTItemCompTypeDBF(m_tDB.tAuth.nDefCompType)))
	{
		WriteLogE("set auth info (tc) dbf_temp fail : [%s]", (char *)(LPCSTR)strTempDBFName);
		return ASI_DBF_ERROR_TYPE_V2_AUTH_INCORRECT_INFO;
	}

	{
		TListDBFTableSearchItor begin, end;
		begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->tDT.tHInfo.nDelFlag)		continue;

			DBF_TABLE_SEARCH tDTS = *begin;
			{
				tDTS.tDT.tHInfo.nLastID			= 0;
				tDTS.tDT.tHInfo.nItemCnt		= 0;
				tDTS.tDT.tHInfo.nItemTCnt		= 0;
				tDTS.tDT.tHInfo.nHeadTableFP	= 0;
				tDTS.tDT.tHInfo.nTailTableFP	= 0;
				tDTS.tDT.tHInfo.nFirstItemFP	= 0;
				tDTS.tDT.tHInfo.nCheckItemFP	= 0;
				tDTS.tDT.tHInfo.nLastItemFP		= 0;
			}		
			tASIDBFUtil.CreateDBF_Table(&tDTS);
		}
	}

	{
		CASIDBFMember* pMember = GetMember();
		pMember->m_strSelTName = "";

		TListDBFTableSearchItor begin, end;
		begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->tDT.tHInfo.nDelFlag)		continue;

			PDBF_TABLE_SEARCH pdts_new = tASIDBFUtil.FindDBF_Table(begin->tDT.tHInfo.szName);
			if(!pdts_new)	continue;

			WriteLogN("recovery database table [%s][%u] start", begin->tDT.tHInfo.szName, begin->tDT.tHInfo.nItemCnt);
			UINT32 nSTime = GetCurrentDateTimeInt();

			FirstDBF_Item(&(*begin));
			while(NextDBF_Item() == 0)
			{
				tASIDBFUtil.InsertDBF_TItem(pdts_new, &(pMember->m_tCurDBFIH), pMember->m_tCurDBFILenMap, pMember->m_pCurItemBuff);
			}
			FreeCommonBuff(&(pMember->m_pCurItemBuff));

			WriteLogN("recovery database table [%s] end : opt : %u(sec)", begin->tDT.tHInfo.szName, GetCurrentDateTimeInt() - nSTime);
		}
	}

	{
		tASIDBFUtil.CloseDBF();
	}

	WriteLogN("backup dbf [%s]", (char *)(LPCSTR)strBkFileName);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::IsAutoRestore(INT32& nMode)
{
	nMode = m_tDB.tInfo.nAutoRestore; 
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::WriteDBF_DI(PDBF_DATABASE pdd)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
	
	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0);
	
	if(SetER(WriteDBF(pdd, sizeof(DBF_DATABASE))))
	{
		WriteLogE("WriteDBF_DI fail [%.8x]", GetER());
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		return GetER();
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ReadDBF_DI(PDBF_DATABASE pdd)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_READ))		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;

	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_READ);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, 0);

	if(SetER(ReadDBF(pdd, sizeof(DBF_DATABASE))))
	{
		WriteLogE("ReadDBF_DI Fail [%.8x]", GetER());
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, nCurFP);
		return GetER();
	}
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, nCurFP);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::UpdateDBF_DI(PDBF_DATABASE pdd)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
	if(!pdd)	pdd = &m_tDB;

	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0);
	
	if(SetER(WriteDBF(pdd, sizeof(DBF_DATABASE))))
	{
		WriteLogE("UpdateDBF_DI fail [%.8x]", GetER());
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		return GetER();
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
	return 0;
}
//---------------------------------------------------------------------




