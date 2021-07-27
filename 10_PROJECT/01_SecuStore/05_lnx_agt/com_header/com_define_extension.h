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


#ifndef COM_DEFINE_EXTENSION_H_1AA3C8B4_F2B7_48CD_A04C_D45238B81DCA
#define COM_DEFINE_EXTENSION_H_1AA3C8B4_F2B7_48CD_A04C_D45238B81DCA


/////////////////////////////////////////////////////////////////////////
// Proc Util
#define DIFF_SESSION_CHECK_TYPE_EXPLORER		0x01
#define DIFF_SESSION_CHECK_TYPE_WINLOGON		0x02
#define DIFF_SESSION_CHECK_TYPE_ALL				DIFF_SESSION_CHECK_TYPE_EXPLORER | DIFF_SESSION_CHECK_TYPE_WINLOGON


/////////////////////////////////////////////////////////////////////////
// System Info Util
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// National Code
#define LOCATION_NATIONAL_TYPE_US						1
#define LOCATION_NATIONAL_TYPE_KOR						2
#define	LOCATION_NATIONAL_TYPE_JAPAN					3
#define LOCATION_NATIONAL_TYPE_CHINA_SIMPLIFIED			4
#define LOCATION_NATIONAL_TYPE_CHINA_TRADITIONAL		5

//-----------------------------------------------------------------------
// System Type Code
#define SYSTEM_NAME_95				1
#define SYSTEM_NAME_98				2
#define SYSTEM_NAME_98SE			3
#define SYSTEM_NAME_ME				5
#define SYSTEM_NAME_2XP				6
#define SYSTEM_NAME_2XS				7
#define SYSTEM_NAME_2XA				8
#define SYSTEM_NAME_XPH				9
#define SYSTEM_NAME_XPP				10
#define SYSTEM_NAME_23				11
#define SYSTEM_NAME_NTWS			21
#define SYSTEM_NAME_NTSVR           22
#define SYSTEM_NAME_VISTA           31
#define SYSTEM_NAME_LONGHORN        32
#define SYSTEM_NAME_7				35
#define SYSTEM_NAME_28R2			36
#define SYSTEM_NAME_8				40
#define SYSTEM_NAME_2012			41
#define SYSTEM_NAME_XPP64           101
//-----------------------------------------------------------------------

#ifndef PRODUCT_UNDEFINED
	#define PRODUCT_UNDEFINED                           0x00000000

	#define PRODUCT_ULTIMATE                            0x00000001
	#define PRODUCT_HOME_BASIC                          0x00000002
	#define PRODUCT_HOME_PREMIUM                        0x00000003
	#define PRODUCT_ENTERPRISE                          0x00000004
	#define PRODUCT_HOME_BASIC_N                        0x00000005
	#define PRODUCT_BUSINESS                            0x00000006
	#define PRODUCT_STANDARD_SERVER                     0x00000007
	#define PRODUCT_DATACENTER_SERVER                   0x00000008
	#define PRODUCT_SMALLBUSINESS_SERVER                0x00000009
	#define PRODUCT_ENTERPRISE_SERVER                   0x0000000A
	#define PRODUCT_STARTER                             0x0000000B
	#define PRODUCT_DATACENTER_SERVER_CORE              0x0000000C
	#define PRODUCT_STANDARD_SERVER_CORE                0x0000000D
	#define PRODUCT_ENTERPRISE_SERVER_CORE              0x0000000E
	#define PRODUCT_ENTERPRISE_SERVER_IA64              0x0000000F
	#define PRODUCT_BUSINESS_N                          0x00000010
	#define PRODUCT_WEB_SERVER                          0x00000011
	#define PRODUCT_CLUSTER_SERVER                      0x00000012
	#define PRODUCT_HOME_SERVER                         0x00000013
	#define PRODUCT_STORAGE_EXPRESS_SERVER              0x00000014
	#define PRODUCT_STORAGE_STANDARD_SERVER             0x00000015
	#define PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER           0x00000017
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019
	#define PRODUCT_HOME_PREMIUM_N                      0x0000001A
	#define PRODUCT_ENTERPRISE_N                        0x0000001B
	#define PRODUCT_ULTIMATE_N                          0x0000001C
	#define PRODUCT_WEB_SERVER_CORE                     0x0000001D
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT    0x0000001E
	#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY      0x0000001F
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING     0x00000020
	#define PRODUCT_SERVER_FOUNDATION                   0x00000021
	#define PRODUCT_HOME_PREMIUM_SERVER                 0x00000022
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V          0x00000023
	#define PRODUCT_STANDARD_SERVER_V                   0x00000024
	#define PRODUCT_DATACENTER_SERVER_V                 0x00000025
	#define PRODUCT_ENTERPRISE_SERVER_V                 0x00000026
	#define PRODUCT_DATACENTER_SERVER_CORE_V            0x00000027
	#define PRODUCT_STANDARD_SERVER_CORE_V              0x00000028
	#define PRODUCT_ENTERPRISE_SERVER_CORE_V            0x00000029
	#define PRODUCT_HYPERV                              0x0000002A
	#define PRODUCT_STORAGE_EXPRESS_SERVER_CORE         0x0000002B
	#define PRODUCT_STORAGE_STANDARD_SERVER_CORE        0x0000002C
	#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE       0x0000002D
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE      0x0000002E
	#define PRODUCT_STARTER_N                           0x0000002F
	#define PRODUCT_PROFESSIONAL                        0x00000030
	#define PRODUCT_PROFESSIONAL_N                      0x00000031
	#define PRODUCT_SB_SOLUTION_SERVER                  0x00000032
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS             0x00000033
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS           0x00000034
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE      0x00000035
	#define PRODUCT_SB_SOLUTION_SERVER_EM               0x00000036
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM          0x00000037
	#define PRODUCT_SOLUTION_EMBEDDEDSERVER             0x00000038
	#define PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE        0x00000039
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE   0x0000003F
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT       0x0000003B
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL       0x0000003C
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC    0x0000003D
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC    0x0000003E
	#define PRODUCT_CLUSTER_SERVER_V                    0x00000040
	#define PRODUCT_EMBEDDED                            0x00000041
	#define PRODUCT_STARTER_E                           0x00000042
	#define PRODUCT_HOME_BASIC_E                        0x00000043
	#define PRODUCT_HOME_PREMIUM_E                      0x00000044
	#define PRODUCT_PROFESSIONAL_E                      0x00000045
	#define PRODUCT_ENTERPRISE_E                        0x00000046
	#define PRODUCT_ULTIMATE_E                          0x00000047
	#define PRODUCT_UNLICENSED                          0xABCDABCD
#endif
//---------------------------------------------------------------------------

#endif	//COM_DEFINE_TIMER_H_1AA3C8B4_F2B7_48CD_A04C_D45238B81DCA
