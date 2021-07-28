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
#include "seed.h"
#include "SeedUtil.h"
#include "Seedx.h"

CSeedUtil* g_pSeed = NULL;

void __attribute__ ((constructor)) seed_init(void)
{
	g_pSeed = new CSeedUtil();
}

void __attribute__ ((destructor)) seed_fini(void)
{	
	if(g_pSeed)
		delete g_pSeed;
}

INT32 Set_Key(BYTE* lpKey)
{
	if(!g_pSeed)
		g_pSeed = new CSeedUtil();
	//vc_seed_init((char*)lpKey, 16);
	g_pSeed->Set_Key(lpKey, 16);

	return 0;
}

INT32 Make_Key(BYTE* lpKey)
{
    if(!g_pSeed)
        g_pSeed = new CSeedUtil();

	g_pSeed->Make_Key(lpKey);

	return 0;
}

INT32 Encrypt_Data(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen)
{
    if(!g_pSeed)
        g_pSeed = new CSeedUtil();
	return g_pSeed->Encrypt_Data(lpData, nLen, lpOutBuf, (UINT32&)nOutLen);
}

INT32 Decrypt_Data(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen)
{
    if(!g_pSeed)
        g_pSeed = new CSeedUtil();

	return g_pSeed->Decrypt_Data(lpData, lpOutBuf, (UINT32&)nOutLen);
}

INT32 Free_Buffer(BYTE* lpBuff)
{
	if(g_pSeed)
	{
		g_pSeed->FreeBuffer(lpBuff);
	}

	return 0;
}
