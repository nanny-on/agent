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

#include "stdafx.h"
#include "com_struct.h"
#include "DocFmtDefine.h"
#include "DocFileFmtUtil.h"

CDocFileFmtUtil::CDocFileFmtUtil(void)
{
	m_n_zip_Count = 0;
	m_n_pptx_Han_Count = 0;
	m_n_pptx_Office_Count = 0;
	m_n_pptx2_Office_Count = 0;	
	m_n_odt_Office_Count = 0;
	m_n_odt_Han_Count = 0;
	m_n_odp_Office_Count = 0;
	m_n_odp_Han_Count = 0;
	m_n_xlsx_Office_Count = 0;
	m_n_xlsx_Han_Count = 0;
	m_n_docx_Office_Count = 0;
	m_n_docx_Han_Count = 0;
	m_n_ods_Han_Count = 0;
	m_n_ppsx_Han_Count = 0;
	m_n_ppsx_Office_Count = 0;
	m_n_show_Han_Count = 0;
	m_n_doc_Han_Count = 0;
	m_n_doc_Office_Count = 0;
	m_n_pps_Office_Count = 0;
	m_n_ppt_Han_Count = 0;
	m_n_ppt_Office_Count = 0;
	m_n_xls_Office_Count = 0;
	m_n_xls_Han_Count = 0;
	m_n_cell_Han_Count = 0;
	m_n_nxl_Han_Count = 0;
	m_n_hwp2_Han_Count = 0;
	m_n_hwp_Han_Count = 0;
	m_n_pdf_Count = 0;
	m_n_hml_Han_Count = 0;
	m_n_hpt_Han_Count = 0;
	m_n_hwp3_Han_Count = 0;
	m_n_hwpx_Han_Count = 0;
	m_n_xlsx_unKnown_Count = 0;    
	
	m_n_nxt_Han_Count = 0;
	m_n_odb_Han_Count = 0;
	m_n_odg_Han_Count = 0;

	m_n_pptx_Known_Count = 0;
	m_n_odt_Known_Count = 0;
	m_n_odp_Known_Count = 0;
	m_n_xlsx_Known_Count = 0;
	m_n_docx_Known_Count = 0;
	m_n_ods_Known_Count = 0;
	m_n_ppsx_Known_Count = 0;
	m_n_show_Known_Count = 0;
	m_n_doc_Known_Count = 0;
	m_n_xls_Known_Count = 0;
	m_n_ppt_Known_Count = 0;
	m_n_pps_Known_Count = 0;
	m_n_cell_Known_Count = 0;
	m_n_nxl_Known_Count = 0;
	m_n_pdf_Known_Count = 0;
	m_n_hml_Known_Count = 0;
	m_n_hwp_Known_Count = 0;
	m_n_hpt_Known_Count = 0;
	m_n_nxt_Known_Count = 0;
	m_n_odb_Known_Count = 0;
	m_n_odg_Known_Count = 0;
	m_n_user_define_Count = 0;

	m_n_ods_Office_Count = 0;
	pthread_mutex_init(&m_mutex, NULL);
}
//-----------------------------------------------------------------------------------------------


CDocFileFmtUtil::~CDocFileFmtUtil(void)
{
	pthread_mutex_destroy(&m_mutex);
}

//-----------------------------------------------------------------------------------------------

INT32 CDocFileFmtUtil::ProcAnalysis(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(pADFFI == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -21;
	}

	pthread_mutex_lock (&m_mutex);
	nRetVal = SetSearchFileList(pADFFI, acLogMsg);
	pthread_mutex_unlock(&m_mutex);

	return nRetVal;
}
//-----------------------------------------------------------------------------------------------

