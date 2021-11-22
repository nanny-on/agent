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
#if !defined(AFX_THREAD_PO_FA_EX_POPUP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
#define AFX_THREAD_PO_FA_EX_POPUP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaExPopup thread

class CThreadPoFaExPopup : public CThreadBase
{
public:
	CThreadPoFaExPopup();           // protected constructor used by dynamic creation
	virtual ~CThreadPoFaExPopup();

private:	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoFaExPopup)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	INT32		IsEndWork()							{	return m_nPause;	};
	INT32		ChkExPopupRegList();
	INT32		ChkExPopupRegInfo(REG_VALUE_INFO& tRVI);
	INT32		ChkExPopupFileInfo(String strFileNameNew, String strFileNameOrg);

// Implementation
protected:
	CRegUtil	m_tRegUtil;

};

extern	CThreadPoFaExPopup*	t_ThreadPoFaExPopup;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREAD_PO_FA_EX_POPUP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
