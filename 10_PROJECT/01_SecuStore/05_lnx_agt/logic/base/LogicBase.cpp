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
#include "LogicBase.h"

//---------------------------------------------------------------------------

CLogicBase::CLogicBase()
{
	m_nPackageType		= 0;
	m_nPolicyType		= 0;
	m_nPolicyUnitType	= 0;
	m_nControlType		= 0;
	m_nEvtSubType		= 0;
	m_nEvtTarType		= 0;
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;
	m_nLastErrCode		= ERR_SUCCESS;

	m_nConAPktType		= 0;
	m_nConAPktCode		= 0;

	m_nAgtPktEditCode	= 0;
	m_nSSPoPkgMode		= 0;
	m_nSSPoSeqIdx		= 0;

	m_nLastTimerDT		= 0;
	m_nForceApplyPolicy	= 0;

	m_nIsMountedEncfs = -1;
	SendToken.Set(1024);
}
//---------------------------------------------------------------------------

CLogicBase::~CLogicBase()
{
}
//---------------------------------------------------------------------------

void	CLogicBase::InitBaseMember(PPKT_DATA pkt_data)
{
	m_nDbgPos			= 0;
    m_nRecvNum			= 0;
    m_nRecvID			= 0;
    m_nRecvDT			= 0;
    m_nRecvValue		= 0;
    m_nHostID			= 0;
	m_nPolicyID			= 0;
    m_nRecvCnt			= 0;
    m_nPktRst			= 0;
	m_nValue			= 0;
	m_nIsStart			= 0;
	m_strName			= "";

	m_nPktType			= 0;
	m_nPktCode			= 0;
	m_nSessionID		= 0;
	m_tPktData			= NULL;

	m_tDPH				= NULL;

	m_nEvtOpType		= 0;
	m_nLastErrCode		= ERR_SUCCESS;
	m_nForceApplyPolicy	= 0;

    m_tIDList.clear();
    m_tGIDList.clear();
    m_tHIDList.clear();
	m_tPIDList.clear();

    m_tRecvIDList.clear();
	m_tRecvGIDList.clear();
	m_tRecvHIDList.clear();
	m_tRecvPIDList.clear();

    m_tSendIDList.clear();
	m_tSendGIDList.clear();
	m_tSendHIDList.clear();
	m_tSendPIDList.clear();

	m_tSendGIDMap.clear();
	m_tSendHIDMap.clear();
	m_tSendPIDMap.clear();

    m_tSendIDStrMap.clear();
    m_tRecvIDStrMap.clear();

    m_tRecvIDListMap.clear();
	m_tRecvGIDListMap.clear();
	m_tRecvHIDListMap.clear();
	m_tRecvPIDListMap.clear();

    m_tSendIDListMap.clear();
    m_tSendGIDListMap.clear();    
	m_tSendHIDListMap.clear();
	m_tSendPIDListMap.clear();

	m_tEvtGIDListMap.clear();
	m_tEvtHIDListMap.clear();
	m_tEvtPIDListMap.clear();

	m_tFailGIDMap.clear();
	m_tFailHIDMap.clear();
	m_tFailPIDMap.clear();
	
	SendToken.Release();
	SendToken.Set(1024);

	if(pkt_data)
	{		
		RecvToken.Set(pkt_data->body.data, pkt_data->hdr.length);

		m_nSessionID	= pkt_data->nSessionID;
		m_nPktType		= pkt_data->hdr.type;
		m_nPktCode		= pkt_data->hdr.code;

		m_tPktData		= pkt_data;
	}
    return;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenRecvIDList(MemToken& RecvToken, TListID& tIDList)
{
	if(!RecvToken.TokenDel_32(m_nTokRecvNum))			goto INVALID_PKT;
    while(m_nTokRecvNum)
    {
    	if(!RecvToken.TokenDel_32(m_nTokRecvID))		goto INVALID_PKT;

        tIDList.push_back(m_nTokRecvID);
        m_nTokRecvNum--;
    }
    return tIDList.size();

INVALID_PKT:
    return -1;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenRecvIDMap(MemToken& RecvToken, TMapID& tIDValueMap)
{
    if(!RecvToken.TokenDel_32(m_nTokRecvNum))				goto INVALID_PKT;
    while(m_nTokRecvNum)
    {
    	if(!RecvToken.TokenDel_32(m_nTokRecvID))			goto INVALID_PKT;
        if(!RecvToken.TokenDel_32(m_nTokRecvValue))			goto INVALID_PKT;

        tIDValueMap[m_nTokRecvID] = m_nTokRecvValue;
        m_nTokRecvNum--;
    }
    return tIDValueMap.size();

INVALID_PKT:
    return -1;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenRecvIDMapList(MemToken& RecvToken, TMapIDList& tIDMapList)
{
	UINT32 nRecvNum = 0;
    UINT32 nRecvID = 0;
    TListID tRecvIDList;

	if(!RecvToken.TokenDel_32(nRecvNum))					goto INVALID_PKT;
    while(nRecvNum)
    {
        if(!RecvToken.TokenDel_32(nRecvID))					goto INVALID_PKT;
        if( TokenRecvIDList(RecvToken, tRecvIDList) < 0)	goto INVALID_PKT;

        tIDMapList[nRecvID] = tRecvIDList;
        tRecvIDList.clear();
        nRecvNum--;
    }
    return tIDMapList.size();

INVALID_PKT:
    return -1;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenSendIDList(TListID& tIDList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
    begin = tIDList.begin();	end = tIDList.end();
    for(begin; begin != end; begin++)
    {
    	SendToken.TokenAdd_32(*begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenSendIDMap(TMapID& tIDValueMap, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDValueMap.size());
    TMapIDItor begin, end;
    begin = tIDValueMap.begin();	end = tIDValueMap.end();
    for(begin; begin != end; begin++)
    {
    	SendToken.TokenAdd_32(begin->first);
        SendToken.TokenAdd_32(begin->second);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::TokenSendIDMapList(TMapIDList& tIDMapList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDMapList.size());
    TMapIDListItor begin, end;
    begin = tIDMapList.begin();	end = tIDMapList.end();
    for(begin; begin != end; begin++)
    {
    	SendToken.TokenAdd_32(begin->first);
     	TokenSendIDList(begin->second, SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::CheckPktResult(MemToken& RecvToken)
{
	if(!RecvToken.TokenDel_32(m_nPktRst))
    {
    	m_nPktRst = ERR_SOCKET_CLT_TO_SVR_INVALID_PKT;
    }
    return m_nPktRst;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::SetHdrAndRtn(INT32 nRtnValue)
{
	if(m_tDPH)	
	{
		m_tBaseDPH = *m_tDPH;
	}
	m_tDPH = &m_tBaseDPH;
	return nRtnValue;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicBase::OnTimer()
{
	m_nLastTimerDT = GetCurrentDateTimeInt();
	OnTimer_Logic();
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::IsValidSchedule(UINT64 nSchInfo, UINT32& nLastChkTime, PCHAR szLog, UINT32 nLogLen)
{
	U64_SCHEDULE tIS, tISExt;
	INT32 nRtn = 0;
	UINT32 nCurTime = GetCurrentDateTimeInt();
	INT32 nChkHour = 0, nSchHour = 0;
	INT32 nChkMin = 0, nSchMin = 0;
	INT32 nWeek = 0;
	INT32 nDay = 0;
	INT32 nScanGap = 0;
	tIS.all = nSchInfo;
	tISExt.all = nSchInfo;
	switch(tIS.U8.type)
	{
		case SCHEDULE_PERIOD_TYPE_BOOT:
		{
			if(difftime(nCurTime, nLastChkTime) >= TIMER_INTERVAL_TIME_SYS_BOOT)
			{
				UINT32 nUpTime = uptime();
				UINT32 nBootTime = nCurTime - nUpTime + TIMER_INTERVAL_TIME_SYS_BOOT;
				INT32 nIsUtfFile = 0;
				if(m_nIsStart == 0)
				{
					if(nUpTime < TIMER_INTERVAL_TIME_MIN*6)
					{
						nLastChkTime = nCurTime - nUpTime;
					}
					m_nIsStart = 1;
				}

				if(nUpTime < TIMER_INTERVAL_TIME_MIN)
					break;

				if(nUpTime < TIMER_INTERVAL_TIME_MIN*10)
				{
					nIsUtfFile = is_utf8_file((char *)t_EnvInfo->m_strRootPath.c_str());
					if(nIsUtfFile < 0)
						WriteLogE("[%s] fail to get utf file info : [bt:%u] [ret:%d]", m_strLogicName.c_str(), nUpTime, nIsUtfFile);
					else if(nIsUtfFile == 1)
					{
						if(tIS.U8.min)
						{
							if((nLastChkTime < nBootTime) && (nUpTime > UINT32(nSchMin * TIMER_INTERVAL_TIME_MIN)))
								nUpTime = UINT32(nSchMin * TIMER_INTERVAL_TIME_MIN) + 1;
						}
					}
				}

				if(tIS.U8.min)
				{
					nSchMin = tIS.U8.min;
					if((nIsUtfFile == 1) && (nLastChkTime < nBootTime) && (nUpTime > UINT32(nSchMin * TIMER_INTERVAL_TIME_MIN)) && (nUpTime < UINT32((nSchMin+2) * TIMER_INTERVAL_TIME_MIN)))
					{
						nRtn = 1;
						if(szLog)
						{
							sprintf_ext(nLogLen, szLog, "[%s] valid schedule time : boot wait min : [bt:%u][%u]", m_strLogicName.c_str(), nUpTime, tIS.U8.min);
						}
						else
						{
							WriteLogN("[%s] valid schedule time : boot wait min : [bt:%u][min:%u]", m_strLogicName.c_str(), nUpTime, tIS.U8.min);
						}
					}
				}
				else
				{
					if((nUpTime < TIMER_INTERVAL_TIME_MIN*6) && (nIsUtfFile == 1))
					{
						WriteLogN("[%s] valid schedule time : boot wait min : [bt:%u]", m_strLogicName.c_str(), nUpTime);
						nRtn = 1;
					}
				}
				if(nRtn)
					nLastChkTime = nCurTime;
			}
			break;
		}

		case  SCHEDULE_PERIOD_TYPE_MOUNT_ENCRYPTFS:
			{
				if(difftime(nCurTime, nLastChkTime) >= TIMER_INTERVAL_TIME_SYS_BOOT)
				{
					if(m_nIsMountedEncfs == -1)
					{
						t_EnvInfo->GetReg_IsMountedEncfs(m_nIsMountedEncfs);
					}

					UINT32 nUpTime = uptime();

					if(nUpTime < TIMER_INTERVAL_TIME_MIN)
						break;

					BOOL bMountedEncfs = is_mounted_ecryptfs();
					if(bMountedEncfs)
					{
						if(m_nIsMountedEncfs == 0)
						{
							nRtn = 1;
							m_nIsMountedEncfs = 1;
							t_EnvInfo->SetReg_IsMountedEncfs(m_nIsMountedEncfs);
							WriteLogN("[%s] valid schedule time : mount encyptfs", m_strLogicName.c_str());
						}
					}
					else
					{
						if(m_nIsMountedEncfs == 1)
						{
							m_nIsMountedEncfs = 0;
							t_EnvInfo->SetReg_IsMountedEncfs(m_nIsMountedEncfs);
							WriteLogN("[%s] valid schedule time : unmount encyptfs", m_strLogicName.c_str());
						}
					}
					if(nRtn)
						nLastChkTime = nCurTime;
				}
				break;
			}

		case SCHEDULE_PERIOD_TYPE_FIX_HOUR:		
			{
				INT32 nLastFixTime = (nLastChkTime / 3600) * 3600;
				INT32 nCurMin = (nCurTime % TIMER_INTERVAL_TIME_HOUR) / 60;
				nChkHour = ((nCurTime - nLastFixTime) / TIMER_INTERVAL_TIME_HOUR);
				nSchHour = tIS.U8.hour;
				if(!nCurMin && nChkHour >= nSchHour)
				{
					nRtn = 1;
					nLastChkTime = (nCurTime / 3600) * 3600;
				}
				break;
			}

		case SCHEDULE_PERIOD_TYPE_DAY:
		{
			nChkHour = ((nCurTime % TIMER_INTERVAL_TIME_DAY) / TIMER_INTERVAL_TIME_HOUR);
			nChkMin = ((nCurTime % TIMER_INTERVAL_TIME_HOUR) / TIMER_INTERVAL_TIME_MIN);

			nSchHour = tIS.U8.hour;
			nSchMin = tIS.U8.min;
			nScanGap = nCurTime - nLastChkTime;

			if(nChkHour == nSchHour && nChkMin == nSchMin && (nScanGap >= (TIMER_INTERVAL_TIME_HOUR * 24)))
			{
				nRtn = 1;
				nLastChkTime = nCurTime;//nLastChkTime = ((nCurTime / TIMER_INTERVAL_TIME_HOUR) * TIMER_INTERVAL_TIME_HOUR);
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_FIX_DAY:
		{
			nChkHour = ((nCurTime % TIMER_INTERVAL_TIME_DAY) / TIMER_INTERVAL_TIME_HOUR);
			nChkMin = ((nCurTime % TIMER_INTERVAL_TIME_HOUR) / TIMER_INTERVAL_TIME_MIN);

			nSchHour = tIS.U8.hour;
			nSchMin = tIS.U8.min;
			nScanGap = nCurTime - nLastChkTime;
			if(nChkHour == nSchHour && nChkMin == nSchMin && (nScanGap >= TIMER_INTERVAL_TIME_MIN))
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_WEEK:
		{
			nChkHour = ((nCurTime % TIMER_INTERVAL_TIME_DAY) / TIMER_INTERVAL_TIME_HOUR);
			nChkMin = ((nCurTime % TIMER_INTERVAL_TIME_HOUR) / TIMER_INTERVAL_TIME_MIN);
			nWeek = GetDayOfWeek(nCurTime, 1);

			if(nWeek != tIS.U8.value)
				break;

			nSchHour = tIS.U8.hour;
			nSchMin = tIS.U8.min;
			nScanGap = nCurTime - nLastChkTime;

			if(nChkHour == nSchHour && nChkMin == nSchMin && (nScanGap >= (TIMER_INTERVAL_TIME_HOUR * 24)))
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_MONTH:
		{
			nChkHour = ((nCurTime % TIMER_INTERVAL_TIME_DAY) / TIMER_INTERVAL_TIME_HOUR);
			nChkMin = ((nCurTime % TIMER_INTERVAL_TIME_HOUR) / TIMER_INTERVAL_TIME_MIN);
			nDay = GetDayOfMonth(nCurTime, 1);

			if(nDay != tIS.U8.value)
				break;

			nSchHour = tIS.U8.hour;
			nSchMin = tIS.U8.min;
			nScanGap = nCurTime - nLastChkTime;

			if(nChkHour == nSchHour && nChkMin == nSchMin && (nScanGap >= (TIMER_INTERVAL_TIME_HOUR * 24)))
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}

		case SCHEDULE_PERIOD_TYPE_WEEK_EXT: 
		{
			INT32 nWeek = GetDayOfWeek(nCurTime, 1);

			if(nWeek != tIS.U8.value)
				break;

			tISExt.U8.type = tIS.U8.hour;
			if(IsValidSchedule(tISExt.all, nLastChkTime, szLog, nLogLen) == 0)
				break;

			nRtn = 1;
			break;
		}

		case SCHEDULE_PERIOD_TYPE_MONTH_EXT:
		{
			INT32 nDay = GetDayOfMonth(nCurTime, 1);

			if(nDay != tIS.U8.value)
				break;

			tISExt.U8.type = tIS.U8.hour;
			if(IsValidSchedule(tISExt.all, nLastChkTime, szLog, nLogLen) == 0)
				break;

			nRtn = 1;
			break;
		}
		case SCHEDULE_PERIOD_TYPE_AUTO:
		{
			nRtn = 1;
			break;
		}
/*
		case SCHEDULE_PERIOD_TYPE_LOGIN:
		{
			if(difftime(nCurTime, nLastChkTime) < TIMER_INTERVAL_TIME_SYS_BOOT)
				break;

			TListID tIDList;
			TListIDItor begin, end;
			t_ManageWinSession.GetItemIDList(tIDList);					
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				PMEM_WIN_SESSION pMWS = t_ManageWinSession.FindItem(*begin);
				if(!pMWS)
					continue;
				
				if(nLastChkTime >= pMWS->nLogonTime)
					continue;

				if(tIS.U8.min)					
				{
					if((nCurTime - pMWS->nLogonTime) > UINT32(tIS.U8.min * TIMER_INTERVAL_TIME_MIN))
					{
						nRtn = 1;
						if(szLog)
						{
							sprintf_ext(nLogLen, szLog, "[%s] valid schedule time : login wait min : [bt:%u][%u]", m_strLogicName.c_str(), pMWS->nLogonTime, tIS.U8.min);
						}
						else
						{
							WriteLogN("[%s] valid schedule time : login wait min : [bt:%u][%u]", m_strLogicName.c_str(), pMWS->nLogonTime, tIS.U8.min);
						}
						break;
					}
				}
				else							
				{
					if(abs(INT32(nCurTime - pMWS->nLogonTime)) > TIMER_INTERVAL_TIME_SYS_BOOT)
						continue;
					
					nRtn = 1;
					if(szLog)
					{
						sprintf_ext(nLogLen, szLog, "[%s] valid schedule time : login time : [bt:%u][%u]", m_strLogicName.c_str(), pMWS->nLogonTime, tIS.U8.min);
					}
					else
					{	
						WriteLogN("[%s] valid schedule time : login time : [bt:%u][%u]", m_strLogicName.c_str(), pMWS->nLogonTime, tIS.U8.min);
					}
					break;
				}
			}
			if(nRtn)
				nLastChkTime = nCurTime;

			break;
		}
*/
		case SCHEDULE_PERIOD_TYPE_SHUTDOWN:
		case SCHEDULE_PERIOD_TYPE_SHUTDOWN_EXT:
		{
			if(t_EnvInfoOp->IsSysOffMode() && nLastChkTime != t_EnvInfoOp->m_nSysOffTime)
			{
				nRtn = 1;	
				nLastChkTime = t_EnvInfoOp->m_nSysOffTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_POWERSAVE_WAKE:
		{
			if (t_EnvInfo->m_nLastOffType == ASI_BOOT_TYPE_POWERSAVING && t_EnvInfo->m_nLastOffTime > nLastChkTime)
			{
				nRtn = 1;	
				nLastChkTime = nCurTime;
				t_EnvInfo->SetReg_LastOffInfo(ASI_BOOT_TYPE_NORMAL);
				if(szLog)
				{
					sprintf_ext(nLogLen, szLog, "[%s] valid schedule time : power saving mode", m_strLogicName.c_str());
				}
				else
				{
					WriteLogN("[%s] valid schedule time : power saving mode", m_strLogicName.c_str());
				}
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_ONCE:
		{
			if(!nLastChkTime)
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_MIN:		
		{
			nChkMin = ((nCurTime - nLastChkTime) / TIMER_INTERVAL_TIME_MIN);
			nSchMin = tIS.U8.min;
			if(nChkMin >= nSchMin)
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_HOUR:		
		{
			nChkHour = ((nCurTime - nLastChkTime) / TIMER_INTERVAL_TIME_HOUR);
			nSchHour = tIS.U8.hour;
			if(nChkHour >= nSchHour)
			{
				nRtn = 1;
				nLastChkTime = nCurTime;
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_MON_WEEK_DOW_EXT:
		{
			INT32 nWeekCnt = GetWeekCntToday(tISExt.U8.ext_value);
			if(nWeekCnt != tIS.U8.value)
			{
				break;
			}

			INT32 nWeek = GetDayOfWeek(nCurTime, 1);
			if(nWeek != tIS.U8.hour)
			{
				break;
			}

			U64_SCHEDULE tISSub;
			
			tISSub.U8.type = tIS.U8.ext_type;
			tISSub.U8.min = tIS.U8.ext_min;

			if(IsValidSchedule(tISSub.all, nLastChkTime, szLog, nLogLen) == 0)
				break;

			nRtn = 1;
			break;
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CLogicBase::IsValidSchedule(UINT32 nType, UINT64 nStart, UINT64 nEnd)
{
	U64_SCHEDULE tISStart, tISEnd;
	tISStart.all = nStart;
	tISEnd.all = nEnd;

	INT32 nRtn = 0;
	UINT32 nCurTime = GetCurrentDateTimeInt();
	INT32 nChkHour = ((nCurTime % TIMER_INTERVAL_TIME_DAY) / TIMER_INTERVAL_TIME_HOUR);
	INT32 nChkMin = ((nCurTime % TIMER_INTERVAL_TIME_HOUR) / TIMER_INTERVAL_TIME_MIN);
	
	UINT32 nCurHM = nChkHour * 100 + nChkMin;
	UINT32 nStartHM = tISStart.U8.hour * 100 + tISStart.U8.min;
	UINT32 nEndHM = tISEnd.U8.hour * 100 + tISEnd.U8.min;

	INT32 nChkTime = 0;

	switch(nType)
	{
		case SCHEDULE_PERIOD_TYPE_DAY:
		{
			nChkTime = 1;
			break;
		}
		case SCHEDULE_PERIOD_TYPE_WEEK:
		{
			INT32 nWeek = GetDayOfWeek(nCurTime, 1);
			if(tISStart.U8.value < tISEnd.U8.value)
			{
				if(tISStart.U8.value <= nWeek && nWeek <= tISEnd.U8.value)
				{
					nChkTime = 1;
				}
			}
			else
			{
				if((ASHIN_WEEK_TYPE_SUN <= nWeek && nWeek <= tISEnd.U8.value) ||
					(tISStart.U8.value <= nWeek && nWeek <= ASHIN_WEEK_TYPE_SAT))
				{
					nChkTime = 1;
				}
			}
			break;
		}
		case SCHEDULE_PERIOD_TYPE_MONTH:
		{
			INT32 nDay = GetDayOfDay(nCurTime, 1);
			if(tISStart.U8.value < tISEnd.U8.value)
			{
				if(tISStart.U8.value <= nDay && nDay <= tISEnd.U8.value)
				{
					nChkTime = 1;
				}
			}
			else
			{
				if((1 <= nDay && nDay <= tISEnd.U8.value) ||
					(tISStart.U8.value <= nDay && nDay <= 31))
				{
					nChkTime = 1;
				}
			}

			break;
		}
	}

	if(nChkTime)
	{
		if(nStartHM <= nEndHM)
		{
			if(nStartHM <= nCurHM && nCurHM <= nEndHM)
			{
				nRtn = 1;
			}
		}
		else
		{
			if((0 <= nCurHM && nCurHM <= nEndHM) ||
				(nStartHM <= nCurHM && nCurHM <= 2400))
			{
				nRtn = 1;
			}
		}
	}

	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
