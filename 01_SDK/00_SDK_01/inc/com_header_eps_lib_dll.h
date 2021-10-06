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


#ifndef COM_HEADER_EPS_LIB_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_EPS_LIB_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

typedef uint64_t	DDWORD;

//------------------------------------------------------------------------

#define AS_EXT_NAME_MAX			7
#define AS_FILE_NAME_MAX		63
#define AS_PATH_NAME_MAX		511
#define AS_DESC_NAME_MAX		255
#define AS_PROC_PATH_MAX		256


#define AS_USER_NAME_MAX		23
#define AS_SUBJ_NAME_MAX		127
#define	AS_DOMAIN_NAME_MAX		127
#define AS_SID_MAX				128
#define AS_SID_MIN				8
#define EP_DISK_MAX				31


#define	AS_STOPPED				0
#define AS_STARTED				1
#define	AS_NLOADED				2

#define	AS_MODE_PEND_DELETE		4
#define	AS_MODE_NANNY_SYS		2
#define	AS_MODE_NANNY_CIRCLE	1
#define	AS_MODE_DISABLE			0

#define ACL_INVALID				3
#define ACL_WARN				2
#define ACL_ENABLE				1
#define ACL_DISABLE				0

#define CMASK_UID				0x01
#define CMASK_PGM				0x08
#define CMASK_GID				0x10
#define CMASK_SUB_PROC			0x20

#define ROLE_BYPASS_FILE		0x00000001
#define ROLE_BYPASS_REG_PROC	0x00000002
#define ROLE_DYNAMIC_WHITE		0x00000004
#define ROLE_DYNAMIC_ALL_FILE	0x00000008
#define ROLE_SINGLE_EFFECT		0x01000000
#define ROLE_PROC_AS			(ROLE_BYPASS_FILE | ROLE_BYPASS_REG_PROC)

#define RET_NONE				0x00
#define RET_ALLOW				0x01
#define RET_DENY				0x02
#define RET_WARN				0x04
#define RET_HIDE				0x08
#define RET_DENY_NOLOG			0x10
#define RET_ALLOW_PEND			0x20
#define RET_ALLOW_FILE			0x40

#define PERM_RENDSTEXT			0x00002000
#define PERM_SHORT				0x00001000
#define PERM_RENSRCEXT			0x00000800
#define PERM_FILELOG			0x00000400
#define PERM_TKILL				0x00000200
#define PERM_PEND				0x00000100
#define PERM_HIDE				0x00000080
#define PERM_PROCKILL			0x00000040
#define PERM_CREATE				0x00000020
#define PERM_DELETE				0x00000010
#define PERM_RENAME				0x00000008
#define PERM_READ				0x00000004
#define PERM_WRITE				0x00000002
#define PERM_EXEC				0x00000001
#define PERM_NONE				0x00000000

#define ASIEPS_PERM_DEFAULT_OPERATION	(PERM_EXEC | PERM_WRITE | PERM_READ | PERM_RENAME | PERM_DELETE | PERM_CREATE | PERM_RENSRCEXT | PERM_RENDSTEXT)

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/* 로깅 옵션 (bit-OR) */
#define ASIEPS_LOGOPT_NONE			0x00
#define ASIEPS_LOGOPT_ALLOW			0x01
#define ASIEPS_LOGOPT_DENY			0x02
#define ASIEPS_LOGOPT_HIDE			0x04
#define ASIEPS_LOGOPT_SO_FILE		0x08
#define ASIEPS_LOGOPT_SO_REG		0x10
//------------------------------------------------------------------------


#define		ASI_EPS_APP_POLICY_GROUP_TYPE_NIMS				1
#define		ASI_EPS_APP_POLICY_GROUP_TYPE_NOMS				2
#define		ASI_EPS_APP_POLICY_GROUP_TYPE_COMMON			3
#define		ASI_EPS_APP_POLICY_GROUP_TYPE_NUMBER			3

