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
#ifndef DB_ENV_SOCKET_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_ENV_SOCKET_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_env_socket
{
	_db_env_socket()
	{
		nConPort		= OPEN_PORT_TYPE_CONSOLE;
		nAgtLgnPort		= OPEN_PORT_TYPE_AGT_LGN;
		nAgtMgrPort		= OPEN_PORT_TYPE_AGT_MGR;
		nAgtUdtPort		= OPEN_PORT_TYPE_AGT_UDT;
		nAgtLgnRegMode	= SS_ENV_SOCKET_AGT_LGN_REG_MODE_RETRY_REG;
		nAgtLgnRegType	= SS_ENV_SOCKET_AGT_ID_TYPE_MAC;

		nSubUsedMode	= 0;
		nSubLgnPort		= OPEN_PORT_TYPE_SVR_LGN;
		nSubMgrPort		= OPEN_PORT_TYPE_SVR_MGR;
		nSubUdtPort		= OPEN_PORT_TYPE_SVR_UDT;
		nSubRptPort		= OPEN_PORT_TYPE_SVR_RPT;

		nLcnChkType		= STATUS_USED_MODE_ON;
		nLcnConPort		= CON_PORT_TYPE_LICENSE_INFO;
		strLcnDomain	= STR_LICENSE_DOMAIN;

		nRemainLog		= 0;
		nRemainPktLog	= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nConPort;
	UINT32				nAgtLgnPort;
	UINT32				nAgtMgrPort;
	UINT32				nAgtUdtPort;
	UINT32				nAgtLgnRegMode;
	UINT32				nAgtLgnRegType;

	UINT32				nSubUsedMode;
	UINT32				nSubLgnPort;
	UINT32				nSubMgrPort;
	UINT32				nSubUdtPort;
	UINT32				nSubRptPort;

	UINT32				nLcnChkType;
	UINT32				nLcnConPort;
	String				strLcnDomain;

	UINT32				nRemainLog;
	UINT32				nRemainPktLog;

	TMapID				tSockErrCodeMap;

}DB_ENV_SOCKET, *PDB_ENV_SOCKET;

typedef list<DB_ENV_SOCKET>					TListDBEnvSocket;
typedef TListDBEnvSocket::iterator			TListDBEnvSocketItor;

typedef map<UINT32, DB_ENV_SOCKET>			TMapDBEnvSocket;
typedef TMapDBEnvSocket::iterator			TMapDBEnvSocketItor;

#endif //DB_ENV_SOCKET_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



