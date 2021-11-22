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

#ifndef _AS_ZIP_H__
#define _AS_ZIP_H__

#define AS_ZIP_WBUF_SIZE		16384

#define AS_ZIP_UNZIP_PASS		"ashin#nanny*udt"

extern int as_unzip_file(char *zip_file, char *unzip_path, char *pass);
extern int as_zip_file(char *zip_file, char *zip_path, char *pass);

#endif /* _AS_ZIP_H__ */


