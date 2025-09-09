// ClassWizard によって自動生成された IDispatch ラップ クラス

#include "stdafx.h"
#include "wavelogger2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CWaveLogger2 プロパティ

BOOL CWaveLogger2::GetVisible()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CWaveLogger2::SetVisible(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CWaveLogger2::GetDisplayAlerts()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CWaveLogger2::SetDisplayAlerts(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CWaveLogger2 オペレーション

void CWaveLogger2::Initialize()
{
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString CWaveLogger2::GetModuleName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CWaveLogger2::GetModulePath()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::SetIdentifier(long lIdentifier)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lIdentifier);
	return result;
}

long CWaveLogger2::GetIdentifier()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CWaveLogger2::Quit()
{
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CWaveLogger2::Start()
{
	long result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CWaveLogger2::Stop()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CWaveLogger2::Trigger()
{
	long result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CWaveLogger2::ResetAlarm()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::GetUnitInfo(long lUnit)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit);
	return result;
}

long CWaveLogger2::GetState()
{
	long result;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::GetAlarm()
{
	long result;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CWaveLogger2::ActivateWindow(long lNum)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lNum);
}

long CWaveLogger2::GetWindowCount()
{
	long result;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CWaveLogger2::Adjust(long lUnitNum)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lUnitNum);
	return result;
}

BOOL CWaveLogger2::AutoBalance(long lUnitNum, short nCh)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lUnitNum, nCh);
	return result;
}

long CWaveLogger2::GetAdjustStatus()
{
	long result;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CWaveLogger2::CopyImage()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CWaveLogger2::Copy()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CWaveLogger2::Paste()
{
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CWaveLogger2::OpenFile(LPCTSTR lpszFilePath)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath);
	return result;
}

long CWaveLogger2::GetFileCount()
{
	long result;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::CloseActiveFile()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::GetActiveFileType()
{
	long result;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CWaveLogger2::JumpToMark(long lNum)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lNum);
	return result;
}

LPDISPATCH CWaveLogger2::GetActiveFile()
{
	LPDISPATCH result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL CWaveLogger2::ActivateFile(long lIndex)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lIndex);
	return result;
}

long CWaveLogger2::GetErrorCode()
{
	long result;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CWaveLogger2::GetWindowType(long lZOrder)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lZOrder);
	return result;
}

long CWaveLogger2::ManualSendDataCan(long lUnitNum, long lId, long lFormat, long lSize, const VARIANT& ArrayData)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnitNum, lId, lFormat, lSize, &ArrayData);
	return result;
}

long CWaveLogger2::ManualRecvSetIdCan(long lUnitNum, long lId, long lFormat)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnitNum, lId, lFormat);
	return result;
}

long CWaveLogger2::ManualRecvDataCan(long lUnitNum, const VARIANT& ArrayData)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnitNum, &ArrayData);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CXdtDocument2 プロパティ

/////////////////////////////////////////////////////////////////////////////
// CXdtDocument2 オペレーション

long CXdtDocument2::SaveFile(LPCTSTR lpszFilePath)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath);
	return result;
}

long CXdtDocument2::SaveSettingFile(LPCTSTR lpszFilePath)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath);
	return result;
}

long CXdtDocument2::SaveCsvFile(LPCTSTR lpszFilePath)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath);
	return result;
}

long CXdtDocument2::SaveCsvFileEx(LPCTSTR lpszFilePath, long lMode, long lVal, long lStartPos, long lEndPos)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath, lMode, lVal, lStartPos, lEndPos);
	return result;
}

long CXdtDocument2::CloseFile()
{
	long result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CXdtDocument2::GetFileTitle()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CXdtDocument2::GetFilePath()
{
	CString result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXdtDocument2::SetTitle(LPCTSTR lpszTitle)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lpszTitle);
}

CString CXdtDocument2::GetTitle()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CXdtDocument2::SetComment(LPCTSTR lpszComment)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lpszComment);
}

