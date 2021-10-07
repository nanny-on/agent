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


#ifndef _COM_DEFINE_PIPE_H__
#define _COM_DEFINE_PIPE_H__

#define FIFO_FILE_PATH "nanny/pem/po_in_sp_req"

#define		CMD_PIPE_EXIT_THREAD				1
#define		CMD_PIPE_ALIVE_MESSAGE				2
#define		CMD_PIPE_PO_IN_REQ					3
#define		CMD_PIPE_PO_IN_CHK_WHITE			4
#define		CMD_PIPE_PO_IN_CREATE_FILE			5
#define		CMD_PIPE_PO_IN_ACCESS_FILE			6
#define		CMD_PIPE_REQ_ACCESS_INFO			7

#define		CMD_PIPE_SET_POLICY_INFO			10
#define		CMD_PIPE_ADD_WHITE_FILE				11
#define		CMD_PIPE_CLEAR_WHITE_FILE			12
#define		CMD_PIPE_ADD_BLACK_FILE				13
#define		CMD_PIPE_CLEAR_BLACK_FILE			14
#define		CMD_PIPE_END_POLICY_INFO			15

#define		SS_PO_IN_PTN_SP_REQ_LEVEL_NONE              0
#define		SS_PO_IN_PTN_SP_REQ_LEVEL_NORMAL            1
#define		SS_PO_IN_PTN_SP_REQ_LEVEL_HIGH              10
#define		SS_PO_IN_PTN_SP_REQ_LEVEL_URGENCY           100

#define		SM_CHK_ACCESS_FILE_KEY	1003

#define		SM_READ_CLIENT_FLAG		1
#define		SM_WRITE_CLIENT_FLAG	2
#define		SM_READ_SERVER_FLAG		3
#define		SM_WRITE_SERVER_FLAG	4



typedef struct _pipe_po_in_req
{
	INT32	nCmdId;
	INT32	nReqLevel;
	INT32	nReqDay;
	INT32	nReserved;
	char	acReqFullPath[MAX_PATH];
	char	acReqPath[MAX_PATH];
	char	acReqFile[MAX_FILE_NAME];
}PIPE_PO_IN_REQ, *PPIPE_PO_IN_REQ;

#endif	//_COM_DEFINE_PIPE_H__