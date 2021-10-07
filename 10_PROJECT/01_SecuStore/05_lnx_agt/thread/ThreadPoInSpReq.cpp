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
#include "ThreadPoInSpReq.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInSpReq

CThreadPoInSpReq*	t_ThreadPoInSpReq = NULL;

CThreadPoInSpReq::CThreadPoInSpReq()
{
	memset(m_acFiFoPath, 0, MAX_PATH);
}

CThreadPoInSpReq::~CThreadPoInSpReq()
{
}

BOOL CThreadPoInSpReq::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInSpReq::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInSpReq message handlers

int CThreadPoInSpReq::Recv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
{
	INT32 nOnceRecvSize = 0;
	INT32 nRecvedSize = 0;
	INT32 nRecvSize = 0;
	INT32 nContinueCount = 0;
	char *pcRecv = NULL;
	if(nFd < -1 || pRecvData == NULL || nReqSize < 1)
	{
		WriteLogE("[%s] invalid input data", m_strThreadName.c_str());
		return -1;
	}
	pcRecv = (char *)pRecvData;
	while (nRecvedSize < nReqSize)
	{
		nOnceRecvSize = nReqSize - nRecvedSize;			

		nRecvSize = read(nFd, &pcRecv[nRecvedSize], nOnceRecvSize);
		if (nRecvSize == -1)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nContinueCount++;
				if(nContinueCount > 30)
				{
					return 1;
				}
				Sleep(30);
				continue;
			}
			else
			{
				WriteLogE("[%s] fail to read pipe data (%d)", m_strThreadName.c_str(), errno);
				return -2;
			}
		}
		else if (nRecvSize == 0)	//	socket close
		{
			return 2;
		}
		else
		{
			nRecvedSize += nRecvSize;
		}
	}
	return 0;
}

int CThreadPoInSpReq::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nFd = -1;
	INT32 nRetVal = 0;
	PPIPE_PO_IN_REQ pPoInReq = NULL;
	INT32 nReqSize = sizeof(PIPE_PO_IN_REQ);
	m_nRunFlag = 1;
	pPoInReq = (PPIPE_PO_IN_REQ)malloc(nReqSize);
	if(pPoInReq == NULL)
	{
		WriteLogE("[%s] fail to allocate memory (%d)", m_strThreadName.c_str(), errno);
		return -1;
	}
	nRetVal = InitEnv();
	if(nRetVal < 0)
	{
		safe_free(pPoInReq);
		return -2;
	}

	nFd=open(m_acFiFoPath, O_RDWR);
	if(nFd == -1)
	{
		WriteLogE("[%s] fail to open %s (%d)", m_strThreadName.c_str(), m_acFiFoPath, errno);
		safe_free(pPoInReq);
		return -3;
	}

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		memset(pPoInReq, 0, nReqSize);
		nRetVal = Recv(nFd, (PVOID)pPoInReq, nReqSize);
		if(nRetVal < 0)
			break;
		else if(nRetVal == 0)
		{
			if(pPoInReq->nCmdId == CMD_PIPE_EXIT_THREAD)
			{
				WriteLogN("[%s] request exit thread", m_strThreadName.c_str());
				break;
			}
			else if(pPoInReq->nCmdId == CMD_PIPE_PO_IN_REQ)
			{
				if(t_LogicPoInPtnSPRule == NULL)
				{
					WriteLogE("[%s] invalid LogicPoInPtnSPRule class. exit thread", m_strThreadName.c_str());
					break;
				}
				t_LogicPoInPtnSPRule->SendMgr_Ext_Req(pPoInReq);
			}
		}
		Sleep(30);
	}

	close(nFd);
	FreeEnv();
	safe_free(pPoInReq);
	m_nRunFlag = 0;
	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInSpReq::InitEnv()
{
	INT32 i = 0;
	snprintf(m_acFiFoPath, MAX_PATH-1, "%s/%s", NANNY_INSTALL_DIR, FIFO_FILE_PATH);
	m_acFiFoPath[MAX_PATH-1] = 0;
	if(is_file(m_acFiFoPath) == FIFO_FILE)
	{
		for(i=0; i<10; i++)
		{
			if(unlink(m_acFiFoPath) == 0)
				break;
			Sleep(300);
		}
	}
	umask(000);
	if(mkfifo(m_acFiFoPath, 0666) == -1)
	{
		WriteLogE("[%s] : fail to make fifo %s (%d)", m_strThreadName.c_str(), m_acFiFoPath, errno);
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInSpReq::FreeEnv()
{
	if(is_file(m_acFiFoPath) == FIFO_FILE)
	{
		unlink(m_acFiFoPath);
	}
	return 0;
}

INT32		CThreadPoInSpReq::SendExitThreadCmd()
{
	INT32 nFd = -1;
	INT32 nRetVal = 0;
	PPIPE_PO_IN_REQ pPoInReq = NULL;
	INT32 nReqSize = sizeof(PIPE_PO_IN_REQ);

	do{
		pPoInReq = (PPIPE_PO_IN_REQ)malloc(nReqSize);
		if(pPoInReq == NULL)
		{
			WriteLogE("[%s] fail to allocate memory (%d)", m_strThreadName.c_str(), errno);
			nRetVal = -1;
			break;
		}
		memset(pPoInReq, 0, nReqSize);

		nFd=open(m_acFiFoPath, O_WRONLY);
		if(nFd == -1)
		{
			WriteLogE("[%s] fail to open fifo %s (%d)", m_strThreadName.c_str(), m_acFiFoPath, errno);
			nRetVal = -2;
			break;
		}
		pPoInReq->nCmdId = CMD_PIPE_EXIT_THREAD;
		if(write(nFd, (PVOID)pPoInReq, nReqSize) == -1)
		{
			WriteLogE("[%s] fail to write data (%d)", m_strThreadName.c_str(), errno);
			nRetVal = -3;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	if(nFd != -1)
		close(nFd);
	safe_free(pPoInReq);
	return nRetVal;
}

