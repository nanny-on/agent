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
 */
#ifndef COM_DEFINE_DB_PO_IN_VULN_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_IN_VULN_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------

#define SS_PO_IN_VULN_SCAN_SCAN_TYPE_NONE					0x00000000
#define SS_PO_IN_VULN_SCAN_SCAN_TYPE_AUTO					0x00000001
#define SS_PO_IN_VULN_SCAN_SCAN_TYPE_DEMAND					0x00000002
#define SS_PO_IN_VULN_SCAN_SCAN_TYPE_ALL					(SS_PO_IN_VULN_SCAN_SCAN_TYPE_AUTO | SS_PO_IN_VULN_SCAN_SCAN_TYPE_DEMAND)
//--------------------------------------------------------------------

#define SS_PO_IN_VULN_SCAN_RESULT_NONE						0x00000000
#define SS_PO_IN_VULN_SCAN_RESULT_SAFE						0x00000001
#define SS_PO_IN_VULN_SCAN_RESULT_DANGER					0x00000010
#define SS_PO_IN_VULN_SCAN_RESULT_CHK_FAIL					0x00000100
#define SS_PO_IN_VULN_SCAN_RESULT_SCANNING					0x00001000
#define SS_PO_IN_VULN_SCAN_RESULT_NOT_SUPPORT				0x00010000

#define SS_PO_IN_VULN_REPAIR_RESULT_SUCCESS					0x00100000
#define SS_PO_IN_VULN_REPAIR_RESULT_FAIL					0x01000000

#define SS_PO_IN_VULN_RESULT_UNKNOWN						0xFFFFFFFF

#define SS_PO_IN_VULN_KEY_TYPE_SCHEDULE						0x01 // 
#define SS_PO_IN_VULN_KEY_TYPE_SUBJECT						0x02 // 
#define SS_PO_IN_VULN_KEY_TYPE_OBJECT						0x03 // 
//--------------------------------------------------------------------

#define SS_PO_IN_VULN_SCAN_REMOTE_SCAN_TYPE_BG				0x00000000
#define SS_PO_IN_VULN_SCAN_REMOTE_SCAN_TYPE_FG				0x00000001
//--------------------------------------------------------------------

#define SS_PO_IN_VULN_SCAN_EXT_OPTION_MGR_SAFE_CLOSE		0x00000001
//--------------------------------------------------------------------


#endif	//COM_DEFINE_DB_PO_IN_VULN_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC