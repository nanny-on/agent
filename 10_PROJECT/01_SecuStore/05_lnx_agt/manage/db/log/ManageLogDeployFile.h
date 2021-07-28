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

//---------------------------------------------------------------------------

#ifndef ManageLogDeployFileH
#define ManageLogDeployFileH
//---------------------------------------------------------------------------

class CManageLogDeployFile : public CManageBase<DB_LOG_DEPLOY_FILE>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf);
    INT32					EditLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf);
    INT32					DelLogDeployFile(UINT32 nID);

public:
	INT32					IsExistLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf);
	PDB_LOG_DEPLOY_FILE		FindItemByDFInfo(DB_LOG_DEPLOY_FILE&	dldf);

public:
	INT32					SetPktSync(TListPVOID& tIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(PDB_LOG_DEPLOY_FILE pdldf, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_DEPLOY_FILE& dldf);

	INT32					SetPkt_Link(MemToken& SendToken);

public:
	CManageLogDeployFile();
    ~CManageLogDeployFile();

};

extern CManageLogDeployFile*	t_ManageLogDeployFile;

#endif
