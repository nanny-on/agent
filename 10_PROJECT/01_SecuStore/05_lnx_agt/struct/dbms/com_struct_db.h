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



#ifndef     COM_STRUCT_DB_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define     COM_STRUCT_DB_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//------------------------------------------------------------------------------

typedef struct _db_id
{
	_db_id()
	{
		nID = 0;
	}
	UINT32 nID;
}DB_ID, *PDB_ID;
//------------------------------------------------------------------------------
#include	"com_struct_po_header.h"
#include	"com_struct_policy.h"
#include	"com_struct_protect.h"
#include	"com_struct_file_info.h"
#include	"com_struct_ptn_file_info.h"

//------------------------------------------------------------------------------

#include	"com_struct_host.h"
#include	"com_struct_host_hw.h"
#include	"com_struct_host_sys.h"
#include	"com_struct_host_sw.h"
#include	"com_struct_host_status_po.h"
#include	"com_struct_host_status_po_nc.h"
#include 	"com_struct_host_policy.h"
#include	"com_struct_host_key.h"
#include	"com_struct_host_patch.h"
#include	"com_struct_host_vuln_rst.h"
//------------------------------------------------------------------------------

#include	"com_struct_user.h"
#include	"com_struct_user_policy.h"

#include	"com_struct_env_license.h"
#include	"com_struct_env_log.h"
#include	"com_struct_env_log_unit.h"
#include	"com_struct_env_notify_info.h"
#include	"com_struct_env_self_protect_agt.h"
#include	"com_struct_env_sorg_link.h"
#include	"com_struct_env_trust_subject.h"
#include	"com_struct_env_policy.h"
#include	"com_struct_env_local.h"
#include	"com_struct_env_socket.h"
//------------------------------------------------------------------------------

#include	"com_struct_error_code.h"
#include	"com_struct_os_type.h"
#include	"com_struct_doc_delete_info.h"

#include	"com_struct_site_file.h"
#include	"com_struct_site_file_alias.h"
#include	"com_struct_site_vuln.h"
#include	"com_struct_site_vuln_scan.h"
//------------------------------------------------------------------------------

#include	"com_struct_link_license.h"
//------------------------------------------------------------------------------

#include	"com_struct_log_deploy_file.h"
#include	"com_struct_log_doc.h"
#include	"com_struct_log_event.h"
#include	"com_struct_log_secu.h"
#include	"com_struct_log_doc_dscan.h"
#include	"com_struct_log_doc_host.h"
#include	"com_struct_log_patch.h"
#include	"com_struct_log_device.h"
#include	"com_struct_log_rs.h"
#include	"com_struct_log_rs_bk.h"
//------------------------------------------------------------------------------

#include	"com_struct_info_proc_reputation.h"
//------------------------------------------------------------------------------

#include	"com_struct_po_ctl_panel.h"
#include	"com_struct_po_ctl_proc.h"
#include	"com_struct_po_deploy_file.h"
#include	"com_struct_po_host_rm_info.h"
#include	"com_struct_po_host_run.h"

#include	"com_struct_po_sch.h"
#include	"com_struct_po_host_notify.h"
#include	"com_struct_po_host_web_sock.h"

#include	"com_struct_po_power.h"
#include	"com_struct_po_win_run.h"
#include	"com_struct_po_self_protect.h"
#include	"com_struct_po_svr_info_lgn.h"
#include	"com_struct_po_svr_info_udt.h"

#include	"com_struct_po_fa_op.h"
#include	"com_struct_po_fa_clear.h"
#include	"com_struct_po_fa_proc.h"
#include	"com_struct_po_fa_env.h"
#include	"com_struct_po_fa_bk.h"
#include	"com_struct_po_fa_notify.h"
#include	"com_struct_po_fa_del_file_after_boot.h"
#include	"com_struct_po_fa_inotify_file.h"

#include	"com_struct_po_fe_ptn_op.h"
#include	"com_struct_po_fe_ptn_lo.h"
#include	"com_struct_po_fe_ptn_bl.h"
#include	"com_struct_po_fe_ptn_wl.h"
#include	"com_struct_po_fe_exclude.h"
#include	"com_struct_po_fe_notify.h"
#include	"com_struct_po_fe_single_ptn.h"
//------------------------------------------------------------------------------

#include	"com_struct_local_env_log.h"
#include	"com_struct_local_env_auth.h"
//------------------------------------------------------------------------------

#include	"com_struct_po_in_ptn_op.h"
#include	"com_struct_po_in_ptn_bl.h"
#include	"com_struct_po_in_ptn_wl.h"
#include	"com_struct_po_in_ptn_ex.h"
#include	"com_struct_po_in_ptn_ex_trust.h"
#include	"com_struct_po_in_ptn_sp.h"
#include	"com_struct_po_in_ptn_sp_rule.h"
#include	"com_struct_po_in_ptn_no.h"

#include	"com_struct_po_in_vuln_ax.h"
#include	"com_struct_po_in_vuln_edit_app.h"
#include	"com_struct_po_in_vuln_op.h"
#include	"com_struct_po_in_vuln_qna.h"
#include	"com_struct_po_in_vuln_scan.h"
#include	"com_struct_po_in_vuln_secu_usb.h"
#include	"com_struct_po_in_vuln_sw.h"
#include	"com_struct_po_in_vuln_patch_exception.h"

#include	"com_struct_po_in_rs_op.h"
#include	"com_struct_po_in_rs_bk.h"
#include	"com_struct_po_in_rs_no.h"

#include	"com_struct_po_in_devo_op.h"
#include	"com_struct_po_in_devo_bl.h"
#include	"com_struct_po_in_devo_wl.h"
#include	"com_struct_po_in_devo_ex.h"
#include	"com_struct_po_in_devo_info.h"
#include	"com_struct_po_in_devo_notify.h"
#include	"com_struct_po_dv_default.h"
//------------------------------------------------------------------------------

#include	"com_struct_po_pm_op.h"
#include	"com_struct_po_pm_dm.h"
#include	"com_struct_po_pm_scan.h"
#include	"com_struct_po_pm_ex.h"
#include	"com_struct_po_pm_no.h"
//------------------------------------------------------------------------------

#include	"com_struct_po_nc_ptn_op.h"
#include	"com_struct_po_nc_ptn_lo.h"
#include	"com_struct_po_nc_ptn_bl.h"
#include	"com_struct_po_nc_ptn_wl.h"
#include	"com_struct_po_nc_ptn_ex.h"
#include	"com_struct_po_nc_ptn_ex_trust.h"
#include	"com_struct_po_nc_ptn_sp.h"
#include	"com_struct_po_nc_ptn_sp_rule.h"
#include	"com_struct_po_nc_ptn_no.h"
//------------------------------------------------------------------------------

#include	"com_struct_ptn.h"
//------------------------------------------------------------------------------


#endif      //COM_STRUCT_DB_H_938EF200_F60B_452A_BD30_A10E8507EDCC
