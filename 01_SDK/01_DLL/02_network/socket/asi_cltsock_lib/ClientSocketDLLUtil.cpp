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
#include "ClientSocketDLLUtil.h"

//---------------------------------------------------------------------------

//CCltSocketUtil*		t_SGACltUtil;

//Client Socket
CCltSocketUtil*	t_LgnCltUtil = NULL;
CCltSocketUtil*	t_MgrCltUtil = NULL;
CCltSocketUtil*	t_UdtCltUtil = NULL;


CCltSocketUtil::CCltSocketUtil()
{
    m_hClientSocketDll = NULL;
    m_pObject = NULL;
    m_nInitalize = 0;
	memset(m_acClassName, 0, MAX_TYPE_LEN);
	ClearFuncAddr();
}
//---------------------------------------------------------------------------

CCltSocketUtil::~CCltSocketUtil()
{
	FreeLibraryExt();
}
//---------------------------------------------------------------------------

void    CCltSocketUtil::ClearFuncAddr()
{
	m_fNewClient_CLD = NULL;
	m_fStartClient_CLD = NULL;
	m_fSend_CLD = NULL;
	m_fCloseSocket_CLD = NULL;
	m_fEditSessionKey_CLD = NULL;
	m_fStopClient_CLD = NULL;
	m_fFreeClient_CLD = NULL;
	m_fIsExistPkt_CLD = NULL;
	m_fGetPkt_CLD = NULL;
	m_fEditNotifyMsgValue_CLD = NULL;
	m_fGetLocalAddress_CLD = NULL;
	m_fFreeMemory_CLD = NULL;
	m_fClearAllPkt_CLD = NULL;
}

void    CCltSocketUtil::SetInitialize(INT32 nFlag)
{
    m_nInitalize = nFlag;
}

//---------------------------------------------------------------------------

INT32   CCltSocketUtil::IsInitialize()
{
	if(m_pObject == NULL || m_fSend_CLD == NULL)
		return 0;
    return 1;
}
//---------------------------------------------------------------------------

void    CCltSocketUtil::FreeLibraryExt()
{
    if(m_hClientSocketDll != NULL)
    {
        if(m_pObject != NULL)
        {
        	if(m_fCloseSocket_CLD)
	            m_fCloseSocket_CLD(m_pObject, 1);
			if(m_fFreeClient_CLD)
	            m_fStopClient_CLD(m_pObject);
            m_pObject = NULL;
        }
        FreeLibrary(m_hClientSocketDll);
        m_hClientSocketDll = NULL;
		ClearFuncAddr();
		memset(m_acClassName, 0, MAX_TYPE_LEN);
        m_nInitalize = 0;
    }
}
//---------------------------------------------------------------------------

