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

#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif
#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _FILE_OFFSET_BIT
#define _FILE_OFFSET_BIT 64
#endif
#endif

#include "stdafx.h"
#include "as_file.h"
#include "zlib.h"
#include "mz_zip.h"
#include "mz_unzip.h"
#include "as_parse.h"
#include "as_zip.h"

static int do_extract_file(unzFile pUnxFile, char *pExtPath, const char *pPass);
static int do_zip_addfile(zipFile zf, char *pFilePath, char *pFileNameInZip, char *pPassword) ;

static int is_large_file(char* pFilePath)
{
	ZPOS64_T pos = 0;
	FILE *fp = NULL;
	int nLargeFile = 0;
	if(pFilePath == NULL)
		return 0;

	if ( (fp = fopen64(pFilePath, "rb")) != NULL)
	{
		fseek(fp, 0, SEEK_END);

		pos = ftell(fp);
		if (pos >= 0xffffffff)
		{
			nLargeFile = 1;
		}

		fclose(fp);
	}

	return nLargeFile;
}

int as_unzip_file(char *zip_file, char *unzip_path, char *pass)
{
	int nErr = UNZ_OK;
	ZPOS64_T i = 0;
	unzFile pUf = NULL;	
	unz_global_info64 gi;

	if(zip_file == NULL || unzip_path == NULL)
		return -1;

	memset(&gi, 0, sizeof(gi));

	if ( (pUf = unzOpen64(zip_file)) == NULL)
	{
		return -2;
	}

	nErr = unzGetGlobalInfo64(pUf, &gi);
	if (nErr != UNZ_OK)
	{
		unzClose(pUf);
		return -3;
	}

	for (i = 0; i < gi.number_entry; i++)
	{
		nErr = do_extract_file(pUf, unzip_path, pass);
		if (nErr != UNZ_OK)
		{
			nErr -= 1000;
			break;
		}

		if ((i+1) < gi.number_entry)
		{
			nErr = unzGoToNextFile(pUf);
			if (nErr != UNZ_OK)
			{
				nErr -= 2000;
				break;
			}
		}
	}

	unzClose(pUf);

	if (nErr != UNZ_OK)
	{
		return nErr;
	}
	return 0;
}

int as_zip_file(char *zip_file, char *zip_path, char *pass)
{
	int nErr = UNZ_OK;
	zipFile zf = NULL;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	char acFileName[MAX_PATH] = {0,};
	char acPath[MAX_PATH] = {0,};
	char szExt[MAX_FILE_NAME] = {0,};

	if(zip_file == NULL || zip_path == NULL)
		return -1;

	if(DirectoryExists(zip_path) == FALSE)
	{

		return -2;
	}

	zf = zipOpen64(zip_file, 0);
	if (zf == NULL)
	{
		return -3;
	}

	dp = opendir(zip_path);
	if (dp == NULL)
	{
		zipClose(zf, NULL);
		return -4;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}
		if(DT_DIR == dirp->d_type)
		{
			continue;
		}
		snprintf(acFileName, MAX_PATH-1, "%s", dirp->d_name);
		if(get_extname(acFileName, szExt, MAX_FILE_NAME) == NULL)
		{
			continue;
		}
		if(_stricmp(szExt, ".sh") && _stricmp(szExt, ".tar") && _stricmp(szExt, ".nfo"))
		{
			continue;
		}
		snprintf(acPath, MAX_PATH-1, "%s/%s", zip_path, acFileName);
		printf(" zip %s\n", acFileName);
		nErr = do_zip_addfile(zf, acPath, acFileName, pass);
		if(nErr != 0)
		{
			nErr -= 1000;
			break;
		}
	}
	closedir(dp);
	zipClose(zf, NULL);
	return nErr;
}



