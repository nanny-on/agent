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
#include "ManageEnvSocket.h"

//---------------------------------------------------------------------------

CManageEnvSocket*	t_ManageEnvSocket = NULL;

//---------------------------------------------------------------------------

CManageEnvSocket::CManageEnvSocket()
{
	t_DBMgrEnvSocket	= new CDBMgrEnvSocket();
}
//---------------------------------------------------------------------------

CManageEnvSocket::~CManageEnvSocket()
{
	if(t_DBMgrEnvSocket)	{	delete t_DBMgrEnvSocket;	t_DBMgrEnvSocket = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvSocket::LoadDBMS()
{
	TListDBEnvSocket tDBEnvSocketList;
    TListDBEnvSocketItor begin, end;
	if(SetER(t_DBMgrEnvSocket->LoadExecute(&tDBEnvSocketList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBEnvSocketList.begin();	end = tDBEnvSocketList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}

	if(tDBEnvSocketList.size() == 0)
	{
		DB_ENV_SOCKET des;
		AddEnvSocket(des);
		WriteLogN("[svr_system] env_socket empty..add default info");
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::InitHash()
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvSocket::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::AddEnvSocket(DB_ENV_SOCKET&	des)
{
	if(SetER(t_DBMgrEnvSocket->InsertExecute(&des)))
    {
    	return g_nErrRtn;
    }

    AddItem(des.tDPH.nID, des);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::EditEnvSocket(DB_ENV_SOCKET&	des)
{
	PDB_ENV_SOCKET pdes = FindItem(des.tDPH.nID);
	if(!pdes)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvSocket->UpdateExecute(&des)))
    {
    	return g_nErrRtn;
    }

    EditItem(des.tDPH.nID, des);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::DelEnvSocket(UINT32 nID)
{
	PDB_ENV_SOCKET pdes = FindItem(nID);
	if(!pdes)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvSocket->DeleteExecute(pdes->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::ApplyEnvSocket(DB_ENV_SOCKET&	des)
{
	if(FindItem(des.tDPH.nID))
	{
		return EditEnvSocket(des);
	}
	return AddEnvSocket(des);
}
//---------------------------------------------------------------------------

String					CManageEnvSocket::GetName(UINT32 nID)
{
	PDB_ENV_SOCKET pdes = FindItem(nID);
    if(!pdes)	return "";
    return pdes->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::SetSocketErrCode(UINT32 nSocketType, UINT32 nErrCode)
{
	PDB_ENV_SOCKET pdes = GetDefaultItem();
	if(!pdes)	return 0;

	pdes->tSockErrCodeMap[nSocketType] = nErrCode;
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::GetSocketErrCode(UINT32 nSocketType)
{
	PDB_ENV_SOCKET pdes = GetDefaultItem();
	if(!pdes)	return -1;

	TMapIDItor find = pdes->tSockErrCodeMap.find(nSocketType);
	if(find == pdes->tSockErrCodeMap.end())	return -1;

	return find->second;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::DelSocketErrCode(UINT32 nSocketType)
{
	PDB_ENV_SOCKET pdes = GetDefaultItem();
	if(!pdes)	return -1;

	pdes->tSockErrCodeMap.erase(nSocketType);
	return 0;
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

INT32					CManageEnvSocket::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvSocketItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_SOCKET pdes = FindItem(nID);
	if(!pdes)	return -1;

	return SetPkt(pdes, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::SetPkt(PDB_ENV_SOCKET pdes, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdes->tDPH);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSocket::GetPkt(MemToken& RecvToken, DB_ENV_SOCKET& des)
{
	if (!RecvToken.TokenDel_DPH(des.tDPH))							goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



