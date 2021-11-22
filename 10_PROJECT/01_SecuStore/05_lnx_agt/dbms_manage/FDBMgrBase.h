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
#ifndef FDBMgrBaseH
#define FDBMgrBaseH
//---------------------------------------------------------------------------

class CFDBMgrBase
{
public:
	CString			m_strDBTName;
    UINT32			m_nLoadMaxID;
    UINT32			m_nLoadNumber;
    UINT32			m_nLoadNum;
    UINT32			m_nSyncDBNum;
    UINT64			m_nNewID;
	UINT32 			m_nDBSVer;
    INT32			m_tTCIdxBuff[200];

public:
//	CEncryptUtil		m_tEncUtil;
	CASIDBFQDLLUtil*	m_tASIDBFQDLLUtil;

public:
	void			SetDBFQUtil(CASIDBFQDLLUtil*	tASIDBFQDLLUtil);

public:
	INT32			DBOP_Check(INT32 nRtn);
    CString			ConvertListToCString(TListID& tIDList);

public:
	INT32			ExecuteQuery(CString strQuery);
	UINT32  		GetLastID();
	INT32			SetAutoInc(UINT32 nInitVal = 0);
	UINT32			GetTbLastID();
    UINT64  		GetLastFp();
	CString			GetTableName();
	INT32       	First();
	INT32       	Next();
    UINT64			GetDBField_Fp();
	UINT64      	GetDBField_UInt64(UINT32 nIndex);
	UINT32      	GetDBField_UInt(UINT32 nIndex);
	INT32       	GetDBField_Int(UINT32 nIndex);
	CString      	GetDBField_String(UINT32 nIndex);
	CStringW		GetDBField_StringW(UINT32 nIndex);

	UINT64      	GetDBFieldLow_UInt64(UINT32 nIndex);
	UINT32      	GetDBFieldLow_UInt(UINT32 nIndex);
	INT32       	GetDBFieldLow_Int(UINT32 nIndex);
	CString      	GetDBFieldLow_String(UINT32 nIndex);

	UINT64      	GetDBFieldLow_UInt64(CString strField);
	UINT32      	GetDBFieldLow_UInt(CString strField);
	INT32       	GetDBFieldLow_Int(CString strField);
	CString      	GetDBFieldLow_String(CString strField);

    INT32			GetDBFieldLow_ColIdx(CString strCName);


	DB_POLICY		GetDBField_Po(INT32& nIndex);
	DB_PO_HEADER	GetDBField_PoHDR(INT32& nIndex);
	DB_PO_HEADER	GetDBField_PoPkgHDR(INT32& nIndex);

public:
	CString			GetPoQuery_InsertValue(DB_POLICY& tDP);
	CString			GetPoQuery_Update(DB_POLICY& tDP);
	
	CString			GetPoHDRQuery_InsertValue(DB_PO_HEADER& tDPH);
	CString			GetPoHDRQuery_Update(DB_PO_HEADER& tDPH);

	CString			GetPoPkgHDRQuery_InsertValue(DB_PO_HEADER& tDPH);
	CString			GetPoPkgHDRQuery_Update(DB_PO_HEADER& tDPH);

public:
	void			SetCurTableName(LPCTSTR lpTableName);
	void			SetCurTableName(CString lpTableName);
	INT32			PreSetValue16(LPCTSTR lpColName, UINT16 nValue);
	INT32			PreSetValue32(LPCTSTR lpColName, UINT32 nValue);
	INT32			PreSetValue64(LPCTSTR lpColName, UINT64 nValue);
	INT32			PreSetValue(LPCTSTR lpColName, CString& strValue); 

public:
	INT32			IsExistTable();

public:
    CString      	ChangeFilePathToDBPath(CString strFilePath);
    CString      	ChangeDBPathToFilePath(CString strDBPath);

	INT32			ResetTable();

public:
	virtual	INT32	LoadExecute(PVOID lpTempletList)					{	return 0;	};
    virtual INT32	SyncDBMSToDB(PVOID lpTempletList)					{	return 0;	};
    virtual	INT32	SelectExecute(UINT32 nID, PVOID lpTemplet)			{	return 0;	};
	virtual	INT32	InsertExecute(PVOID lpTemplet)						{	return 0;	};
	virtual	INT32	UpdateExecute(PVOID lpTemplet)						{	return 0;	};
	virtual	INT32	DeleteExecute(UINT32 nID);
	virtual	INT32	GetCountExecute()									{	return 0;	};

	virtual INT32	DeleteExecuteFp(UINT64 nDBFp);
	virtual INT32	DeleteExecuteFps(TListID64& tListFp);

public:
	virtual INT32	SetInitalize()			{ return 0;	};
	virtual	INT32	LoadDB();


public:
	CFDBMgrBase();
    ~CFDBMgrBase();
};


#endif /*FDBMgrBaseH*/