static int do_extract_file(unzFile uf, char *ext_path, const char *pass)
{
	FILE             *fout = NULL;
	int              err = UNZ_OK;
	int              buf_size = AS_ZIP_WBUF_SIZE;
	void             *pBuff = NULL;
	char			 szExt[MAX_TYPE_LEN] = {0, };
	char             cur_dname[CHAR_MAX_SIZE] = { 0,};
	char             create_dname[CHAR_MAX_SIZE] = { 0,};
	char             fname_in_zip[CHAR_MAX_SIZE] = { 0,};
	char             write_fname[CHAR_MAX_SIZE] = { 0,};
	char             *pt, *fname_without_path;
	unz_file_info64  file_info;
 
	pBuff = (void *)malloc(AS_ZIP_WBUF_SIZE);
	if(pBuff == NULL)
	{
		return -1;
	}
	memset(pBuff, 0, AS_ZIP_WBUF_SIZE);

	err = unzGetCurrentFileInfo64(uf, &file_info, fname_in_zip, sizeof(fname_in_zip), NULL, 0, NULL, 0);
	if (err != UNZ_OK)
	{
		safe_free(pBuff);
		unzCloseCurrentFile(uf);
		return -2;
	}

	pt = fname_without_path = fname_in_zip;
	while (*pt != '\0')
	{
		if (*pt == '/')
		{
			fname_without_path = pt + 1;
		}
		pt++;
	}

	if (*fname_without_path == '\0')
	{
		if (ext_path[0] != '/')
		{
			getcwd(cur_dname, CHAR_MAX_SIZE-1);
			snprintf(create_dname, CHAR_MAX_SIZE-1, "%s/%s/%s", cur_dname, ext_path, fname_in_zip);
		}
		else
		{
			snprintf(create_dname, CHAR_MAX_SIZE-1, "%s/%s", ext_path, fname_in_zip);
		}

		if(DirectoryExists(create_dname) == FALSE)
		{
			if (CreateDirectory(create_dname, NULL) == FALSE)
			{
				safe_free(pBuff);
				unzCloseCurrentFile(uf);
				return -3;
			}
		}
	}
	else
	{
		if (ext_path[0] != '/')
		{
			getcwd(cur_dname, CHAR_MAX_SIZE-1);
			snprintf(create_dname, CHAR_MAX_SIZE-1, "%s/%s", cur_dname, ext_path);
		}
		else
		{
			snprintf(create_dname, CHAR_MAX_SIZE-1, "%s", ext_path);
		}

		if(DirectoryExists(create_dname) == FALSE)
		{
			if (CreateDirectory(create_dname, NULL) == FALSE)
			{
				safe_free(pBuff);
				unzCloseCurrentFile(uf);
				return -5;
			}
		}

		snprintf(write_fname, CHAR_MAX_SIZE-1, "%s/%s", create_dname, fname_in_zip);
#ifndef _STRING_CASE_SENSITIVE_ // 윈도우 파일 대소문자 구분 안하도록 일괄 소문자 변환.
		{
			int nLen = strlen(write_fname);
			for(int i = nLen-1; i>=0; i--)
			{
				if(write_fname[i] == '\\' || write_fname[i] == '/') break;
				if(write_fname[i] <= 'Z' && write_fname[i] >= 'A')
					write_fname[i] = write_fname[i] + (char)('a'-'A');
			}
		}
#endif
		err = unzOpenCurrentFilePassword(uf,pass); 
		if (err!=UNZ_OK) 
		{ 
			unzCloseCurrentFile(uf);
			safe_free(pBuff);
			return -7;
		} 
		if ( (fout = fopen64(write_fname, "wb")) == NULL)
		{
			unzCloseCurrentFile(uf);
			safe_free(pBuff);
			return -8;
		}

		do {
			err = unzReadCurrentFile(uf, pBuff, buf_size);
			if (err < 0)
			{
				err -= 100;
				break;
			}
			else if (err == 0)
			{
				break;
			}
			if (fwrite((char *)pBuff, err, 1, fout) != 1)
			{
				err = -10;
				break;
			}
		} while (err > 0);

		fclose(fout);

		if(get_extname((char *)write_fname, szExt, MAX_TYPE_LEN) != NULL)
		{
			if(!_stricmp(szExt, ".sh"))
				chmod(write_fname, 0755);
		}
	}

	unzCloseCurrentFile(uf);
	safe_free(pBuff);

	return err;
}


static int do_zip_addfile(zipFile zf, char *pFilePath, char *pFileNameInZip, char *pPassword) 
{ 
	zip_fileinfo zi = { 0 }; 
	FILE *fin = NULL; 
	int nReadSize = 0; 
	int nLargeFIle = 0; 
	int err = ZIP_OK; 
	void *pBuff = NULL;

	if(zf == NULL || pFilePath == NULL || pFileNameInZip == NULL)
		return -1;

	nLargeFIle = is_large_file(pFilePath); 

	/* Add to zip file */ 
	err = zipOpenNewFileInZip3_64(zf, pFileNameInZip, &zi, 
		NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0, 
		-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, 
		pPassword, 0, nLargeFIle); 

	if (err != ZIP_OK) 
	{ 
		zipCloseFileInZip(zf);
		return -2;
	} 

	fin = fopen64(pFilePath, "rb"); 
	if (fin == NULL) 
	{ 
		zipCloseFileInZip(zf);
		return -2;
	}

	pBuff = (void *)malloc(AS_ZIP_WBUF_SIZE);
	if(pBuff == NULL)
	{
		zipCloseFileInZip(zf);
		fclose(fin); 
		return -3;
	}
	memset(pBuff, 0, AS_ZIP_WBUF_SIZE);


	/* Read contents of file and write it to zip */ 
	do 
	{ 
		nReadSize = (int)fread(pBuff, 1, AS_ZIP_WBUF_SIZE, fin); 
		if ((nReadSize < AS_ZIP_WBUF_SIZE) && (feof(fin) == 0)) 
		{ 
			err = ZIP_ERRNO; 
			break;
		} 

		if (nReadSize > 0) 
		{ 
			err = zipWriteInFileInZip(zf, pBuff, nReadSize); 
			if (err < 0) 
				break;
		} 
	} while ((err == ZIP_OK) && (nReadSize > 0)); 

	fclose(fin); 
	safe_free(pBuff);
	zipCloseFileInZip(zf);

	if (err < 0) 
	{ 
		return -4; 
	} 

	return 0; 
}