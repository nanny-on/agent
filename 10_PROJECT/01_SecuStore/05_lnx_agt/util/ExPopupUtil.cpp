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

#include "stdafx.h"
#include "com_struct.h"
#include "ExPopupUtil.h"

CExPopupUtil*		t_ExPopupUtil = NULL;

CExPopupUtil::CExPopupUtil(void)
{
	m_strUtilName = "util ex popup";

//	m_tRegUtil.SetRootPath(STR_REG_DEFAULT_SVC_EX_POPUP_PATH);
//	m_tRegUtil.SetWowDefaultKey();
}
//---------------------------------------------------------------------------

CExPopupUtil::~CExPopupUtil(void)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CExPopupUtil::SetOpType(UINT32 nOpType)
{
	t_EnvInfo->SetReg_ExPopup_OpType(nOpType);

	if(!nOpType)	RemoveComObject();
	else			RegistComObject();

	return 0;
}
//---------------------------------------------------------------------------

INT32		CExPopupUtil::GetOpType()
{
	return t_EnvInfo->GetReg_ExPopup_OpType();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CExPopupUtil::AddOpType(UINT32 nOpType)
{
	UINT32 nOldOpType = GetOpType();

	UINT32 nNewOpType = (nOldOpType | nOpType) ;

	return SetOpType(nNewOpType);
}
//---------------------------------------------------------------------------

INT32		CExPopupUtil::DelOpType(UINT32 nOpType)
{
	UINT32 nOldOpType = GetOpType();

	UINT32 nNewOpType = nOldOpType - (nOldOpType & nOpType) ;

	return SetOpType(nNewOpType);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CExPopupUtil::RegistComObject()
{		
/*
	CProcUtil tProcUtil;
	String strComObjName;
	strComObjName.Format("%s/dll/asi_ex_popup.so", t_EnvInfo->m_strRootPath);
	if(tProcUtil.RegistOcx(strComObjName))
	{		
		WriteLogN("[%s] execute %s regist success ", m_strUtilName, STR_PROC_NAME_REGSVR32);		
	}
	else
	{
		WriteLogE("[%s] execute %s regist fail : [%d] ", m_strUtilName, STR_PROC_NAME_REGSVR32, GetLastError());		
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CExPopupUtil::RemoveComObject()
{
/*
	CProcUtil tProcUtil;
	
	String strComObjName;
	strComObjName.Format("%s/dll/asi_ex_popup.so", t_EnvInfo->m_strRootPath);
	if(tProcUtil.UnRegistOcx(strComObjName))
	{		
		WriteLogN("[%s] execute %s un-regist success ", m_strUtilName, STR_PROC_NAME_REGSVR32);		
	}
	else
	{
		WriteLogE("[%s] execute %s un-regist fail : [%d] ", m_strUtilName, STR_PROC_NAME_REGSVR32, GetLastError());		
	}
*/
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
