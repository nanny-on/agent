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


#ifndef COM_HEADER_EXCEL_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_EXCEL_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#define RPT_SUCCEED(a)	(!a) ? 	1 : 0
#define RPT_FAILED(a)	(a) ? 1 : 0
#define RPT_EOF(a) RPT_SUCCEED(a)
#define RPT_RTN_CHK(a)	if(a) {ASSERT(0);}  

#ifndef EXCEL_XLS_USING
#define EXCEL_XLS_USING

enum {xlTextUnderLineNone=-4142, xlTextUnderLineSingle=4, xlTextUnderLineDouble};
enum {xlAlignHLeft=1, xlAlignHCenter=-4108, xlAlignHRight=-4152};
enum {xlAlignVTop=-4160, xlAlignVCenter=-4108, xlAlignVBottom=-4107};
enum {xlLineDiagonalDown=5, xlLineDiagonalUp
, xlLineLeft, xlLineTop, xlLineBottom, xlLineRight
, xlLineInsideVertical, xlLineInsideHorizontal
, xlLineNone=-4142
};
enum {xlThin=2, xlMedium=-4138, xlThick=4};
enum {xlLineStyle_Continuous=1, xlLineStyle_Dot=-4118
, xlLineStyle_Dash=-4115, xlLineStyle_Double=-4119
, xlLineStyle_None=-4142
};

typedef enum XlChartType
{
	xlColumnClustered = 51, 
	xlColumnStacked = 52, 
	xlColumnStacked100 = 53, 
	xl3DColumnClustered = 54, 
	xl3DColumnStacked = 55, 
	xl3DColumnStacked100 = 56, 
	xlBarClustered = 57, 
	xlBarStacked = 58, 
	xlBarStacked100 = 59, 
	xl3DBarClustered = 60, 
	xl3DBarStacked = 61, 
	xl3DBarStacked100 = 62, 
	xlLineStacked = 63, 
	xlLineStacked100 = 64, 
	xlLineMarkers = 65, 
	xlLineMarkersStacked = 66, 
	xlLineMarkersStacked100 = 67, 
	xlPieOfPie = 68, 
	xlPieExploded = 69, 
	xl3DPieExploded = 70, 
	xlBarOfPie = 71, 
	xlXYScatterSmooth = 72, 
	xlXYScatterSmoothNoMarkers = 73, 
	xlXYScatterLines = 74, 
	xlXYScatterLinesNoMarkers = 75, 
	xlAreaStacked = 76, 
	xlAreaStacked100 = 77, 
	xl3DAreaStacked = 78, 
	xl3DAreaStacked100 = 79, 
	xlDoughnutExploded = 80, 
	xlRadarMarkers = 81, 
	xlRadarFilled = 82, 
	xlSurface = 83, 
	xlSurfaceWireframe = 84, 
	xlSurfaceTopView = 85, 
	xlSurfaceTopViewWireframe = 86, 
	xlBubble = 15, 
	xlBubble3DEffect = 87, 
	xlStockHLC = 88, 
	xlStockOHLC = 89, 
	xlStockVHLC = 90, 
	xlStockVOHLC = 91, 
	xlCylinderColClustered = 92, 
	xlCylinderColStacked = 93, 
	xlCylinderColStacked100 = 94, 
	xlCylinderBarClustered = 95, 
	xlCylinderBarStacked = 96, 
	xlCylinderBarStacked100 = 97, 
	xlCylinderCol = 98, 
	xlConeColClustered = 99, 
	xlConeColStacked = 100, 
	xlConeColStacked100 = 101, 
	xlConeBarClustered = 102, 
	xlConeBarStacked = 103, 
	xlConeBarStacked100 = 104, 
	xlConeCol = 105, 
	xlPyramidColClustered = 106, 
	xlPyramidColStacked = 107, 
	xlPyramidColStacked100 = 108, 
	xlPyramidBarClustered = 109, 
	xlPyramidBarStacked = 110, 
	xlPyramidBarStacked100 = 111, 
	xlPyramidCol = 112, 
	xl3DColumn = 0xFFFFEFFC, 
	xlLine = 4, 
	xl3DLine = 0xFFFFEFFB, 
	xl3DPie = 0xFFFFEFFA, 
	xlPie = 5, 
	xlXYScatter = 0xFFFFEFB7, 
	xl3DArea = 0xFFFFEFFE, 
	xlArea = 1, 
	xlDoughnut = 0xFFFFEFE8, 
	xlRadar = 0xFFFFEFC9
} XlChartType;

#endif


#define REPORT_SUCCEED						0
#define REPORT_ERROR_INITIALIZE_FAILED		-1
#define REPORT_ERROR_CREATE_FAILED			-2
#define REPORT_ERROR_HEAP_ALLOCATE_FAILED	-3
#define REPORT_ERROR_NOT_INITIALIZE			-4
#define REPORT_ERROR_QUERY_FAILED			-5
#define REPORT_ERROR_OPEN_FAILED			-6
#define REPORT_ERROR_SETVALUE_FAILED		-7
#define REPORT_ERROR_VALUE_UNKNOWN			-8


#define REPORT_ERROR						-10000

//-------------------------------------------------------------------------

