// ClassWizard によって自動生成された IDispatch ラップ クラス
/////////////////////////////////////////////////////////////////////////////
// CWaveLogger2 ラップ クラス

class CWaveLogger2 : public COleDispatchDriver
{
public:
	CWaveLogger2() {}		// COleDispatchDriver デフォルト コンストラクション
	CWaveLogger2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWaveLogger2(const CWaveLogger2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// アトリビュート
public:
	BOOL GetVisible();
	void SetVisible(BOOL);
	BOOL GetDisplayAlerts();
	void SetDisplayAlerts(BOOL);

// オペレーション
public:
	void Initialize();
	CString GetModuleName();
	CString GetModulePath();
	long SetIdentifier(long lIdentifier);
	long GetIdentifier();
	void Quit();
	long Start();
	void Stop();
	long Trigger();
	BOOL ResetAlarm();
	long GetUnitInfo(long lUnit);
	long GetState();
	long GetAlarm();
	void ActivateWindow(long lNum);
	long GetWindowCount();
	BOOL Adjust(long lUnitNum);
	BOOL AutoBalance(long lUnitNum, short nCh);
	long GetAdjustStatus();
	void CopyImage();
	void Copy();
	void Paste();
	long OpenFile(LPCTSTR lpszFilePath);
	long GetFileCount();
	long CloseActiveFile();
	long GetActiveFileType();
	BOOL JumpToMark(long lNum);
	LPDISPATCH GetActiveFile();
	BOOL ActivateFile(long lIndex);
	long GetErrorCode();
	long GetWindowType(long lZOrder);
	long ManualSendDataCan(long lUnitNum, long lId, long lFormat, long lSize, const VARIANT& ArrayData);
	long ManualRecvSetIdCan(long lUnitNum, long lId, long lFormat);
	long ManualRecvDataCan(long lUnitNum, const VARIANT& ArrayData);
};
/////////////////////////////////////////////////////////////////////////////
// CXdtDocument2 ラップ クラス

class CXdtDocument2 : public COleDispatchDriver
{
public:
	CXdtDocument2() {}		// COleDispatchDriver デフォルト コンストラクション
	CXdtDocument2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CXdtDocument2(const CXdtDocument2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// アトリビュート
public:

// オペレーション
public:
	long SaveFile(LPCTSTR lpszFilePath);
	long SaveSettingFile(LPCTSTR lpszFilePath);
	long SaveCsvFile(LPCTSTR lpszFilePath);
	long SaveCsvFileEx(LPCTSTR lpszFilePath, long lMode, long lVal, long lStartPos, long lEndPos);
	long CloseFile();
	CString GetFileTitle();
	CString GetFilePath();
	void SetTitle(LPCTSTR lpszTitle);
	CString GetTitle();
	void SetComment(LPCTSTR lpszComment);
	CString GetComment();
	long GetWindowCount();
	long GetWindowType(long lIndex);
	BOOL BringWindowToTop(long lIndex);
	long GetDataCount();
	float GetData(long lUnit, long lChannel, long lPos);
	float GetCurrentData(long lUnit, long lChannel);
	long GetRepeatCount();
	long GetAutosaveCount();
	void ClearData();
	long FindMaxValPos(long lUnit, long lChannel);
	long FindMinValPos(long lUnit, long lChannel);
	long FindEdgePos(long lUnit, long lChannel, long lEdge, float fVal, long lPos, long lMode);
	long FindLocalMaximum(long lUnit, long lChannel, long lPos, long lMode, long lOption);
	long FindLocalMinimum(long lUnit, long lChannel, long lPos, long lMode, long lOption);
	long GetMarkCount();
	BOOL AddMark(long lUnit, long lChannel, long lPos, LPCTSTR lpszComment, long lAll);
	BOOL DelMark(long lNum);
	CString GetMarkComment(long lNum);
	BOOL SetMarkComment(long lNum, LPCTSTR lpszComment);
	long GetMarkPos(long lNum);
	long GetMarkUnit(long lNum);
	long GetMarkChannel(long lNum);
	BOOL CreateNewWindow(long lType);
	BOOL CloseWindow(long lIndex);
	long GetArrayData(long lUnit, long lChannel, long lIndex, long lCount, const VARIANT& Array, long lArrayIndex);
	long SaveCsvSplitFile(LPCTSTR lpszFilePath, long lDiv, long lMode, long lVal, long lStartPos, long lEndPos);
	long GetArrayDataEx(long lUnit, long lChannel, long lIndex, long lCount, const VARIANT& ArrayData, long lArrayIndex, long* plVarType);
	double GetDataEx(long lUnit, long lChannel, long lPos, long* plVarType);
	double GetCurrentDataEx(long lUnit, long lChannel, long* plVarType);
};
