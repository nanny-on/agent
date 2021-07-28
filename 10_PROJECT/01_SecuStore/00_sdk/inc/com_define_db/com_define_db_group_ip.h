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
 */
#ifndef COM_DEFINE_DB_DB_GROUP_IP_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_DB_GROUP_IP_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//set ip match type
#define SS_GROUP_IP_MATCH_TYPE_NAT_IP			0x00000000 // 
#define SS_GROUP_IP_MATCH_TYPE_PRI_IP			0x00000001 // 
#define SS_GROUP_IP_MATCH_TYPE_NAT_AND_PRI_IP	0x00000002 //

//-------------------------------------------------------------------
//set ip match chk flag
#define SS_GROUP_IP_MATCH_FLAG_NAT_IP			0x00000001 // 
#define SS_GROUP_IP_MATCH_FLAG_PRI_IP			0x00000002 // 
#define SS_GROUP_IP_MATCH_FLAG_NAT_AND_PRI_IP	(SS_GROUP_IP_MATCH_FLAG_NAT_IP | SS_GROUP_IP_MATCH_FLAG_PRI_IP) //


//-------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_DB_GROUP_IP_H_938EF200_F60B_452A_BD30_A10E8507EDCC