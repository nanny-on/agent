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

#if !defined(AFX_THREAD_PO_FE_SCAN_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
#define AFX_THREAD_PO_FE_SCAN_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreadPoFeScan.h : header file
//

#define		ASI_EPS_APP_LOG_TYPE_SITE_FILE				3



/////////////////////////////////////////////////////////////////////////////
// CThreadPoFeScan thread

class CThreadPoFeScan : public CThreadBase
{
public:
	CThreadPoFeScan();           // protected constructor used by dynamic creation
	virtual ~CThreadPoFeScan();

private:	
	CASIFFDLLUtil	m_tFFDLLUtil;
//	CASIWEngDLLUtil	m_tWEDLLUtil;
//	CASIFIDLLUtil	m_tFIDLLUtil;
//	CASIHASHDLLUtil	m_tHashDLLUtil;
	TMapWEngWLEx	m_tWEngWLExMap;

private:
	DB_PO_FE_PTN_OP			m_tCurPolicy;

private:
	UINT32					m_nOrderID;

private:
	UINT32					m_nWorkID;
	TMapMemFindOrderInfo	m_tWorkInfoMap;
	CMutexExt				m_tWorkMutex;

public:
	UINT32		GetNextOrderID()					{	return ++m_nOrderID;			};

	INT32		AddWork(PMEM_FIND_ORDER_INFO pMFOI);
	INT32		DelWork(UINT32 nID);
	UINT32		GetNextWork();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoFeScan)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL


private:
	INT32		StartScan();
	INT32		StopScan();

	INT32		ChkScanFile();
	INT32		ProcessScanResult();
	

// Implementation
protected:

};

extern	CThreadPoFeScan*	t_ThreadPoFeScan;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREAD_PO_FE_SCAN_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
