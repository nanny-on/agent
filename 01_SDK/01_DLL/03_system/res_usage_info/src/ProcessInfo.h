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

// FindProcess.h: interface for the CFindProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDPROCESS_H__3F000A3F_8AEF_46DD_ABB2_CF892C731C3F__INCLUDED_)
#define AFX_FINDPROCESS_H__3F000A3F_8AEF_46DD_ABB2_CF892C731C3F__INCLUDED_

#ifdef _POSIX_ARG_MAX
#define MAX_COMM_LEN    _POSIX_ARG_MAX
#define MAX_CMDLINE_LEN _POSIX_ARG_MAX
#else
#define MAX_COMM_LEN    128
#define MAX_CMDLINE_LEN 128
#endif

#ifdef LOGIN_NAME_MAX
#define MAX_USER_LEN    LOGIN_NAME_MAX
#else
#define MAX_USER_LEN    32
#endif

typedef struct _pid_stats {
	unsigned long long read_bytes			__attribute__ ((aligned (16)));
	unsigned long long write_bytes			__attribute__ ((packed));
	unsigned long long cancelled_write_bytes	__attribute__ ((packed));
	unsigned long long total_vsz			__attribute__ ((packed));
	unsigned long long total_rss			__attribute__ ((packed));
	unsigned long long total_stack_size		__attribute__ ((packed));
	unsigned long long total_stack_ref		__attribute__ ((packed));
	unsigned long long total_threads		__attribute__ ((packed));
	unsigned long long total_fd_nr			__attribute__ ((packed));
	unsigned long long blkio_swapin_delays		__attribute__ ((packed));
	unsigned long long minflt			__attribute__ ((packed));
	unsigned long long cminflt			__attribute__ ((packed));
	unsigned long long majflt			__attribute__ ((packed));
	unsigned long long cmajflt			__attribute__ ((packed));
	unsigned long long utime			__attribute__ ((packed));
	long long          cutime			__attribute__ ((packed));
	unsigned long long stime			__attribute__ ((packed));
	long long          cstime			__attribute__ ((packed));
	unsigned long long gtime			__attribute__ ((packed));
	long long          cgtime			__attribute__ ((packed));
	unsigned long long wtime			__attribute__ ((packed));
	unsigned long long vsz				__attribute__ ((packed));
	unsigned long long rss				__attribute__ ((packed));
	unsigned long      start_time		__attribute__ ((packed));
	unsigned long      nvcsw			__attribute__ ((packed));
	unsigned long      nivcsw			__attribute__ ((packed));
	unsigned long      stack_size		__attribute__ ((packed));
	unsigned long      stack_ref		__attribute__ ((packed));
	/* If pid is null, the process has terminated */
	unsigned int       pid				__attribute__ ((packed));
	unsigned int       ppid				__attribute__ ((packed));
	unsigned int       pgrp				__attribute__ ((packed));
	unsigned int       session			__attribute__ ((packed));	
	unsigned int       tty				__attribute__ ((packed));		
	/* If tgid is not null, then this PID is in fact a TID */
	unsigned int       tgid				__attribute__ ((packed));
	unsigned int       rt_asum_count		__attribute__ ((packed));
	unsigned int       rc_asum_count		__attribute__ ((packed));
	unsigned int       uc_asum_count		__attribute__ ((packed));
	unsigned int       tf_asum_count		__attribute__ ((packed));
	unsigned int       sk_asum_count		__attribute__ ((packed));
	unsigned int       delay_asum_count		__attribute__ ((packed));
	unsigned int       processor			__attribute__ ((packed));
	unsigned int       priority			__attribute__ ((packed));
	unsigned int       policy			__attribute__ ((packed));
	unsigned int       flags			__attribute__ ((packed));
	unsigned int       uid				__attribute__ ((packed));
	unsigned int       threads			__attribute__ ((packed));
	unsigned int       fd_nr			__attribute__ ((packed));
	char               comm[MAX_COMM_LEN];
	char               cmdline[MAX_CMDLINE_LEN];
}pid_stats;

#define PID_STATS_SIZE	(sizeof(pid_stats))

typedef struct _stats_memory {
	unsigned long long frmkb;
	unsigned long long bufkb;
	unsigned long long camkb;
	unsigned long long tlmkb;
	unsigned long long frskb;
	unsigned long long tlskb;
	unsigned long long caskb;
	unsigned long long comkb;
	unsigned long long activekb;
	unsigned long long inactkb;
	unsigned long long dirtykb;
	unsigned long long anonpgkb;
	unsigned long long slabkb;
	unsigned long long kstackkb;
	unsigned long long pgtblkb;
	unsigned long long vmusedkb;
	unsigned long long availablekb;
}stats_memory;

#define STATS_MEMORY_SIZE	(sizeof(stats_memory))


#define PROC		"/proc"
#define PID_STAT	"/proc/%u/stat"
#define PID_CMDLINE	"/proc/%u/cmdline"
#define PID_FD		"/proc/%u/fd"

#define MEMINFO		"/proc/meminfo"
#define UPTIME		"/proc/uptime"


class CFindProcess  
{
private:
	LONG 	m_TicksPerSec;
	
private:
	INT32	GetProcessCount();
	INT32	GetTotalMeminfo(unsigned     long long *pddwTotal);
	VOID  	GetProcessUserName(UINT32 dwUid, char *pUserName, INT32 nMaxLen);
	INT32 	GetProcessNameAndID(INT32 nPid, ASIRES_INFO_PROCESS_INFO &pi);
	void 	GetProcessCPUTime(ULONG dwStartTime, INT32 *pnTime, char *pcStartTime, int nMaxLen);
	INT32	ReadProcStat(INT32 nPid, pid_stats *pPidStats);
	INT32 	ReadProcCmdLine(INT32 nPid, char *acProcPath, int nMaxLen);
	INT32	ReadProcFd(INT32 nPid);
	INT32 	ReadUptime(UINT64 *pddwTime);
	INT32	Find_Process(TListResInfoProcInfo *tList_ProcessInfo);

public:
	INT32	GetResourceData(TListResInfoProcInfo &tList_ProcessInfo);
	CFindProcess();
	virtual ~CFindProcess();


};


#endif // !defined(AFX_FINDPROCESS_H__3F000A3F_8AEF_46DD_ABB2_CF892C731C3F__INCLUDED_)
