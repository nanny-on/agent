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
#pragma once

typedef struct _db_po_in_vuln_secu_usb
{
	_db_po_in_vuln_secu_usb()
	{
	}

	DB_PO_HEADER		tDPH;
}DB_PO_IN_VULN_SECU_USB, *PDB_PO_IN_VULN_SECU_USB;

typedef list<DB_PO_IN_VULN_SECU_USB>				TListDBPoInVulnSecuUsb;
typedef TListDBPoInVulnSecuUsb::iterator			TListDBPoInVulnSecuUsbItor;

typedef map<UINT32, DB_PO_IN_VULN_SECU_USB>			TMapDBPoInVulnSecuUsb;
typedef TMapDBPoInVulnSecuUsb::iterator				TMapDBPoInVulnSecuUsbItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_secu_usb_pkg
{
	_db_po_in_vuln_secu_usb_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_VULN_SECU_USB_PKG, *PDB_PO_IN_VULN_SECU_USB_PKG;

typedef list<DB_PO_IN_VULN_SECU_USB_PKG>			TListDBPoInVulnSecuUsbPkg;
typedef TListDBPoInVulnSecuUsbPkg::iterator			TListDBPoInVulnSecuUsbPkgItor;

typedef map<UINT32, DB_PO_IN_VULN_SECU_USB_PKG>		TMapDBPoInVulnSecuUsbPkg;
typedef TMapDBPoInVulnSecuUsbPkg::iterator			TMapDBPoInVulnSecuUsbPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_secu_usb_unit
{
	_db_po_in_vuln_secu_usb_unit()
	{
		strUsbValue.empty();
	}
	DB_PO_HEADER			tDPH;

	String					strUsbValue;
	String					strRegValue;
	String					strProcess;
	String					strFilePath;
}DB_PO_IN_VULN_SECU_USB_UNIT, *PDB_PO_IN_VULN_SECU_USB_UNIT;

typedef list<DB_PO_IN_VULN_SECU_USB_UNIT>			TListDBPoInVulnSecuUsbUnit;
typedef TListDBPoInVulnSecuUsbUnit::iterator		TListDBPoInVulnSecuUsbUnitItor;

typedef map<UINT32, DB_PO_IN_VULN_SECU_USB_UNIT>	TMapDBPoInVulnSecuUsbUnit;
typedef TMapDBPoInVulnSecuUsbUnit::iterator			TMapDBPoInVulnSecuUsbUnitItor;
//---------------------------------------------------------------------------