#define		ASI_EPS_APP_POLICY_GROUP_ID_CTL_PANEL			10000
#define		ASI_EPS_APP_POLICY_GROUP_ID_SELF_PROTECT		20000
#define		ASI_EPS_APP_POLICY_GROUP_ID_CTRL_PROC			21000
#define		ASI_EPS_APP_POLICY_GROUP_ID_CTRL_DV				22000
#define		ASI_EPS_APP_POLICY_GROUP_ID_CTRL_REG			23000
#define		ASI_EPS_APP_POLICY_GROUP_ID_CTL_SHARE_FILE		24000

#define		ASI_EPS_APP_POLICY_GROUP_ID_FA_OP				30000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FA_CLEAR			31000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FA_PROC				32000


#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_OP			34000
#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_LO			35000
#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_BL			36000
#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_WL			37000
#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_EX			38000
#define		ASI_EPS_APP_POLICY_GROUP_ID_NC_PTN_SP			39000

#define		ASI_EPS_APP_POLICY_GROUP_ID_FE_PTN_OP			40000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FE_PTN_LO			41000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FE_PTN_BL			42000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FE_PTN_WL			43000
#define		ASI_EPS_APP_POLICY_GROUP_ID_FE_EXCLUDE			44000

#define		ASI_EPS_APP_POLICY_GROUP_ID_PR_OP				50000
#define		ASI_EPS_APP_POLICY_GROUP_ID_CTL_MEDIA			51000

#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_OP			52000
#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL			53000
#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL			54000
#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_EX			55000
#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_SP			56000

#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_RS_OP			57000
#define		ASI_EPS_APP_POLICY_GROUP_ID_IN_VULN_OP			58000

#define		ASI_EPS_APP_POLICY_GROUP_ID_SELF_PROTECT_SVR	60000

#define		ASI_EPS_APP_POLICY_GROUP_ID_MAX					65000


#define ASIEPS_LOGREC_NONE				0x00000000
#define ASIEPS_LOGREC_EXE				0x00000001
#define ASIEPS_LOGREC_WRITE				0x00000002
#define ASIEPS_LOGREC_READ				0x00000004
#define ASIEPS_LOGREC_RENAME			0x00000008
#define ASIEPS_LOGREC_DELETE			0x00000010
#define ASIEPS_LOGREC_CREATE			0x00000020
#define ASIEPS_LOGREC_PROC_KILL			0x00000040
#define ASIEPS_LOGREC_HIDE				0x00000080
#define ASIEPS_LOGREC_APP_CTRL			0x00000100
#define ASIEPS_LOGREC_THREAD_KILL		0x00000200
#define ASIEPS_LOGREC_DOC_TRACE			0x00000400
#define ASIEPS_LOGREC_RENAME_SRC_EXT	0x00000800
#define ASIEPS_LOGREC_SHORT				0x00001000
#define ASIEPS_LOGREC_RENAME_DST_EXT	0x00002000

#define	ASIEPS_LOGREC_DEFAULT_ALL		(ASIEPS_LOGREC_EXE | ASIEPS_LOGREC_WRITE | ASIEPS_LOGREC_READ | ASIEPS_LOGREC_RENAME | ASIEPS_LOGREC_DELETE |\
										ASIEPS_LOGREC_CREATE | ASIEPS_LOGREC_RENAME_SRC_EXT | ASIEPS_LOGREC_SHORT | ASIEPS_LOGREC_RENAME_DST_EXT | ASIEPS_LOGREC_PROC_KILL)

//------------------------------------------------------------------------

