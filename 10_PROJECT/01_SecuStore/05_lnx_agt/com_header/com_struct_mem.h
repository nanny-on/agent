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



#ifndef     COM_STRUCT_MEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define     COM_STRUCT_MEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//------------------------------------------------------------------------------

#include	"com_struct_mem_ctrl_instant_msg.h"
#include	"com_struct_mem_ctrl_remote_ctrl.h"
#include	"com_struct_mem_ctrl_remote_power.h"
#include	"com_struct_mem_ctrl_remote_order.h"
#include	"com_struct_mem_logic_timer.h"
#include	"com_struct_mem_win_session.h"
#include	"com_struct_mem_sys_drive.h"
#include	"com_struct_mem_find_order_info.h"
#include	"com_struct_mem_evt_mon.h"
#include	"com_struct_mem_devo_info.h"
#include	"com_struct_mem_dv_disable.h"
#include	"com_struct_mem_work_execute.h"
#include	"com_struct_mem_link_env.h"
#include	"com_struct_mem_patch_file.h"
#include	"com_struct_mem_vuln_rst.h"
#include	"com_struct_mem_chk_exist_interval.h"
#include	"com_struct_mem_process_info.h"
#include	"com_struct_mem_vuln_prescan.h"

#include	"com_struct_mem_asi_eps_reg_info.h"

#include	"com_struct_mem_db_mgr_member.h"

//------------------------------------------------------------------------------

typedef struct _mem_svr_info
{
	_mem_svr_info()
	{
		nSvrPort = 0;
	}

	String strSvrInfo;
	UINT32	nSvrPort;
}MEM_SVR_INFO, *PMEM_SVR_INFO;

typedef map<UINT32, MEM_SVR_INFO>		TMapMemSvrInfo;
typedef TMapMemSvrInfo::iterator		TMapMemSvrInfoItor;
//------------------------------------------------------------------------------

typedef struct _mem_agt_up_file_info
{
	_mem_agt_up_file_info()
	{
		nFileType		= 0;
	}

	UINT32		nFileType;
	String		strFileName;
	String		strFileHash;
}MEM_AGT_UP_FILE_INFO, *PMEM_AGT_UP_FILE_INFO;

typedef map<UINT32, MEM_AGT_UP_FILE_INFO>			TMapMemAgtUpFileInfo;
typedef TMapMemAgtUpFileInfo::iterator				TMapMemAgtUpFileInfoItor;
typedef TMapMemAgtUpFileInfo::reverse_iterator		TMapMemAgtUpFileInfoRItor;
//------------------------------------------------------------------------------


#endif      //COM_STRUCT_MEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