INT32 CDocFileFmtUtil::SetSearchFileList(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg)
{
	INT32 nFmtType = 0;
	INT32 nRetVal = 0;
	if(pADFFI == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -31;
	}

	if(pADFFI->szFileName[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid file name");
		return -32;
	}
	nRetVal = CheckDocumentFile(pADFFI->szFileName, nFmtType, acLogMsg);
	if(nRetVal != 0)
	{
		return nRetVal;
	}
	if(nFmtType == ASIDFF_FILE_FMT_MORE_CHECK)
	{
		nFmtType = 0;
		nRetVal = CheckFileCmdAnalysis(pADFFI->szFileName, nFmtType, acLogMsg);
		if(acLogMsg)
		{
			snprintf(acLogMsg, MAX_LOGMSG, "more check : %s, ret:%d, type:%d", pADFFI->szFileName, nRetVal, nFmtType);
		}
		if(nRetVal != 0)
		{
			return nRetVal;
		}
	}
	pADFFI->nFmtType = nFmtType;
	return ConvertFmtTypeToName(pADFFI, acLogMsg);
}
//-----------------------------------------------------------------------------------------------

INT32	CDocFileFmtUtil::ConvertFmtTypeToName(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(pADFFI == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid file name");
		return -61;
	}
	
	switch(pADFFI->nFmtType)
	{
		case ASIDFF_FILE_FMT_TYPE_PPT:		strncpy(pADFFI->szFmtType, "ppt", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_PPTX:		strncpy(pADFFI->szFmtType, "pptx", MAX_TYPE_LEN-1);	break;
		case ASIDFF_FILE_FMT_TYPE_PPS:		strncpy(pADFFI->szFmtType, "pps", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_PPSX:		strncpy(pADFFI->szFmtType, "ppsx", MAX_TYPE_LEN-1);	break;
		case ASIDFF_FILE_FMT_TYPE_XLS:		strncpy(pADFFI->szFmtType, "xls", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_XLSX:		strncpy(pADFFI->szFmtType, "xlsx", MAX_TYPE_LEN-1);	break;
		case ASIDFF_FILE_FMT_TYPE_DOC:		strncpy(pADFFI->szFmtType, "doc", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_DOCX:		strncpy(pADFFI->szFmtType, "docx", MAX_TYPE_LEN-1);	break;

		case ASIDFF_FILE_FMT_TYPE_ZIP:		strncpy(pADFFI->szFmtType, "zip", MAX_TYPE_LEN-1);		break;

		case ASIDFF_FILE_FMT_TYPE_PDF:		strncpy(pADFFI->szFmtType, "pdf", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_HWP:		strncpy(pADFFI->szFmtType, "hwp", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_HPT:		strncpy(pADFFI->szFmtType, "hpt", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_HWPX:		strncpy(pADFFI->szFmtType, "hwpx", MAX_TYPE_LEN-1);		break;

		case ASIDFF_FILE_FMT_TYPE_HML:		strncpy(pADFFI->szFmtType, "hml", MAX_TYPE_LEN-1);		break;

		case ASIDFF_FILE_FMT_TYPE_ODT:		strncpy(pADFFI->szFmtType, "odt", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_ODP:		strncpy(pADFFI->szFmtType, "odp", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_ODS:		strncpy(pADFFI->szFmtType, "ods", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_ODB:		strncpy(pADFFI->szFmtType, "odb", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_ODG:		strncpy(pADFFI->szFmtType, "odg", MAX_TYPE_LEN-1);		break;

		case ASIDFF_FILE_FMT_TYPE_SHOW:		strncpy(pADFFI->szFmtType, "show", MAX_TYPE_LEN-1);	break;
		case ASIDFF_FILE_FMT_TYPE_CELL:		strncpy(pADFFI->szFmtType, "cell", MAX_TYPE_LEN-1);	break;
		case ASIDFF_FILE_FMT_TYPE_NXL:		strncpy(pADFFI->szFmtType, "nxl", MAX_TYPE_LEN-1);		break;
		case ASIDFF_FILE_FMT_TYPE_NXT:		strncpy(pADFFI->szFmtType, "nxt", MAX_TYPE_LEN-1);		break;
		default:							strncpy(pADFFI->szFmtType, "unknown", MAX_TYPE_LEN-1);		break;
	}
	return nRetVal;
}
//-----------------------------------------------------------------------------------------------

INT32 CDocFileFmtUtil::CheckFileExtAnalysis(char *pcFilePath, INT32 *pnFileFmtType)
{
	char *pExt = NULL;
	if(pcFilePath == NULL || pnFileFmtType == NULL)
		return -1;
	pExt = strrchr(pcFilePath, '.');
	if(pExt == NULL)
		return -2;
	if(_stricmp(pExt, ".ppt")==0)
	{
		m_n_ppt_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(_stricmp(pExt, ".pptx")==0)
	{
		m_n_pptx_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(_stricmp(pExt, ".doc")==0)
	{
		m_n_doc_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(_stricmp(pExt, ".docx")==0)
	{
		m_n_docx_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(_stricmp(pExt, ".pps")==0)
	{
		m_n_pps_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_PPS;
		return 0;
	}
	else if(_stricmp(pExt, ".ppsx")==0)
	{
		m_n_ppsx_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_PPSX;
		return 0;
	}
	else if(_stricmp(pExt, ".xls")==0)
	{
		m_n_xls_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(_stricmp(pExt, ".xlsx")==0)
	{
		m_n_xlsx_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}	
	else if(_stricmp(pExt, ".pdf")==0)
	{
		m_n_pdf_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_PDF;
		return 0;
	}	
	else if(_stricmp(pExt, ".hwp")==0)
	{
		m_n_hwp_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(_stricmp(pExt, ".hpt")==0)
	{
		m_n_hpt_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_HPT;
		return 0;
	}
	else if(_stricmp(pExt, ".hml")==0)
	{
		m_n_hml_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_HML;
		return 0;
	}
	else if(_stricmp(pExt, ".odt")==0)
	{
		m_n_odt_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_ODT;
		return 0;
	}
	else if(_stricmp(pExt, ".odp")==0)
	{
		m_n_odp_Office_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_ODP;
		return 0;
	}
	else if(_stricmp(pExt, ".ods")==0)
	{
		m_n_ods_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_ODS;
		return 0;
	}
	else if(_stricmp(pExt, ".odb")==0)
	{
		m_n_odb_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_ODB;
		return 0;
	}
	else if(_stricmp(pExt, ".odg")==0)
	{
		m_n_odg_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_ODG;
		return 0;
	}
	else if(_stricmp(pExt, ".show")==0)
	{
		m_n_show_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_SHOW;
		return 0;
	}
	else if(_stricmp(pExt, ".cell")==0)
	{
		m_n_cell_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	else if(_stricmp(pExt, ".nxl")==0)
	{
		m_n_nxl_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_NXL;
		return 0;
	}
	else if(_stricmp(pExt, ".nxt")==0)
	{
		m_n_nxt_Han_Count++;
		*pnFileFmtType = ASIDFF_FILE_FMT_TYPE_NXT;
		return 0;
	}
	return -3;
}

INT32 CDocFileFmtUtil::ParseFileFormat(char *szCmdFmt, INT32 &nFmtType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(szCmdFmt == NULL || szCmdFmt[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -211;
	}

	if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_PPTX))
	{
		m_n_pptx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_XLSX))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX; 
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_DOCX))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_CELL))
	{
		m_n_cell_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_ODP))
	{
		m_n_odp_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODP;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_ODS))
	{
		m_n_ods_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODS;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_ODB))
	{
		m_n_odb_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODB;
		return 0;
	}
	else if(!_stricmp(szCmdFmt, CMD_FILE_MATCH_FORMAT_ODG))
	{
		m_n_odg_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODG;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_PPT))
	{
		m_n_ppt_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_DOC))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_XLS))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_PDF))
	{
		m_n_pdf_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PDF;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_HWP))
	{
		m_n_hwp_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_HWPX))
	{
		m_n_hwpx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWPX;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_ZIP))
	{
		m_n_zip_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ZIP;
		return 0;
	}
	else if(strstr(szCmdFmt, CMD_FILE_IS_FORMAT_GZIP))
	{
		m_n_zip_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ZIP;
		return 0;
	}
	nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
	return 0;
}

INT32 CDocFileFmtUtil::CheckFileCmdAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 nLen = 0;
	FILE* pPipe = NULL;
	CHAR *pBuff = NULL;
	CHAR *pFormatString = NULL;
	if(pcFilePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -201;
	}

	nRetVal = is_file(CMD_FILE_FORMAT);
	if(nRetVal != 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to find file cmd");
		return -202;
	}
	do{
		pBuff = (CHAR *)malloc(CHAR_MAX_SIZE);
		if(pBuff == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -203;
			break;
		}
		memset(pBuff, 0, CHAR_MAX_SIZE);

		snprintf(pBuff, CHAR_MAX_SIZE-1, "%s \"%s\"", CMD_FILE_FORMAT, pcFilePath);

		pPipe = popen(pBuff, "r");
		if(pPipe == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to popen %s (%d)", pBuff, errno);
			nRetVal = -204;
			break;
		}
		memset(pBuff, 0, CHAR_MAX_SIZE);

		if(fgets(pBuff, CHAR_MAX_SIZE-1, pPipe) == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to get buffer (%d)",  errno);
			nRetVal = -205;
			break;
		}
		pBuff[CHAR_MAX_SIZE-1] = 0;

		nLen = strlen(pBuff);
		if(nLen < 3)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "invalid buffer length (%d)",  nLen);
			nRetVal = -206;
			break;
		}
		pBuff[nLen-1] = 0;

		pFormatString = strchr(pBuff, ':');
		if(pFormatString == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "invalid buffer data");
			nRetVal = -207;
			break;
		}
		if(*(++pFormatString) == 0x20)
		{
			pFormatString++;
		}

		nRetVal = ParseFileFormat(pFormatString, nFmtType, acLogMsg);
	}while(FALSE);

	if(pPipe != NULL)
		pclose(pPipe);
	safe_free(pBuff);

	return nRetVal;
}

