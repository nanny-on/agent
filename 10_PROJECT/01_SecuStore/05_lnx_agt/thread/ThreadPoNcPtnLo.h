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

#if !defined(AFX_THREAD_PO_NC_PTN_LO_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
#define AFX_THREAD_PO_NC_PTN_LO_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CThreadPoNcPtnLo thread

class CThreadPoNcPtnLo : public CThreadBase
{
public:
	CThreadPoNcPtnLo();           // protected constructor used by dynamic creation
	virtual ~CThreadPoNcPtnLo();

private:
	UINT32			m_nOrderID;
	UINT32			m_nWorkOrderID;
//	CASIFFDLLUtil	m_tFFDLLUtil;
//	CASIWEngDLLUtil	m_tWEDLLUtil;
//	CASIFIDLLUtil	m_tFIDLLUtil;
//	CASIHASHDLLUtil	m_tHashDLLUtil;
//	CASIFCIDLLUtil	m_tFCIDLLUtil;

private:
//	TMapWEngWLEx	m_tWEngWLExMap;
	TMapID64		m_tOrderIDMap;
	TListID64		m_tOrderIDList;
	CMutexExt		m_tOrderMutex;

public:
	void			AddWork(UINT64 nOrderID);
	UINT32			IsWork();
	UINT32			NextWork();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoNcPtnLo)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

private:
	DB_PO_NC_PTN_LO		m_tCurPolicy;

public:
	INT32		InitEnv();
	INT32		FreeEnv();

	INT32		ChkScanFile();
	INT32		SavePtnFile();
	INT32		GatherFileInfo(PDB_PTN_PROC_FILE pdata);
	

// Implementation
protected:

};

extern	CThreadPoNcPtnLo*	t_ThreadPoNcPtnLo;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREAD_PO_NC_PTN_LO_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
