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

#ifndef _AS_PARSE_H_
#define _AS_PARSE_H_

extern char *skip_space(char *str);
extern void strip_space(char *str);
extern void trim_space(char *str);
extern void trim_left_white_space(char *str);
extern void trim_right_white_space(char *str);
extern void trim_white_space(char *str);
extern void trim_line_feed(char *str);
extern int is_valid_num(char *str);
extern int is_valid_float_num(char *str);
extern char *str_ntoken(char *str, const char *delim, char **nstr);
extern char *copy_token(char *dest, char *src, int ch);
extern void remove_tail_crlf(char *szString);
extern char *get_basename(char *pSrc, char *pDest, int nSize);
extern char *get_dirname(char *src, char *dest, int size);
extern BOOL split_filename(char *pPath, char *pFile, int size);
extern char *get_extname(char *pFileName, char *pExtName, int nExtSize);
extern char *remove_extname(char *pSrcName, char *pDestName, int nDestSize);
extern void remove_tail_crlf(char *szString);



#endif /* _AS_PARSE_H_ */