INT32 CDocFileFmtUtil::CheckPkFmtAnalysis(char *pcFilePath, char *szFmt, INT32 &nFmtType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	char szPkFmt[MAX_TYPE_LEN+1] = {0,};
	if(pcFilePath == NULL || szFmt == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -101;
	}

	if(!_stricmp(szFmt, DOCUFORMAT_pptx_office_16))
	{
		m_n_pptx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_pptx_officeppt_16))
	{
		m_n_pptx2_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX; 
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odt_office_16))
	{
		m_n_odt_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODT;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odt_han_16))
	{
		m_n_odt_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODT;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odp_officeppt_16))
	{
		m_n_odp_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODP;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odp_han_16))
	{
		m_n_odp_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODP;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_han_16))
	{
		m_n_xlsx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_office_16))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_han_16))
	{
		m_n_docx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ods_han_16))
	{
		m_n_ods_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODS;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ppsx_han_16))
	{
		m_n_ppsx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16_unknown))
	{
		m_n_xlsx_unKnown_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16_unknown2))
	{
		m_n_xlsx_unKnown_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_office2007_16))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_pptx_office2007_16))
	{
		m_n_pptx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office2007_16))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx2_office2007_16))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ppsx_office2007_16))
	{
		m_n_ppsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx2_office2007_16))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odt_han2014_16))
	{
		m_n_odt_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODT;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_han2014_16))
	{
		m_n_docx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ods_han2014_16))
	{
		m_n_ods_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODS;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_cell_han2014_16))
	{
		m_n_cell_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office2013_16))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ods_office2013_16))
	{
		m_n_ods_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODS;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_odp_office2013ppt_16))
	{
		m_n_odp_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ODP;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_ppsx_office2013ppt_16))
	{
		m_n_ppsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_office2013pub_16))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16_uknwn))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16_uknwn_02))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_pptx_office_16_uknwn))
	{
		m_n_pptx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_office_16_uknwn))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_docx_office_16_365V18))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_pptx_office_16_365V18))
	{
		m_n_pptx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szFmt, DOCUFORMAT_xlsx_office_16_365V18))
	{
		m_n_xlsx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}

	nRetVal = GetPkFileFmtString(pcFilePath, szPkFmt, MAX_TYPE_LEN, acLogMsg);
	if(nRetVal != 0)
	{
		if(nRetVal == 1)
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
			nRetVal = 0;
		}
		return nRetVal;
	}
	szPkFmt[MAX_TYPE_LEN] = 0;
	
	if(!_stricmp(szPkFmt, DOCUFORMAT_pptx_han_4))
	{
		m_n_pptx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPTX;
		return 0;
	}
	else if(!_stricmp(szPkFmt, DOCUFORMAT_show_han_4))
	{
		m_n_show_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_SHOW;
		return 0;
	}
	else if(!_stricmp(szPkFmt, DOCUFORMAT_xlsx_han2014_4))
	{
		m_n_xlsx_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLSX;
		return 0;
	}
	nFmtType = ASIDFF_FILE_FMT_MORE_CHECK;
	return 0;
