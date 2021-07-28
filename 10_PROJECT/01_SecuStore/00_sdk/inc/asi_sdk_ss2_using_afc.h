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


#ifndef ASI_SDK_SS2_USING_AFC_HEADER_15BE18C4_D543_413E_96F8_61C24AD45985
#define	ASI_SDK_SS2_USING_AFC_HEADER_15BE18C4_D543_413E_96F8_61C24AD45985

//-----------------------------------------------------------------------------------------------

#ifdef ASI_SDK_SS2_USING_AFC_ALL

#define ASI_SDK_SS2_USING_AFC_DLL_EX_POPUP

#endif

#ifdef ASI_SDK_SS2_USING_AFC_DDK_ALL

#define ASI_SDK_SS2_USING_AFC_DLL_EPS_APP

#endif

//-----------------------------------------------------------------------------------------------

#ifdef ASI_SDK_SS2_USING_AFC_DLL_EX_POPUP

	#include	"com_header_ex_popup_dll.h"
	
#endif //ASI_SDK_SS2_USING_AFC_DLL_EX_POPUP
//-----------------------------------------------------------------------------------------------

#ifdef ASI_SDK_SS2_USING_AFC_DLL_EPS_APP

#include	"asiepsappdllutil.h"

#endif //ASI_SDK_SS2_USING_AFC_DLL_EPS_APP
//-----------------------------------------------------------------------------------------------

#endif