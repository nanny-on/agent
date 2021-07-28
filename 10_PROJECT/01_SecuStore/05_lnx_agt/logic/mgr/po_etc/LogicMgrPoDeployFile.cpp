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
#include "LogicMgrPoDeployFile.h"

//---------------------------------------------------------------------------

CLogicMgrPoDeployFile*		t_LogicMgrPoDeployFile = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoDeployFile::CLogicMgrPoDeployFile()
{
	t_ManagePoDeployFile				= new CManagePoDeployFile();
	t_ManagePoDeployFilePkg				= new CManagePoDeployFilePkg();
	t_ManagePoDeployFileUnit			= new CManagePoDeployFileUnit();

	t_ManagePoDeployFileUnitInfoPkg		= new CManagePoDeployFileUnitInfoPkg();
	t_ManagePoDeployFileUnitChkPkg		= new CManagePoDeployFileUnitChkPkg();
	t_ManagePoDeployFileUnitTarPkg		= new CManagePoDeployFileUnitTarPkg();
	t_ManagePoDeployFileUnitSchPkg		= new CManagePoDeployFileUnitSchPkg();

	t_ManagePoDeployFileInfoUnit		= new CManagePoDeployFileInfoUnit();
	t_ManagePoDeployFileChkUnit			= new CManagePoDeployFileChkUnit();
	t_ManagePoDeployFileTarUnit			= new CManagePoDeployFileTarUnit();
	t_ManagePoDeployFileSchUnit			= new CManagePoDeployFileSchUnit();


	t_ManagePoDeployFile->LoadDBMS();
	t_ManagePoDeployFilePkg->LoadDBMS();
	t_ManagePoDeployFileUnit->LoadDBMS();

	t_ManagePoDeployFileUnitInfoPkg->LoadDBMS();
	t_ManagePoDeployFileUnitChkPkg->LoadDBMS();
	t_ManagePoDeployFileUnitTarPkg->LoadDBMS();
	t_ManagePoDeployFileUnitSchPkg->LoadDBMS();

	t_ManagePoDeployFileInfoUnit->LoadDBMS();
	t_ManagePoDeployFileChkUnit->LoadDBMS();
	t_ManagePoDeployFileTarUnit->LoadDBMS();
	t_ManagePoDeployFileSchUnit->LoadDBMS();

	{
		t_ManagePoDeployFileUnitInfoPkg->InitPkg();
		t_ManagePoDeployFileUnitChkPkg->InitPkg();
		t_ManagePoDeployFileUnitTarPkg->InitPkg();
		t_ManagePoDeployFileUnitSchPkg->InitPkg();
	}

	t_ManagePoDeployFile->InitHash();

	m_strLogicName		= "mgr agt po deploy file";
	
	m_nPolicyType		= SS_POLICY_TYPE_DEPLOY_FILE;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DEPLOY_FILE;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DEPLOY_FILE;
}
//---------------------------------------------------------------------------

