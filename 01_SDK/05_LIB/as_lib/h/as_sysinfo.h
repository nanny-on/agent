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

#ifndef _AS_SYSINFO_H_
#define _AS_SYSINFO_H_

#define SYS_DMI_PATH		"/sys/class/dmi/id/"

#define LINUX_UBUNTU_NAME 		"Ubuntu"
#define LINUX_TMAXOS_NAME 		"TmaxOS"
#define LINUX_GOOROOM_NAME		"Gooroom"
#define LINUX_HAMONIKR_NAME 	"HamoniKR"


#define PATH_MAX_PARTS   	32
#define PATH_SEP_LENGTH		1


extern int get_cpu_core_num();
extern int get_cpu_type(char *cpu_type, int len);
extern int get_cpu_speed();
extern int get_bios_date(char *pcBiosDate, int nMaxLen);
extern int get_total_mem();
extern int get_total_disk_size();
extern int get_netiface_count();
extern int get_system_name(char *acSystemName, int nMaxLen);
extern void get_os_info(char *acOsName, int nMaxLen, UINT64 *pddwOsID, UINT32 *pdwOsType);



#endif /* _AS_SYSINFO_H_ */


