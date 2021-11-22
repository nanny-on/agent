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

#ifndef COM_HEADER_DVI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_DVI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
#define UnknownDevice "<Unknown Device>"

//------------------------------------------------------------------------------
typedef list<String>		TListStr;
typedef TListStr::iterator	TListStrItor;

typedef map<String, String>	TMapStr;
typedef TMapStr::iterator		TMapStrItor;

typedef map<String, UINT32>    		TMapStrID;
typedef TMapStrID::iterator     		TMapStrIDItor;
//------------------------------------------------------------------------------
typedef struct _asidevice_item_info
{
	String		strClassName;
	String		strDeviceName;
	String		strGUID;
	String		strDeviceInstanceID;
	String		strPaDeviceInstanceID;
	INT32		nUsed;
	INT32		nIconIndex;
	_asidevice_item_info()
	{
		strClassName			= "";
		strDeviceName			= "";
		strGUID					= "";
		strDeviceName			= "";
		strGUID					= "";
		strDeviceInstanceID		= "";
		strPaDeviceInstanceID	= "";
		nUsed					= 0;
		nIconIndex				= 0;
	}	
}ASIDEVICE_ITEM_INFO, *PASIDEVICE_ITEM_INFO;

typedef list<ASIDEVICE_ITEM_INFO>		TListDeviceInfo;
typedef TListDeviceInfo::iterator		TListDeviceInfoItor;

//------------------------------------------------------------------------------

typedef struct _device_item_class
{
	String					strClassName;
	TListDeviceInfo			tDeviceItemInfoList;
	~_device_item_class()
	{
		tDeviceItemInfoList.clear();
	}
}DEVICE_ITEM_CLASS, *PDEVICE_ITEM_CLASS;

typedef map<String, DEVICE_ITEM_CLASS>	TMapDeviceItemClass;
typedef TMapDeviceItemClass::iterator	TMapDeviceItemClassItor;
//------------------------------------------------------------------------------
typedef INT32 (CALLBACK *CallBack_DeviceInfo)(PVOID lParam, ASIDEVICE_ITEM_INFO);


typedef	INT32 (*ASIDVI_InitType)(PVOID* lpObject);
typedef	INT32 (*ASIDVI_FreeType)(PVOID lpObject);
//--------------------------------------------------------------------------

typedef INT32 (*ASIDVI_EnumDeviceInfoType)(PVOID lpObject, void * lParam, void * fnCallBack);
typedef INT32 (*ASIDVI_StateChangeType)(PVOID lpObject, LPCTSTR pszDeviceInstanceID, DWORD nEnableFlag);
typedef INT32 (*ASIDVI_GetDeviceStatusType)(PVOID lpObject, LPCTSTR pszDeviceInstanceID);
typedef	INT32 (*ASIDVI_RemoveDeviceType)(PVOID lpObject, LPCTSTR pszDeviceInstanceID);
typedef	INT32 (*ASIDVI_IsExistDeviceType)(PVOID lpObject, LPCTSTR pszDeviceInstanceID);

//--------------------------------------------------------------------------

#endif      //COM_HEADER_DVI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