/*
	if(CheckFileExtAnalysis(pcFilePath, &nFileFmt) == 0)
		return nFileFmt;

	if(CheckZipFileContentAnalysis(pcFilePath, &nFileFmt, acLogMsg) == 0)
		return nFileFmt;
	m_n_doc_Office_Count++;
	return ASIDFF_FILE_FMT_TYPE_DOC;
*/
}

INT32 CDocFileFmtUtil::CheckHmlFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	FILE* fp = NULL;
	BYTE *pData = NULL; 
	INT32 nRead = 0;
	INT32 nRetVal = 0;
	char szHmlFmt[MAX_TYPE_LEN+1] = {0,};
	if(pcFilePath == NULL || pcFilePath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -201;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);	
		return -202;
	}

	do{
		pData = (BYTE *)malloc(READ_HML_FMT_BIN_MAX+1);
		if(pData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -203;
			break;
		}
		memset(pData, 0x00, READ_HML_FMT_BIN_MAX+1);	

		fseek(fp, 58L, SEEK_SET);

		nRead = (INT32)fread(pData, 1, READ_HML_FMT_BIN_MAX, fp);
		if (nRead != READ_HML_FMT_BIN_MAX)
		{
			Sleep(10);
			fseek(fp, 58L, SEEK_SET);
			nRead = (INT32)fread(pData, 1, READ_HML_FMT_BIN_MAX, fp);
			if (nRead != READ_HML_FMT_BIN_MAX)
			{
				nRetVal = 1;
				break;
			}
		}
		pData[READ_HML_FMT_BIN_MAX] = 0;
	
		snprintf(szHmlFmt, MAX_TYPE_LEN, "%02X%02X%02X%02X%02X%02X", 
			(BYTE)pData[0], (BYTE)pData[1], (BYTE)pData[2], (BYTE)pData[3], (BYTE)pData[4], (BYTE)pData[5]);
		szHmlFmt[MAX_TYPE_LEN] = 0;
		nRetVal = 0;
	}while(FALSE);

	fclose(fp);
	safe_free(pData);

	if(nRetVal != 0)
		return nRetVal;
	
	if(!_stricmp(szHmlFmt, DOCUFORMAT_hml_han_6_adbit))
	{
		m_n_hml_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HML;
	}
	else
	{
		nFmtType = ASIDFF_FILE_FMT_MORE_CHECK;
	}
	return 0;
}