#define		LOG_FILE_ACCESS_DENIED				0x01
#define		LOG_GETDIR_ACCESS_DENIED			0x02
#define		LOG_PROCESS_ACCESS_DENIED			0x03
#define		LOG_PROCESS_KILL					0x04
#define		LOG_FILE_WRITE_DENIED				0x05
#define		LOG_PROCESS_KILL_DENY				0x06
#define		LOG_FILE_CREATE_DENIED				0x07
#define		LOG_FILE_DELETE_DENIED				0x08
#define		LOG_FILE_RENAME_DENIED				0x09
#define		LOG_SHARE_ACCESS_DENIED				0x0a
#define		LOG_RSTCMD_DENIED					0x0b
#define		LOG_NOTIFY_EXCUTE_DENIED			0x0c
#define		LOG_NOTIFY_AUTO_UPDATE				0x0d
#define		LOG_FILE_ACCESS_HIDE				0x0e
#define		LOG_NOTIFY_CREATE_DOC				0x0f
#define		LOG_EXT_REN_SRC_EXT_DENIED			0x10
#define		LOG_EXT_REN_DST_EXT_DENIED			0x11
#define		LOG_NOTIFY_AUTO_UPDATE_WRITE		0x12

#define		LOG_EXECUTE_EXT						0x20
#define		LOG_SHORT_PATH						0x21

#define		LOG_FILE_ACCESS						0x15
#define		LOG_GETDIR_ACCESS					0x16
#define		LOG_PROCESS_ACCESS					0x17
#define		LOG_PROCESS_CLOSE					0x18
#define		LOG_WRITE_ACCESS					0x19
#define		LOG_PROCESS_CREATE					0x1a
#define		LOG_PROCESS_INIT					0x1b

#define		LOG_REGISTRY_READ					0x33
#define		LOG_REGISTRY_WRITE					0x34
#define		LOG_REGISTRY_CREATE					0x35
#define		LOG_REGISTRY_DELETE					0x36

//------------------------------------------------------------------------

#define		TYPE_DENY_LOG					0
#define		TYPE_ALLOW_LOG					1
#define		TYPE_WARN_LOG					2
#define		TYPE_HIDE_LOG					3
//------------------------------------------------------------------------

#define 	DETECT_MODE_BLACK		1
#define 	DETECT_MODE_YELLOW		2
#define 	DETECT_MODE_WHITE		3
#define 	DETECT_MODE_MBR			4
#define 	DETECT_MODE_ACL			5
#define 	DETECT_MODE_REG			6
#define 	DETECT_MODE_PK			7
#define 	DETECT_MODE_USB			8
#define 	DETECT_MODE_CFILE		9
#define 	DETECT_MODE_SOPROC		10
#define		DETECT_MODE_AS_EXT		11
#define		DETECT_MODE_SHORT_PATH	12

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define ASI_EPS_OBJECT_LAST_COMP_CHECK_MODE_DISABLE		0
#define ASI_EPS_OBJECT_LAST_COMP_CHECK_MODE_ENABLE		1

#define ASI_EPS_OBJECT_AC_TYPE_LOCAL		0x0001
#define ASI_EPS_OBJECT_AC_TYPE_NETWORK		0x0002
#define ASI_EPS_OBJECT_AC_TYPE_ALL			(ASI_EPS_OBJECT_AC_TYPE_LOCAL | ASI_EPS_OBJECT_AC_TYPE_NETWORK)


#define		ASI_EPS_APP_LOG_TYPE_SECU					1
#define		ASI_EPS_APP_LOG_TYPE_DOC					2
#define		ASI_EPS_APP_LOG_TYPE_SITE_FILE				3
#define		ASI_EPS_APP_LOG_TYPE_PO_IN_PTN_EX_TRUST		4
#define		ASI_EPS_APP_LOG_TYPE_DEVICE					5
#define		ASI_EPS_APP_LOG_TYPE_RS						6
#define		ASI_EPS_APP_LOG_TYPE_RS_BK					7
#define		ASI_EPS_APP_LOG_TYPE_PO_NC_PTN_EX_TRUST		8
#define		ASI_EPS_APP_LOG_TYPE_SITE_FILE_ALIAS		9

