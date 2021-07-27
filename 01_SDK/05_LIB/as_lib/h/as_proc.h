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

#ifndef _AS_PROC_H_
#define _AS_PROC_H_


#define ASI_PROC_EXIST				1
#define ASI_PROC_NOT_EXIST			2


struct proc_info {
	pid_t	ppid;
	char	name[MAX_PROC_PATH];
};
typedef struct proc_info proc_info_t;


extern int get_proc_info_by_pid(int pid, proc_info_t *pProcInfo);

extern int check_proc_exist_by_pid_file(const char *acPidPath, const char *acProcName);

extern int check_proc_exist_by_name(const char *acProcName, int nExceptPid);

extern int make_pid_file(const char *acPidPath, const char *acProcName, pid_t pid);


#endif /* _AS_PROC_H_ */