INT32 CDocFileFmtUtil::CheckHwpFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	FILE* fp = NULL;
	BYTE *pData = NULL; 
	INT32 nRead = 0;
	INT32 nRetVal = 0;
	char szHwpFmt[MAX_QQBUFF+1] = {0,};
	if(pcFilePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -301;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);	
		return -302;
	}

	do{
		pData = (BYTE *)malloc(READ_HWP_FMT_BIN_MAX+1);
		if(pData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -303;
			break;
		}
		memset(pData, 0x00, READ_HWP_FMT_BIN_MAX+1);	

		fseek(fp, 0L, SEEK_SET);
		nRead = (INT32)fread(pData, 1, READ_HWP_FMT_BIN_MAX, fp);
		if (nRead != READ_HWP_FMT_BIN_MAX)
		{
			Sleep(10);
			fseek(fp, 0L, SEEK_SET);
			nRead = (INT32)fread(pData, 1, READ_HWP_FMT_BIN_MAX, fp);
			if (nRead != READ_HWP_FMT_BIN_MAX)
			{
				nRetVal = 1;
				break;
			}
		}

		snprintf(szHwpFmt, MAX_QQBUFF, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
			(BYTE)pData[0], (BYTE)pData[1], (BYTE)pData[2], (BYTE)pData[3], (BYTE)pData[4], (BYTE)pData[5],
			(BYTE)pData[6], (BYTE)pData[7], (BYTE)pData[8], (BYTE)pData[9], (BYTE)pData[10], (BYTE)pData[11],
			(BYTE)pData[12], (BYTE)pData[13], (BYTE)pData[14], (BYTE)pData[15]);
		szHwpFmt[MAX_QQBUFF] = 0;
		nRetVal = 0;
	}while(FALSE);

	fclose(fp);
	safe_free(pData);

	if(nRetVal != 0)
		return nRetVal;

	if(!_stricmp(szHwpFmt, DOCUFORMAT_hml_han_6_adbit))
	{
		m_n_hml_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HML;
	}
	else
	{
		nFmtType = ASIDFF_FILE_FMT_MORE_CHECK;
	}

	return 0;
}


