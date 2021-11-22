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

#ifndef MEM_CTRL_REMOTE_CTRL_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_CTRL_REMOTE_CTRL_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

// 자세한 정보는 아래의 위키 페이지를 참조하시기 바랍니다.
// [PoRemoteRequest]
// http://mw.ashindev.com/wiki/index.php/PoRemoteRequest

typedef struct _mem_ctrl_remote_ctrl
{
	String					strSvrAddress;		//	remote host(=sc console) ip
	String					strSvrPort;			//	remote host(=sc console) port
	String					strAlertMsg;		//	원격 접속 전 사용자 고지 메시지
	String					strModuleVersion;	//	원격 모듈 버전
	String					strDownloadInfo;	//	원격 모듈 다운로드 정보(파일명, URL 등 파이프라인 구분)
	String					strHash;			//	원격 모듈 해쉬값 (md5 or sha1 or sha2)
	UINT32					nAlertType;			//	사용자 고지 메시지 사용여부 (0 : 사용안함 / 1 : 사용함) 기본값=0
	UINT32					nConnectMode;		//	0 : Bitmap Cache / 1 : Driver Mode (Driver Mod could not support 2.0 to be updated.) 기본값=0
	UINT32					nViewOnly;			//	화면 제어사용 여부 (1 : 사용안함 / 0 : 사용) 기본값=0
	UINT32					nLocation;			//	원격모듈 로케일 (0 : 클라이언트 설정값 / 1 : 한국어 / 2 : 영어 / 3 : 일어 / 중국어간체 : 4 / 중국어번체 : 5) 기본값=0

	void SetEmpty()
	{
		strSvrAddress.empty();
		strSvrPort.empty();
		strAlertMsg.empty();
		strModuleVersion.empty();
		strDownloadInfo.empty();
		strHash.empty();
		nAlertType			= 0;
		nConnectMode		= 0;
		nViewOnly			= 0;
		nLocation			= 0;
	}

	_mem_ctrl_remote_ctrl()
	{
		SetEmpty();
	}

}MEM_CTRL_REMOTE_CTRL, *PMEM_CTRL_REMOTE_CTRL;

typedef list<MEM_CTRL_REMOTE_CTRL>			TListMemCtrlRemoteCtrl;
typedef TListMemCtrlRemoteCtrl::iterator	TListMemCtrlRemoteCtrlItor;

typedef map<UINT32, MEM_CTRL_REMOTE_CTRL>	TMapMemCtrlRemoteCtrl;
typedef TMapMemCtrlRemoteCtrl::iterator		TMapMemCtrlRemoteCtrlItor;

#endif //MEM_CTRL_REMOTE_CTRL_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