#define		ASI_EPS_LOG_TYPE_DENY					0x00000010
#define		ASI_EPS_LOG_TYPE_ALLOW					0x00000020
#define		ASI_EPS_LOG_TYPE_WARN					0x00000040

//------------------------------------------------------------------------

typedef enum _notification_command
{
	GetFileDescCommand = 1,
	ExitEventMessage,
	CheckCreateCommand,
	CheckCreateLogCommand,
	CheckWriteCommand,
	CheckDelRenameCommand,
	ExitCheckThreadCommand,
	CheckDeleteDocCommand,
	CheckDynamicWhiteCommand

} NOTIFICATION_COMMAND;
//------------------------------------------------------------------------

typedef struct _eps_notification 
{
	NOTIFICATION_COMMAND Command;
	DWORD 		dwProcId;
	DWORD 		dwPendTypes;
	DWORD 		dwRuleId;
	DWORD 		dwEnable;
	DWORD 		dwRetVal;
	DWORD		dwProcRole;
	DWORD		dwSid;
	char		acSid[AS_SID_MAX];
	CHAR  		acFilePath[AS_PATH_NAME_MAX+1];
} EPS_NOTIFICATION, *PEPS_NOTIFICATION;

typedef struct _as_media_mode
{
	DWORD dwMode;
	DWORD dwPerm;
} AS_MEDIA_MODE, *PAS_MEDIA_MODE;
//------------------------------------------------------------------------

typedef struct _as_media_info
{
	DWORD dwIndex;			//4Bytes
	DWORD dwPerm;			//4Bytes
	DWORD dwRuleCount;		//4Bytes
	DWORD dwLogOpt;			//4Bytes
	DWORD dwLogAllowOps;	//4Bytes
	DWORD dwLogDenyOps;		//4Bytes
	char acDiskName[EP_DISK_MAX+1];	//32Bytes
}AS_MEDIA_INFO, *PAS_MEDIA_INFO;
//------------------------------------------------------------------------

typedef struct _as_proc_role 
{
	INT32 	nProcRole;		//4Bytes
	INT32 	nLogOpt;		//4Bytes
	INT32 	nFileLogOps;	//4Bytes
	INT32 	nRegLogOps;		//4Bytes
} AS_PROC_ROLE, *PAS_PROC_ROLE;

typedef struct _pend_ext_hdr 
{
	DWORD 	dwExtType;
	DWORD 	dwReserved;
	CHAR 	szExtName[AS_EXT_NAME_MAX+1];
} PEND_EXT_HDR, *PPEND_EXT_HDR;
//------------------------------------------------------------------------

typedef struct _process_list 
{
	INT32	nIndex;		//4Bytes
	INT32	nEnable;	//4Bytes
	INT32	nResponse;	//4Bytes
	INT32	nCompare;	//4Bytes
	INT32 	nCategory;	//4Bytes
	INT32	nOps;		//4Bytes
	CHAR	acProcName[AS_FILE_NAME_MAX+1];		//64Bytes
	CHAR	acPathName[AS_PROC_PATH_MAX];		//256Bytes
} PROCESS_LIST, *PPROCESS_LIST;
//------------------------------------------------------------------------

typedef struct _as_real_group
{
	DWORD 		dwGroupLength;						//4Bytes
	DWORD 		dwReserved;							//4Bytes
	CHAR 		acGrouprName[AS_SUBJ_NAME_MAX+1];	//128Bytes
}AS_REAL_GROUP, *PAS_REAL_GROUP;

typedef struct _as_sess_info
{
	DWORD 			dwSessEnable;		//4Bytes
	DWORD 			dwSessId;			//4Bytes
	DWORD 			dwAliveTime;		//4Bytes
	DWORD 			dwSessIP;			//4Bytes
	DWORD 			dwSessPort;			//4Bytes
	DWORD 			dwGroupCount;		//4Bytes
	DWORD 			dwUserLength;		//4Bytes
	DWORD 			dwReserved;			//4Bytes
	CHAR 			acUserName[AS_SUBJ_NAME_MAX+1];	//128Bytes
	PAS_REAL_GROUP 	pRealGroup;
}AS_SESS_INFO, *PAS_SESS_INFO;