INT32 CDocFileFmtUtil::CheckDocFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	char szDocFmt[MAX_QQBUFF+1] = {0,};

	INT32 nFileFmt = ASIDFF_FILE_FMT_TYPE_UNKNOWN;	
	if(pcFilePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -401;
	}

	nRetVal = GetDocFileFmtString(pcFilePath, szDocFmt, MAX_QQBUFF, acLogMsg);
	if(nRetVal != 0)
	{
		if(nRetVal == 1)
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
			nRetVal = 0;
		}
		return nRetVal;
	}

	if(!_strnicmp(szDocFmt, DOCUFORMAT_doc_office_1_adbit, 2))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_doc_office_1_adbit_unk, 1))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_xls_office_1_adbit, 2))
	{
		m_n_xls_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_ppt_han_1_adbit, 2))
	{
		m_n_ppt_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_doc_office2007_1_adbit, 2))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_doc2_office2007_1_adbit, 2))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls_han_4_adbit))
	{
		m_n_xls_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_pps_office_4_adbit))
	{
		m_n_pps_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_ppt_office_4_adbit))
	{
		m_n_ppt_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp2_han_4_adbit))
	{
		m_n_hwp2_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_cell_han_4_adbit))
	{
		m_n_cell_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_nxl_han_4_adbit))
	{
		m_n_nxl_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_NXL;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp3_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp3_1_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp4_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp5_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_ppt_office2007_4_adbit))
	{
		m_n_ppt_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}		
	else if(strstr(szDocFmt, DOCUFORMAT_pps_office2007_4_adbit))
	{
		m_n_pps_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPS;
		return 0;
	}	
	else if(strstr(szDocFmt, DOCUFORMAT_xls_office2007_4_adbit))
	{
		m_n_xls_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls2_office2007_4_adbit))
	{
		m_n_xls_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp6_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp7_han_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp_han2014_4_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_nxl_han2014_4_adbit)) 
	{
		m_n_nxl_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_NXL;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls_han2014_4_adbit)) 
	{
		m_n_xls_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_cell_han2014_4_adbit))
	{
		m_n_cell_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_ppt_han2014_1_adbit, 2))
	{
		m_n_ppt_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls_office2013_4_adbit)) 
	{
		m_n_xls_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls2_office2013_4_adbit)) 
	{
		m_n_xls_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_pps_office2013ppt_4_adbit))
	{
		m_n_pps_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_ppt_office2013ppt_4_adbit)) 
	{
		m_n_ppt_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_PPT;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_doc_office2013pub_1_adbit, 2))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(!_strnicmp(szDocFmt, DOCUFORMAT_doc_office2013_1_adbit, 2))
	{
		m_n_doc_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_hwp_han2010_8_adbit))
	{
		m_n_hwp3_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HWP;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls_han2010_8_adbit))
	{
		m_n_xls_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}
	else if(strstr(szDocFmt, DOCUFORMAT_xls_chinese_8_adbit))
	{
		m_n_xls_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_XLS;
		return 0;
	}		
	else if(strstr(szDocFmt, DOCUFORMAT_cell_han2010_8_adbit))
	{
		m_n_cell_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_CELL;
		return 0;
	}
	nFmtType = ASIDFF_FILE_FMT_MORE_CHECK;
	return 0;
/*
	if(CheckFileExtAnalysis(pcFilePath, &nFileFmt) == 0)
		return nFileFmt;

	if(CheckZipFileContentAnalysis(pcFilePath, &nFileFmt, acLogMsg) == 0)
		return nFileFmt;

	m_n_doc_Office_Count++;
	return ASIDFF_FILE_FMT_TYPE_DOC;
*/
}

INT32 CDocFileFmtUtil::CheckZipFileContentAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	FILE* fp = NULL;
	char *pData = NULL; 
	INT32 nErrCode = 0;
	INT32 nRead = 0;
	UINT32 dwMagicNumber = 0;
	ZIP_FILE_HEADER stZipHeader;
	if(pcFilePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -301;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);
		return -302;
	}

	nRead = fread( &dwMagicNumber, sizeof(UINT32), 1, fp );
	if ((!feof(fp) || (nErrCode =  ferror(fp))) && nRead != sizeof(UINT32))
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to read %s (%d)", pcFilePath, errno);
		fclose(fp);
		return -303;
	}

	if(ASIDFF_LOCAL_HEADER_MAGIC!=dwMagicNumber)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid magic number (%d)", dwMagicNumber);
		fclose(fp);
		return -304;
	}

	nRead = fread( &stZipHeader, sizeof(stZipHeader), 1, fp );
	if ((!feof(fp) || (nErrCode =  ferror(fp))) && nRead != sizeof(stZipHeader))
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to read %s (%d)", pcFilePath, errno);
		fclose(fp);
		return -305;
	}

	if(stZipHeader.wNameSize < 1 || stZipHeader.wNameSize > 65535)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid header name size (%d)", stZipHeader.wNameSize);
		fclose(fp);
		return -306;
	}
	if(stZipHeader.wNameSize > DOCUFORMAT_OPEN_XML_LENGTH)
		stZipHeader.wNameSize = DOCUFORMAT_OPEN_XML_LENGTH;
	pData = (char *)malloc(stZipHeader.wNameSize + 1);
	if(pData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d) (%d)", stZipHeader.wNameSize, errno);
		fclose(fp);
		return -307;
	}
	nRead = fread(pData, stZipHeader.wNameSize, 1, fp );
	if ((!feof(fp) || (nErrCode =  ferror(fp))) && nRead != stZipHeader.wNameSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to read %s (%d)", pcFilePath, errno);
		fclose(fp);
		return -308;
	}
	pData[stZipHeader.wNameSize] = 0;

	fclose(fp);

	if(!_stricmp(pData, DOCUFORMAT_office_open_xml))
	{
		m_n_docx_Office_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOCX;
	}
	else
	{
		m_n_zip_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_ZIP;
	}

	safe_free(pData);
	return 0;
}

