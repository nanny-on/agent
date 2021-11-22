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

#ifndef _ASI_FIND_DOCF_H__

#define _ASI_FIND_DOCF_H__

#define READ_FIRST_FMT_BIN_MAX	16
#define READ_PK_FMT_BIN_MAX 	8
#define READ_HML_FMT_BIN_MAX	12
#define READ_HWP_FMT_BIN_MAX	32
#define READ_DOC_FMT_BIN_MAX	1680

#define CMD_FILE_FORMAT		"/usr/bin/file"

class CDocFileFmtUtil
{
public:
	CDocFileFmtUtil(void);
	~CDocFileFmtUtil(void);

	INT32 ProcAnalysis(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);		

protected:
	INT32 ParseFileFormat(char *szCmdFmt, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckFileCmdAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 SetSearchFileList(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);
	INT32 CheckPkFmtAnalysis(char *pcFilePath, char *szFmt, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckHmlFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckHwpFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckDocFmtAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckDocumentFile(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 CheckFileExtAnalysis(char *pcFilePath, INT32 *pnFileFmtType);
	INT32 CheckZipFileContentAnalysis(char *pcFilePath, INT32 &nFmtType, char *acLogMsg);
	INT32 ConvertFmtTypeToName(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);
	INT32 GetFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg);
	INT32 GetPkFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg);
	INT32 GetDocFileFmtString(char *pcFilePath, char *pcFmtString, INT32 nLength, char *acLogMsg);

protected:
	INT32 m_n_zip_Count;
	INT32 m_n_pptx_Han_Count;
	INT32 m_n_pptx_Office_Count;
	INT32 m_n_pptx2_Office_Count;
	INT32 m_n_odt_Office_Count;
	INT32 m_n_odt_Han_Count;
	INT32 m_n_odp_Office_Count;
	INT32 m_n_odp_Han_Count;
	INT32 m_n_xlsx_Office_Count;
	INT32 m_n_xlsx_Han_Count;
	INT32 m_n_docx_Office_Count;
	INT32 m_n_docx_Han_Count;
	INT32 m_n_ods_Han_Count;
	INT32 m_n_ppsx_Han_Count;
	INT32 m_n_ppsx_Office_Count;
	INT32 m_n_show_Han_Count;
	INT32 m_n_xlsx_unKnown_Count;
	INT32 m_n_ods_Office_Count;
	INT32 m_n_doc_Han_Count;
	INT32 m_n_doc_Office_Count;
	INT32 m_n_pps_Office_Count;
	INT32 m_n_ppt_Han_Count;
	INT32 m_n_ppt_Office_Count;
	INT32 m_n_xls_Office_Count;
	INT32 m_n_xls_Han_Count;
	INT32 m_n_cell_Han_Count;
	INT32 m_n_nxl_Han_Count;
	INT32 m_n_hwp2_Han_Count;
	INT32 m_n_hwp_Han_Count;
	INT32 m_n_hwp3_Han_Count;	
	INT32 m_n_hwpx_Han_Count;
	INT32 m_n_pdf_Count;	
	INT32 m_n_hml_Han_Count;
	INT32 m_n_hpt_Han_Count;		
	INT32 m_n_nxt_Han_Count;
	INT32 m_n_odb_Han_Count;
	INT32 m_n_odg_Han_Count;	
	INT32 m_n_pptx_Known_Count;
	INT32 m_n_odt_Known_Count;
	INT32 m_n_odp_Known_Count;
	INT32 m_n_xlsx_Known_Count;
	INT32 m_n_docx_Known_Count;
	INT32 m_n_ods_Known_Count;
	INT32 m_n_ppsx_Known_Count;
	INT32 m_n_show_Known_Count;
	INT32 m_n_doc_Known_Count;
	INT32 m_n_xls_Known_Count;
	INT32 m_n_ppt_Known_Count;
	INT32 m_n_pps_Known_Count;
	INT32 m_n_cell_Known_Count;
	INT32 m_n_nxl_Known_Count;
	INT32 m_n_pdf_Known_Count;
	INT32 m_n_hml_Known_Count;
	INT32 m_n_hwp_Known_Count;
	INT32 m_n_hpt_Known_Count;
	INT32 m_n_nxt_Known_Count;
	INT32 m_n_odb_Known_Count;
	INT32 m_n_odg_Known_Count;
	INT32 m_n_user_define_Count;
	pthread_mutex_t m_mutex;
};

#endif /*_ASI_FIND_DOCF_H__*/
