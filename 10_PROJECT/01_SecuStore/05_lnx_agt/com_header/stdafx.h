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

#ifndef _STDAFX_H__

#define _STDAFX_H__


#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <elf.h>
#include <errno.h>
#include <time.h>
#include <libgen.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <dlfcn.h>
#include <iostream> 
#include <pthread.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <semaphore.h>
#include <pwd.h>
#include <utime.h>
#include <iconv.h>
#include <mntent.h>
#include <uuid/uuid.h>
#include <sys/sysinfo.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/timeb.h>
#include <sys/resource.h>
#include <sys/vfs.h>
#include <sys/inotify.h>
#include <sys/fanotify.h>
#include <sys/wait.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <linux/limits.h>

using namespace std;

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

//1Byte
typedef char CHAR, CCHAR, *PCHAR;
typedef char *LPSTR, *LPTSTR;
typedef signed char INT8, *PINT8;
//typedef bool BOOL, BOOLEAN;
typedef const char *LPCSTR, *LPCTSTR;

typedef unsigned char BYTE, *LPBYTE, *PBYTE;
typedef unsigned char UINT8, *PUINT8;


//2Bytes
typedef signed short INT16, *PINT16;
typedef unsigned short WORD;
typedef unsigned short UINT16, *PUINT16;
typedef wchar_t *PWSTR, *PWCHAR;


//4Bytes
typedef int SOCKET;
typedef int INT;
typedef signed int INT32, *PINT32;
typedef signed int LONG32, *PLONG32;

typedef unsigned int WPARAM;
typedef unsigned int DWORD, *PDWORD;
typedef unsigned int UINT, *PUINT;
typedef unsigned int UINT32, *PUINT32;
typedef unsigned int ULONG32, *PULONG32, *PULONG;


//4~8Bytes
//typedef long LPARAM;
typedef uint64_t LPARAM;
typedef long LONG;
typedef unsigned long ULONG;
typedef LONG *PLONG;

typedef void *HANDLE;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef LPSTR HKEY;
typedef void VOID, *PVOID, *LPVOID;
typedef const PVOID LPCVOID;
typedef void *HWND;

//8Bytes
typedef int64_t INT64, *PINT64;
typedef int64_t LONGLONG;
typedef double DOUBLE;

typedef uint64_t UINT64, *PUINT64;
typedef uint64_t ULONGLONG;

#if defined(_AS_X86_64)
	typedef uint64_t ULONG_PTR, *PULONG_PTR;
#else
	typedef unsigned int ULONG_PTR, *PULONG_PTR;
#endif


typedef string String;
typedef wstring StringW;

#define CHARSET_EUCKR	"euc-kr//TRANSLIT//IGNORE"
#define CHARSET_UTF8	"utf-8//TRANSLIT//IGNORE"

#define	OUT
#define	IN

#define	_stricmp	strcasecmp
#define _strnicmp	strncasecmp
#define	FALSE	0
#define TRUE	1

#define	REG_FILE		0
#define DIR_FILE		1
#define FIFO_FILE		2
#define SOCK_FILE		3

#define	TCHAR_MAX_SIZE	1024
#define	CHAR_MAX_SIZE	1024
#define MAX_TIME_STR	64
#define MAX_FILE_NAME	64
#define MAX_USER_NAME	64
#define MAX_PASSWD		256
#define MAX_PROC_PATH	256
#define	MAX_PATH		260
#define MAX_LOGMSG 		1023
#define MAX_BUFF		1024
#define MAX_HBUFF		512
#define MAX_QBUFF		256
#define MAX_QHBUFF		128
#define MAX_QQBUFF		64
#define MAX_TYPE_LEN	32
#define MAX_NETADDR_LEN	128
#define	HASH_PADDING_LEN	8
#define	SHA512_BLOCK_SIZE	128


#define AS_FILE_READ_MAX	4096


#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE)((LONG*)(-1)))
#endif

#define WM_USER 	0x0400
#define WM_CLOSE	0x0010
#define WM_QUIT		0x0012

#define NANNY_AGENT_VERSION		"2.0.4.8"
#define NANNY_AGENT_IDENT		"nannysvc"
#define PWEVT_MON_VERSION		"2.0.4.1"
#define PWEVT_MON_IDENT			"pwevt_mon"

#define FANOTIFYD_VERSION		"2.0.4.1"
#define FANOTIFYD_IDENT			"fanotifyd"

#define ACCNOTIFYD_VERSION		"2.0.4.1"
#define ACCNOTIFYD_IDENT		"accnotifyd"

#define NANNY_UPDATE_IDENT		"update"
#define NANNY_AGENT_DIR			"nanny"
#define NANNY_UTIL_DIR			".tools"

#define NANNY_INSTALL_DIR		"/usr/local/ashin"
#define NANNY_INSTALL_DIR_LEN	16


#define REQ_WHITE_PATH		"/bin/reqwhite"


#define UNIX_SOCK_FILE		"white_check_socket"
#define UNIX_SOCK_ACC_FILE  "access_check_socket"
#define UNIX_SOCK_POL_FILE  "access_policy_socket"

#define ASHIN_STRING_CLS_NAME "ASSTRING_938EF200_F60B_452A"
#define ASHIN_STRING_CLS_NAME_W L"ASSTRING_938EF200_F60B_452A"

#define	safe_free(x)   if(x != NULL) { free(x);x=NULL; }
#define	ZeroMemory(x,y) memset((x), 0, (y))
#define	Sleep(x)	usleep((x)*1000)
#define	TOLOWER(Char) (((Char) >= 'A') && ((Char) <= 'Z') ? ((Char) - 'A' + 'a') : (Char))
#define TOUPPER(Char) (((Char) >= 'a') && ((Char) <= 'z') ? ((Char) - 'a' + 'A') : (Char))

#define INSTALL_REG_PATH		"/etc/.ashini"

#define HKEY_CLASSES_ROOT		"root"
#define HKEY_CURRENT_USER		"user"
#define HKEY_LOCAL_MACHINE		"machine"

#define ERROR_SUCCESS		0L
#define STATUS_WAIT_0       ((DWORD)0x00000000L)    
#define WAIT_TIMEOUT		258L
#define WAIT_FAILED			((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0       STATUS_WAIT_0


#define DRIVE_UNKNOWN     0
#define DRIVE_NO_ROOT_DIR 1
#define DRIVE_REMOVABLE   2
#define DRIVE_FIXED       3
#define DRIVE_REMOTE      4
#define DRIVE_CDROM       5
#define DRIVE_RAMDISK     6
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((LONG)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#endif
#define MAKELONGLONG(a, b)      ((LONGLONG)(((LONG)(((LONG)(a)) & 0xffffffff)) | ((LONGLONG)((LONG)(((LONG)(b)) & 0xffffffff))) << 32))

#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))

typedef struct tagMSG {
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
} MSG, *PMSG, *LPMSG;

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct {
	unsigned int gp_offset;
	unsigned int fp_offset;
	void *overflow_arg_area;
	void *reg_save_area;
} va_list_tag_ex;

typedef struct _asi_file_hash
{
	char acFullPath[MAX_PATH];
	char acWhiteHash[SHA512_BLOCK_SIZE+HASH_PADDING_LEN];
}ASI_FILE_HASH, *PASI_FILE_HASH;

typedef list<ASI_FILE_HASH>				TListFileHashInfo;
typedef TListFileHashInfo::iterator		TListFileHashInfoItor;

#endif /*_STDAFX_H__*/

