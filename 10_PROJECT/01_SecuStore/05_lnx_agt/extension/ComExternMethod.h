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


#ifndef COM_GLOBAL_EXTERN_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984
#define COM_GLOBAL_EXTERN_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984

INT32			HexToMap(String strHexValue, TMapID& tValueMap, INT32 nMaxLen);
INT32			MapToHex(TMapID& tValueMap, String& strHexValue, INT32 nMaxLen);
INT32			HexToMap_64(String strHexValue, TMapID64& tValueMap, INT32 nMaxLen);
INT32			MapToHex_64(TMapID64& tValueMap, String& strHexValue, INT32 nMaxLen);

INT32			MapIDStrToString(TMapIDStr& tIDMapStr, String& strMapList, INT32 nMaxLen, CHAR chBlockTok = '|');
INT32			StringToMapIDStr(String strMapList, TMapIDStr& tIDMapStr, INT32 nMaxLen, CHAR chBlockTok = '|');


String			MapStrToStr(TMapStr& tStrMap, CHAR chCellTok = '/', CHAR chBlockTok = '|');
INT32			StrToMapStr(String strMapList, TMapStr& tStrMap, CHAR chCellTok = '/', CHAR chBlockTok = '|');

String			MapIDStrToStr(TMapIDStr& tIDMapStr, CHAR chCellTok = ',');
INT32			StrToMapIDStr(String strMapList, TMapIDStr& tIDMapStr, CHAR chCellTok = ',');

String			ListStrToStr(TListStr& tStrList, CHAR chBlockTok);
INT32			StrToListStr(String strList, TListStr& tStrList, CHAR chBlockTok);

String			MapIDToStr_Str(TMapIDStr& tIDMapStr, CHAR chCellTok = '/', CHAR chBlockTok = '|');
INT32			StrToMapID_Str(String strMapList, TMapIDStr& tIDMapStr, CHAR chCellTok = '/', CHAR chBlockTok = '|');

String			MapIDToStr_ID(TMapID& tIDMap, CHAR chCellTok = '/', CHAR chBlockTok = '|');
INT32			StrToMapID_ID(String strMapList, TMapID& tIDMap, CHAR chCellTok = '/', CHAR chBlockTok = '|');

INT32			MaptoMapStr_64(TMapID64& tIDMap, TMapStr& tStrMap);
INT32			MapStrToMap_64(TMapStr& tStrMap, TMapID64& tIDMap);

INT32			HexToP32(String strHexValue, PINT32 pValue, INT32 nValNum);
INT32			P32ToHex(PINT32 pValue, INT32 nValNum, String& strHexValue);

INT32			CheckDumpFile(String &strPath, INT32 nRestDumpCount = 3);

INT32			RemoveDumpFiles();
INT32			RemoveSdeltemp();
INT32			RemoveInvenFiles();
void			DeletePrefixFiles(String strPath, LPCTSTR lpszPrefixed);

INT32			FindMemory(PBYTE lpBuff, UINT32 nBuffLen, PBYTE lpFindBuff, UINT32 nFindBuffLen);

void			WriteLogA(char* fmt,...);


#endif //COM_GLOBAL_EXTERN_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984