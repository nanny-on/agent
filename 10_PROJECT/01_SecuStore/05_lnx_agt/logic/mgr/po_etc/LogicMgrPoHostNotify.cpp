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


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPoHostNotify.h"

//---------------------------------------------------------------------------

CLogicMgrPoHostNotify*		t_LogicMgrPoHostNotify = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoHostNotify::CLogicMgrPoHostNotify()
{
	t_ManagePoHostNotify			= new CManagePoHostNotify();
	t_ManagePoHostNotifyPkg			= new CManagePoHostNotifyPkg();
	t_ManagePoHostNotifyUnit		= new CManagePoHostNotifyUnit();

	t_ManagePoHostNotify->LoadDBMS();
	t_ManagePoHostNotifyPkg->LoadDBMS();
	t_ManagePoHostNotifyUnit->LoadDBMS();

	t_ManagePoHostNotify->InitHash();

	m_strLogicName		= "mgr po host notify";
	
	m_nPolicyType		= SS_POLICY_TYPE_HOST_NOTIFY;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_HOST_NOTIFY;
}
//---------------------------------------------------------------------------
CLogicMgrPoHostNotify::~CLogicMgrPoHostNotify()
{
	SAFE_DELETE(t_ManagePoHostNotifyUnit);
	SAFE_DELETE(t_ManagePoHostNotifyPkg);
	SAFE_DELETE(t_ManagePoHostNotify);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoHostNotify::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoHostNotify::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_HOST_NOTIFY pdphn = NULL;
	DB_PO_HOST_NOTIFY dphn;
	m_tDPH = &(dphn.tDPH);
	TListDBPoHostNotifyPkg	tPkgList;
	TListDBPoHostNotifyUnit	tUnitList;

	if( t_ManagePoHostNotify->GetPkt(RecvToken, dphn))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_HOST_NOTIFY_PKG dppp;		
		DB_PO_HOST_NOTIFY_UNIT dppu;

		if( t_ManagePoHostNotifyPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoHostNotifyUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		{
			U64_SCHEDULE tDT;
			tDT.all = dppu.nSchTime;

			if(tDT.U8.type == SCHEDULE_PERIOD_TYPE_BOOT)
			{
				dppu.nNotifyTime = GetCurrentDateTimeInt();

				if(tDT.U8.min && t_EnvInfoOp->IsSysBootTime() == 0)
				{
					dppu.nNotifyTime = t_EnvInfo->m_nBootChkTime;
					WriteLogN("[%s] auto setting last scan time  : [%d][%u]", m_strLogicName, dppu.tDPH.nID, dppu.nNotifyTime);
				}
			}
		}

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dphn.tDPH._add_id(dppp.tDPH.nID);
	}

	pdphn = (PDB_PO_HOST_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdphn)
	{
		t_ManagePoHostNotifyPkg->ClearItemByPolicyID(pdphn->tDPH.nID);		
		t_ManagePoHostNotify->DelPoHostNotify(pdphn->tDPH.nID);
	}

	StartRunRegClear();
	DeleteInfoFile();

	{
		{
			TListDBPoHostNotifyUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoHostNotifyUnit->ApplyPoHostNotifyUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
				if(dphn.tDPH.nUsedMode == STATUS_USED_MODE_ON && begin->tDPH.nUsedMode == STATUS_USED_MODE_ON)
				{
					ClearSkipTimeInfo(*begin);
					U64_SCHEDULE tSchDT;
					tSchDT.all =  begin->nSchTime;
					if(tSchDT.U8.min == 0 && tSchDT.U8.type == SCHEDULE_PERIOD_TYPE_BOOT)
						AddStartMenu(*begin);
				}
			}
		}

		{
			TListDBPoHostNotifyPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoHostNotifyPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				
				t_ManagePoHostNotifyPkg->AddPoHostNotifyPkg(*begin);
			}
		}

		if(SetER(t_ManagePoHostNotify->ApplyPoHostNotify(dphn)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostNotify::OnTimer_Logic()
{
	PDB_PO_HOST_NOTIFY pdb_pol = (PDB_PO_HOST_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_pol)
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}
	if(pdb_pol->tDPH.nUsedMode != STATUS_USED_MODE_ON)	return 0;

	TListID tIDList;
	t_ManagePoHostNotifyPkg->GetKeyIDList(pdb_pol->tDPH.nID, tIDList);
	TListIDItor begin, end;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HOST_NOTIFY_UNIT pdphnu = t_ManagePoHostNotifyUnit->FindItem(*begin);
		if(!pdphnu)			continue;
		if(pdphnu->tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;
		{
			U64_SCHEDULE tSchDT;
			tSchDT.all =  pdphnu->nSchTime;
			UINT32 nLastChkTime = pdphnu->nNotifyTime;
			if(tSchDT.U8.min == 0 && tSchDT.U8.type == SCHEDULE_PERIOD_TYPE_BOOT)
				continue;
			if(!tSchDT.U8.type || IsValidSchedule(tSchDT.all, nLastChkTime) == 0)	
				continue;
			pdphnu->nNotifyTime = nLastChkTime;
			ExecHostNotify(pdphnu);

			t_ManagePoHostNotifyUnit->EditPoHostNotifyUnit(*pdphnu);
		}
	}

	return 1;
}
//---------------------------------------------------------------------------
void	CLogicMgrPoHostNotify::ExecHostNotify(PDB_PO_HOST_NOTIFY_UNIT pdphnu)
{
	CFileUtil tFileUtil;
	String strNotifyInfoFile;
	UINT32 nSkipTime = 0;
	{
		strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());
		tFileUtil.ForceDirectory(strNotifyInfoFile.c_str());
		strNotifyInfoFile = SPrintf("%s/inven/info/po_host_notify_%s.info", t_EnvInfo->m_strRootPath.c_str(), pdphnu->tDPH.strName.c_str());
		if(tFileUtil.FileExists(strNotifyInfoFile.c_str()))
			nSkipTime = GetSkipTimeInfo(strNotifyInfoFile);
		unlink(strNotifyInfoFile.c_str());
	}

	{
		String strChildPath, strChildArg;

		{
			CHAR szShortPath[CHAR_MAX_SIZE] = {0, };
			strChildPath = SPrintf("%s/util/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_INTRO);
//			GetShortPathName(strChildPath, szShortPath, MAX_PATH);
			strChildPath = szShortPath;
		}

		{
			strChildArg = SPrintf("used_mode=%u\f"
				"pos_type=%u\f"
				"show_size=%u\f"
				"show_pos=%u\f"
				"show_time=%u\f"
				"msg_fmt_type=%u\f"
				"msg_info=%s\f"
				"notify_skip_day=%u\f"
				"skip_click_time=%u\f",
				pdphnu->tDPH.nUsedMode, pdphnu->nPosType, pdphnu->nShowSize, pdphnu->nShowPos, pdphnu->nShowTime,
				pdphnu->nMsgFmtType, pdphnu->strMsgInfo.c_str(), pdphnu->nNotifySkipDay, nSkipTime);
		}

		{
			tFileUtil.WriteFileBuffer(strNotifyInfoFile, strChildArg);
			strChildArg = SPrintf("/%s/", strNotifyInfoFile.c_str());
		}

		ASI_TS_INFO tATI;
		ASI_TS_MGR_PARAM tATMP;
		{
			sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, "%s", STR_TS_NAME_HOST_NOTIFY); 
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
			tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

			tATMP.strTSChildPath = strChildPath;
			tATMP.strTSChildArgument = strChildArg;
			tATMP.nTSSingleRun		= 1;
			tATMP.nTSWaitMode		= 0;
			tATMP.nTSAutoDel		= 1;
		}
		t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
	}
}
//---------------------------------------------------------------------------
UINT32	CLogicMgrPoHostNotify::GetSkipTimeInfo(String strPath)
{
	CFileUtil tFileUtil;
	String strFileData = "";
	UINT32 nSkiptime = 0;
	tFileUtil.ReadFileBuffer(strPath, strFileData);
	CTokenString Token(strFileData.c_str(), strFileData.length(), '\f');
	while(Token.IsNextToken())
	{
		String strInfo = Token.NextToken();
		CTokenString TokenInfo(strInfo.c_str(), strInfo.length(), '=');

		String strName = TokenInfo.NextToken();

		if(!_stricmp(strName.c_str(), "skip_click_time"))
		{
			nSkiptime = TokenInfo.NextToken_UInt();
			continue;
		}
	}
	return nSkiptime;
}
//---------------------------------------------------------------------------
void	CLogicMgrPoHostNotify::ClearSkipTimeInfo(DB_PO_HOST_NOTIFY_UNIT&	dphnu)
{
	CFileUtil tFileUtil;
	String strNotifyInfoFile = "", strRead = "";
	strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());
	tFileUtil.ForceDirectory(strNotifyInfoFile.c_str());
	strNotifyInfoFile = SPrintf("%s/inven/info/po_host_notify_%s.info", t_EnvInfo->m_strRootPath.c_str(), dphnu.tDPH.strName.c_str());
	if(tFileUtil.FileExists(strNotifyInfoFile.c_str()))
	{
		if(tFileUtil.ReadFileBuffer(strNotifyInfoFile, strRead))
		{
			if(strRead.find("skip_click_time") == string::npos)
				strRead = strRead + "skip_click_time=0\f";
			else
			{
				CTokenString Token(strRead.c_str(), strRead.length(), '\f');
				while(Token.IsNextToken())
				{
					String strInfo = Token.NextToken();
					CTokenString TokenInfo(strInfo.c_str(), strInfo.length(), '=');
					String strName = TokenInfo.NextToken();
					if(!_stricmp(strName.c_str(), "skip_click_time"))
					{
						strRead.replace(0, 15, "skip_click_time=0");
						break;
					}
				}
			}
			unlink(strNotifyInfoFile.c_str());
			tFileUtil.WriteFileBuffer(strNotifyInfoFile, strRead);
		}
	}
}

