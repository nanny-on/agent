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

// MainRun.cpp: implementation of the CMainRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "MainDlg.h"

//--------------------------------------------------------------------

INT32		CMainDlg::SyncDatabase()
{

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN log_reset BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=83;");
		m_tDBSyncMap[82] = tQueryList;	
	}


	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_run_option BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_rm_log_day BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_lgn_con_period BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_boot_chk_type BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_once_pkt_num BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN lnx_conn_manage BIGINT DEFAULT 0;");

		tQueryList.push_back("ALTER TABLE po_fa_env ADD COLUMN notify_info_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_env ADD COLUMN lnx_sys_off_max_work_time BIGINT DEFAULT 0;");
		

		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_sch_time BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_del_cnt BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_limit_size BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_limit_cnt BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_chk_fdt_type BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_chk_fdt_value BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_fa_op ADD COLUMN lnx_del_method BIGINT DEFAULT 0;");
		
		tQueryList.push_back("UPDATE schema_info SET db_ver=82;");
		m_tDBSyncMap[81] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE po_host_notify_unit ADD COLUMN notify_time BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=81;");
		m_tDBSyncMap[80] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE local_env_doc_del_sch (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, descr TEXT, used_mode BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, del_path TEXT, sch_time BIGINT DEFAULT 0, scan_time BIGINT DEFAULT 0);");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN conn_manage BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=80;");
		m_tDBSyncMap[79] = tQueryList;	
	}

	//20210105 add jhjung
	{
		TListStr tQueryList;

		tQueryList.push_back("DROP TABLE po_host_notify;");
		tQueryList.push_back("DROP TABLE po_host_notify_pkg;");
		tQueryList.push_back("DROP TABLE po_in_devo_op;");
		tQueryList.push_back("DROP TABLE po_in_devo_info;");
		tQueryList.push_back("DROP TABLE po_in_devo_bl;");
		tQueryList.push_back("DROP TABLE po_in_devo_bl_pkg;");
		tQueryList.push_back("DROP TABLE po_in_devo_wl;");
		tQueryList.push_back("DROP TABLE po_in_devo_wl_pkg;");
		tQueryList.push_back("DROP TABLE po_in_devo_ex;");
		tQueryList.push_back("DROP TABLE po_in_devo_ex_pkg;");
		tQueryList.push_back("DROP TABLE po_in_devo_ex_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_ax;");
		tQueryList.push_back("DROP TABLE po_in_vuln_ax_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_ax_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_edit_app;");
		tQueryList.push_back("DROP TABLE po_in_vuln_edit_app_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_edit_app_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_qna;");
		tQueryList.push_back("DROP TABLE po_in_vuln_qna_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_qna_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_secu_usb;");
		tQueryList.push_back("DROP TABLE po_in_vuln_secu_usb_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_secu_usb_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_sw;");
		tQueryList.push_back("DROP TABLE po_in_vuln_sw_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_sw_unit;");
		tQueryList.push_back("DROP TABLE po_dv_default;");
		tQueryList.push_back("DROP TABLE po_pm_op;");
		tQueryList.push_back("DROP TABLE po_pm_dm;");
		tQueryList.push_back("DROP TABLE po_pm_dm_pkg;");
		tQueryList.push_back("DROP TABLE po_pm_dm_unit;");
		tQueryList.push_back("DROP TABLE po_pm_scan;");
		tQueryList.push_back("DROP TABLE po_pm_scan_pkg;");
		tQueryList.push_back("DROP TABLE po_pm_scan_unit;");
		tQueryList.push_back("DROP TABLE po_pm_ex;");
		tQueryList.push_back("DROP TABLE po_pm_ex_pkg;");
		tQueryList.push_back("DROP TABLE po_pm_ex_unit;");
		tQueryList.push_back("DROP TABLE po_pm_no;");
		tQueryList.push_back("DROP TABLE po_pm_no_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_patch_exception;");
		tQueryList.push_back("DROP TABLE po_in_vuln_patch_exception_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_op;");
		tQueryList.push_back("DROP TABLE po_in_vuln_patch_exception_unit;");
		tQueryList.push_back("DROP TABLE po_in_devo_notify;");
		tQueryList.push_back("DROP TABLE po_in_devo_notify_pkg;");
		tQueryList.push_back("DROP TABLE po_in_rs_op;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_pkg;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_unit;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_unit_sub_pkg;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_unit_obj_pkg;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_sub_unit;");
		tQueryList.push_back("DROP TABLE po_in_rs_op_obj_unit;");
		tQueryList.push_back("DROP TABLE po_in_rs_no;");
		tQueryList.push_back("DROP TABLE po_in_rs_no_pkg;");
		tQueryList.push_back("DROP TABLE po_in_rs_bk;");
		tQueryList.push_back("DROP TABLE po_in_ptn_ex_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_ex_unit;");
		tQueryList.push_back("DROP TABLE po_in_ptn_sp_unit;");
		tQueryList.push_back("DROP TABLE po_host_notify_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_op;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_op_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_op_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_lo;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_bl;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_bl_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_bl_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_wl;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_wl_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_wl_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_ex;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_ex_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_ex_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp_unit;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_no;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_no_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_sp_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_sp_rule;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp_rule;");
		tQueryList.push_back("DROP TABLE po_host_web_sock;");
		tQueryList.push_back("DROP TABLE po_in_ptn_op;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_ex_trust;");
		tQueryList.push_back("DROP TABLE po_in_ptn_ex_trust;");

		tQueryList.push_back("CREATE TABLE po_host_notify  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_host_notify_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_op (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_info (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_type TEXT, device_name TEXT, device_type BIGINT DEFAULT 0, device_ins_id TEXT, device_pa_ins_id TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_devo_bl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_bl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_wl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_wl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_ex (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_type BIGINT DEFAULT 0, exclude_ptn BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_ax  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, unuse_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_ax_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_ax_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ax_filter BIGINT DEFAULT 0, ax_name TEXT, ax_clsid TEXT, download_site TEXT, publisher TEXT, ax_path TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, class_name TEXT, product_name TEXT, reg_key TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_qna  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_qna_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_qna_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_question TEXT, msg_answer TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, usb_value TEXT, reg_value TEXT, process_value TEXT, file_value TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sw_chk_key TEXT, sw_version TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sw_id BIGINT DEFAULT 0, class_name TEXT, sw_regkey TEXT);");
		tQueryList.push_back("CREATE TABLE po_dv_default (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_perm BIGINT DEFAULT 0, device_pa_name TEXT, device_name TEXT, device_ins_id TEXT, device_pa_ins_id TEXT);");
		tQueryList.push_back("CREATE TABLE po_pm_op  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, log_sync_mode BIGINT DEFAULT 0, dn_try_num BIGINT DEFAULT 0, mon_rm_patch TEXT, auto_patch_type BIGINT DEFAULT 0, ptn_file_sv_name TEXT, ptn_version TEXT, ptn_file_name TEXT, ptn_file_hash TEXT, ptn_dn_file_type BIGINT DEFAULT 0, ptn_dn_file_name TEXT, ptn_dn_file_hash TEXT);");
		tQueryList.push_back("CREATE TABLE po_pm_dm  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_dm_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_dm_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, apply_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, patch_id TEXT);");
		tQueryList.push_back("CREATE TABLE po_pm_scan  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_scan_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_scan_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, scan_type BIGINT DEFAULT 0, scan_time TEXT, scan_svr_type BIGINT DEFAULT 0, scan_last_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_ex  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ex_mode BIGINT DEFAULT 0, patch_id TEXT);");
		tQueryList.push_back("CREATE TABLE po_pm_no  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_no_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_op  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_file_sv_name TEXT, ptn_version TEXT, ptn_file_name TEXT, ptn_file_hash TEXT, ptn_dn_file_type BIGINT DEFAULT 0, ptn_dn_file_name TEXT, ptn_dn_file_hash TEXT, env_scan_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, patch_info TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_devo_notify  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_notify_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_pkg  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ac_mode BIGINT DEFAULT 0, log_mode TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit_sub_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit_obj_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_sub_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sb_type TEXT, sb_proc TEXT, ac_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_op_obj_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_type TEXT, chk_path TEXT, chk_ext TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_rs_no  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_no_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_rs_bk  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, keep_size BIGINT DEFAULT 0, free_size BIGINT DEFAULT 0, file_max_size BIGINT DEFAULT 0, file_keep_num BIGINT DEFAULT 0, fext_type BIGINT DEFAULT 0, fext_info TEXT, file_keep_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, unit_type BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, user_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, confirm_info BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_host_notify_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, notify_skip_day BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, pos_type BIGINT DEFAULT 0, show_size BIGINT DEFAULT 0, show_pos BIGINT DEFAULT 0, show_time BIGINT DEFAULT 0, msg_fmt_type BIGINT DEFAULT 0, msg_info LTEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, kill_mode BIGINT DEFAULT 0, rt_fg_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, last_scan_time BIGINT DEFAULT 0, scan_path TEXT, file_gather_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_lo (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_range_type BIGINT DEFAULT 0, ptn_obj_type BIGINT DEFAULT 0, user_edit_ptn BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, unit_type BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, user_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, confirm_level BIGINT DEFAULT 0, confirm_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, confirm_info BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_no (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0, req_skip_opt BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_no_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_rule (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, file_id BIGINT DEFAULT 0, req_level BIGINT DEFAULT 0, req_day BIGINT DEFAULT 0, confirm_type BIGINT DEFAULT 0, confirm_id BIGINT DEFAULT 0, confirm_rst BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, file_key TEXT, file_path TEXT, file_name TEXT, evt_date BIGINT DEFAULT 0, end_date BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_rule (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, file_id BIGINT DEFAULT 0, req_level BIGINT DEFAULT 0, req_day BIGINT DEFAULT 0, confirm_type BIGINT DEFAULT 0, confirm_id BIGINT DEFAULT 0, confirm_rst BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, file_key TEXT, file_path TEXT, file_name TEXT, evt_date BIGINT DEFAULT 0, end_date BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_host_web_sock (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, svr_port BIGINT DEFAULT 0, web_key TEXT, policy_type TEXT, policy_opt TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_op (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, kill_mode BIGINT DEFAULT 0, rt_fg_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_trust (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, sf_name TEXT, of_name TEXT, file_key TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_trust (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, sf_name TEXT, of_name TEXT, file_key TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=79;");
		m_tDBSyncMap[78] = tQueryList;	
	}

	// 20201008 add jhjung
	{
		TListStr tQueryList;

		tQueryList.push_back("DROP TABLE env_license;");
		tQueryList.push_back("DROP TABLE env_policy;");
		tQueryList.push_back("DROP TABLE env_trust_subject;");
		tQueryList.push_back("DROP TABLE env_self_protect_agt;");
		tQueryList.push_back("DROP TABLE env_socket;");
		tQueryList.push_back("DROP TABLE env_sorg_link;");
		tQueryList.push_back("DROP TABLE env_notify_info;");
		tQueryList.push_back("DROP TABLE env_log;");
		tQueryList.push_back("DROP TABLE env_log_unit;");
		tQueryList.push_back("DROP TABLE link_license;");
		tQueryList.push_back("DROP TABLE po_ctl_panel;");
		tQueryList.push_back("DROP TABLE po_ctl_panel_pkg;");
		tQueryList.push_back("DROP TABLE po_ctl_panel_unit;");
		tQueryList.push_back("DROP TABLE po_ctl_proc;");
		tQueryList.push_back("DROP TABLE po_ctl_proc_pkg;");
		tQueryList.push_back("DROP TABLE po_ctl_proc_unit;");
		tQueryList.push_back("DROP TABLE po_deploy_file;");
		tQueryList.push_back("DROP TABLE po_deploy_file_pkg;");
		tQueryList.push_back("DROP TABLE po_deploy_file_unit;");
		tQueryList.push_back("DROP TABLE po_deploy_file_unit_info_pkg;");
		tQueryList.push_back("DROP TABLE po_deploy_file_unit_chk_pkg;");
		tQueryList.push_back("DROP TABLE po_deploy_file_unit_sch_pkg;");
		tQueryList.push_back("DROP TABLE po_deploy_file_info_unit;");
		tQueryList.push_back("DROP TABLE po_deploy_file_chk_unit;");
		tQueryList.push_back("DROP TABLE po_host_rm_info;");
		tQueryList.push_back("DROP TABLE po_host_run;");
		tQueryList.push_back("DROP TABLE po_power;");
		tQueryList.push_back("DROP TABLE po_power_pkg;");
		tQueryList.push_back("DROP TABLE po_power_unit;");
		tQueryList.push_back("DROP TABLE po_self_protect;");
		tQueryList.push_back("DROP TABLE po_self_protect_pkg;");
		tQueryList.push_back("DROP TABLE po_self_protect_unit;");
		tQueryList.push_back("DROP TABLE po_fa_op;");
		tQueryList.push_back("DROP TABLE po_fa_op_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_op_unit;");
		tQueryList.push_back("DROP TABLE po_fa_op_unit_sub_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_op_unit_obj_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_op_unit_rul_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_op_unit_sch_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_op_obj_unit;");
		tQueryList.push_back("DROP TABLE po_fa_op_sub_unit;");
		tQueryList.push_back("DROP TABLE po_fa_op_sch_unit;");
		tQueryList.push_back("DROP TABLE po_fa_proc;");
		tQueryList.push_back("DROP TABLE po_fa_proc_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_proc_unit;");
		tQueryList.push_back("DROP TABLE po_fa_clear;");
		tQueryList.push_back("DROP TABLE po_fa_clear_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_env;");
		tQueryList.push_back("DROP TABLE po_fa_env_pkg;");
		tQueryList.push_back("DROP TABLE po_fa_env_unit;");
		tQueryList.push_back("DROP TABLE po_fa_bk;");
		tQueryList.push_back("DROP TABLE po_fa_clear_unit;");
		tQueryList.push_back("DROP TABLE po_win_run;");
		tQueryList.push_back("DROP TABLE po_deploy_file_sch_unit;");
		tQueryList.push_back("DROP TABLE po_svr_info_lgn;");
		tQueryList.push_back("DROP TABLE po_svr_info_udt;");
		tQueryList.push_back("DROP TABLE po_svr_info_mgr;");
		tQueryList.push_back("DROP TABLE po_deploy_file_unit_tar_pkg;");
		tQueryList.push_back("DROP TABLE po_deploy_file_tar_unit;");
		tQueryList.push_back("DROP TABLE po_fe_ptn_bl;");
		tQueryList.push_back("DROP TABLE po_fe_exclude;");
		tQueryList.push_back("DROP TABLE po_fe_exclude_pkg;");
		tQueryList.push_back("DROP TABLE po_fe_exclude_unit;");
		tQueryList.push_back("DROP TABLE po_fe_notify;");
		tQueryList.push_back("DROP TABLE po_fe_notify_pkg;");
		tQueryList.push_back("DROP TABLE po_fe_single_ptn;");
		tQueryList.push_back("DROP TABLE po_fe_single_ptn_unit;");
		tQueryList.push_back("DROP TABLE po_fa_notify;");
		tQueryList.push_back("DROP TABLE po_fa_notify_pkg;");
		tQueryList.push_back("DROP TABLE po_fe_ptn_op;");
		tQueryList.push_back("DROP TABLE po_fe_ptn_lo;");
		tQueryList.push_back("DROP TABLE po_fe_ptn_wl;");
		tQueryList.push_back("DROP TABLE po_ctl_media;");
		tQueryList.push_back("DROP TABLE po_sch;");
		tQueryList.push_back("DROP TABLE po_sch_pkg;");
		tQueryList.push_back("DROP TABLE po_sch_unit;");
		tQueryList.push_back("DROP TABLE po_in_ptn_bl;");
		tQueryList.push_back("DROP TABLE po_in_ptn_bl_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_bl_unit;");
		tQueryList.push_back("DROP TABLE po_in_ptn_wl;");
		tQueryList.push_back("DROP TABLE po_in_ptn_wl_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_wl_unit;");
		tQueryList.push_back("DROP TABLE po_in_ptn_ex;");
		tQueryList.push_back("DROP TABLE po_in_ptn_sp;");
		tQueryList.push_back("DROP TABLE po_in_ptn_no;");
		tQueryList.push_back("DROP TABLE po_in_ptn_no_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_op_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_scan;");
		tQueryList.push_back("DROP TABLE po_in_vuln_scan_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_scan_unit;");
		tQueryList.push_back("DROP TABLE po_in_ptn_op_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ptn_op_unit;");
		tQueryList.push_back("DROP TABLE po_in_vuln_scan_unit_pkg;");

		tQueryList.push_back("CREATE TABLE env_license (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, license_key TEXT, right_package TEXT, right_policy TEXT, right_control TEXT, right_class TEXT, sup_os BIGINT DEFAULT 2);");
		tQueryList.push_back("CREATE TABLE env_policy  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_type BIGINT DEFAULT 0, policy_option BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE env_trust_subject (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, subject_info TEXT);");
		tQueryList.push_back("CREATE TABLE env_self_protect_agt (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, protect_type BIGINT DEFAULT 0, protect_info TEXT, apply_perm BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE env_socket (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE env_sorg_link (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, user_catch_type BIGINT DEFAULT 0, user_catch_value TEXT);");
		tQueryList.push_back("CREATE TABLE env_notify_info (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_type TEXT, policy_code TEXT, pos_type BIGINT DEFAULT 0, show_size BIGINT DEFAULT 0, show_pos BIGINT DEFAULT 0, show_time BIGINT DEFAULT 0, msg_fmt_type BIGINT DEFAULT 0, msg_info LTEXT, notify_before_day BIGINT DEFAULT 0, match_info TEXT, notify_skip_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE env_log	(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, record_mode BIGINT DEFAULT 0, svr_load_mode TEXT, svr_load_num TEXT, agt_load_mode TEXT, agt_load_num TEXT, sch_time BIGINT DEFAULT 0, use_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE env_log_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, evt_type BIGINT DEFAULT 0, evt_code BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, subject_type BIGINT DEFAULT 0, subject_info TEXT, target_type BIGINT DEFAULT 0, target_info TEXT, object_type BIGINT DEFAULT 0, object_code BIGINT DEFAULT 0, object_info TEXT, operation_type BIGINT DEFAULT 0, event_descr TEXT, record_mode BIGINT DEFAULT 0, sms_value TEXT, ext_info TEXT);");
		tQueryList.push_back("CREATE TABLE link_license (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, license_key TEXT);");
		tQueryList.push_back("CREATE TABLE po_ctl_panel (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, control_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_ctl_panel_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_ctl_panel_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, control_type BIGINT DEFAULT 0, ctl_reg_info TEXT, ctl_file_info TEXT);");
		tQueryList.push_back("CREATE TABLE po_ctl_proc (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_ctl_proc_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_ctl_proc_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign TEXT, file_sign_comp TEXT);");
		tQueryList.push_back("CREATE TABLE po_deploy_file (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_unit_info_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_unit_chk_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_unit_sch_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, scan_dt BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_info_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, exe_user_type BIGINT DEFAULT 0, file_type BIGINT DEFAULT 0, file_enc_type BIGINT DEFAULT 0, file_or_name TEXT, file_save_name TEXT, file_hash TEXT, file_command TEXT, target_path TEXT, exe_num_type BIGINT DEFAULT 0, schedule_used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_chk_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, comp_type BIGINT DEFAULT 0, comp_val_type BIGINT DEFAULT 0, comp_op_type BIGINT DEFAULT 0, comp_value TEXT);");
		tQueryList.push_back("CREATE TABLE po_host_rm_info (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rm_pw_type BIGINT DEFAULT 0, rm_pw TEXT);");
		tQueryList.push_back("CREATE TABLE po_host_run (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, run_option BIGINT DEFAULT 0, rm_log_day BIGINT DEFAULT 0, lgn_con_period BIGINT DEFAULT 0, mgr_show_type BIGINT DEFAULT 0, show_log_day BIGINT DEFAULT 0, boot_chk_type BIGINT DEFAULT 0, integrity_mode BIGINT DEFAULT 0, cross_auth_mode BIGINT DEFAULT 0, once_pkt_num BIGINT DEFAULT 0, mgr_show_order_pkg TEXT, mgr_show_order_cls TEXT);");
		tQueryList.push_back("CREATE TABLE po_power  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, fastboot_ctrl BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_power_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_power_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sc_period_type BIGINT DEFAULT 0, sc_start_time BIGINT DEFAULT 0, sc_end_time BIGINT DEFAULT 0, ctl_mode BIGINT DEFAULT 0, ctl_wait_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_self_protect (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, protect_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_self_protect_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_self_protect_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, protect_type BIGINT DEFAULT 0, protect_info TEXT);");
		tQueryList.push_back("CREATE TABLE po_fa_op  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, po_type BIGINT DEFAULT 0, sch_time BIGINT DEFAULT 0, del_cnt BIGINT DEFAULT 0, scan_dt BIGINT DEFAULT 0, chk_fdt_type BIGINT DEFAULT 0, chk_fdt_value BIGINT DEFAULT 0, limit_size BIGINT DEFAULT 0, limit_cnt BIGINT DEFAULT 0, del_method BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_pkg  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rule_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_unit  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, log_mode TEXT, unit_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_unit_sub_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_unit_obj_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_unit_rul_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rule_order BIGINT DEFAULT 0, ac_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_unit_sch_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_obj_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, file_path TEXT, chk_ac_path BIGINT DEFAULT 0, chk_self_path BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_op_sub_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sb_type BIGINT DEFAULT 0, sb_link_id BIGINT DEFAULT 0, sb_unicode TEXT, sb_domain TEXT, sb_name TEXT, sb_proc TEXT, log_mode TEXT);");
		tQueryList.push_back("CREATE TABLE po_fa_op_sch_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_mon BIGINT DEFAULT 0, sch_day BIGINT DEFAULT 0, sch_week BIGINT DEFAULT 0, sch_hour BIGINT DEFAULT 0, sch_min BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_proc  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_proc_pkg  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_proc_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign TEXT, file_sign_comp TEXT);");
		tQueryList.push_back("CREATE TABLE po_fa_clear  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, mdel_def_scan_id BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_clear_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_env  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sys_off_max_work_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_env_pkg  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_env_unit  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, file_ptn_type BIGINT DEFAULT 0, file_ptn_global TEXT);");
		tQueryList.push_back("CREATE TABLE po_fa_bk (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, keep_day BIGINT DEFAULT 0, keep_size BIGINT DEFAULT 0, free_size BIGINT DEFAULT 0, file_max_size BIGINT DEFAULT 1);");
		tQueryList.push_back("CREATE TABLE po_fa_clear_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, file_ptn_type BIGINT DEFAULT 0, file_ptn_local TEXT, sch_type BIGINT DEFAULT 0, sch_time BIGINT DEFAULT 0, scan_type BIGINT DEFAULT 0, chk_fdt_type BIGINT DEFAULT 0, chk_fdt_value BIGINT DEFAULT 0, ex_path TEXT, exe_path TEXT, exe_command TEXT, scan_dt BIGINT DEFAULT 0, del_cnt BIGINT DEFAULT 0, net_drive BIGINT DEFAULT 0, in_path TEXT, limit_size BIGINT DEFAULT 0, limit_cnt BIGINT DEFAULT 0, del_method BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_win_run (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, mbr_used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_sch_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_type BIGINT DEFAULT 0, sch_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_svr_info_lgn (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, svr_info_list TEXT);");
		tQueryList.push_back("CREATE TABLE po_svr_info_udt (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_svr_info_list TEXT, data_svr_info_list TEXT);");
		tQueryList.push_back("CREATE TABLE po_svr_info_mgr (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, svr_info_list TEXT, match_addr TEXT);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_unit_tar_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_deploy_file_tar_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sys_mode TEXT, sys_info TEXT);");
		tQueryList.push_back("CREATE TABLE po_fe_ptn_bl  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_exclude (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_exclude_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_exclude_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, unit_type BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign TEXT, user_name TEXT, file_sign_comp TEXT, chk_level BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_notify  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_notify_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_single_ptn (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_single_ptn_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, block_type BIGINT DEFAULT 0, fe_key TEXT, pe_type BIGINT DEFAULT 0, pe_bit BIGINT DEFAULT 0, limit_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_notify  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fa_notify_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_ptn_op  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, ptn_pkg_id BIGINT DEFAULT 0, ptn_sch_time BIGINT DEFAULT 0, ptn_scan_time BIGINT DEFAULT 0, ptn_range_type BIGINT DEFAULT 0, ptn_obj_type BIGINT DEFAULT 0, new_file_send_type BIGINT DEFAULT 0, new_file_confirm_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_ptn_lo  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_range_type BIGINT DEFAULT 0, ptn_obj_type BIGINT DEFAULT 0, new_file_send_type BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_fe_ptn_wl  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_pkg_id BIGINT DEFAULT 0, ptn_sch_time BIGINT DEFAULT 0, ptn_obj_type BIGINT DEFAULT 0, ptn_pe_bit BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_ctl_media (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, default_mode BIGINT DEFAULT 0, md_access BIGINT DEFAULT 0, cd_access BIGINT DEFAULT 0, reset_exp BIGINT DEFAULT 0, log_mode TEXT);");
		tQueryList.push_back("CREATE TABLE po_sch (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_sch_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, scan_last_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_sch_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, package_type TEXT, policy_info TEXT, start_time BIGINT DEFAULT 0, end_time BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_bl  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_bl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_bl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_wl  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_wl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_wl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_sp  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, confirm_level BIGINT DEFAULT 0, confirm_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_no (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0, req_skip_opt BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_no_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_op_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, site_score BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_scan (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rec_std_time TEXT);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_scan_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_scan_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, scan_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_op_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_ptn_op_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, last_scan_time BIGINT DEFAULT 0, scan_path TEXT, file_gather_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_scan_unit_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, target_os BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, site_score BIGINT DEFAULT 0);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=78;");
		m_tDBSyncMap[77] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE site_file_alias (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 1, sync_svr_step BIGINT DEFAULT 0, confirm_status BIGINT DEFAULT 0, fe_key TEXT, file_path TEXT, file_name TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=77;");
		m_tDBSyncMap[76] = tQueryList;	
	}
	
	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_host_web_sock (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, svr_port BIGINT DEFAULT 0, web_key TEXT, policy_type TEXT, policy_opt TEXT);");
		tQueryList.push_back("INSERT INTO po_host_web_sock	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN phws_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN phws_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("DROP TABLE po_in_ptn_op;");
		tQueryList.push_back("CREATE TABLE po_in_ptn_op (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, kill_mode BIGINT DEFAULT 0, rt_fg_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_in_ptn_op	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	

		tQueryList.push_back("DROP TABLE po_nc_ptn_ex_trust;");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_trust (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, sf_name TEXT, of_name TEXT, file_key TEXT);");

		tQueryList.push_back("DROP TABLE po_in_ptn_ex_trust;");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_trust (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, sf_name TEXT, of_name TEXT, file_key TEXT);");

		tQueryList.push_back("ALTER TABLE env_log_unit ADD COLUMN ext_info TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=76;");
		m_tDBSyncMap[75] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("DROP TABLE po_in_ptn_sp_pkg;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp_pkg;");

		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		
		tQueryList.push_back("DROP TABLE po_in_ptn_sp_rule;");
		tQueryList.push_back("DROP TABLE po_nc_ptn_sp_rule;");

		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_rule (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, file_id BIGINT DEFAULT 0, req_level BIGINT DEFAULT 0, req_day BIGINT DEFAULT 0, confirm_type BIGINT DEFAULT 0, confirm_id BIGINT DEFAULT 0, confirm_rst BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, file_key TEXT, file_path TEXT, file_name TEXT, evt_date BIGINT DEFAULT 0, end_date BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_rule (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, file_id BIGINT DEFAULT 0, req_level BIGINT DEFAULT 0, req_day BIGINT DEFAULT 0, confirm_type BIGINT DEFAULT 0, confirm_id BIGINT DEFAULT 0, confirm_rst BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, file_key TEXT, file_path TEXT, file_name TEXT, evt_date BIGINT DEFAULT 0, end_date BIGINT DEFAULT 0);");

		tQueryList.push_back("DROP TABLE log_secu;");
		tQueryList.push_back("CREATE TABLE log_secu (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, skip_target BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, user_id BIGINT DEFAULT 0, policy_type BIGINT DEFAULT 0, op_type BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, block_type BIGINT DEFAULT 0, object_type BIGINT DEFAULT 0, subject_path TEXT, subject_name TEXT, object_path TEXT, object_name TEXT, object_wpath BLOB, ext_info TEXT);");
		
		tQueryList.push_back("UPDATE schema_info SET db_ver=75;");
		m_tDBSyncMap[74] = tQueryList;	
	}	
	
	{
		TListStr tQueryList;
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, kill_mode BIGINT DEFAULT 0, rt_fg_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_op_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, last_scan_time BIGINT DEFAULT 0, scan_path TEXT, file_gather_type BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_op	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpo_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpo_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_nc_ptn_lo (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_range_type BIGINT DEFAULT 0, ptn_obj_type BIGINT DEFAULT 0, user_edit_ptn BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_lo	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpl_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpl_id BIGINT DEFAULT 0;");	
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_bl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_bl	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpb_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpb_id BIGINT DEFAULT 0;");	
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, save_path TEXT, save_name TEXT, file_hash TEXT, ptn_version TEXT, ptn_unit_num BIGINT DEFAULT 0, ptn_chk_value BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_wl_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, rt_mode BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_wl	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpw_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpw_id BIGINT DEFAULT 0;");	
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_trust (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, sf_name TEXT, of_name TEXT, file_key TEXT);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, unit_type BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, user_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_ex	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpe_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpe_id BIGINT DEFAULT 0;");	
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, confirm_level BIGINT DEFAULT 0, confirm_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_rule (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, file_id BIGINT DEFAULT 0, req_level BIGINT DEFAULT 0, req_day BIGINT DEFAULT 0, confirm_type BIGINT DEFAULT 0, confirm_id BIGINT DEFAULT 0, confirm_rst BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, file_key TEXT, evt_date BIGINT DEFAULT 0, end_date BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_sp_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, confirm_info BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_sp	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcps_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcps_id BIGINT DEFAULT 0;");	
		
		tQueryList.push_back("CREATE TABLE po_nc_ptn_no (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0, req_skip_opt BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_nc_ptn_no_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_nc_ptn_no	(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pcpn_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pcpn_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE host_status_po_nc (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, host_id BIGINT DEFAULT 0, work_mode TEXT, ptn_ver TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=74;");
		m_tDBSyncMap[73] = tQueryList;	
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN mgr_show_order_pkg TEXT;");
		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN mgr_show_order_cls TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=73;");
		m_tDBSyncMap[72] = tQueryList;	
	}
	
	{
		TListStr tQueryList;
		tQueryList.push_back("CREATE TABLE info_proc_reputation (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, edit_date BIGINT DEFAULT 0, site_file_id BIGINT DEFAULT 0, reputation BIGINT DEFAULT 0, num_of_user BIGINT DEFAULT 0);");

		tQueryList.push_back("ALTER TABLE po_in_vuln_op ADD COLUMN env_scan_mode BIGINT DEFAULT 0;");
		tQueryList.push_back("UPDATE po_in_vuln_op SET env_scan_mode = 3;");
		
		//second add query : 2020-09-04 18:25
		tQueryList.push_back("DROP TABLE po_in_ptn_ex_pkg;");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_pkg (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_order BIGINT DEFAULT 0);");
		tQueryList.push_back("DROP TABLE po_in_ptn_ex_unit;");
		tQueryList.push_back("CREATE TABLE po_in_ptn_ex_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, unit_type BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, user_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		
		tQueryList.push_back("ALTER TABLE po_in_vuln_scan ADD COLUMN rec_std_time TEXT;");

		//second add query : 2020-08-24 14:22
		tQueryList.push_back("DROP TABLE po_in_ptn_sp_unit;");
		tQueryList.push_back("CREATE TABLE po_in_ptn_sp_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, confirm_info BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0, use_day BIGINT DEFAULT 0, flt_chk_type BIGINT DEFAULT 0, file_name TEXT, file_path TEXT, file_comp TEXT, file_desc TEXT, file_hash TEXT, file_sign_pub_name TEXT, file_sign_pub_sn TEXT, file_sign_cnt_name TEXT, file_sign_cnt_sn TEXT);");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN file_keep_day BIGINT DEFAULT 0;");

		tQueryList.push_back("DROP TABLE po_host_notify_unit;");
		tQueryList.push_back("CREATE TABLE po_host_notify_unit (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, notify_skip_day BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time TEXT, pos_type BIGINT DEFAULT 0, show_size BIGINT DEFAULT 0, show_pos BIGINT DEFAULT 0, show_time BIGINT DEFAULT 0, msg_fmt_type BIGINT DEFAULT 0, msg_info LTEXT);");	

		tQueryList.push_back("UPDATE schema_info SET db_ver=72;");
		m_tDBSyncMap[71] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE env_license ADD COLUMN sup_os BIGINT DEFAULT 2;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=71;");
		m_tDBSyncMap[70] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE log_notify_file (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, order_id BIGINT DEFAULT 0, notify_file_path TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=70;");
		m_tDBSyncMap[69] = tQueryList;	
	}
	
	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE env_socket (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("ALTER TABLE po_in_vuln_sw_pkg ADD COLUMN sw_version TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=69;");
		m_tDBSyncMap[68] = tQueryList;	
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE log_deploy_file ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_device ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_doc ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_patch ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_rs ADD COLUMN user_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_secu ADD COLUMN user_id BIGINT DEFAULT 0;");


		tQueryList.push_back("UPDATE schema_info SET db_ver=68;");
		m_tDBSyncMap[67] = tQueryList;	
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE env_notify_info ADD COLUMN notify_skip_day BIGINT DEFAULT 0;");
		tQueryList.push_back("UPDATE schema_info SET db_ver=67;");
		m_tDBSyncMap[66] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE po_power ADD COLUMN fastboot_ctrl BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=66;");
		m_tDBSyncMap[65] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE po_in_ptn_no ADD COLUMN req_skip_opt BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=65;");
		m_tDBSyncMap[64] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_in_rs_op					(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_pkg				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit_sub_pkg		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_unit_obj_pkg		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0,  notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_sub_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sb_type TEXT, sb_proc TEXT, ac_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_op_obj_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, chk_type TEXT, chk_path TEXT, chk_ext TEXT);");	
		tQueryList.push_back("INSERT INTO po_in_rs_op				(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN piro_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN piro_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_in_rs_no				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_rs_no_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("INSERT INTO po_in_rs_no				(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pirn_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pirn_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_in_rs_bk				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("INSERT INTO po_in_rs_bk				(reg_date, name, admin_id) VALUES (1585048737, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pirb_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pirb_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE log_rs		 	(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, skip_target BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, policy_type BIGINT DEFAULT 0, op_type BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, block_type BIGINT DEFAULT 0, object_type BIGINT DEFAULT 0, subject_path TEXT, subject_name TEXT, object_path TEXT, object_name TEXT, object_wpath BLOB);");

		tQueryList.push_back("DROP TABLE po_in_ac_doc;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_obj_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_sub_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_unit_obj_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_doc_unit_sub_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_file;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_obj_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_sch_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_sub_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_unit_obj_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_unit_rul_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_unit_sch_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_file_unit_sub_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_obj_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_sub_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_unit;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_unit_obj_pkg;");
		tQueryList.push_back("DROP TABLE po_in_ac_sf_unit_sub_pkg;");

		//second add query : 2020-03-31 14:22

		tQueryList.push_back("ALTER TABLE po_in_rs_op_unit ADD COLUMN ac_mode BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_op_unit ADD COLUMN log_mode TEXT;");

		//third add query : 2020-04-06 15:55
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN keep_size BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN free_size BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN file_max_size BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN file_keep_num BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN fext_type BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE po_in_rs_bk ADD COLUMN fext_info TEXT;");

		tQueryList.push_back("CREATE TABLE log_rs_bk (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, skip_target BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, policy_type BIGINT DEFAULT 0, op_type BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, backup_size BIGINT DEFAULT 0, backup_type BIGINT DEFAULT 0, backup_time BIGINT DEFAULT 0, subject_path TEXT, subject_name TEXT, object_path TEXT, object_name TEXT, object_wpath BLOB, bk_file_name TEXT, or_file_hash TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=64;");

		m_tDBSyncMap[63] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_in_devo_notify				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_in_devo_notify_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("INSERT INTO po_in_devo_notify				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pidn_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pidn_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("UPDATE schema_info SET db_ver=63;");

		m_tDBSyncMap[62] = tQueryList;	
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE env_license ADD COLUMN right_class TEXT;");

		tQueryList.push_back("ALTER TABLE user ADD COLUMN src_group_name TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=62;");
		m_tDBSyncMap[61] = tQueryList;
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE po_in_vuln_sw_pkg ADD COLUMN sw_chk_key TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=61;");
		m_tDBSyncMap[60] = tQueryList;
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("DROP TABLE po_in_vuln_patch_exception_unit;");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, patch_info TEXT);");

		tQueryList.push_back("ALTER TABLE po_host_run ADD COLUMN once_pkt_num BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=60;");
		m_tDBSyncMap[59] = tQueryList;
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("ALTER TABLE po_fa_env ADD COLUMN sys_off_max_work_time BIGINT DEFAULT 0;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=59;");
		m_tDBSyncMap[58] = tQueryList;
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE log_device			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, skip_target BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, policy_type BIGINT DEFAULT 0, op_type BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0,block_type BIGINT DEFAULT 0,dv_instance_id TEXT);");

		tQueryList.push_back("DROP TABLE site_vuln;");
		tQueryList.push_back("DROP TABLE site_vuln_scan;");
		tQueryList.push_back("DROP TABLE site_vuln_repair;");
		tQueryList.push_back("DROP TABLE site_vuln_lock;");

		tQueryList.push_back("CREATE TABLE site_vuln			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, admin_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, class BIGINT DEFAULT 0, name TEXT, descr TEXT, sys_type TEXT, sys_sp_type BIGINT DEFAULT 0, sys_arch_type BIGINT DEFAULT 0, support_option BIGINT DEFAULT 0, risk_level  BIGINT DEFAULT 0, def_score  BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE site_vuln_scan		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, admin_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, name TEXT, vuln_id  BIGINT DEFAULT 0, vuln_sub_id  BIGINT DEFAULT 0, scan_order  BIGINT DEFAULT 0, sys_type TEXT, sys_sp_type BIGINT DEFAULT 0, sys_arch_type BIGINT DEFAULT 0, scan_type  BIGINT DEFAULT 0, custom_id  BIGINT DEFAULT 0, scan_path TEXT, value_type  BIGINT DEFAULT 0, decision_type  BIGINT DEFAULT 0, comp_type  BIGINT DEFAULT 0, comp_value TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=58;");
		m_tDBSyncMap[57] = tQueryList;
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("DROP TABLE po_in_vuln_sw_version;");
		tQueryList.push_back("DROP TABLE po_in_vuln_sw_version_pkg;");
		tQueryList.push_back("DROP TABLE po_in_vuln_sw_version_unit;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_patch_exception_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, patch_id BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_in_vuln_patch_exception				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");

		tQueryList.push_back("DROP TABLE host_policy;");
		tQueryList.push_back("DROP TABLE user_policy;");

		tQueryList.push_back("CREATE TABLE host_policy					(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, pup_id BIGINT DEFAULT 0, psil_id BIGINT DEFAULT 0, phrmi_id BIGINT DEFAULT 0, phr_id BIGINT DEFAULT 0, phc_id BIGINT DEFAULT 0, pp_id BIGINT DEFAULT 0, pcp_id BIGINT DEFAULT 0, pcpr_id BIGINT DEFAULT 0, psp_id BIGINT DEFAULT 0, pdf_id BIGINT DEFAULT 0, pfao_id BIGINT DEFAULT 0, pfac_id BIGINT DEFAULT 0, pfap_id BIGINT DEFAULT 0, pfae_id BIGINT DEFAULT 0, pfab_id BIGINT DEFAULT 0, pwr_id BIGINT DEFAULT 0, psiu_id BIGINT DEFAULT 0, psim_id BIGINT DEFAULT 0, pfepo_id BIGINT DEFAULT 0, pfepb_id BIGINT DEFAULT 0, pfepw_id BIGINT DEFAULT 0, pfee_id BIGINT DEFAULT 0, pfen_id BIGINT DEFAULT 0, pfesp_id BIGINT DEFAULT 0, pfepl_id BIGINT DEFAULT 0, pfan_id BIGINT DEFAULT 0, pcm_id BIGINT DEFAULT 0, ps_id BIGINT DEFAULT 0, pipo_id BIGINT DEFAULT 0, pipb_id BIGINT DEFAULT 0, pipw_id BIGINT DEFAULT 0, pipe_id BIGINT DEFAULT 0, pips_id BIGINT DEFAULT 0, pipn_id BIGINT DEFAULT 0, reserved_a BIGINT DEFAULT 0, reserved_b BIGINT DEFAULT 0, reserved_c BIGINT DEFAULT 0, pivo_id BIGINT DEFAULT 0, pivs_id BIGINT DEFAULT 0, phn_id BIGINT DEFAULT 0, pido_id BIGINT DEFAULT 0, pidb_id BIGINT DEFAULT 0, pidw_id BIGINT DEFAULT 0, pide_id BIGINT DEFAULT 0, piva_id BIGINT DEFAULT 0, pive_id BIGINT DEFAULT 0, pivq_id BIGINT DEFAULT 0, pivu_id BIGINT DEFAULT 0, pivw_id BIGINT DEFAULT 0, pivpe_id BIGINT DEFAULT 0, ppmop_id BIGINT DEFAULT 0, ppmdm_id BIGINT DEFAULT 0, ppmscan_id BIGINT DEFAULT 0, ppmex_id BIGINT DEFAULT 0, ppmno_id BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE user_policy 					(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, user_id BIGINT DEFAULT 0, pup_id BIGINT DEFAULT 0, psil_id BIGINT DEFAULT 0, phrmi_id BIGINT DEFAULT 0, phr_id BIGINT DEFAULT 0, phc_id BIGINT DEFAULT 0, pp_id BIGINT DEFAULT 0, pcp_id BIGINT DEFAULT 0, pcpr_id BIGINT DEFAULT 0, psp_id BIGINT DEFAULT 0, pdf_id BIGINT DEFAULT 0, pfao_id BIGINT DEFAULT 0, pfac_id BIGINT DEFAULT 0, pfap_id BIGINT DEFAULT 0, pfae_id BIGINT DEFAULT 0, pfab_id BIGINT DEFAULT 0, pwr_id BIGINT DEFAULT 0, psiu_id BIGINT DEFAULT 0, psim_id BIGINT DEFAULT 0, pfepo_id BIGINT DEFAULT 0, pfepb_id BIGINT DEFAULT 0, pfepw_id BIGINT DEFAULT 0, pfee_id BIGINT DEFAULT 0, pfen_id BIGINT DEFAULT 0, pfesp_id BIGINT DEFAULT 0, pfepl_id BIGINT DEFAULT 0, pfan_id BIGINT DEFAULT 0, ppre_id BIGINT DEFAULT 0, pprn_id BIGINT DEFAULT 0, ppro_id BIGINT DEFAULT 0, pprr_id BIGINT DEFAULT 0, ps_id BIGINT DEFAULT 0, pipo_id BIGINT DEFAULT 0, pipb_id BIGINT DEFAULT 0, pipw_id BIGINT DEFAULT 0, pipe_id BIGINT DEFAULT 0, pips_id BIGINT DEFAULT 0, pipn_id BIGINT DEFAULT 0, reserved_a BIGINT DEFAULT 0, reserved_b BIGINT DEFAULT 0, reserved_c BIGINT DEFAULT 0, pivo_id BIGINT DEFAULT 0, pivs_id BIGINT DEFAULT 0, phn_id BIGINT DEFAULT 0, pido_id BIGINT DEFAULT 0, pidb_id BIGINT DEFAULT 0, pidw_id BIGINT DEFAULT 0, pide_id BIGINT DEFAULT 0, piva_id BIGINT DEFAULT 0, pive_id BIGINT DEFAULT 0, pivq_id BIGINT DEFAULT 0, pivu_id BIGINT DEFAULT 0, pivw_id BIGINT DEFAULT 0, pivpe_id BIGINT DEFAULT 0, ppmop_id BIGINT DEFAULT 0, ppmdm_id BIGINT DEFAULT 0, ppmscan_id BIGINT DEFAULT 0, ppmex_id BIGINT DEFAULT 0, ppmno_id BIGINT DEFAULT 0);");

		tQueryList.push_back("DROP TABLE po_in_vuln_op;");
		tQueryList.push_back("CREATE TABLE po_in_vuln_op				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ptn_file_sv_name TEXT, ptn_version TEXT, ptn_file_name TEXT, ptn_file_hash TEXT, ptn_dn_file_type BIGINT DEFAULT 0, ptn_dn_file_name TEXT, ptn_dn_file_hash TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=57;");
		m_tDBSyncMap[56] = tQueryList;
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_pm_op				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, log_sync_mode BIGINT DEFAULT 0, dn_try_num BIGINT DEFAULT 0, mon_rm_patch TEXT, auto_patch_type BIGINT DEFAULT 0, ptn_file_sv_name TEXT, ptn_version TEXT, ptn_file_name TEXT, ptn_file_hash TEXT, ptn_dn_file_type BIGINT DEFAULT 0, ptn_dn_file_name TEXT, ptn_dn_file_hash TEXT);");
		tQueryList.push_back("INSERT INTO po_pm_op				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN ppmop_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN ppmop_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_pm_dm				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_dm_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_dm_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, apply_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, patch_id TEXT);");
		tQueryList.push_back("INSERT INTO po_pm_dm				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN ppmdm_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN ppmdm_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_pm_scan				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_scan_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_scan_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, scan_type BIGINT DEFAULT 0, scan_time TEXT, scan_svr_type BIGINT DEFAULT 0, scan_last_time BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_pm_scan				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN ppmscan_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN ppmscan_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_pm_ex				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_ex_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_ex_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ex_mode BIGINT DEFAULT 0, patch_id TEXT);");
		tQueryList.push_back("INSERT INTO po_pm_ex				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN ppmex_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN ppmex_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_pm_no				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_pm_no_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_pm_no				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN ppmno_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN ppmno_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE host_patch 			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, find_type BIGINT DEFAULT 0, kb_id BIGINT DEFAULT 0, category TEXT, name TEXT, publisher TEXT, ins_date TEXT, guid TEXT, no_remove BIGINT DEFAULT 0, uninstall TEXT);");		
		tQueryList.push_back("CREATE TABLE log_patch			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, skip_target BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, patch_id BIGINT DEFAULT 0, patch_status BIGINT DEFAULT 0, patch_type BIGINT DEFAULT 0, apply_num BIGINT DEFAULT 0);");

		tQueryList.push_back("CREATE TABLE env_local 			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, chk_type BIGINT DEFAULT 0, chk_code BIGINT DEFAULT 0, n_value TEXT, s_value TEXT);");
		tQueryList.push_back("ALTER TABLE env_notify_info ADD COLUMN match_info TEXT;");

		tQueryList.push_back("UPDATE schema_info SET db_ver=56;");
		m_tDBSyncMap[55] = tQueryList;
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("CREATE TABLE po_dv_default			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_perm BIGINT DEFAULT 0, device_pa_name TEXT, device_name TEXT, device_ins_id TEXT, device_pa_ins_id TEXT);");
		tQueryList.push_back("ALTER TABLE po_in_devo_info ADD COLUMN device_pa_ins_id TEXT;");	
		tQueryList.push_back("UPDATE schema_info SET db_ver=55;");
		m_tDBSyncMap[54] = tQueryList;	
	}

	{	
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_in_vuln_ax				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, unuse_day BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_ax_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_ax_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, ax_filter BIGINT DEFAULT 0, ax_name TEXT, ax_clsid TEXT, download_site TEXT, publisher TEXT, ax_path TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_ax					(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN piva_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN piva_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app_pkg		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_edit_app_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, class_name TEXT, product_name TEXT, reg_key TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_edit_app			(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pive_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pive_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_qna				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_qna_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_qna_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_question TEXT, msg_answer TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_qna				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pivq_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pivq_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb_pkg		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_secu_usb_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, usb_value TEXT, reg_value TEXT, process_value TEXT, file_value TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_secu_usb			(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pivu_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pivu_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_sw				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_unit			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sw_id BIGINT DEFAULT 0, class_name TEXT, sw_regkey TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_sw					(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pivw_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pivw_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_version		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_version_pkg	(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_vuln_sw_version_unit	(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, patch_id BIGINT DEFAULT 0, chk_type BIGINT DEFAULT 0, sw_product TEXT, sw_name TEXT, reg_key TEXT, reg_value TEXT, file_name TEXT, sw_version TEXT);");
		tQueryList.push_back("INSERT INTO po_in_vuln_sw_version			(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pivv_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pivv_id BIGINT DEFAULT 0;");

		tQueryList.push_back("CREATE TABLE host_vuln_rst 			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, scan_mon BIGINT DEFAULT 0, scan_date BIGINT DEFAULT 0, group_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, ip_addr TEXT, user_name TEXT, sys_name TEXT, module_version TEXT, scan_score BIGINT DEFAULT 0, scan_rst TEXT);");
		tQueryList.push_back("CREATE TABLE host_vuln_rst_unit 		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, rst_vuln_id BIGINT DEFAULT 0, site_vuln_id BIGINT DEFAULT 0, default_score BIGINT DEFAULT 0, scan_result_type BIGINT DEFAULT 0, scan_result_info TEXT);");

		tQueryList.push_back("UPDATE schema_info SET db_ver=54;");
		m_tDBSyncMap[53] = tQueryList;
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_in_devo_op			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("INSERT INTO po_in_devo_op				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pido_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pido_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_in_devo_info		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, policy_type TEXT, device_name TEXT, device_type BIGINT DEFAULT 0, device_ins_id TEXT);");

		tQueryList.push_back("CREATE TABLE po_in_devo_bl		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_bl_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_in_devo_bl		(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pidb_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pidb_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_in_devo_wl		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_wl_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_in_devo_wl		(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pidw_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pidw_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("CREATE TABLE po_in_devo_ex		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_ex_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");
		tQueryList.push_back("CREATE TABLE po_in_devo_ex_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, block_type BIGINT DEFAULT 0, exclude_ptn BIGINT DEFAULT 0);");
		tQueryList.push_back("INSERT INTO po_in_devo_ex		(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN pide_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN pide_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("UPDATE schema_info SET db_ver=53;");
		m_tDBSyncMap[52] = tQueryList;
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("CREATE TABLE 	doc_delete_info	(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, last_scan_time BIGINT DEFAULT 0, last_del_time BIGINT DEFAULT 0, doc_cnt BIGINT DEFAULT 0, sch_cnt BIGINT DEFAULT 0, menual_cnt BIGINT DEFAULT 0);");	

		tQueryList.push_back("UPDATE schema_info SET db_ver=52;");
		m_tDBSyncMap[51] = tQueryList;	
	}

	{
		TListStr tQueryList;

		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN skip_target BIGINT DEFAULT 0;");	

		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN host_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN reg_svr_id BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN sync_svr_mode BIGINT DEFAULT 0;");
		tQueryList.push_back("ALTER TABLE log_event ADD COLUMN sync_svr_step BIGINT DEFAULT 0;");

		tQueryList.push_back("DROP TABLE log_status;");


		tQueryList.push_back("UPDATE schema_info SET db_ver=51;");
		m_tDBSyncMap[50] = tQueryList;	
	}


	{
		TListStr tQueryList;

		tQueryList.push_back("CREATE TABLE po_host_notify				(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, msg_type BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_host_notify_pkg			(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, policy_id BIGINT DEFAULT 0, unit_id BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0);");	
		tQueryList.push_back("CREATE TABLE po_host_notify_unit		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, reg_date BIGINT DEFAULT 0, seq_no BIGINT DEFAULT 0, name TEXT, descr TEXT, admin_id BIGINT DEFAULT 0, sub_admin_lock BIGINT DEFAULT 0, target_lock BIGINT DEFAULT 0, notify_info_id BIGINT DEFAULT 0, ext_option BIGINT DEFAULT 0, used_mode BIGINT DEFAULT 0, sch_time BIGINT DEFAULT 0, pos_type BIGINT DEFAULT 0, show_size BIGINT DEFAULT 0, show_pos BIGINT DEFAULT 0, show_time BIGINT DEFAULT 0, msg_fmt_type BIGINT DEFAULT 0, msg_info LTEXT);");	
		tQueryList.push_back("INSERT INTO po_host_notify				(reg_date, name, admin_id) VALUES (1462187932, 'default policy', 1);");	
		tQueryList.push_back("ALTER TABLE host_policy ADD COLUMN phn_id BIGINT DEFAULT 0;");	
		tQueryList.push_back("ALTER TABLE user_policy ADD COLUMN phn_id BIGINT DEFAULT 0;");	

		tQueryList.push_back("UPDATE schema_info SET db_ver=50;");
		m_tDBSyncMap[49] = tQueryList;	
	}

	{
		TListStr tQueryList;
		tQueryList.push_back("CREATE TABLE log_status		(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, used_flag BIGINT DEFAULT 1, skip_target BIGINT DEFAULT 0, reg_date BIGINT DEFAULT 0, evt_time BIGINT DEFAULT 0, evt_ecode BIGINT DEFAULT 0, notify_type BIGINT DEFAULT 0, notify_id BIGINT DEFAULT 0, host_id BIGINT DEFAULT 0, reg_svr_id BIGINT DEFAULT 0, sync_svr_step BIGINT DEFAULT 0, subject_type BIGINT DEFAULT 0, subject_id BIGINT DEFAULT 0, subject_info TEXT, target_type BIGINT DEFAULT 0, target_id BIGINT DEFAULT 0, target_info TEXT, object_type BIGINT DEFAULT 0, object_code BIGINT DEFAULT 0, object_id BIGINT DEFAULT 0, object_info TEXT, operation_type BIGINT DEFAULT 0, evt_descr TEXT, ext_option BIGINT DEFAULT 0);");
		tQueryList.push_back("UPDATE schema_info SET db_ver=49;");
		m_tDBSyncMap[48] = tQueryList;	
	}
	
	{
		INT32 nDBVer = 0;
		TMapIDListStrItor begin, end;
		CDBMgrSchemaInfo tDBSchemaInfo;
		begin = m_tDBSyncMap.begin();	end = m_tDBSyncMap.end();
		for(begin; begin != end; begin++)
		{
			nDBVer = tDBSchemaInfo.GetDBVer();
			if(begin->first != nDBVer)
				continue;

			tDBSchemaInfo.UpdateDatabase(begin->second);

			WriteLogN("[%s] dbf schema update success : [%d]", "db sync", begin->first);
		}
	}
	return 0;
}
//--------------------------------------------------------------------