//------------------------------------------------------------------------
typedef struct _as_proc_gid
{
	DDWORD ddwGroupId;						/* 8Bytes */
	AS_PROC_ROLE stProcRole;
}AS_PROC_GID, *PAS_PROC_GID;
//------------------------------------------------------------------------


typedef struct _proc_group_list
{
	DWORD dwCount;							/* 4Bytes */
	DWORD dwReserved;						/* 4Bytes */
	PAS_PROC_GID pProcGid;					/* 4 (8) Bytes */
} PROC_GROUP_LIST, *PPROC_GROUP_LIST;
//------------------------------------------------------------------------


typedef struct _as_proc_info
{
	pid_t 				proc_id;		//8Bytes
	pid_t 				pproc_id;		//8Bytes
	INT32 				nType;			//4Bytes
	DWORD 				dwOffset;		//4Bytes
	AS_PROC_ROLE 		stProcRole;
	CHAR 				szSid[AS_SID_MAX];		//64Bytes
	CHAR 				acProcName[AS_FILE_NAME_MAX+1];	//64Bytes
	CHAR 				acPathName[AS_PROC_PATH_MAX];	//256Bytes
	AS_SESS_INFO 		stSessInfo;
	PROC_GROUP_LIST		stGroupList;		// add nannyon	
}AS_PROC_INFO, *PAS_PROC_INFO;
//------------------------------------------------------------------------

typedef struct _as_conf_mode
{
	BYTE 		bFileAclMode;
	BYTE 		bRegAclMode;
	BYTE 		bProcAclMode;
	BYTE		bUsbAclMode;
	BYTE		bBlackListMode;
	BYTE		bSubjectAclMode;
	BYTE	 	abReservd[2];
} AS_CONF_MODE, *PAS_CONF_MODE;
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define MAX_SCHED_CNT 				5
#define ACL_WILDCARD_LEN			23
#define AS_GROUP_RULE_NAME_MAX 		31

/* wildcard 사용 문자열 정보 */
typedef struct _wildstr_entry
{
	INT32		nWildType;       /* WILDTYPE_xxx */
	INT32		nWildLen;        /* wildcard 사용 문자열 길이 */
	CHAR		szWildStr[ACL_WILDCARD_LEN+1];  /* wildcard 사용 문자열 (wildcard 제외) */
} WILDSTR_ENTRY, *PWILDSTR_ENTRY;

typedef struct _rule_entry 
{
	INT32			nCMask;					/* mask for used condition */
	INT32			nEnable;				/* enabled(ACL_ENABLE) or disabled(ACL_DISABLE) */
	INT32			nPerm;					/* access permission */
	INT32			nLogOpt;				/* logging option. bit-OR field. (LOGOPT_xxx) 4 */		
	INT32			nAllowLogOps;			/* allow log operation. */
	INT32			nDenyLogOps;			/* deny log operation. */	
	INT32			nLastCompCheck		;	/* flag for checking last component.4*/
	INT32			nProcessLen;			/* process */	
	DDWORD			ddwGid;					/* proc gid */
	CHAR			szSid[AS_SID_MAX];			/* User Sid 128Bytes */	
	CHAR			szGroupSid[AS_SID_MAX];			/* Group Sid 128Bytes */	
	CHAR			szProcessPath[AS_PROC_PATH_MAX];		/* program name 256 Bytes */
	WILDSTR_ENTRY 	stProcWild;				/* proc wildcard 사용 문자열 정보 */
} RULE_ENTRY, *PRULE_ENTRY;

