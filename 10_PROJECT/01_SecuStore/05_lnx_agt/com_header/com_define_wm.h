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


#ifndef COM_DEFINE_WM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_WM_H_938EF200_F60B_452A_BD30_A10E8507EDCC


#define		SENDMESSAGE_DEFAULT_WAITING				15000

#define		WM_GLOBAL_CLT_LGN_SOCK_EVT				WM_USER + 0x0001
#define		WM_GLOBAL_CLT_MGR_SOCK_EVT				WM_USER + 0x0002
#define		WM_GLOBAL_CLT_UDT_SOCK_EVT				WM_USER + 0x0003
#define		WM_GLOBAL_SVR_LINK_SOCK_EVT				WM_USER + 0x0004

#define		WM_GLOBAL_TIMER_EVT				        WM_USER + 0x0010
#define		WM_GLOBAL_TIMER_EVT_SUB					WM_USER + 0x0011
#define     WM_AGENT_INITIALIZE						WM_USER + 0x0012
#define     WM_AGENT_AUTO_DESTORY					WM_USER + 0x0013
#define     WM_AGENT_TERMINATED						WM_USER + 0x0014
#define		WM_GLOBAL_PKT_OP_EVT					WM_USER + 0x0015

#define		WM_GLOBAL_FDL_NOTIFY					WM_USER + 0x0020
#define		WM_GLOBAL_KERNEL_LOG_EVT				WM_USER + 0x0030
#define		WM_GLOBAL_THREAD_LOG_EVT				WM_USER + 0x0031
#define		WM_GLOBAL_THREAD_END					WM_USER + 0x0040
#define		WM_GLOBAL_THREAD_FA_CLEAR_UNIT			WM_USER + 0x0041
#define		WM_GLOBAL_THREAD_FA_CLEAR_END			WM_USER + 0x0042
#define		WM_GLOBAL_THREAD_FA_CLEAR_PROGRESS		WM_USER + 0x0043
#define		WM_GLOBAL_THREAD_WORK_EXECUTE			WM_USER + 0x0044
#define		WM_GLOBAL_THREAD_PMS_WORK_NEW			WM_USER + 0x0045
#define		WM_GLOBAL_THREAD_PMS_WORK_OLD			WM_USER + 0x0046
#define		WM_GLOBAL_THREAD_VULN_PMS_WORK_NEW		WM_USER + 0x0047
#define		WM_GLOBAL_THREAD_VULN_PMS_WORK_PRE_NEW	WM_USER + 0x0048

#define     WM_PWEVT_MON_INITIALIZE					WM_USER + 0x0052
#define     WM_PWEVT_MON_CLOSE						WM_USER + 0x0053
#define     WM_PWEVT_MON_TERMINATED					WM_USER + 0x0054
#define		WM_PWEVT_MON_TIMER_EVT				    WM_USER + 0x0060



#define		WM_GLOBAL_MEDIA_CHANGED					WM_USER + 0x0100

#define		WM_GLOBAL_WEBSOCK_MSG					WM_USER + 0x0200

#define		WM_GLOBAL_DEBUG_MSG_TEST				WM_USER + 0x0500


#endif	//COM_DEFINE_WM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
