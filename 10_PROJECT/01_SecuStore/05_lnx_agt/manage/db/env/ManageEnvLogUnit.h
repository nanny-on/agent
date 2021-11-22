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

//---------------------------------------------------------------------------

#ifndef ManageEnvLogUnitH
#define ManageEnvLogUnitH
//---------------------------------------------------------------------------

class CManageEnvLogUnit : public CManagePoBase<DB_ENV_LOG_UNIT>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddEnvLogUnit(DB_ENV_LOG_UNIT&	deleu);
    INT32					EditEnvLogUnit(DB_ENV_LOG_UNIT&	deleu);
    INT32					DelEnvLogUnit(UINT32 nID);
	INT32					ApplyEnvLogUnit(DB_ENV_LOG_UNIT&	deleu);

public:
	String					GetName(UINT32 nID); 
	String					GetExtInfo(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType);
	UINT32					GetExtInfoInt(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType);
	UINT32					IsExistExtInfo(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType);


public:
	INT32					GetExtInfoByDp(DB_ENV_LOG_UNIT& tdelu, DB_LOG_DEPLOY_FILE& tdldf);
	INT32					GetExtInfoByDv(DB_ENV_LOG_UNIT& tdelu, DB_LOG_DEVICE& tdld);
	INT32					GetExtInfoByPatch(DB_ENV_LOG_UNIT& tdelu, DB_LOG_PATCH& tdlp);
	INT32					GetExtInfoByRs(DB_ENV_LOG_UNIT& tdelu, DB_LOG_RS& tdlr);

public:
	PDB_ENV_LOG_UNIT	FindRecordLogEvtUnit(PDB_LOG_EVENT pdle);
	PDB_ENV_LOG_UNIT	FindRecordLogDocUnit(PDB_LOG_DOC pdld);
	PDB_ENV_LOG_UNIT	FindRecordLogSecuUnit(PDB_LOG_SECU pdls);
	PDB_ENV_LOG_UNIT	FindRecordLogDeployFileUnit(PDB_LOG_DEPLOY_FILE pdldf);
	PDB_ENV_LOG_UNIT	FindRecordLogPatchUnit(PDB_LOG_PATCH pdlp);
	PDB_ENV_LOG_UNIT	FindRecordLogDeviceUnit(PDB_LOG_DEVICE pdld);
	PDB_ENV_LOG_UNIT	FindRecordLogRsUnit(PDB_LOG_RS pdata);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_ENV_LOG_UNIT pdeleu, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_ENV_LOG_UNIT& deleu);

public:
	CManageEnvLogUnit();
    ~CManageEnvLogUnit();

};

extern CManageEnvLogUnit*	t_ManageEnvLogUnit;

#endif