typedef struct _object_entry 
{
	CHAR		szFullPath[AS_PATH_NAME_MAX + 1]; //512
	INT32		nNetworkFlag;		/* Network File Access Control Rule Flag */
	INT32		nLastCompCheck;		/* flag for checking last component.*/
	INT32		nSelfOpt;			/* self exception option */
	INT32		nSelfDepth;			/* self exception depth */
} OBJECT_ENTRY, *POBJECT_ENTRY;


typedef struct _subject_entry 
{
	CHAR		szSubjectName[AS_SUBJ_NAME_MAX+1];			// name of subject
	INT32		nEnable;									// rule enabled
	INT32		nLogOpt;									// logging option. bit-OR field
	INT32		nAllowLogOps;								// allow log operation
	INT32		nDenyLogOps;								// deny log operation
} SUBJECT_ENTRY, *PSUBJECT_ENTRY;

typedef struct _sched_entry
{
	INT32		s_hour;    /* hours since midnight - [0,24] */
	INT32		e_hour;    /* hours since midnight - [0,24] */
	INT32		s_mday;    /* day of the month - [1,31] */
	INT32		e_mday;    /* day of the month - [1,31] */
	INT32		s_mon;     /* months since January - [0,11] */
	INT32		e_mon;     /* months since January - [0,11] */
	INT32		s_wday;    /* days since Sunday - [0,6] */
	INT32		e_wday;    /* days since Sunday - [0,6] */
	INT32		nOperMode; /* Module Operation Mode - [0,2]  */
	INT32		nReserved; /* Module Operation Mode - [0,2]  */
}SCHED_ENTRY, *PSCHED_ENTRY;

typedef struct _object_acl_policy_hdr 
{
	ULONG 		dwIndex;		/* ACL Rule Index */
	ULONG 		dwType;		/* ACL Rule Type */
	CHAR		szFullPath[AS_PATH_NAME_MAX +1];  /* fullpath of object 1024 */
	INT32		nRuleCount;		/* rule count of object. Always, nRuleCount >= 1 */
	INT32		nEnable;		/* rule enabled(ACL_ENABLE) or disabled(ACL_DISABLE) */
	INT32		nNetworkFlag;	/* Network File Access Control Rule Flag */
	INT32		nPolGrpID;		/* policy group id. if not member of policy group, policy group id = 0. */
	INT32		nLogOpt;		/* logging option. bit-OR field. (LOGOPT_xxx) */
	INT32		nAllowLogOps;	/* allow log operation. */
	INT32		nDenyLogOps;	/* deny log operation. */
	INT32		nReserved;		/* reserved */
	INT32		nSeverity;		/* severity level. (SEVE_xxx) */
	INT32		nResponse;		/* response option. bit-OR field. (RESP_xxx)*/
	INT32		nLastCompCheck;	/* flag for checking last component. (1 : check, 0 : not check) */
	INT32		nSchedCount;	/* schedule count of object.  */
	INT32		nSelfOpt;			/* self exception option */
	INT32		nSelfDepth;			/* self exception depth */
	SCHED_ENTRY stSchedInfo[MAX_SCHED_CNT];	/* schedule info of object.160 */
} OBJECT_ACL_POLICY_HDR, *POBJECT_ACL_POLICY_HDR;


typedef struct _group_object_policy_hdr 
{
	INT32		nGroupID;							/* group id 4 */
	ULONG 		dwType;								/* ACL Rule Type */
	CHAR		szGroupName[AS_GROUP_RULE_NAME_MAX + 1];  /* group name 32 */
	INT32		nRuleCount;							/* rule count of FileGroup 4*/
	INT32		nObjectMemberCnt;					/* member count of FileGroup 4 */
	INT32		nEnable;							/* rule enabled(ACL_ENABLE) or disabled(ACL_DISABLE) 4 */
	INT32		nPolGrpID;							/* policy group id. 4*/
	INT32		nLogOpt;							/* logging option. bit-OR field. (LOGOPT_xxx) 4 */
	INT32		nAllowLogOps;						/* allow log operation. */
	INT32		nDenyLogOps;						/* deny log operation. */
	INT32		nReserved;							/* reserved */
	INT32		nSeverity;							/* severity level. (SEVE_xxx) 4 */
	INT32		nResponse;							/* response option. bit-OR field. (RESP_xxx) 4 */
	INT32		nLastCompCheck;						/* flag for checking last component.4*/
	INT32		nSchedCount;						/* schedule count of object.4 */
	SCHED_ENTRY stSchedInfo[MAX_SCHED_CNT];			/* schedule info of object. 160 */
} GROUP_OBJECT_POLICY_HDR, *PGROUP_OBJECT_POLICY_HDR;