INT32 CDocFileFmtUtil::GetFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg)
{
	FILE* fp = NULL;	
	BYTE *pData = NULL;  
	INT32 nRead = 0;
	INT32 nRetVal = 0;
	if(pcFilePath == NULL || pcFmtString == NULL || nLength < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data (%d)", nLength);
		return -51;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);
		return -52;
	}

	do{
		pData = (BYTE *)malloc(READ_FIRST_FMT_BIN_MAX+1);
		if(pData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -53;
			break;
		}
		memset(pData, 0, READ_FIRST_FMT_BIN_MAX+1);
		fseek(fp, 0L, SEEK_SET);
		nRead = (INT32)fread(pData, 1, READ_FIRST_FMT_BIN_MAX, fp);
		if (nRead != READ_FIRST_FMT_BIN_MAX)
		{
			Sleep(10);
			fseek(fp, 0L, SEEK_SET);
			nRead = (INT32)fread(pData, 1, READ_FIRST_FMT_BIN_MAX, fp);
			if (nRead != READ_FIRST_FMT_BIN_MAX)
			{
				nRetVal = 1;
				break;
			}
		}
		pData[READ_FIRST_FMT_BIN_MAX] = 0;

		snprintf(pcFmtString, nLength, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			(BYTE)pData[0], (BYTE)pData[1], (BYTE)pData[2], (BYTE)pData[3], (BYTE)pData[4], (BYTE)pData[5], (BYTE)pData[6], (BYTE)pData[7],
			(BYTE)pData[8], (BYTE)pData[9], (BYTE)pData[10], (BYTE)pData[11], (BYTE)pData[12], (BYTE)pData[13], (BYTE)pData[14], (BYTE)pData[15]);
		nRetVal = 0;
	}while(FALSE);
	fclose(fp);
	safe_free(pData);
	return nRetVal;
}

INT32 CDocFileFmtUtil::GetPkFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg)
{
	FILE* fp = NULL;	
	BYTE *pData = NULL;  
	INT32 nRead = 0;
	INT32 nRetVal = 0;

	if(pcFilePath == NULL || pcFmtString == NULL || nLength < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data (%d)", nLength);
		return -111;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);
		return -112;
	}

	do{
		pData = (BYTE *)malloc(READ_PK_FMT_BIN_MAX+1);
		if(pData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -113;
			break;
		}
		memset(pData, 0, READ_PK_FMT_BIN_MAX+1);
		fseek(fp, 1146L, SEEK_SET);
		nRead = (INT32)fread(pData, 1, READ_PK_FMT_BIN_MAX, fp);
		if (nRead != READ_PK_FMT_BIN_MAX)
		{
			Sleep(10);
			fseek(fp, 1146L, SEEK_SET);
			nRead = (INT32)fread(pData, 1, READ_PK_FMT_BIN_MAX, fp);
			if (nRead != READ_PK_FMT_BIN_MAX)
			{
				nRetVal = 1;
				break;
			}
		}
		pData[READ_PK_FMT_BIN_MAX] = 0;

		snprintf(pcFmtString, nLength, "%02X%02X%02X%02X", (BYTE)pData[0], (BYTE)pData[1], (BYTE)pData[2], (BYTE)pData[3]);
		nRetVal = 0;
	}while(FALSE);
	fclose(fp);
	safe_free(pData);
	return nRetVal;
}


