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
 
#ifndef ASI_LNX_DEFINE_W32_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define ASI_LNX_DEFINE_W32_H_938EF200_F60B_452A_BD30_A10E8507EDCC
 
#include "asi_lnx_base_hdr.h"

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
		
#define	OUT
#define	IN

#define	_stricmp	strcasecmp
#define _strnicmp	strncasecmp
#define	FALSE	0
#define TRUE	1

#define	REG_FILE		0
#define DIR_FILE		1


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

#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define INVALID_HANDLE_VALUE ((HANDLE)((LONG*)(-1)))

#define WM_USER 	0x0400
#define WM_CLOSE	0x0010
#define WM_QUIT		0x0012

#define NANNY_AGENT_VERSION		"1.0.0.1"
#define NANNY_AGENT_IDENT		"nannysvc"
#define NANNY_AGENT_DIR			"nanny"


#define	safe_free(x)   if(x != NULL) { free(x);x=NULL; }
#define	ZeroMemory(x,y) memset((x), 0, (y))
#define	Sleep(x)	usleep((x)*1000)
#define	TOLOWER(Char) (((Char) >= 'A') && ((Char) <= 'Z') ? ((Char) - 'A' + 'a') : (Char))
#define TOUPPER(Char) (((Char) >= 'a') && ((Char) <= 'z') ? ((Char) - 'a' + 'A') : (Char))

#define INSTALL_REG_PATH		"/etc/.ashini"

#define HKEY_CLASSES_ROOT		"root"
#define HKEY_CURRENT_USER		"user"
#define HKEY_LOCAL_MACHINE		"machine"
#define HKEY_USERS				"users"

#define ERROR_SUCCESS			0L
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
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))


//Registry 
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR_NO_MORE_ITEMS      259
#define ERROR_MORE_DATA			 234

#define KEY_READ	1
#define	KEY_WRITE	2
#define	KEY_ALL_ACCESS	3

#define REG_NONE                    ( 0 )   // No value type
#define REG_SZ                      ( 1 )   // Unicode nul terminated string
#define REG_EXPAND_SZ               ( 2 )   // Unicode nul terminated string with environment variable references
#define REG_BINARY                  ( 3 )   // Free form binary
#define REG_DWORD                   ( 4 )   // 32-bit number
#define REG_MULTI_SZ                ( 7 )   // Multiple Unicode strings
#define REG_QWORD                   ( 11 )  // 64-bit number
//////////////////////////////////////////////////////////////////////////////////////////////////////

//HANDLE
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define HANDLE_TYPE_FILE	0

//File
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

//
//  These are the generic rights.
//

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5
//////////////////////////////////////////////////////////////////////////////////////////////////////


//Erro
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR_SHARING_VIOLATION          32L
#define ERROR_HANDLE_EOF                 38L
//////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //ASI_LNX_DEFINE_W32_H_938EF200_F60B_452A_BD30_A10E8507EDCC