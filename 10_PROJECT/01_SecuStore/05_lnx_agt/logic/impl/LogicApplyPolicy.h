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

//---------------------------------------------------------------------------
#pragma once


class CLogicApplyPolicy : public CLogicBase
{
private:
	UINT32		m_nChkTSProtectMode;
	TMapID64	m_tStatusMap;
	UINT32		m_nDrvStartMode;

public:
	INT32		ApplyPolicy();
	INT32		ApplyBootPolicy();

public:
	INT32		ApplyPolicy_Timer();

public:
	INT32		CheckRunEnv();


public:
	INT32		ChkTSProtectMode();

	INT32		ResetEPSDrv();
	INT32		ForceResetEPSDrv();
	INT32		IsActiveEPS();
	INT32		SetEPSSvcDisable();
	INT32		GetDrvStatus(UINT32 nPoType, UINT32 nStartDrv);

private:
	INT32		SetEPSDrvPolicy();
	INT32		SetEPSSvcPolicy(UINT32 nUserMode = 0);

private:
	INT32		SetEPSDrvPolicy_Env();
	INT32		SetEPSDrvPolicy_Etc();
	INT32		SetEPSDrvPolicy_PoFa();
	INT32		SetEPSDrvPolicy_PoFe();
	INT32		SetEPSDrvPolicy_PoNc();
	INT32		SetEPSDrvPolicy_PoIn();
	INT32		SetEPSDrvPolicy_PoDv();
	
private:
	TListID		m_tEPSHkPIDList;
	String		m_strHijackDLL;

public:	
	INT32		SetEPSHookPolicy(INT32 nMode = 0);
	INT32		SetEPSHookModuleByTS(INT32 nMode, UINT32 nHookPID);
	INT32		SetEPSHookModuleByTS(INT32 nMode, TListID tHookPIDList);

	INT32		SetUser32Hook(BOOL bInstall = TRUE);
	
public:
	CLogicApplyPolicy();
    ~CLogicApplyPolicy();
};

extern CLogicApplyPolicy*		t_LogicApplyPolicy;