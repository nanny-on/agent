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

#ifndef ASI_SDK_DEFINE_SYSTEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define ASI_SDK_DEFINE_SYSTEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//--------------------------------------------------------------------
//System ID DataBase Version

#define ASI_SYSTEM_ID_95				0x0000000000000001
#define ASI_SYSTEM_ID_98				0x0000000000000002
#define ASI_SYSTEM_ID_98SE				0x0000000000000004
#define ASI_SYSTEM_ID_ME				0x0000000000000008

#define ASI_SYSTEM_ID_NTWS				0x0000000000000010
#define ASI_SYSTEM_ID_NTSVR          	0x0000000000000020
#define ASI_SYSTEM_ID_2XP				0x0000000000000100
#define ASI_SYSTEM_ID_2XS				0x0000000000000200
#define ASI_SYSTEM_ID_2XA				0x0000000000000400
#define ASI_SYSTEM_ID_XPH				0x0000000000000800
#define ASI_SYSTEM_ID_XPP				0x0000000000001000
#define ASI_SYSTEM_ID_23				0x0000000000002000
#define ASI_SYSTEM_ID_VISTA          	0x0000000000004000
#define ASI_SYSTEM_ID_28		        0x0000000000008000

#define ASI_SYSTEM_ID_7					0x0000000000010000
#define ASI_SYSTEM_ID_28R2	        	0x0000000000020000
#define ASI_SYSTEM_ID_8					0x0000000000040000
#define ASI_SYSTEM_ID_212		        0x0000000000080000
#define ASI_SYSTEM_ID_81				0x0000000000100000
#define ASI_SYSTEM_ID_212R2	       		0x0000000000200000
#define ASI_SYSTEM_ID_10				0x0000000000400000
#define ASI_SYSTEM_ID_16				0x0000000000800000
#define ASI_SYSTEM_ID_219				0x0000000001000000
#define ASI_SYSTEM_ID_LAST				0x0000000002000000

#define ASI_SYSTEM_ID_ALL				ASI_SYSTEM_ID_95 | ASI_SYSTEM_ID_98 | ASI_SYSTEM_ID_98SE | ASI_SYSTEM_ID_ME |\
																				ASI_SYSTEM_ID_NTWS | ASI_SYSTEM_ID_NTSVR |\
																				ASI_SYSTEM_ID_2XP | ASI_SYSTEM_ID_2XS | ASI_SYSTEM_ID_2XA |\
																				ASI_SYSTEM_ID_XPH | ASI_SYSTEM_ID_XPP |\
																				ASI_SYSTEM_ID_23 |\
																				ASI_SYSTEM_ID_VISTA | ASI_SYSTEM_ID_28 |\
																				ASI_SYSTEM_ID_7 | ASI_SYSTEM_ID_28R2 |\
																				ASI_SYSTEM_ID_8 | ASI_SYSTEM_ID_212 | ASI_SYSTEM_ID_81 | ASI_SYSTEM_ID_212R2 |\
																				ASI_SYSTEM_ID_10 | ASI_SYSTEM_ID_16 |\
																				ASI_SYSTEM_ID_219


#define ASI_LINUX_SYSTEM_ID_REDHAT		0x0000000100000000
#define ASI_LINUX_SYSTEM_ID_CENTOS		0x0000000200000000
#define ASI_LINUX_SYSTEM_ID_ORACLE		0x0000000400000000

#define ASI_LINUX_SYSTEM_ID_DEBIAN		0x0000010000000000
#define ASI_LINUX_SYSTEM_ID_UBUNTU		0x0000020000000000
#define ASI_LINUX_SYSTEM_ID_MINT		0x0000040000000000
#define ASI_LINUX_SYSTEM_ID_HAMONIKR	0x0000080000000000
#define ASI_LINUX_SYSTEM_ID_GOOROOM		0x0000100000000000
#define ASI_LINUX_SYSTEM_ID_TMAXOS		0x0000200000000000

#define ASI_LINUX_SYSTEM_ID_SUSE		0x0100000000000000

#define ASI_LINUX_SYSTEM_ID_UNKNOWN		0x8000000000000000										


#define ASI_LINUX_SYSTEM_ID_ALL			ASI_LINUX_SYSTEM_ID_REDHAT | ASI_LINUX_SYSTEM_ID_CENTOS | ASI_LINUX_SYSTEM_ID_ORACLE |\
										ASI_LINUX_SYSTEM_ID_DEBIAN | ASI_LINUX_SYSTEM_ID_UBUNTU | ASI_LINUX_SYSTEM_ID_MINT |\
										ASI_LINUX_SYSTEM_ID_HAMONIKR | ASI_LINUX_SYSTEM_ID_GOOROOM | ASI_LINUX_SYSTEM_ID_TMAXOS |\
										ASI_LINUX_SYSTEM_ID_SUSE

//--------------------------------------------------------------------

#define ASI_SYSTEM_ARCH_X86				0x00000001
#define ASI_SYSTEM_ARCH_X64				0x00000002
#define ASI_SYSTEM_ARCH_ARM				0x00000004

#define ASI_SYSTEM_ARCH_ALL				ASI_SYSTEM_ARCH_X86 | ASI_SYSTEM_ARCH_X64 | ASI_SYSTEM_ARCH_ARM

//--------------------------------------------------------------------

#define ASI_SYSTEM_TYPE_WORKSTATION		0x00000001
#define ASI_SYSTEM_TYPE_SERVER			0x00000002
#define ASI_SYSTEM_TYPE_ALL				ASI_SYSTEM_TYPE_WORKSTATION | ASI_SYSTEM_TYPE_SERVER

//--------------------------------------------------------------------

#define ASI_SYSTEM_OS_TYPE_WIN			0x00000001
#define ASI_SYSTEM_OS_TYPE_LINUX		0x00000002
#define ASI_SYSTEM_OS_TYPE_UNIX			0x00000004

#define ASI_SYSTEM_OS_TYPE_ALL			0x0000FFFF	/*ASI_SYSTEM_OS_TYPE_WIN | ASI_SYSTEM_OS_TYPE_LINUX | ASI_SYSTEM_OS_TYPE_UNIX*/
//-----------------------------------------------------------------------

#define PROCESSOR_ARCHITECTURE_INTEL            0
#define PROCESSOR_ARCHITECTURE_MIPS             1
#define PROCESSOR_ARCHITECTURE_ALPHA            2
#define PROCESSOR_ARCHITECTURE_PPC              3
#define PROCESSOR_ARCHITECTURE_SHX              4
#define PROCESSOR_ARCHITECTURE_ARM              5
#define PROCESSOR_ARCHITECTURE_IA64             6
#define PROCESSOR_ARCHITECTURE_ALPHA64          7
#define PROCESSOR_ARCHITECTURE_MSIL             8
#define PROCESSOR_ARCHITECTURE_AMD64            9

//-----------------------------------------------------------------------
// National Code
#define LOCATION_NATIONAL_TYPE_US						1
#define LOCATION_NATIONAL_TYPE_KOR						2
#define	LOCATION_NATIONAL_TYPE_JAPAN					3
#define LOCATION_NATIONAL_TYPE_CHINA_SIMPLIFIED			4
#define LOCATION_NATIONAL_TYPE_CHINA_TRADITIONAL		5
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

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
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
#endif	//ASI_SDK_DEFINE_SYSTEM_H_938EF200_F60B_452A_BD30_A10E8507EDCC
