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
#include "com_struct.h"
#include "MutexExt.h"


CMutexExt::CMutexExt()
{
	pthread_mutex_init(&m_mutex, NULL);
}


CMutexExt::~CMutexExt()
{
	pthread_mutex_destroy(&m_mutex);
}


int CMutexExt::Lock()
{
	pthread_mutex_lock (&m_mutex);
	return 1;
}


int CMutexExt::UnLock()
{
	pthread_mutex_unlock (&m_mutex);
	return 1;
}

LONG CMutexExt::InterlockedExchangeAdd(volatile LONG *pnAddend, LONG nValue)
{
	LONG nRetVal = 0;
	if(pnAddend != NULL)
	{
		pthread_mutex_lock (&m_mutex);
		*pnAddend += nValue;
		nRetVal = *pnAddend;
		pthread_mutex_unlock (&m_mutex);	
	}
	return nRetVal;
}

LONG CMutexExt::InterlockedExchange(volatile LONG *pnTarget, LONG nValue)
{
	LONG nRetVal = 0;
	if(pnTarget != NULL)
	{
		pthread_mutex_lock (&m_mutex);
		*pnTarget = nValue;
		nRetVal = *pnTarget;
		pthread_mutex_unlock (&m_mutex);	
	}
	return nRetVal;
}

