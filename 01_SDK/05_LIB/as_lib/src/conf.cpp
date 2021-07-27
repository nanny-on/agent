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

#include "stdafx.h"

#include "as_parse.h"
#include "as_util.h"
#include "as_conf.h"


char *get_conf_r(const char *path, const char *name, const char *delim, char *buf, int buf_len)
{
	FILE	*fp;
	char	conf_val[256] = { 0,};
	char	*tok, *ptr, *last;

	if ( (fp = fopen(path, "r")) == NULL) {
		return buf;
	}

	memset(buf, 0x00, buf_len);

	while (fgets(conf_val, sizeof(conf_val), fp)) {

		trim_line_feed(conf_val);
		ptr = skip_space(conf_val);

		if (ptr[0] == '#') {
			memset(conf_val, 0x00, sizeof(conf_val));
			continue;
		}

		tok = strtok_r(ptr, delim, &last);
		if (tok == NULL) {
			memset(conf_val, 0x00, sizeof(conf_val));
			continue;
		} else {
			trim_space(tok);
		}

		if (strcmp(tok, name) != 0) {
			memset(conf_val, 0x00, sizeof(conf_val));
			continue;
		}

		tok = strtok_r(NULL, "", &last);
		if (tok == NULL) {
			memset(conf_val, 0x00, sizeof(conf_val));
			continue;
		}

		tok = skip_space(tok);
		trim_right_white_space(tok);

		/* 설정 값 얻기 */
		snprintf(buf, buf_len, "%s", tok);
		break;
	}

	fclose(fp);
	return buf;
}


char *get_conf(const char *path, const char *name, const char *delim)
{
	static char buf[MAX_BUFF] = { 0,};

	memset(buf, 0x00, sizeof(buf));
	return get_conf_r(path, name, delim, buf, sizeof(buf));
}