CString CXdtDocument2::GetComment()
{
	CString result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CXdtDocument2::GetWindowCount()
{
	long result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CXdtDocument2::GetWindowType(long lIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lIndex);
	return result;
}

BOOL CXdtDocument2::BringWindowToTop(long lIndex)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lIndex);
	return result;
}

long CXdtDocument2::GetDataCount()
{
	long result;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

float CXdtDocument2::GetData(long lUnit, long lChannel, long lPos)
{
	float result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
		lUnit, lChannel, lPos);
	return result;
}

float CXdtDocument2::GetCurrentData(long lUnit, long lChannel)
{
	float result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
		lUnit, lChannel);
	return result;
}

long CXdtDocument2::GetRepeatCount()
{
	long result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CXdtDocument2::GetAutosaveCount()
{
	long result;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CXdtDocument2::ClearData()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CXdtDocument2::FindMaxValPos(long lUnit, long lChannel)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel);
	return result;
}

long CXdtDocument2::FindMinValPos(long lUnit, long lChannel)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel);
	return result;
}

long CXdtDocument2::FindEdgePos(long lUnit, long lChannel, long lEdge, float fVal, long lPos, long lMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_R4 VTS_I4 VTS_I4;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel, lEdge, fVal, lPos, lMode);
	return result;
}

long CXdtDocument2::FindLocalMaximum(long lUnit, long lChannel, long lPos, long lMode, long lOption)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel, lPos, lMode, lOption);
	return result;
}

long CXdtDocument2::FindLocalMinimum(long lUnit, long lChannel, long lPos, long lMode, long lOption)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel, lPos, lMode, lOption);
	return result;
}

long CXdtDocument2::GetMarkCount()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CXdtDocument2::AddMark(long lUnit, long lChannel, long lPos, LPCTSTR lpszComment, long lAll)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lUnit, lChannel, lPos, lpszComment, lAll);
	return result;
}

BOOL CXdtDocument2::DelMark(long lNum)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lNum);
	return result;
}

CString CXdtDocument2::GetMarkComment(long lNum)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		lNum);
	return result;
}

BOOL CXdtDocument2::SetMarkComment(long lNum, LPCTSTR lpszComment)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lNum, lpszComment);
	return result;
}

long CXdtDocument2::GetMarkPos(long lNum)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lNum);
	return result;
}

long CXdtDocument2::GetMarkUnit(long lNum)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lNum);
	return result;
}

long CXdtDocument2::GetMarkChannel(long lNum)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lNum);
	return result;
}

BOOL CXdtDocument2::CreateNewWindow(long lType)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lType);
	return result;
}

BOOL CXdtDocument2::CloseWindow(long lIndex)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lIndex);
	return result;
}

long CXdtDocument2::GetArrayData(long lUnit, long lChannel, long lIndex, long lCount, const VARIANT& Array, long lArrayIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT VTS_I4;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel, lIndex, lCount, &Array, lArrayIndex);
	return result;
}

long CXdtDocument2::SaveCsvSplitFile(LPCTSTR lpszFilePath, long lDiv, long lMode, long lVal, long lStartPos, long lEndPos)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszFilePath, lDiv, lMode, lVal, lStartPos, lEndPos);
	return result;
}

long CXdtDocument2::GetArrayDataEx(long lUnit, long lChannel, long lIndex, long lCount, const VARIANT& ArrayData, long lArrayIndex, long* plVarType)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT VTS_I4 VTS_PI4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lUnit, lChannel, lIndex, lCount, &ArrayData, lArrayIndex, plVarType);
	return result;
}

double CXdtDocument2::GetDataEx(long lUnit, long lChannel, long lPos, long* plVarType)
{
	double result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		lUnit, lChannel, lPos, plVarType);
	return result;
}

double CXdtDocument2::GetCurrentDataEx(long lUnit, long lChannel, long* plVarType)
{
	double result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		lUnit, lChannel, plVarType);
	return result;
}