INT32   CCltSocketUtil::LoadLibraryExt(LPCTSTR strDLLPath)
{
	INT32 nRetVal = 0;
	do{
		if(m_hClientSocketDll == NULL)
		{
		    m_hClientSocketDll = LoadLibrary(strDLLPath);
		    if(m_hClientSocketDll == NULL)
		    {
		        nRetVal = -1;
				break;
		    }
		}

		if(m_fNewClient_CLD == NULL)
		{
			m_fNewClient_CLD = (NewClient_CLDType)GetProcAddress(m_hClientSocketDll, "NewClient_CLD");
		    if(m_fNewClient_CLD == NULL)
		    {
		        nRetVal = -10;
				break;
		    }
		}

		if(m_fFreeClient_CLD == NULL)
		{
		    m_fFreeClient_CLD = (FreeClient_CLDType)GetProcAddress(m_hClientSocketDll, "FreeClient_CLD");
		    if(m_fFreeClient_CLD == NULL)
		    {
		        nRetVal = -19;
				break;
		    }
		}

		if(m_fStartClient_CLD == NULL)
		{
		    m_fStartClient_CLD = (StartClient_CLDType)GetProcAddress(m_hClientSocketDll, "StartClient_CLD");
		    if(m_fStartClient_CLD == NULL)
		    {
		        nRetVal = -11;
				break;
		    }
		}

		if(m_fSend_CLD == NULL)
		{
		    m_fSend_CLD = (Send_CLDType)GetProcAddress(m_hClientSocketDll, "Send_CLD");
		    if(m_fSend_CLD == NULL)
		    {
		        nRetVal = -12;
				break;
		    }
		}

		if(m_fCloseSocket_CLD == NULL)
		{
		    m_fCloseSocket_CLD = (CloseSocket_CLDType)GetProcAddress(m_hClientSocketDll, "CloseSocket_CLD");
		    if(m_fCloseSocket_CLD == NULL)
		    {
		        nRetVal = -13;
				break;
		    }
		}

		if(m_fEditSessionKey_CLD == NULL)
		{
		    m_fEditSessionKey_CLD = (EditSessionKey_CLDType)GetProcAddress(m_hClientSocketDll, "EditSessionKey_CLD");
		    if(m_fEditSessionKey_CLD == NULL)
		    {
		        nRetVal = -14;
				break;
		    }
		}

		if(m_fStopClient_CLD == NULL)
		{
		    m_fStopClient_CLD = (StopClient_CLDType)GetProcAddress(m_hClientSocketDll, "StopClient_CLD");
		    if(m_fStopClient_CLD == NULL)
		    {
		        nRetVal = -15;
				break;
		    }
		}

		if(m_fIsExistPkt_CLD == NULL)
		{
			m_fIsExistPkt_CLD = (IsExistPkt_CLDType)GetProcAddress(m_hClientSocketDll, "IsExistPkt_CLD");
			if(m_fIsExistPkt_CLD == NULL)
			{
		        nRetVal = -16;
				break;
			}
		}
		
		if(m_fGetPkt_CLD == NULL)
		{
			m_fGetPkt_CLD = (GetPkt_CLDType)GetProcAddress(m_hClientSocketDll, "GetPkt_CLD");
		    if(m_fGetPkt_CLD == NULL)
		    {
		        nRetVal = -17;
				break;
		    }
		}


		if(m_fFreeMemory_CLD == NULL)
		{
		    m_fFreeMemory_CLD = (FreeMemory_CLDType)GetProcAddress(m_hClientSocketDll, "FreeMemory_CLD");
		    if(m_fFreeMemory_CLD == NULL)
		    {
		        nRetVal = -18;
				break;
		    }
		}


		if(m_fEditNotifyMsgValue_CLD == NULL)
		{
		    m_fEditNotifyMsgValue_CLD = (EditNotifyMsgValueType)GetProcAddress(m_hClientSocketDll, "EditNotifyMsgValue_CLD");
		    if(m_fEditNotifyMsgValue_CLD == NULL)
		    {
		        nRetVal = -20;
				break;
		    }
		}

		if(m_fGetLocalAddress_CLD == NULL)
		{
		    m_fGetLocalAddress_CLD = (GetLocalAddress_CLDType)GetProcAddress(m_hClientSocketDll, "GetLocalAddress_CLD");
		    if(m_fGetLocalAddress_CLD == NULL)
		    {
		        nRetVal = -21;
				break;
		    }
		}

		if(m_fFreeClient_CLD == NULL)
		{
		    m_fFreeClient_CLD = (FreeClient_CLDType)GetProcAddress(m_hClientSocketDll, "FreeClient_CLD");
		    if(m_fFreeClient_CLD == NULL)
		    {
		        nRetVal = -22;
				break;
		    }
		}

		if(m_fClearAllPkt_CLD == NULL)
		{
			m_fClearAllPkt_CLD = (ClearAllPkt_CLDType)GetProcAddress(m_hClientSocketDll, "ClearAllPkt_CLD");
			if(m_fClearAllPkt_CLD == NULL)
			{
				nRetVal = -23;
				break;
			}
		}

		nRetVal = 0;
		m_nInitalize = 1;
	}while(FALSE);

	if(nRetVal != 0)
	{
		if(m_hClientSocketDll != NULL)
		{
			FreeLibrary(m_hClientSocketDll);
			m_hClientSocketDll = NULL;
			ClearFuncAddr();
		}
	}
    return nRetVal;
}
//---------------------------------------------------------------------------

