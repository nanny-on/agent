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



#ifndef     COM_HEADER_TS_MGR_UTIL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_TS_MGR_UTIL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//-------------------------------------------------------------------

#define		TS_MGR_PARAM_KEY_TS_NAME				TEXT("t_nm")
#define		TS_MGR_PARAM_KEY_TS_DLL					TEXT("t_dl")
#define		TS_MGR_PARAM_KEY_TS_AD					TEXT("t_ad")
#define		TS_MGR_PARAM_KEY_TS_SINGLE_RUN			TEXT("t_sr")
#define		TS_MGR_PARAM_KEY_TS_SHOW				TEXT("t_sh")
#define		TS_MGR_PARAM_KEY_TS_SHOW_MODE			TEXT("t_sm")
#define		TS_MGR_PARAM_KEY_TS_ARGUMENT			TEXT("t_ar")
#define		TS_MGR_PARAM_KEY_TS_CONFIRM				TEXT("t_cf")

#define		TS_MGR_PARAM_KEY_TS_RESULT_PATH			TEXT("t_rp")
#define		TS_MGR_PARAM_KEY_TS_RESULT_VALUE		TEXT("t_rv")

#define		TS_MGR_PARAM_KEY_CHILD_PATH				TEXT("c_pt")
#define		TS_MGR_PARAM_KEY_CHILD_ARGUMENT			TEXT("c_ag")
#define		TS_MGR_PARAM_KEY_CHILD_WAIT_CONDITION	TEXT("c_wc")
#define		TS_MGR_PARAM_KEY_CHILD_HIDE_OPTION		TEXT("c_hd")
//-------------------------------------------------------------------

#define		SHOW_MODE_TYPE_FILE_COPY				0
#define		SHOW_MODE_TYPE_REMOTE_CTRL				1
#define		SHOW_MODE_TYPE_INSTANT_MSG				2
#define		SHOW_MODE_TYPE_MONITOR_PW				3

//-------------------------------------------------------------------

typedef struct _asi_ts_mgr_param
{
	_asi_ts_mgr_param()
	{
		nTSAutoDel			= 0;
		nTSSingleRun		= 1;
		nTSShow				= 0;
		nTSShowMode			= 0;
		nTSWaitMode			= 0;
		nTSChildWaitMode	= 0;
		nTSChildHideMode	= 0;
	}
	String			strTSName;
	String			strTSDLL;
	UINT32			nTSAutoDel;
	UINT32			nTSSingleRun;
	UINT32			nTSShow;
	UINT32			nTSShowMode;
	UINT32			nTSWaitMode;
	String			strTSArgument;

	String			strTSConfirmMsg;

	String			strTSResultPath;
	String			strTSResultValue;

	String			strTSChildPath;
	String			strTSChildArgument;
	UINT32			nTSChildWaitMode;
	UINT32			nTSChildHideMode;
}ASI_TS_MGR_PARAM, *PASI_TS_MGR_PARAM;

//-------------------------------------------------------------------



#endif      //COM_HEADER_TS_MGR_UTIL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
