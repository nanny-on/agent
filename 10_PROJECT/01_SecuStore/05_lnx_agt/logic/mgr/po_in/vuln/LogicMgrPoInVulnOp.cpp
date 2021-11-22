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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPoInVulnOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnOp*		t_LogicMgrPoInVulnOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnOp::CLogicMgrPoInVulnOp()
{
	t_ManagePoInVulnOp			= new CManagePoInVulnOp();
	t_ManagePoInVulnOpPkg		= new CManagePoInVulnOpPkg();

	t_ManageVulnPreScan			= new CManageVulnPreScan();

	t_ManagePoInVulnOp->LoadDBMS();
	t_ManagePoInVulnOpPkg->LoadDBMS();

	t_ManagePoInVulnOp->InitHash();

	m_strLogicName		= "mgr agt po in vuln op";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnOp::~CLogicMgrPoInVulnOp()
{
	SAFE_DELETE(t_ManagePoInVulnOp);
	SAFE_DELETE(t_ManagePoInVulnOpPkg);

	SAFE_DELETE(t_ManageVulnPreScan);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnOp::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_OP pdata = NULL;
	DB_PO_IN_VULN_OP data, data_old;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnOpPkg	tPkgList;
	
	if( t_ManagePoInVulnOp->GetPkt(RecvToken, data))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_OP_PKG	dppp;			
		if( t_ManagePoInVulnOpPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);	

		tPkgList.push_back(dppp);	
		data.tDPH._add_id(dppp.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		data_old = *pdata;
		t_ManagePoInVulnOpPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnOp->DelPoInVulnOp(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnOpPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnOpPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnOpPkg->AddPoInVulnOpPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnOp->ApplyPoInVulnOp(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	if(data_old.nEnvScanMode != data.nEnvScanMode)
	{
		t_LogicMgrPtnVuln->InitPtnVulnWsus();
		WriteLogN("[%s] reset env scan mode : [%d]", m_strLogicName.c_str(), data.nEnvScanMode);
	}

	if(data_old.tDPIVOPF.strPtnFileName != data.tDPIVOPF.strPtnFileName)
	{
		m_nForceApplyPolicy = STATUS_MODE_ON;
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::ApplyPolicy()
{
	PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoInVulnOp->IsValidPtnFile(pdata) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				AddDpDownInfo();
			}
			else if(m_nForceApplyPolicy)
			{
				if(t_LogicMgrPtnVuln->ReloadPtnVuln() == 0)
				{
					t_LogicMgrPtnVuln->InitPtnDeploy();
				}
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::CheckRunEnv()
{
	PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -1;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoInVulnOp->IsValidPtnFile(pdata) == 0)
		{
			AddDpDownInfo();
			return -10;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::PreScanVuln()
{
/*
	PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	return -1;

	CSimpleVulnPreScanUtil tSimpleVulnPreScanUtil;
	TListID tIDList;
	t_ManagePoInVulnOpPkg->GetItemIDList(tIDList);

	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_VULN_OP_PKG pdata_pkg = t_ManagePoInVulnOpPkg->FindItem(*begin);
			if (!pdata_pkg)	continue;

			UINT32 nPreScan = pdata_pkg->tDPH.nExtOption & SS_PTN_VULN_SUPPORT_OPTION_USE_PRESCAN_RESULT;

			if(!nPreScan)	continue;

			UINT8	nType = LOTYPEID32(pdata_pkg->tDPH.nUnitID);
			UINT32	nUID = HITYPEID32(pdata_pkg->tDPH.nUnitID);

			switch(nType)
			{
				case SS_COMMON_UNIT_TYPE_GLOBAL:
				{
//					PDB_PTN_VULN pdata = t_MMPPVuln->t_ManagePtnVuln->FindItem(nUID);
					PDB_PTN_VULN pdata = NULL;
					if(!pdata)		break;					
					
					TListID tVulnScanIDList;
//					t_MMPPVuln->t_ManagePtnVulnScan->GetKeyIDList(nUID, tVulnScanIDList);

					TListStr tSIDList;
					t_ManageWinSession.GetSIDList(tSIDList);

					{
						TListStrItor begin_sid, end_sid;
						begin_sid = tSIDList.begin();	end_sid = tSIDList.end();
						for(begin_sid; begin_sid != end_sid; begin_sid++)
						{
							tSimpleVulnPreScanUtil.ChkVulnPreScan(pdata_pkg->tDPH.nUnitID, tVulnScanIDList, *begin_sid, 0);
						}
					}
					break;
				}
			}
		}
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::SimpleScanVuln(UINT32& nDangerVulnID)
{
/*
	PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	return -1;

	CSimpleVulnPreScanUtil tSimpleVulnPreScanUtil;
	TListID tIDList;
	t_ManagePoInVulnOpPkg->GetItemIDList(tIDList);

	INT32 nSafe = 0, nDanger = 0, nWait = 0;

	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_VULN_OP_PKG pdata_pkg = t_ManagePoInVulnOpPkg->FindItem(*begin);
			if (!pdata_pkg)	continue;

			UINT32 nPreScan = pdata_pkg->tDPH.nExtOption & SS_PTN_VULN_SUPPORT_OPTION_USE_PRESCAN_RESULT;

			if(nPreScan)
			{
				UINT32 nScanRst = t_ManageVulnPreScan->GetPreScanResult(pdata_pkg->tDPH.nUnitID);
				switch(nScanRst)
				{
					case SS_PTN_VULN_SCAN_RESULT_DANGER:
					{
						if(!nDangerVulnID)	nDangerVulnID = pdata_pkg->tDPH.nUnitID;
						nDanger += 1;		
						break;
					}
					case SS_PTN_VULN_SCAN_RESULT_SCANNING:	nWait += 1;			break;
					case SS_PTN_VULN_SCAN_RESULT_SAFE:		nSafe += 1;			break;
				}
				continue;
			}

			UINT8	nType = LOTYPEID32(pdata_pkg->tDPH.nUnitID);
			UINT32	nUID = HITYPEID32(pdata_pkg->tDPH.nUnitID);

			switch(nType)
			{
				case SS_COMMON_UNIT_TYPE_GLOBAL:
				{
//					PDB_PTN_VULN pdata = t_MMPPVuln->t_ManagePtnVuln->FindItem(nUID);
					PDB_PTN_VULN pdata = NULL;
					if(!pdata)		break;					

					TListID tVulnScanIDList;
//					t_MMPPVuln->t_ManagePtnVulnScan->GetKeyIDList(nUID, tVulnScanIDList);

					TListStr tSIDList;
					t_ManageWinSession.GetSIDList(tSIDList);

					{
						TListStrItor begin_sid, end_sid;
						begin_sid = tSIDList.begin();	end_sid = tSIDList.end();
						for(begin_sid; begin_sid != end_sid; begin_sid++)
						{
							UINT32 nScanRst = tSimpleVulnPreScanUtil.ChkVulnPreScan(pdata_pkg->tDPH.nUnitID, tVulnScanIDList, *begin_sid, 0);

							switch(nScanRst)
							{
								case SS_PTN_VULN_SCAN_RESULT_DANGER:
								{
									if(!nDangerVulnID)	nDangerVulnID = pdata_pkg->tDPH.nUnitID;
									nDanger += 1;		
									break;
								}
								case SS_PTN_VULN_SCAN_RESULT_SCANNING:	nWait += 1;			break;
								case SS_PTN_VULN_SCAN_RESULT_SAFE:		nSafe += 1;			break;
							}
						}
					}
					break;
				}
			}
		}
	}

	{
		if(nDanger)		return SS_PTN_VULN_SCAN_RESULT_DANGER;
		if(nWait)		return SS_PTN_VULN_SCAN_RESULT_SCANNING;
	}
*/
	return SS_PTN_VULN_SCAN_RESULT_SAFE;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnOp::AddDpDownInfo()
{
	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName.c_str());
		return -1;
	}
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName.c_str());
		return -2;
	}

	PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -3;
	}

	String strDnFileName, strDnFileHash;
	strDnFileName = (pdata->tDPIVOPF.nPtnDnFileType ? pdata->tDPIVOPF.strPtnDnFileName : pdata->tDPIVOPF.strPtnFileSvName);
	strDnFileHash = (pdata->tDPIVOPF.nPtnDnFileType ? pdata->tDPIVOPF.strPtnDnFileHash : pdata->tDPIVOPF.strPtnFileHash);

	if(strDnFileName.empty() || strDnFileHash.empty())
	{
//		WriteLogE("[%s] invalid ptn file info [%s][%s]", m_strLogicName.c_str(), strDnFileName.c_str(), strDnFileHash.c_str());
		return -4;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = SS_DN_FILE_TYPE_VPTN_ASV;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdata->tDPH.nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_PPTN, MAX_PATH-1);

		{	
			strncpy(tAFI.szFileName, strDnFileName.c_str(), MAX_PATH-1);
			strncpy(tAFI.szFileHash, strDnFileHash.c_str(), MAX_PATH-1);
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists po in vuln download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
			return 0;
		}

		WriteLogN("start po in vuln download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
/*
		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
*/
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
		AddDLInfo(&tAFI);
		t_ManageFileDown->AddItem(tAFI);
	}
	return 0;
}
//---------------------------------------------------------------------------