INT32 CDocFileFmtUtil::GetDocFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg)
{
	FILE* fp = NULL;	
	BYTE *pData = NULL;  
	INT32 nRead = 0;
	INT32 nRetVal = 0;

	if(pcFilePath == NULL || pcFmtString == NULL || nLength < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data (%d)", nLength);
		return -411;
	}

	fp = fopen(pcFilePath, "rb");
	if(fp==NULL)
	{
		if(errno == 2)
			return 1;
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pcFilePath, errno);
		return -412;
	}

	do{
		pData = (BYTE *)malloc(READ_DOC_FMT_BIN_MAX+1);
		if(pData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate memory (%d)", errno);
			nRetVal = -413;
			break;
		}
		memset(pData, 0x00, READ_DOC_FMT_BIN_MAX+1);

		fseek(fp, 0L, SEEK_SET);
		nRead = (INT32)fread(pData, 1, READ_DOC_FMT_BIN_MAX, fp);
		if (nRead != READ_DOC_FMT_BIN_MAX)
		{
			Sleep(10);
			fseek(fp, 0L, SEEK_SET);
			nRead = (INT32)fread(pData, 1, READ_DOC_FMT_BIN_MAX, fp);
			if (nRead != READ_DOC_FMT_BIN_MAX)
			{
				nRetVal = 1;
				break;
			}
		}

		snprintf(pcFmtString, nLength, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			(BYTE)pData[48], (BYTE)pData[1132], (BYTE)pData[1133], (BYTE)pData[1134], (BYTE)pData[1135], (BYTE)pData[1644], (BYTE)pData[1645], (BYTE)pData[1646], (BYTE)pData[1647], (BYTE)pData[648],
			(BYTE)pData[649], (BYTE)pData[650], (BYTE)pData[651], (BYTE)pData[1664], (BYTE)pData[1666], (BYTE)pData[1668], (BYTE)pData[1670], (BYTE)pData[1672], (BYTE)pData[1674], (BYTE)pData[1676],
			(BYTE)pData[1678], (BYTE)pData[1152], (BYTE)pData[1154], (BYTE)pData[1156], (BYTE)pData[1158], (BYTE)pData[1160], (BYTE)pData[1162], (BYTE)pData[1164], (BYTE)pData[1166]);

		nRetVal = 0;
	}while(FALSE);
	fclose(fp);
	safe_free(pData);
	return nRetVal;
}

INT32 CDocFileFmtUtil::CheckDocumentFile(char *pcFilePath, INT32 &nFmtType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 nRead = 0;
	char szFmt[MAX_QQBUFF+1] = {0,};
	char *pExt = NULL;

	if(pcFilePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -41;
	}

	nRetVal = GetFileFmtString(pcFilePath, szFmt, MAX_QQBUFF, acLogMsg);
	if(nRetVal != 0)
	{
		if(nRetVal == 1)
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
			nRetVal = 0;
		}
		return nRetVal;
	}
	szFmt[MAX_QQBUFF] = 0;
	/***************************************************************************
		1.                               PK... 
     ***************************************************************************/
	if(!_strnicmp(szFmt, HEADER_PK_COMMON_VAL, 8)) 
	{
		nRetVal = CheckPkFmtAnalysis(pcFilePath, szFmt, nFmtType, acLogMsg);
	}
	/***************************************************************************
		2.          <?xml..
     ***************************************************************************/
	else if(!_strnicmp(szFmt, DOCUFORMAT_hml_han_8, 16))
	{
		nRetVal = CheckHmlFmtAnalysis(pcFilePath, nFmtType, acLogMsg);
		if(nRetVal == 1)
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
			return 0;
		}
	}
	/***************************************************************************
		3.          HWP(ฐ๘น้)..
     ***************************************************************************/	
	else if(!_strnicmp(szFmt, DOCUFORMAT_hwp_han_4, 8)) 
	{
		nRetVal = CheckHwpFmtAnalysis(pcFilePath, nFmtType, acLogMsg);
		if(nRetVal == 1)
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
			return 0;
		}
  	}
	/***************************************************************************
		4.          D0CF11E0A1B11AE1
     ***************************************************************************/
	else if(!_strnicmp(szFmt, HEADER_D0CF_COMMON_VAL, 16))
	{
		nRetVal = CheckDocFmtAnalysis(pcFilePath, nFmtType, acLogMsg);
	}
	/***************************************************************************
		5.          %PDF.. or 25504446..
     ***************************************************************************/
	else if(!_strnicmp(szFmt, DOCUFORMAT_pdf_ALL_4, 8)) 
	{
		pExt = (char *)strrchr(pcFilePath, '.');
		if(pExt != NULL && !_stricmp(pExt, ".ai"))
		{
			nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
		}
		else
		{
			m_n_pdf_Count++;
			nFmtType = ASIDFF_FILE_FMT_TYPE_PDF;
		}
		nRetVal = 0;
	}
	/***************************************************************************
		6.          {\rtf..
     ***************************************************************************/
	else if(!_strnicmp(szFmt, DOCUFORMAT_doc_han_16, 32))
	{
		m_n_doc_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_DOC;
		nRetVal = 0;
	}
	/***************************************************************************
		7.          Robu..
     ***************************************************************************/
	else if(!_strnicmp(szFmt, DOCUFORMAT_hpt_han_16, 32))
	{
		m_n_hpt_Han_Count++;
		nFmtType = ASIDFF_FILE_FMT_TYPE_HPT;
		nRetVal = 0;
	}
	else
	{
		nFmtType = ASIDFF_FILE_FMT_TYPE_UNKNOWN;
		nRetVal = 0;
	}
	return nRetVal;
}
//-----------------------------------------------------------------------------------------------
