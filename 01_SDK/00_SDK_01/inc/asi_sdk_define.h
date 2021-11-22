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


#ifndef ASI_SDK_DEFINE_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define ASI_SDK_DEFINE_H_938EF200_F60B_452A_BD30_A10E8507EDCC

#define	CHAR_MAX_SIZE			1024
#define ZeroMemoryExt(a)		(ZeroMemory(a, CHAR_MAX_SIZE))

#define IS_SAFE(a) ((a) ? (a->GetSafeHwnd()) ? TRUE : FALSE : FALSE)
#define IS_UNSAFE(a) ((a) ? (a->GetSafeHwnd()) ? FALSE : TRUE : TRUE)

#define SAFE_FREE(a) if(a) { free(a); a = NULL; }
#define SAFE_DESTROY(a) if (IS_SAFE(a)) { a->DestroyWindow(); } { delete a; a = NULL; }
#define SAFE_DELETE(a)			{	if(a){	delete a;	a = NULL;	}} 
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_DELETE_N_RETURN(a) SAFE_DELETE(a) return;
#define SAFE_KILLTIMER(a) if (a) { this->KillTimer(a); a = NULL; }
#define SAFE_CLOHANDLE(a) if (a) { fclose((FILE*)a); a = NULL; }

#define CP_MAX_PATH			(MAX_PATH - 1)
#define CP_CHAR_MAX_SIZE	(CHAR_MAX_SIZE - 1)

#define MSG_RY				1
#define MSG_RN				0

#define MAKEUINT64(a, b) 	((UINT64) (((UINT32) (a)) | ((UINT64) ((UINT32) (b))) << 32)) 

#define HIUINT64(l)    		((UINT32) (((UINT64) (l) >> 32) & 0xFFFFFFFF))
#define LOUINT64(I)   	 	((UINT32)(I))

#define MAKEUINT32(a, b) 	((UINT32) (((UINT16) (a)) | ((UINT32) ((UINT16) (b))) << 16)) 

#ifndef HIUINT32
#define HIUINT32(l)    		((UINT16) (((UINT32) (l) >> 16) & 0xFFFFFFFF))
#endif

#ifndef LOUINT32
#define LOUINT32(I)   	 	((UINT16)(I))
#endif

#define MAKEUINT8(l, h) 	((UINT8) (((UINT8) (l)) | ((UINT8) ((h))) << 4)) 
#define HIUINT8(n)    		((UINT8) (((UINT8) (n) >> 4) & 0xFF))
#define LOUINT8(n)   	 	((UINT8)((n) << 4) >> 4)

#define MAKETYPEID32(l, h)	((UINT32) (((UINT32) (l << 24)) | ((UINT32)(h))))
#define LOTYPEID32(n)   	((UINT8)((n >> 24)))
#define HITYPEID32(n)    	((UINT32) (((UINT32) (n)) & 0x00FFFFFF))

#define MAKETYPEID64(l, h)	((UINT64) (((UINT64) (l << 56)) | ((UINT64)(h))))
#define LOTYPEID64(n)   	((UINT8)((n >> 56)))
#define HITYPEID64(n)    	((UINT64) (((UINT64) (n)) & 0x00FFFFFFFFFFFFFF))

#define HISYNCSTEP(a)		((UINT16)(((UINT32) (a) >> 16) & 0xFFFF))
#define LOSYNCSTEP(b)		((UINT16)(b))
#define ISSYNCSTEP(n)		(HISYNCSTEP(n) == LOSYNCSTEP(n))
#define HISYNCSTEPUP(n)		(n = ((UINT32) (((UINT16) (n)) | ((UINT32)(HISYNCSTEP(n) + 1)) << 16)))
#define LOSYNCSTEPUP(n)		(n = ((UINT32) ((UINT16)(LOSYNCSTEP(n) + 1) | ((UINT32)(HISYNCSTEP(n))) << 16)))
#define HISYNCSTEPSYNC(n)	(n = ((UINT32) ((UINT16)(LOSYNCSTEP(n)) | ((UINT32)(LOSYNCSTEP(n))) << 16)))
#define LOSYNCSTEPSYNC(n)	(n = ((UINT32) ((UINT16)(HISYNCSTEP(n)) | ((UINT32)(HISYNCSTEP(n))) << 16)))

#define GET_RIGHT_POS(a)	(BYTE)(a >> 56)
#define GET_RIGHT_VALUE(a)	(UINT64)((a << 8) >> 8)

#define VOID_CB static void
#define INT32_CB static INT32

#define	ASI_KILOBYTE	1024
#define	ASI_MEGABYTE	(ASI_KILOBYTE*ASI_KILOBYTE)
#define ASI_GIGABYTE	(ASI_MEGABYTE*ASI_KILOBYTE)
#define ASI_TERABYTE	(ASI_GIGABYTE*ASI_KILOBYTE)
                                     
#endif	//ASI_SDK_DEFINE_H_938EF200_F60B_452A_BD30_A10E8507EDCC