INT32   CCltSocketUtil::LoadLibraryExt(LPCTSTR pcDLLPath, PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM, char *pClassName)
{
    INT32 nRetVal = 0;

	if(pcDLLPath == NULL || pSID == NULL || pClassName == NULL)
		return -1;
	
	do{
		nRetVal = LoadLibraryExt(pcDLLPath);
	    if(nRetVal != 0)
	    {
	    	nRetVal -= 100;
			break;
	    }

		if(m_pObject == NULL)
		{
			nRetVal = m_fNewClient_CLD(&m_pObject);
			if(nRetVal)
			{
		        nRetVal = -200;
				break;
		    }
		}		
    	nRetVal = m_fStartClient_CLD(m_pObject, pSID, sizeof(SOCK_INIT_DATA), pSNM, sizeof(SOCK_NOTIFY_MSG), pClassName);
		if(nRetVal != 0 || m_pObject == NULL)
	    {
	        nRetVal -= 300;
			break;
	    }
		nRetVal = 0;
		strncpy(m_acClassName, pClassName, MAX_TYPE_LEN-1);
	}while(FALSE);

	if(nRetVal != 0)
	{
		FreeLibraryExt();
	}
	
    return nRetVal;
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::StartClient_CLD(PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM, char *pClassName)
{
	INT32 nRetVal = 0;
	if(pSID == NULL || pClassName == NULL)
		return -1;
	if(m_fStartClient_CLD == NULL || m_fNewClient_CLD == NULL)
		return -2;
	do{
		if(m_pObject == NULL)
		{
			nRetVal = m_fNewClient_CLD(&m_pObject);
			if(nRetVal)
			{
				nRetVal -= 100;
				break;
			}
		}	
		nRetVal = m_fStartClient_CLD(m_pObject, pSID, sizeof(SOCK_INIT_DATA), pSNM, sizeof(SOCK_NOTIFY_MSG), pClassName);
		if(nRetVal != 0)
		{
			nRetVal -= 200;
			break;
		}
		strncpy(m_acClassName, pClassName, MAX_TYPE_LEN-1);
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::Send_CLD(UINT16 nType, UINT16 nCode, UINT32 length , PVOID data)
{
    if(m_pObject == NULL || m_fSend_CLD == NULL)
        return -1000;
    return m_fSend_CLD(m_pObject, nType, nCode, length, data);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::Send_CLD(UINT16 nType, UINT16 nCode , PVOID data, UINT32 length)
{ 
    if(m_pObject == NULL || m_fSend_CLD == NULL)
        return -1000;
    return m_fSend_CLD(m_pObject, nType, nCode, length, data);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::CloseSocket_CLD(INT32 nFlag)
{   
    if(m_pObject == NULL || m_fCloseSocket_CLD == NULL)
        return -1000;
    return m_fCloseSocket_CLD(m_pObject, nFlag);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::EditSessionKey_CLD(UINT32 nSessionKey)
{ 
    if(m_pObject == NULL || m_fEditSessionKey_CLD == NULL)
        return -1000;
    return m_fEditSessionKey_CLD(m_pObject, nSessionKey);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::StopClient_CLD()
{  
    if(m_pObject == NULL || m_fStopClient_CLD == NULL)
        return -1000;
    return m_fStopClient_CLD(m_pObject);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::FreeClient_CLD()
{
    if(m_pObject == NULL || m_fFreeClient_CLD == NULL)
        return -1000;

	m_fFreeClient_CLD(m_pObject);
    m_pObject = NULL;
    return 0;
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::IsExistPkt_CLD()
{
	if(m_pObject == NULL || m_fIsExistPkt_CLD == NULL)	
		return -1000;
	return m_fIsExistPkt_CLD(m_pObject);
}

INT32 CCltSocketUtil::ClearAllPkt_CLD()
{
	if(m_pObject == NULL || m_fClearAllPkt_CLD == NULL)	
		return -1000;
	return m_fClearAllPkt_CLD(m_pObject);
}

//---------------------------------------------------------------------------

INT32 CCltSocketUtil::GetPkt_CLD(PKT_DATA& pkt_data)
{   
    if(m_pObject == NULL || m_fGetPkt_CLD == NULL)
        return -1000;
    return m_fGetPkt_CLD(m_pObject, pkt_data);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::EditNotifyMsgValue_CLD(SOCK_NOTIFY_MSG notify_msg_value)
{   
    if(m_pObject == NULL || m_fEditNotifyMsgValue_CLD == NULL)
        return -1000;
    return m_fEditNotifyMsgValue_CLD(m_pObject, notify_msg_value);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::GetLocalAddress_CLD(UINT32& nLocalAddress)
{   
    if(m_pObject == NULL || m_fGetLocalAddress_CLD == NULL)
        return -1000;
    return m_fGetLocalAddress_CLD(m_pObject, nLocalAddress);
}
//---------------------------------------------------------------------------

INT32 CCltSocketUtil::FreeMemory_CLD(void* pVoid)
{
    if(m_pObject == NULL || m_fFreeMemory_CLD == NULL)
        return -1000;
    return m_fFreeMemory_CLD(m_pObject, pVoid);
}