typedef struct _group_subject_policy_hdr
{
	CHAR		szGroupName[AS_GROUP_RULE_NAME_MAX + 1];	//group name
	INT32		nGroupID;									// group id
	INT32		nSubjectMask;								// subject mask
	INT32		nEnable;									// rule enabled
	INT32		nLogOpt;									// logging option. bit-OR field
	INT32		nAllowLogOps;								// allow log operation
	INT32		nDenyLogOps;								// deny log operation
	INT32		nPolGrpID;									// pol group id
	INT32		nSeverity;									// severity level
	INT32		nResponse;									// response option
	INT32		nLastCompCheck;								// flag for check last component
	INT32		nRuleCount;									// rule count
	INT32		nSubjectMemberCnt;							// subject member count
	INT32		nObjectMemberCnt;							// object member count
	INT32		nSchedCount;								// schedule count
	SCHED_ENTRY stSchedInfo[MAX_SCHED_CNT];		// schedule list
} GROUP_SUBJECT_POLICY_HDR, *PGROUP_SUBJECT_POLICY_HDR;
//------------------------------------------------------------------------

typedef struct _log_rule_info
{
	DWORD dwType;						//4Bytes
	DWORD dwIndex;						//4Bytes
	int	nLogMode;						//4Bytes
	int nSeverity;						//4Bytes
	int nEnable;						//4Bytes
	DWORD dwLogValue;					//4Bytes
}LOG_RULE_INFO, *PLOG_RULE_INFO;
//------------------------------------------------------------------------


typedef struct _log_subject_info
{
	pid_t pid;							//8Bytes
	int nType;							//4Bytes
	int nProcRole;						//4Bytes
	char szSid[AS_SID_MAX];		//64Bytes
	char szProcName[AS_FILE_NAME_MAX+1];		//64Bytes
	char szProcPath[AS_PROC_PATH_MAX];		//256Bytes
} LOG_SUBJECT_INFO, *PLOG_SUBJECT_INFO;
//------------------------------------------------------------------------


typedef struct _log_object_info
{
	char szFileName[AS_FILE_NAME_MAX+1];		//128Bytes
	char szFilePath[AS_PATH_NAME_MAX+1];		//512Bytes
	WCHAR wcFilePath[AS_PATH_NAME_MAX+1];		//1024Bytes
} LOG_OBJECT_INFO, *PLOG_OBJECT_INFO;
//------------------------------------------------------------------------

typedef struct _log_entry
{	
	ULONG dwLogType;
	ULONG dwDetectMode;
	LOG_RULE_INFO 		RuleInfo;
	LOG_SUBJECT_INFO 	SubjInfo;
	LOG_OBJECT_INFO 	ObjInfo;
	LOG_OBJECT_INFO 	ObjInfoNew;
}LOG_ENTRY, *PLOG_ENTRY;


#define LOGBUFSIZE ((ULONG)(sizeof(LOG_ENTRY)*32))
//------------------------------------------------------------------------


typedef struct _proc_gid_info_hdr
{
	char acPathName[AS_PROC_PATH_MAX];	
	int	nPathLen;
	int nProcGidCount;
} PROC_GID_INFO_HDR, *PPROC_GID_INFO_HDR;
//------------------------------------------------------------------------


#endif