CLogicMgrPoDeployFile::~CLogicMgrPoDeployFile()
{
	SAFE_DELETE(t_ManagePoDeployFile);
	SAFE_DELETE(t_ManagePoDeployFilePkg);
	SAFE_DELETE(t_ManagePoDeployFileUnit);

	SAFE_DELETE(t_ManagePoDeployFileUnitInfoPkg);
	SAFE_DELETE(t_ManagePoDeployFileUnitChkPkg);
	SAFE_DELETE(t_ManagePoDeployFileUnitTarPkg);
	SAFE_DELETE(t_ManagePoDeployFileUnitSchPkg);

	SAFE_DELETE(t_ManagePoDeployFileInfoUnit);
	SAFE_DELETE(t_ManagePoDeployFileChkUnit);
	SAFE_DELETE(t_ManagePoDeployFileTarUnit);
	SAFE_DELETE(t_ManagePoDeployFileSchUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoDeployFile::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_DEPLOY_FILE ppdf = NULL;
	DB_PO_DEPLOY_FILE dpdf;

	m_tDPH = &(dpdf.tDPH);

	TListDBPoDeployFilePkg	tPkgList;
	TListDBPoDeployFileUnit	tUnitList;

	TListDBPoDeployFileUnitInfoPkg	tInfoPkgList;
	TListDBPoDeployFileUnitChkPkg	tChkPkgList;
	TListDBPoDeployFileUnitTarPkg	tTarPkgList;
	TListDBPoDeployFileUnitSchPkg	tSchPkgList;

	TMapDBPoDeployFileInfoUnit		tInfoMap;
	TMapDBPoDeployFileChkUnit		tChkMap;
	TMapDBPoDeployFileTarUnit		tTarMap;
	TMapDBPoDeployFileSchUnit		tSchMap;

	if( t_ManagePoDeployFile->GetPkt(RecvToken, dpdf))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_DEPLOY_FILE_PKG	dppp;		
		DB_PO_DEPLOY_FILE_UNIT	dppu;

		if( t_ManagePoDeployFilePkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoDeployFileUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_DEPLOY_FILE_UNIT_INFO_PKG sub_pkg;
				DB_PO_DEPLOY_FILE_INFO_UNIT sub_unit;

				if( t_ManagePoDeployFileUnitInfoPkg->GetPkt(RecvToken, sub_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoDeployFileInfoUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tInfoPkgList.push_back(sub_pkg);
				tInfoMap[sub_unit.tDPH.nID] = sub_unit;

				dppu.tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block();
		}
		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_DEPLOY_FILE_UNIT_CHK_PKG sub_pkg;
				DB_PO_DEPLOY_FILE_CHK_UNIT sub_unit;

				if( t_ManagePoDeployFileUnitChkPkg->GetPkt(RecvToken, sub_pkg))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoDeployFileChkUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tChkPkgList.push_back(sub_pkg);
				tChkMap[sub_unit.tDPH.nID] = sub_unit;

				dppu.tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_CHK, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block();
		}
		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_DEPLOY_FILE_UNIT_TAR_PKG sub_pkg;
				DB_PO_DEPLOY_FILE_TAR_UNIT sub_unit;

				if( t_ManagePoDeployFileUnitTarPkg->GetPkt(RecvToken, sub_pkg))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoDeployFileTarUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tTarPkgList.push_back(sub_pkg);
				tTarMap[sub_unit.tDPH.nID] = sub_unit;

				dppu.tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block();
		}
		{
			UINT32 nSubRecvNum = 0;
			if(!RecvToken.TokenDel_32(nSubRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			while(nSubRecvNum--)
			{
				DB_PO_DEPLOY_FILE_UNIT_SCH_PKG sub_pkg;
				DB_PO_DEPLOY_FILE_SCH_UNIT sub_unit;

				if( t_ManagePoDeployFileUnitSchPkg->GetPkt(RecvToken, sub_pkg))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
				if( t_ManagePoDeployFileSchUnit->GetPkt(RecvToken, sub_unit))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

				tSchPkgList.push_back(sub_pkg);
				tSchMap[sub_unit.tDPH.nID] = sub_unit;

				dppu.tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH, sub_pkg.tDPH.nID, sub_unit.tDPH.nID);
			}
			RecvToken.TokenSkip_Block(); 
		}
		RecvToken.TokenSkip_Block();			
		
		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpdf.tDPH._add_id(dppp.tDPH.nID);
	}

	ppdf = (PDB_PO_DEPLOY_FILE)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppdf)
	{
		if(ppdf->tDPH.nID == dpdf.tDPH.nID)
		{
			t_ManagePoDeployFileUnitSchPkg->CopyContinueInfo(tSchPkgList);
		}	
		
		t_ManagePoDeployFilePkg->ClearPkgUnitByPolicyID(ppdf->tDPH.nID);		
		t_ManagePoDeployFile->DelPoDeployFile(ppdf->tDPH.nID);
		
		t_ManagePoDeployFileUnit->ClearKeyIDMap();
	}

	{
		{
			{
				TMapDBPoDeployFileInfoUnitItor begin, end;
				begin = tInfoMap.begin();	end = tInfoMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileInfoUnit->ApplyPoDeployFileInfoUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub info unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TMapDBPoDeployFileChkUnitItor begin, end;
				begin = tChkMap.begin();	end = tChkMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileChkUnit->ApplyPoDeployFileChkUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub chk unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TMapDBPoDeployFileTarUnitItor begin, end;
				begin = tTarMap.begin();	end = tTarMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileTarUnit->ApplyPoDeployFileTarUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub tar unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}
			
			{
				TMapDBPoDeployFileSchUnitItor begin, end;
				begin = tSchMap.begin();	end = tSchMap.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileSchUnit->ApplyPoDeployFileSchUnit(begin->second))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy sub sch unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}
		}

		{
			{
				TListDBPoDeployFileUnitInfoPkgItor begin, end;
				begin = tInfoPkgList.begin();	end = tInfoPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileUnitInfoPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub info pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoDeployFileUnitInfoPkg->AddPoDeployFileUnitInfoPkg(*begin);
				}
			}
			{
				TListDBPoDeployFileUnitChkPkgItor begin, end;
				begin = tChkPkgList.begin();	end = tChkPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileUnitChkPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub chk pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoDeployFileUnitChkPkg->AddPoDeployFileUnitChkPkg(*begin);
				}
			}
			{
				TListDBPoDeployFileUnitTarPkgItor begin, end;
				begin = tTarPkgList.begin();	end = tTarPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileUnitTarPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub tar pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoDeployFileUnitTarPkg->AddPoDeployFileUnitTarPkg(*begin);
				}
			}
			{
				TListDBPoDeployFileUnitSchPkgItor begin, end;
				begin = tSchPkgList.begin();	end = tSchPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileUnitSchPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy sub sch pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoDeployFileUnitSchPkg->AddPoDeployFileUnitSchPkg(*begin);
				}
			}
		}	

		{
			{
				TListDBPoDeployFileUnitItor begin, end;
				begin = tUnitList.begin();	end = tUnitList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFileUnit->ApplyPoDeployFileUnit(*begin))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TListDBPoDeployFilePkgItor begin, end;
				begin = tPkgList.begin();	end = tPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoDeployFilePkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoDeployFilePkg->AddPoDeployFilePkg(*begin);
				}
			}

			if(SetER(t_ManagePoDeployFile->ApplyPoDeployFile(dpdf)))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
			}
		}		
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ApplyPolicy()
{
	PDB_PO_DEPLOY_FILE pdpdf = NULL;
	pdpdf = (PDB_PO_DEPLOY_FILE)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpdf)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return AZPKT_CB_RTN_SUCCESS;
	}
	
	TListID tUnitIDList;
	t_ManagePoDeployFilePkg->GetKeyIDList(pdpdf->tDPH.nID, tUnitIDList);

	{
		TListID tLogDFIDList;
		t_ManageLogDeployFile->GetItemIDList(tLogDFIDList);
		{
			TListIDItor begin, end;
			begin = tLogDFIDList.begin();	end = tLogDFIDList.end();
			for(begin; begin != end; begin++)
			{
				PDB_LOG_DEPLOY_FILE pCurDLDF = t_ManageLogDeployFile->FindItem(*begin);
				if(!pCurDLDF)	continue;

				if(pCurDLDF->nEndTime)		continue;				
				if(pCurDLDF->nPolicyID == pdpdf->tDPH.nID && IsExistsIDFromList(tUnitIDList, pCurDLDF->nUnitID))
				{
					PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(pCurDLDF->nUnitID);
					if(pdpdf->tDPH.nUsedMode == STATUS_USED_MODE_ON && pdpdfu && pdpdfu->tDPH.nUsedMode == STATUS_USED_MODE_ON)
						continue;
				}

				DB_LOG_DEPLOY_FILE tDLDF = *pCurDLDF;
				tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_CANCEL_ADMIN;
				tDLDF.nEndTime = GetCurrentDateTimeInt();

				if(!pCurDLDF || pCurDLDF->nEndReason != tDLDF.nEndReason || pCurDLDF->nEvtErrCode != tDLDF.nEvtErrCode ||
					!tDLDF.nEvtTime || (tDLDF.nEvtTime/TIMER_INTERVAL_TIME_DAY != GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
				{
					tDLDF.nEvtTime	= t_ValidTimeUtil->GetValidTime();

					HISYNCSTEPUP(tDLDF.nSyncSvrStep);
					t_LogicMgrLogDeployFile->SetLogDeployFile(tDLDF);
				}
			}
		}
	}

	if(pdpdf->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return AZPKT_CB_RTN_SUCCESS;
	}

	{		
		TListIDItor begin, end;
		begin = tUnitIDList.begin();	end = tUnitIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(*begin);
			if(!pdpdfu)
			{
				WriteLogE("[%s] not find policy unit : [%d]", m_strLogicName.c_str(), *begin);
				continue;
			}

			DB_LOG_DEPLOY_FILE tDLDF;
			PDB_LOG_DEPLOY_FILE pCurDLDF = NULL;
			{
				tDLDF.nPolicyID = pdpdf->tDPH.nID;
				tDLDF.nUnitID = *begin;
				pCurDLDF = t_ManageLogDeployFile->FindItemByDFInfo(tDLDF);
				if(pCurDLDF)	tDLDF = *pCurDLDF;
			}

			if(pdpdfu->tDPH.nUsedMode != STATUS_USED_MODE_ON)
			{
				continue;
			}

			PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = t_ManagePoDeployFileInfoUnit->FindItemByPoUnitID(*begin);
			if(!pdpdfiu)
			{
				WriteLogE("[%s] not find policy unit info : [%d]", m_strLogicName.c_str(), *begin);
				continue;
			}

			do 
			{
				TListID tValidSchPkgIDList;
				INT32 nRtn = 0;

				if(pdpdfiu->nExeNumType == SS_PO_DEPLOY_FILE_EXE_NUM_TYPE_SINGLE && tDLDF.nEndTime)
				{
					tDLDF.nEvtErrCode = ERR_SUCCESS;
					break;
				}

				if(pdpdfiu->nSchUsedMode == SS_PO_DEPLOY_FILE_SCHEDULE_USED_MODE_ENABLE && IsValidSchedule(*begin, tValidSchPkgIDList) == 0)
				{
					break;
				}

				if(IsValidTarget(*begin))
				{
					tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_INVALID_SYS;
					tDLDF.nEvtErrCode = ERR_SUCCESS;
					break;
				}

				if(IsValidCheck(*begin))
				{
					tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_INVALID_CHK;
					tDLDF.nEvtErrCode = ERR_SUCCESS;
					break;
				}

				if(AddDpDownInfo(pdpdf->tDPH.nID, *begin, tValidSchPkgIDList))
				{
					break;
				}
			} while (FALSE);

			if(!pCurDLDF || pCurDLDF->nEndReason != tDLDF.nEndReason || pCurDLDF->nEndTime != tDLDF.nEndTime || 
				pCurDLDF->nEvtErrCode != tDLDF.nEvtErrCode ||
				!tDLDF.nEvtTime || (tDLDF.nEvtTime/TIMER_INTERVAL_TIME_DAY != GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
			{
				tDLDF.nEvtTime	= t_ValidTimeUtil->GetValidTime();
				HISYNCSTEPUP(tDLDF.nSyncSvrStep);
				t_LogicMgrLogDeployFile->SetLogDeployFile(tDLDF);
			}
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::IsValidSchedule(UINT32 nUnitID, TListID& tValidSchIDList)
{	
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(nUnitID);
	if(!pdpdfu)
	{
		WriteLogE("[%s] not find policy unit : [%d]", m_strLogicName.c_str(), nUnitID);
		return 0;
	}

	TListID tUnitSIDList;
	t_ManagePoDeployFileUnitSchPkg->GetKeyIDList(nUnitID, tUnitSIDList);

	{
		TListIDItor begin, end;
		begin = tUnitSIDList.begin();	end = tUnitSIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu = t_ManagePoDeployFileSchUnit->FindItem(*begin);
			if(!pdpdfsu)
			{
				WriteLogE("[%s] not find policy unit sch : [u:%d][us:%d]", m_strLogicName.c_str(), nUnitID, *begin);
				continue;
			}

			PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = t_ManagePoDeployFileUnitSchPkg->FindItemByPUID(nUnitID, *begin);
			if(!pdpdfsu)
			{
				WriteLogE("[%s] not find policy unit sch pkg : [u:%d][us:%d]", m_strLogicName.c_str(), nUnitID, *begin);
				continue;
			}

			if(pdpdfsu->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
				continue;
			
			UINT32 nLastChkTime = pdpdfusp->nScanTime;
			U64_SCHEDULE tIS;
			tIS.all = pdpdfsu->nSchTime;
			tIS.U8.type = pdpdfsu->nSchType;
			CHAR szLogBuf[CHAR_MAX_SIZE] = {0, };

			if(CLogicBase::IsValidSchedule(tIS.all, nLastChkTime, szLogBuf, CHAR_MAX_SIZE) == 0)
				continue;
				
			tValidSchIDList.push_back(pdpdfusp->tDPH.nID);
			//WriteLogN("[%s] match schedule information : [uid:%d][sid:%d]:[spid:%d]", m_strLogicName.c_str(), nUnitID, *begin, pdpdfusp->tDPH.nID);
		}
	}
	return tValidSchIDList.size();
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::IsValidTarget(UINT32 nUnitID)
{	
	TListID tUnitTIDList;
	t_ManagePoDeployFileUnitTarPkg->GetKeyIDList(nUnitID, tUnitTIDList);

	{
		CSystemInfo tSysInfo;

		TListIDItor begin, end;
		begin = tUnitTIDList.begin();	end = tUnitTIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdftu = t_ManagePoDeployFileTarUnit->FindItem(*begin);
			if(!pdpdftu)
			{
				WriteLogE("[%s] not find policy unit tar : [u:%d][uc:%d]", m_strLogicName.c_str(), nUnitID, *begin);
				continue;
			}

			if(pdpdftu->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			{
				continue;
			}

			if(pdpdftu->nSysMode & tSysInfo.GetASIProcArchitecture() && 
				pdpdftu->nSysInfo & tSysInfo.GetSystemID())
			{
				return 0;
			}
		}
	}
	return -1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::IsValidCheck(UINT32 nUnitID)
{	
	TListID tUnitCIDList;
	TMapID tCompRtnMap;
	t_ManagePoDeployFileUnitChkPkg->GetKeyIDList(nUnitID, tUnitCIDList);

	{
		TListIDItor begin, end;
		begin = tUnitCIDList.begin();	end = tUnitCIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(*begin);
			if(!pdpdfcu)
			{
				WriteLogE("[%s] not find policy unit chk : [u:%d][uc:%d]", m_strLogicName.c_str(), nUnitID, *begin);
				continue;
			}

			if(pdpdfcu->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			{
				continue;
			}

			INT32 nCompRtn = 1;
			switch(pdpdfcu->nCompType)
			{
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_INSTALL:	nCompRtn = ComppareInfo_AppWiz(*begin);			break;
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_REGISTRY:	nCompRtn = ComppareInfo_Registry(*begin);		break;
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_FILE_PATH:	nCompRtn = ComppareInfo_FilePath(*begin);		break;
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_FILE_VER:	nCompRtn = ComppareInfo_FileVersion(*begin);	break;
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_PROCESS:	nCompRtn = ComppareInfo_Process(*begin);		break;
			}
			//WriteLogN("[%s] deploy file valid check result : [%d][%d][%d]", m_strLogicName.c_str(), pdpdfcu->tDPH.nID, pdpdfcu->nCompType, nCompRtn);

			{
				UINT32 nCompKey = (pdpdfcu->tDPH.nExtOption & SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_MULTI);
				TMapIDItor find = tCompRtnMap.find(nCompKey);
				if(find == tCompRtnMap.end())
				{
					tCompRtnMap[nCompKey] = nCompRtn;
				}
				else
				{
					if(nCompKey == SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_SINGLE)
					{
						if(find->second != 0)	find->second = nCompRtn;
					}
					else
					{
						if(find->second == 0)	find->second = nCompRtn;
					}
				}
			}
		}
	}
	
	{
		TMapIDItor find = tCompRtnMap.find(SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_SINGLE);
		if(find != tCompRtnMap.end())
		{
			return find->second;
		}

		find = tCompRtnMap.find(SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_MULTI);
		if(find != tCompRtnMap.end())
		{
			return find->second;
		}
	}
	return -1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::AddDpDownInfo(UINT32 nPoID, UINT32 nUnitID, TListID& tSchPkgIDList)
{

	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt : [u:%d]", m_strLogicName.c_str(), nUnitID);
		return -1;
	}
	/*else if(pdpsiu->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	{
		WriteLogE("[%s] not used udt chk svr info list : [u:%d]", m_strLogicName.c_str(), nUnitID);
		return -2;
	}*/
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list : [u:%d]", m_strLogicName.c_str(), nUnitID);
		return -2;
	}

	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(nUnitID);
	if(!pdpdfu)
	{
		WriteLogE("[%s] not find policy unit : [%d]", m_strLogicName.c_str(), nUnitID);
		return -1;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nItemType = ASIFDL_DL_FILE_TYPE_DEPLOY;
		tAFI.nItemID = nUnitID;	
		tAFI.nPolicyID = nPoID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_FILE, MAX_PATH);

		{					
			PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = t_ManagePoDeployFileInfoUnit->FindItemByPoUnitID(nUnitID);

			strncpy(tAFI.szFileName, pdpdfiu->strFileSaveName.c_str(), MAX_PATH-1);
			strncpy(tAFI.szFileHash, pdpdfiu->strFileHash.c_str(), MAX_PATH-1);
		}	

		if(t_ManageFileDown->ChkDFInfo(tAFI, tSchPkgIDList))
		{
			//WriteLogN("already exists deploy file download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
			return 0;
		}

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		WriteLogN("start deploy file download : [%d][%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tSchPkgIDList.size(), tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
/*
		if(t_ASIFDLDLLUtil)
		{
			t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
			t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
		}
*/
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
		AddDLInfo(&tAFI);

		t_ManageFileDown->AddDFInfo(tAFI, tSchPkgIDList);
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ComppareInfo_AppWiz(UINT32 nChkUnitID)
{
	PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(nChkUnitID);
	if(!pdpdfcu)
	{
		WriteLogE("[%s] not find policy unit chk : [uc:%d]", m_strLogicName.c_str(), nChkUnitID);
		return -1;
	}
	
	INT32 nCompRst = -1;
	TListSwInfo tSwInfoList;

	if(t_ASISIDLLUtil)
	{
		CSystemInfo tSysInfo;
		INT32 nProcArchitect = (INT32)tSysInfo.GetASIProcArchitecture();
		t_ASISIDLLUtil->GetSwInfo(nProcArchitect, (GetSwInfoType)GetSoftwareInfo, (PVOID)&tSwInfoList);
	}

	{			
		INT32 nMatchFind = 0;
		TListSwInfoItor begin, end;
		begin = tSwInfoList.begin();	end = tSwInfoList.end();
		for(begin; begin != end; begin++)
		{
			if(!_stricmp(pdpdfcu->strCompValue.c_str(), begin->szName))
			{
				nMatchFind = 1;
				break;
			}
		}

		{
			switch(pdpdfcu->nCompOpType)
			{
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST:		nCompRst = !nMatchFind;	break;
				case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST:	nCompRst = nMatchFind;	break;
			}
		}
	}
	return nCompRst;
}
//---------------------------------------------------------------------------

VOID		CLogicMgrPoDeployFile::GetSoftwareInfo(PASISI_SW_INFO pasi, INT32 nSize, PVOID lParam)
{
	ASISI_SW_INFO tASI;
	if(pasi == NULL)
		return;

	memcpy(&tASI, pasi, sizeof(ASISI_SW_INFO));

	if(!lParam)
		return;

	TListSwInfo* tSwInfoList = (TListSwInfo*)lParam;
	tSwInfoList->push_back(tASI);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ComppareInfo_Registry(UINT32 nChkUnitID)
{
	PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(nChkUnitID);
	if(!pdpdfcu)
	{
		WriteLogE("[%s] not find policy unit chk : [uc:%d]", m_strLogicName.c_str(), nChkUnitID);
		return -1;
	}

	String strCompInfo = pdpdfcu->strCompValue;

	INT32 nCompRst = -1;
	INT32 nMatchFind = 0;
	INT32 nDataCompRst = 0;

	HKEY nKey = (HKEY)HKEY_LOCAL_MACHINE;
	String strCompPath, strCompValue, strCompData, strRegCompData;
	INT32 nRegCompData = 0;
	{
		CTokenString Token(strCompInfo.c_str(), strCompInfo.length(), '/');
		String strKey = Token.NextToken();
/*
		if(!strKey.CompareNoCase("HKLM"))		nKey = HKEY_LOCAL_MACHINE;
		else if(!strKey.CompareNoCase("HKCR"))	nKey = HKEY_CLASSES_ROOT;
		else if(!strKey.CompareNoCase("HKCU"))	nKey = HKEY_CURRENT_USER;
		else if(!strKey.CompareNoCase("HKU"))	nKey = HKEY_USERS;
		else if(!strKey.CompareNoCase("HKCC"))	nKey = HKEY_CURRENT_CONFIG;
*/
		strCompInfo = Token.RemainValue();
	}
	{
		CTokenString Token(strCompInfo.c_str(), strCompInfo.length(), '/', 1);
		strCompData = Token.NextToken();
		strCompValue = Token.NextToken();
		strCompPath = Token.RemainValue();		
	}

	{	
		CRegUtil tRegUtil;
//		tRegUtil.SetWowDefaultKey();		

		switch(pdpdfcu->nCompValType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_STRING:	
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_VERSION:
			{
				CHAR szRegBuf[CHAR_MAX_SIZE] = {0, };
				if(tRegUtil.RegReadString(nKey, strCompPath.c_str(), strCompValue.c_str(), szRegBuf))
				{					
					break;
				}
				nMatchFind = 1;
				strRegCompData = szRegBuf;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_NUMBER:
			{
				if(tRegUtil.RegReadInt(nKey, strCompPath.c_str(), strCompValue.c_str(), nRegCompData))
				{					
					break;
				}
				nMatchFind = 1;
				break;
			}
		}
	}

	if(nMatchFind)
	{
		switch(pdpdfcu->nCompValType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_STRING:		nDataCompRst = CompareText(strCompData.c_str(), strRegCompData.c_str());		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_VERSION:	nDataCompRst = CompareVersion(strCompData, strRegCompData);		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_NUMBER:
			{
				INT32 nValueA = 0, nValueB = 0;
				nValueA = StrToIntExt(strCompData.c_str());	nValueB = StrToIntExt(strRegCompData.c_str());
				nDataCompRst = CompareValue(nValueA, nValueB);		
				break;
			}
		}
	}

	{
		switch(pdpdfcu->nCompOpType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST:		nCompRst = !nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST:	nCompRst = nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LARGE:		
			{
				if(nMatchFind && nDataCompRst > 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LESS:		
			{
				if(nMatchFind && nDataCompRst < 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EQUAL:		
			{
				if(nMatchFind && nDataCompRst == 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EQUAL:		
			{
				if(nMatchFind && nDataCompRst != 0)		nCompRst = 0;
				break;
			}
		}
	}
	return nCompRst;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ComppareInfo_FilePath(UINT32 nChkUnitID)
{
	PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(nChkUnitID);
	if(!pdpdfcu)
	{
		WriteLogE("[%s] not find policy unit chk : [uc:%d]", m_strLogicName.c_str(), nChkUnitID);
		return -1;
	}

	String strCompInfo = pdpdfcu->strCompValue;

	INT32 nCompRst = -1;
	INT32 nMatchFind = 0;
	
	{	
		CFileUtil tFileUtil;
		if(tFileUtil.DirectoryExists(strCompInfo.c_str()))	nMatchFind = 1;
		else if(tFileUtil.FileExists(strCompInfo.c_str()))	nMatchFind = 1;
	}

	{
		switch(pdpdfcu->nCompOpType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST:		nCompRst = !nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST:	nCompRst = nMatchFind;		break;
		}
	}
	return nCompRst;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ComppareInfo_FileVersion(UINT32 nChkUnitID)
{
	PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(nChkUnitID);
	if(!pdpdfcu)
	{
		WriteLogE("[%s] not find policy unit chk : [uc:%d]", m_strLogicName.c_str(), nChkUnitID);
		return -1;
	}

	String strCompInfo = pdpdfcu->strCompValue;

	INT32 nCompRst = -1;
	INT32 nMatchFind = 0;
	INT32 nDataCompRst = 0;

	String strCompPath, strCompData, strFileCompData;
	{
		CTokenString Token(strCompInfo.c_str(), strCompInfo.length(), '/', 1);
		strCompData = Token.NextToken();
		strCompPath = Token.RemainValue();		
	}

	{	
		CFileUtil tFileUtil;
		if(tFileUtil.FileExists(strCompInfo.c_str()))	
		{
			CHAR szFileCompData[CHAR_MAX_SIZE] = {0, };
//			t_ASIFIDLLUtil->ASIFI_GetFileVersion(strCompInfo.c_str(), szFileCompData);
			strFileCompData = szFileCompData;
			nMatchFind = 1;
		}
	}

	if(nMatchFind)
	{
		nDataCompRst = CompareVersion(strCompData, strFileCompData);
	}

	{
		switch(pdpdfcu->nCompOpType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST:		nCompRst = !nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST:	nCompRst = nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LARGE:		
			{
				if(nMatchFind && nDataCompRst > 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LESS:		
			{
				if(nMatchFind && nDataCompRst < 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EQUAL:		
			{
				if(nMatchFind && nDataCompRst == 0)		nCompRst = 0;
				break;
			}
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EQUAL:		
			{
				if(nMatchFind && nDataCompRst != 0)		nCompRst = 0;
				break;
			}
		}
	}
	return nCompRst;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ComppareInfo_Process(UINT32 nChkUnitID)
{
	PDB_PO_DEPLOY_FILE_CHK_UNIT pdpdfcu = t_ManagePoDeployFileChkUnit->FindItem(nChkUnitID);
	if(!pdpdfcu)
	{
		WriteLogE("[%s] not find policy unit chk : [uc:%d]", m_strLogicName.c_str(), nChkUnitID);
		return -1;
	}

	String strCompInfo = pdpdfcu->strCompValue;

	INT32 nCompRst = -1;
	INT32 nMatchFind = 0;
	
	{	
		CProcUtil tProcUtil;
		if(tProcUtil.ProcessExist(strCompInfo.c_str()))	nMatchFind = 1;
	}

	{
		switch(pdpdfcu->nCompOpType)
		{
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST:		nCompRst = !nMatchFind;		break;
			case SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST:	nCompRst = nMatchFind;		break;
		}
	}
	return nCompRst;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::OnTimer_Logic()
{
	ApplyPolicy();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDeployFile::ChkEndDpFile()
{
	CProcUtil	tProcUtil;

	INT32 nRtn = 0;
	TListID tIDList, tIDDelList;
	TListIDItor begin, end;

	if(!t_ManageFileDown)
		return nRtn;

	t_ManageFileDown->GetKeyIDList(tIDList);
	begin = tIDList.begin();	end	  = tIDList.end();
	for(; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi || tProcUtil.ProcessExist(t_ManageFileDown->FindKeyID(pafi->nID)))	continue;

		DB_LOG_DEPLOY_FILE tDLDF;
		PDB_LOG_DEPLOY_FILE pCurDLDF = NULL;
		tDLDF.nPolicyID	= pafi->nPolicyID;
		tDLDF.nUnitID	= pafi->nItemID;
		pCurDLDF = t_ManageLogDeployFile->FindItemByDFInfo(tDLDF);
		if(pCurDLDF)	tDLDF = *pCurDLDF;

		{
			tDLDF.nEvtErrCode = ERR_SUCCESS;
			tDLDF.nEndTime = GetCurrentDateTimeInt();

			PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = t_ManagePoDeployFileInfoUnit->FindItemByPoUnitID(pafi->nItemID);
			if(!pdpdfiu)	continue;

			switch(pdpdfiu->nFileType)
			{
			case SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL_EXE:	tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE;			break;	
			case SS_PO_DEPLOY_FILE_FILE_TYPE_SETUP:			tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE_SETUP;	break;
			case SS_PO_DEPLOY_FILE_FILE_TYPE_BATCH:			tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE_BAT;		break;
			case SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL:		tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_COPY_END;			break;
			}
		}

		if(!pCurDLDF || pCurDLDF->nEndReason != tDLDF.nEndReason || pCurDLDF->nEndTime != tDLDF.nEndTime || 
			pCurDLDF->nEvtErrCode != tDLDF.nEvtErrCode ||
			!tDLDF.nEvtTime || (tDLDF.nEvtTime/TIMER_INTERVAL_TIME_DAY != GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
		{
			tDLDF.nEvtTime	= t_ValidTimeUtil->GetValidTime();
			HISYNCSTEPUP(tDLDF.nSyncSvrStep);
			t_LogicMgrLogDeployFile->SetLogDeployFile(tDLDF);
		}

		tIDDelList.push_back(*begin);
	}

	begin = tIDDelList.begin();		end	= tIDDelList.end();
	for(; begin != end; begin++)
	{
		t_ManageFileDown->DelKeyID(*begin);
		t_ManageFileDown->DeleteItem(*begin);
		t_ManageFileDown->DelKeyIDList(*begin);
	}

	if(!t_ManageFileDown->LastKey())
		nRtn = 1;

	return nRtn;
}
//---------------------------------------------------------------------------