typedef INT32 (*ASI_XLS_CreateType) (LPCTSTR pFp);
typedef INT32 (*ASI_XLS_ExecuteType) (LPCTSTR pQry);
typedef INT32 (*ASI_XLS_ExecuteExType) (LPCTSTR pQry);
typedef INT32 (*ASI_XLS_RecordSetType) (char *pBuf, LPCTSTR pSplitChar);
typedef INT32 (*ASI_XLS_CloseType) ();
typedef INT32 (*ASI_XLS_CreateDetailType) (LPCTSTR pFp, LPTSTR szErrMsg);
typedef LPCTSTR (*ASI_XLS_GetErrorMessageType) ();

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

typedef INT32 (*ASI_XLS_CreateExType) (LPCTSTR pFp);
typedef INT32 (*ASI_XLS_OpenExType) (LPCTSTR pFp);
typedef INT32 (*ASI_XLS_SaveAsExType) (LPCTSTR pFp);
typedef INT32 (*ASI_XLS_SaveExType) ();
typedef INT32 (*ASI_XLS_SetActiveSheetExType) (INT32 nIdx);
typedef INT32 (*ASI_XLS_ShowExType) (BOOL bShow);
typedef INT32 (*ASI_XLS_CloseExType) ();
typedef INT32 (*ASI_XLS_SetRangeExType) (INT32 nCol1, INT32 nRow1, INT32 nCol2, INT32 nRow2);
typedef INT32 (*ASI_XLS_SetSheetNameExType) (INT32 nIdx, LPCTSTR pSheetName);
typedef INT32 (*ASI_XLS_SetColWidthExType) (INT32 nCol1, INT32 nCol2, INT32 nWidth);
typedef INT32 (*ASI_XLS_SetRowHeightExType) (INT32 nRow1, INT32 nRow2, INT32 nHeight);
typedef INT32 (*ASI_XLS_SetValueStrExType) (INT32 nCol, INT32 nRow, LPCTSTR pValue);
typedef INT32 (*ASI_XLS_SetValueIntExType) (INT32 nCol, INT32 nRow, INT32 nValue);
typedef INT32 (*ASI_XLS_SetTextColorExType) (INT32 nCol1, INT32 nRow1, INT32 nCol2, INT32 nRow2, INT32 nColor);
typedef INT32 (*ASI_XLS_SetBkColorExType) (INT32 nCol1, INT32 nRow1, INT32 nCol2, INT32 nRow2, INT32 nColor);
typedef INT32 (*ASI_XLS_SetFontExType) (INT32 nCol1, INT32 nRow1, INT32 nCol2, INT32 nRow2, LPCTSTR pFn, INT32 nSize, BOOL bBold);
typedef INT32 (*ASI_XLS_SetFontUnderLineExType) (INT32 nStyle);
typedef INT32 (*ASI_XLS_SetFontItalicExType) (BOOL bItalic);
typedef INT32 (*ASI_XLS_SetFontBoldExType) (BOOL bBold);
typedef INT32 (*ASI_XLS_SetTextAlignExType) (INT32 nHorizon, INT32 nVertical);
typedef INT32 (*ASI_XLS_SetTextHAlignExType) (INT32 nHorizon);
typedef INT32 (*ASI_XLS_SetTextVAlignExType) (INT32 nVertical);
typedef INT32 (*ASI_XLS_SetWrapTextExType) (BOOL bWrap);
typedef INT32 (*ASI_XLS_SetMergeCellExType) (INT32 nCol1, INT32 nRow1, INT32 nCol2, INT32 nRow2, BOOL bMerge);
typedef INT32 (*ASI_XLS_AutoFitColumnExType) (INT32 nCol1, INT32 nCol2);
typedef INT32 (*ASI_XLS_SetFormulaExType) (INT32 nCol, INT32 nRow, LPCTSTR pFormula);
typedef INT32 (*ASI_XLS_SetLineStyleExType) (INT32 nType);
typedef INT32 (*ASI_XLS_SetLineWeightExType) (INT32 nWeight);
typedef INT32 (*ASI_XLS_SetLineColorExType) (INT32 nColor);
typedef INT32 (*ASI_XLS_SetLineEdgeExType) (INT32 nEdge);
typedef void  (*ASI_XLS_SampleType) ();
typedef INT32 (*ASI_XLS_AttachChartType) (INT32 nType, LPCTSTR pTitle, LPCTSTR pCategoryTitle, LPCTSTR pValueTitle, INT32 nXPos, INT32 nYPos, INT32 nWidth, INT32 nHeight);
typedef INT32 (*ASI_XLS_GetValueStrExType) (INT32 nCol, INT32 nRow, char *pBuf);
typedef INT32 (*ASI_XLS_GetValueIntExType) (INT32 nCol, INT32 nRow, INT32 &nBuf);
typedef INT32 (*ASI_XLS_GetRowCountType) ();
typedef INT32 (*ASI_XLS_GetColumnCountType) ();
typedef INT32 (*ASI_XLS_GetColumnWidthType) (INT32 nCol1, INT32 nCol2, INT32& nWidth);
typedef INT32 (*ASI_XLS_GetRowHeightType) (INT32 nRow1, INT32 nRow2, INT32& nHeight);
//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_EXCEL_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
