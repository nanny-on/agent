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

#ifndef _AS_UTIL_H_
#define _AS_UTIL_H_

extern pid_t gettid(void);
extern int copy_file(const char *src_file, const char *dst_file);
extern int make_dir(const char *path);
extern DWORD GetTickCount();
extern void OutputDebugString(LPCSTR pString);
extern DWORD GetLastError();


#endif /* _AS_UTIL_H_ */