//---------------------------------------------------------------------------
INT32		CLogicMgrPoHostNotify::AddStartMenu(DB_PO_HOST_NOTIFY_UNIT&	dphnu)
{
	String strJobTitle	= "";
	strJobTitle = SPrintf("SecuStore_Notify_%s", dphnu.tDPH.strName.c_str());
	String strRegPath	= "SOFTWARE/Microsoft/Windows/CurrentVersion/Run";

	CFileUtil tFileUtil;
	String strNotifyInfoFile;
	{
		strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());
		tFileUtil.ForceDirectory(strNotifyInfoFile.c_str());
		strNotifyInfoFile = SPrintf("%s/inven/info/po_host_notify_%s.info", t_EnvInfo->m_strRootPath.c_str(), dphnu.tDPH.strName.c_str());

		unlink(strNotifyInfoFile.c_str());
	}

	CRegUtil tRegUtil;
	tRegUtil.RegDeleteValueExt(HKEY_LOCAL_MACHINE, strRegPath.c_str(), strJobTitle.c_str());

	String strArgument;
	{
		strArgument = SPrintf("used_mode=%u\f"
			"pos_type=%u\f"
			"show_size=%u\f"
			"show_pos=%u\f"
			"show_time=%u\f"
			"msg_fmt_type=%u\f"
			"msg_info=%s\f"
			"notify_skip_day=%d\f",
			dphnu.tDPH.nUsedMode, dphnu.nPosType, dphnu.nShowSize, dphnu.nShowPos, dphnu.nShowTime,
			dphnu.nMsgFmtType, dphnu.strMsgInfo.c_str(), dphnu.nNotifySkipDay);
	}

	{
		tFileUtil.WriteFileBuffer(strNotifyInfoFile, strArgument);
		strArgument = SPrintf("/%s/", strNotifyInfoFile.c_str());
	}


	{
		String strRegValue;
		strRegValue = SPrintf("\"%s/util/%s\" \"%s\"", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_INTRO, strArgument.c_str());

		tRegUtil.RegWriteString(HKEY_LOCAL_MACHINE, strRegPath.c_str(), strJobTitle.c_str(), strRegValue.c_str());
	}

	return 0;
}
//---------------------------------------------------------------------------
void	CLogicMgrPoHostNotify::DeleteInfoFile()
{
	CFileUtil tFileUtil;
	String strNotifyInfoFile = "";
	strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());

	TListGatherFileInfo tFileNameList;
	tFileUtil.GetFileNameList(strNotifyInfoFile, "", "", tFileNameList, 0);

	TListGatherFileInfoItor begin, end;
	begin = tFileNameList.begin();	end = tFileNameList.end();
	for(begin; begin != end; begin++)
	{
		if(begin->strFileName.find("po_host_notify") != string::npos)
			unlink(begin->strFullPath.c_str());
	}
}
void	CLogicMgrPoHostNotify::StartRunRegClear()
{
	String strRegPath = "SOFTWARE/Microsoft/Windows/CurrentVersion/Run";
	CRegUtil tRegUtil;
	TListRegValueInfo tList;
	tRegUtil.GetEnumSubValueName(HKEY_LOCAL_MACHINE, strRegPath.c_str(), tList);

	TListRegValueInfoItor begin, end;

	begin = tList.begin();	end = tList.end();
	for(begin; begin != end; begin++)
	{
		if(begin->strValue.find("SecuStore_Notify") != string::npos)
			UINT32 nRet= tRegUtil.RegDeleteValueExt(HKEY_LOCAL_MACHINE, strRegPath.c_str(), begin->strValue.c_str());
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
