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

#pragma once

//------------------------------------------------------------------------------

#define		CHAR_TOKEN_F					'\f'
#define		CHAR_TOKEN_B					'\b'
#define		STR_TOKEN_DB_PATH				'\a'
#define		STR_TOKEN_FILE_PATH				'/'

//------------------------------------------------------------------------------

#define		STR_SERVICE_NAME				"nannysvc"
#define 	STR_SERVICE_DESCRIPTION			"ashin nanny service"

#define		STR_PROC_NAME_NANNY_MGR			"nannymgr"
#define		STR_PROC_NAME_NANNY_INTRO		"intro"
#define		STR_PROC_NAME_NANNY_DTLOCK		"dktop_lock"
#define		STR_PROC_NAME_SERVICE_CONTROL	"sc"
#define		STR_PROC_NAME_CMD				"cmd"
#define		STR_PROC_NAME_REGEDIT			"regedit"

#define		STR_SH_NAME_NANNY_UNINSTALL		"nanny_uninstall.sh"

#define		STR_WIN_SESSION					"win_session"
//------------------------------------------------------------------------------

#define		STR_TS_NAME_NANNY_MGR			"nannymanager"
#define		STR_TS_NAME_DSCAN				"dscan_%u"
#define		STR_TS_NAME_NOTIFY_MSG			"notify_msg"
#define		STR_TS_NAME_PWEVT_MON			"pwevt_mon"
#define		STR_TS_NAME_RM					"nannyrm"
#define		STR_TS_NAME_PWEVT_MON_ONCE		"pwevt_mon_%u"
#define		STR_TS_NAME_MONITOR_PW			"mpmgr_%u"
#define		STR_TS_NAME_DEPLOY_FILE			"dpfile_%u"
#define		STR_TS_NAME_REMOTE_CTRL			"rmtctrl_%u"
#define		STR_TS_NAME_INSTANT_MSG			"instantmsg_%u"
#define		STR_TS_NAME_NANNY_MGR_ONCE		"nannymanager_%u"
#define		STR_TS_NAME_SDELETE_ONCE		"sdelete_%u"
#define		STR_TS_NAME_DTLOCK_ONCE			"dtlock_%u"
#define		STR_TS_NAME_DTLOCK_ONCE_BY_HOOK	"dtlock_%u_%u"
#define		STR_TS_NANE_RENAMECOM			"rename_computer"
#define		STR_TS_NAME_HOST_NOTIFY			"host_notify"
#define		STR_TS_NAME_PTN_PATCH			"ptn_patch_%u_%u"

//------------------------------------------------------------------------------


#define		STR_MANAGER_NAME				"nannymanager"
#define		STR_HANDLER_NAME				"nannyeventhandler"
#define		STR_DEFAULT_UNINTALL_NAME		"uninstall"

//------------------------------------------------------------------------------
#define		STR_REG_DEFAULT_PATH				"software/ashin/nanny"
#define		STR_REG_DEFAULT_SVC_PATH			"software/ashin/nanny/svc"
#define		STR_REG_DEFAULT_SVC_LOCAL_PATH		"software/ashin/nanny/svc/localenv"
#define		STR_REG_DEFAULT_SVC_CON_PATH		"software/ashin/nanny/svc/conenv"
#define		STR_REG_DEFAULT_SVC_DB_PATH			"software/ashin/nanny/svc/dbenv"
#define		STR_REG_DEFAULT_SVC_DEBUG_PATH		"software/ashin/nanny/svc/debugenv"
#define		STR_REG_DEFAULT_SVC_UNINS_PATH		"software/ashin/nanny/svc/unins"
#define		STR_REG_DEFAULT_SVC_PATH_ENV_PATH	"software/ashin/nanny/svc/pathenv"
#define		STR_REG_DEFAULT_SVC_EX_POPUP_PATH				"software/ashin/nanny/svc/ex_popup"
#define		STR_REG_DEFAULT_SVC_EX_POPUP_SECU_PATH			"software/ashin/nanny/svc/ex_popup/secu"
#define		STR_REG_DEFAULT_SVC_EX_POPUP_SCHEDULE_PATH		"software/ashin/nanny/svc/ex_popup/schedule"


#define		STR_SYS_LOG_FILE_NAME				"/nanny_agt_sys_log_"

//------------------------------------------------------------------------------
#define		STR_DEPLOY_FILE_HOME_AGT_UP		"/agt_up"
#define		STR_DEPLOY_FILE_HOME_FILE		"/file"
#define		STR_DEPLOY_FILE_HOME_WPTN		"/wptn"
#define		STR_DEPLOY_FILE_HOME_PPTN		"/pptn"
#define		STR_DEPLOY_FILE_HOME_PPTN_PATCH	"/pptn/patch"
#define		STR_DEPLOY_FILE_HOME_PPTN_UTIL	"/pptn/util"
#define		STR_DEPLOY_FILE_HOME_VPTN_PATCH	"/pptn/patch"

#define		STR_DEPLOY_FILE_LOCAL_DOWN		"inven/_down"
#define		STR_DEPLOY_FILE_LOCAL_TEMP		"inven/_temp"
#define		STR_BACKUP_DOC_FILE				"inven/_docbk"
#define		STR_BACKUP_MBR_FILE				"inven/_md"
#define		STR_BACKUP_RS_BK				"inven/_rsbk"
#define		STR_WENG_PTN_FILE				"inven/_wptn"
#define		STR_BIN_CAPTURE_FILE			"inven/_bc"
#define		STR_UNZIP_PATH_FOR_UPDATE		"inven/_update"
#define		STR_PTN_PATCH_FILE				"inven/ptn"
#define		STR_PTN_PATCH_FILE_PATCH		"inven/ptn/patch"
#define		STR_PTN_VULN_FILE				"inven/ptn_vuln"
#define		STR_PTN_VULN_FILE_PATCH			"inven/ptn_vuln/patch"
#define		STR_BACKUP_CRC_FILE				"crc"
#define		STR_BIN_CAPTURE_FILE_NAME		"bc.info"

//------------------------------------------------------------------------------
#define		STR_NOTIFY_INI_FILE				"notify_time.ini"

//------------------------------------------------------------------------------

#ifdef _X86_64	
#define STR_MODULE_RUN_PATH				"/x64"
#define BIN_MODULE_RUN_TYPE				101
#else
#define STR_MODULE_RUN_PATH				"/x86"
#define BIN_MODULE_RUN_TYPE				100
#endif

//------------------------------------------------------------------------------
