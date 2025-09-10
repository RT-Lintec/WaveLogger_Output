/////////////////////////////////////////////////////////////////////////////////
//
//  ﾌｧｲﾙ名称：	Sample3Dlg.cpp
//  説　　明：	オートメーションサーバ機能サンプル3ダイアログクラスの動作定義
//  注意事項：	
//
//  Copyright(C) 2003-2005 KEYENCE CORPORATION All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sample3.h"
#include "Sample3Dlg.h"
#include <cmath>

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define UNITID_NULL				(0x00)	// 無効値
#define UNITID_TH				(0x81)	// THユニット
#define UNITID_HA				(0x82)	// HAユニット
#define UNITID_ST				(0x83)	// STユニット

#define	MSR_UNITNUM_MIN			1		// 計測ユニット番号最小値
#define	MSR_UNITNUM_MAX			8		// 計測ユニット番号最大値

#define	TH_CHNUM_MAX			8		// THユニット最大チャンネル数
#define	HA_CHNUM_MAX			12		// HAユニット最大チャンネル数
#define	ST_CHNUM_MAX			4		// STユニット最大チャンネル数

#define GET_CHDATA_MAX			1024	// 1チャンネルの波形データ取得最大数

#define MAX_ARRAY_SIZE			1048576 // データ最大数
#define SUBSTRUCT_THRESHOLD	0.05f
#define PARAM_SIZE				12		// param数
#define SAMPLE_INTERVAL			4000	// 通常サンプル時間
#define LONG_SAMPLE_INTERVAL	10000	// 2%上昇時サンプル時間
#define FIFTY_CHECK_INTERVAL	50		// 50msecカウント用
#define ARRAY_OFFSET			10		// FlowOutの現在位置からのオフセット(立ち上がり時にのみ利用するが、大きさに注意)

int nonStepResult[PARAM_SIZE * 2]; // 12*2通りのデータ数
int stepUpResult[PARAM_SIZE]; // ステップ上昇時のデータ
int stepDwnResult[PARAM_SIZE]; // ステップ下昇時のデータ

CListCtrl* m_listOutput_NonStep;
CListCtrl* m_listOutput_StepUp;
CListCtrl* m_listOutput_StepDwn;
// 出力モードのラジオボタンローカルポインタ
CButton* pRadio_NonStep;
CButton* pRadio_Step;

float roundTo(float value, int digits);

/// <summary>
/// 非ステップパラメータ
/// </summary>
struct thresholdSt_NonStep
{
	float volt[13] = { 5.0f, 4.5f, 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.25f, 0.1f, 0.0f };
	float threshold[13] = { 0.1f ,0.09f ,0.08f ,0.07f ,0.06f ,0.05f ,0.04f ,0.03f ,0.025f ,0.025f ,0.025f ,0.025f, 0.025f };
	int settingPercentage[13] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 5, 2, 0};
};
thresholdSt_NonStep threshold_NonStep;

/// <summary>
/// ステップパラメータ
/// </summary>
struct thresholdSt_Step
{
	float volt_Up[12] = { 0.1f, 0.25f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f };
	float volt_Dwn[12] = { 4.5f, 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.25f, 0.1f, 0.0f };
	float threshold_Up[12] = { 0.025f ,0.025f ,0.025f, 0.025f, 0.03f, 0.04f, 0.05f, 0.06f, 0.07f, 0.08f, 0.09f, 0.1f };
	float threshold_Dwn[12] = { 0.09f ,0.08f ,0.07f ,0.06f ,0.05f ,0.04f ,0.03f ,0.025f, 0.025f ,0.025f ,0.025f, 0.025f };
	int settingUpPercentage[12] = { 2, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
	int settingDwnPercentage[12] = { 90, 80, 70, 60, 50, 40, 30, 20, 10, 5, 2, 0 };

};
thresholdSt_Step threshold_Step;

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

//////////////////////////////////////////////////////////////////////
//
//	クラス名：	CAboutDlg
//	日本語名：	バージョン情報ダイアログクラス
//	説    明：	バージョン情報を表示する
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	CAboutDlg
//	日本語名：	コンストラクタ
//	説    明：	コンストラクタ
//	引    数：	なし
//	戻 り 値：	なし
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	DoDataExchange
//	日本語名：  ダイアログデータ交換
//	説    明：	ダイアログデータ交換と正当性評価のために呼び出される
//	引    数：	デフォルト
//	戻 り 値：	デフォルト
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////
//
//	メッセージマップ
//
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample3Dlg ダイアログ

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	CSample3Dlg
//	日本語名：	コンストラクタ
//	説    明：	コンストラクタ
//	引    数：	なし
//	戻 り 値：	なし
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
CSample3Dlg::CSample3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSample3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSample3Dlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	~CSample3Dlg
//	日本語名：	デストラクタ
//	説    明：	デストラクタ
//	引    数：	なし
//	戻 り 値：	なし
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
CSample3Dlg::~CSample3Dlg()
{
	ReleaseWaveLogger();
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	DoDataExchange
//	日本語名：  ダイアログデータ交換
//	説    明：	ダイアログデータ交換と正当性評価のために呼び出される
//	引    数：	デフォルト
//	戻 り 値：	デフォルト
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
void CSample3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSample3Dlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////
//
//	メッセージマップ
//
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSample3Dlg, CDialog)
	//{{AFX_MSG_MAP(CSample3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE_BTN, OnBrowseBtn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO1, &CSample3Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSample3Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &CSample3Dlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample3Dlg メッセージ ハンドラ

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnInitDialog
//	日本語名：  ダイアログ初期化処理
//	説    明：	ダイアログ内コントロール初期表示
//	引    数：	なし
//	戻 り 値：	デフォルト
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
BOOL CSample3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	// 説明文ロード
	CString strSummary, strRemarks;
	strSummary.LoadString(IDS_SUMMARY);
	strRemarks.LoadString(IDS_REMARKS);
	strSummary += _T("\n\n");
	strSummary += strRemarks;
	SetDlgItemText(IDC_SUMMARY_STA, strSummary);

	// WAVE LOGGER 起動
	if (RunWaveLogger() == FALSE) {
		AfxMessageBox(IDS_ERR_RUN_WAVELOGGER, MB_ICONSTOP);
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnSysCommand
//	日本語名：	システムメニュー選択時処理
//	説　　明：	バージョン情報の表示等を行う
//	引　　数：	デフォルト
//	戻 り 値：	なし
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnPaint
//	日本語名：	描画処理処理
//	説　　明：	もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
//				コードを以下に記述する必要があります。MFC アプリケーションは document/view
//				モデルを使っているので、この処理はフレームワークにより自動的に処理されます。
//	引　　数：	なし
//	戻 り 値：	なし
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnQueryDragIcon
//	日本語名：	最小化ウィンドウドラッグ時処理
//	説　　明：	システムは、ユーザーが最小化ウィンドウをドラッグしている間、
//				カーソルを表示するためにここを呼び出します。
//	引　　数：	なし
//	戻 り 値：	デフォルト
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
HCURSOR CSample3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnBrowseBtn
//	日本語名：	参照ボタン押下時処理
//	説    明：	ファイル参照ダイアログを表示する
//	引    数：	デフォルト
//	戻 り 値：	デフォルト
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnBrowseBtn() 
{
	// フィルタ文字列作成
	CString strFilter, strFilterXdt;
	strFilterXdt.LoadString(IDS_FILEFILTER_XDT);
	strFilter.Format(_T("%s||"), strFilterXdt);

	// ファイル参照ダイアログ表示
	CFileDialog fdlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST, strFilter, this);
	if (fdlg.DoModal() == IDOK) {
		SetDlgItemText(IDC_FILE_EDIT, fdlg.GetPathName());
	}
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	OnOK
//	日本語名：	実行ボタン押下時処理
//	説    明：	平均値の波形データ作成処理を行う
//	引    数：	デフォルト
//	戻 り 値：	デフォルト
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnOK() 
{
	CWaitCursor WaitCursor;
	CButton* pRadio_NonStep = (CButton*)GetDlgItem(IDC_RADIO1);
	CButton* pRadio_Step = (CButton*)GetDlgItem(IDC_RADIO2);

	// ファイルパスチェック
	CString strFilePath;
	GetDlgItemText(IDC_FILE_EDIT, strFilePath);
	if (CheckFile(strFilePath) == FALSE) {
		AfxMessageBox(IDS_ERR_FILEPATH, MB_ICONEXCLAMATION);
		return;
	}

	// ドキュメントインターフェース取得
	if (m_WaveLogger.OpenFile(strFilePath) != 0) {
		AfxMessageBox(IDS_ERR_OPENFILE, MB_ICONEXCLAMATION);
		return;
	}
	CXdtDocument2 XdtDoc;
	XdtDoc.AttachDispatch(m_WaveLogger.GetActiveFile());

	// 波形データ配列取得用バッファ確保
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = GET_CHDATA_MAX * HA_CHNUM_MAX;
	SAFEARRAY* psa = SafeArrayCreate(VT_R4, 1, rgsabound);
	if (psa == NULL) {
		AfxMessageBox(IDS_ERR_MEMORY, MB_ICONEXCLAMATION);
		return;
	}

	// 各バッファ確保
	float* flowOut = new float[MAX_ARRAY_SIZE];
	float* mfmOut = new float[MAX_ARRAY_SIZE];

	VARIANT vntArray;
	vntArray.vt = VT_ARRAY | VT_R4 | VT_BYREF;
	vntArray.pparray = &psa;

	// 対象データ取得
	long lDataCnt = GetMFCDataArray(&XdtDoc, vntArray, flowOut, mfmOut);
	XdtDoc.CloseFile();

	// 波形データ配列取得用バッファ解放
	SafeArrayDestroy(psa);

	// 非ステップモード
	if (pRadio_NonStep->GetCheck())
	{
		if (lDataCnt > 0) {
			// 応答時間データ作成
			NonStepResonseTImeOutput(flowOut, mfmOut, lDataCnt, TRUE);

			// リストコントロール1に出力
			for (int i = 0; i < sizeof(nonStepResult) / sizeof(nonStepResult[0]); i++)
			{
				CString str;
				str.Format(_T("%d"), i);
				int index = m_listOutput_NonStep->InsertItem(i, str); // i番目に挿入

				CString strValue;
				strValue.Format(_T("%.3f"), (float)(nonStepResult[i])/1000);  // 小数点3桁まで表示（必要に応じて調整）

				m_listOutput_NonStep->SetItemText(index, 1, strValue);

				//m_listOutput_NonStep->InsertColumn(0, _T("出力設定(%)"), LVCFMT_LEFT, 80);
				//m_listOutput_NonStep->InsertColumn(1, _T("出力時間(msec)"), LVCFMT_LEFT, 100);
			}


			//// CSVファイルに保存
			//CString strCsvFile(strFilePath, strFilePath.ReverseFind(_T('.')));
			//strCsvFile += _T(".csv");
			//if (SaveCsvFile(strCsvFile, TRUE) == ERROR_SUCCESS) {
			//	// 正常終了
			//	AfxMessageBox(IDS_COMPLETE_AVERAGE, MB_ICONINFORMATION);
			//}
		}
		else if (lDataCnt == 0) {
			// 波形データなし
			AfxMessageBox(IDS_ERR_NODATA, MB_ICONEXCLAMATION);
		}
		else {
			// エラー
			AfxMessageBox(IDS_ERR_GETWAVEDATA, MB_ICONEXCLAMATION);
		}
	}
	// ステップモード
	else if (pRadio_Step->GetCheck())
	{
		if (lDataCnt > 0) {
			// 応答時間データ作成
			StepResonseTImeOutput(flowOut, mfmOut, lDataCnt, TRUE);

			// リストコントロール2に出力


			//// CSVファイルに保存
			//CString strCsvFile(strFilePath, strFilePath.ReverseFind(_T('.')));
			//strCsvFile += _T(".csv");
			//if (SaveCsvFile(strCsvFile, TRUE) == ERROR_SUCCESS) {
			//	// 正常終了
			//	AfxMessageBox(IDS_COMPLETE_AVERAGE, MB_ICONINFORMATION);
			//}
		}
		else if (lDataCnt == 0) {
			// 波形データなし
			AfxMessageBox(IDS_ERR_NODATA, MB_ICONEXCLAMATION);
		}
		else {
			// エラー
			AfxMessageBox(IDS_ERR_GETWAVEDATA, MB_ICONEXCLAMATION);
		}
	}

	// 平均値用バッファバッファ解放
	delete [] flowOut;
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	RunWaveLogger
//	日本語名：	WAVE LOGGER起動
//	説    明：	WAVE LOGGER PROを起動する
//	引    数：	pError		OLE 例外オブジェクトへのポインタ
//	戻 り 値：	TRUE：成功、FALSE：失敗
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
BOOL CSample3Dlg::RunWaveLogger(COleException* pError /*= NULL*/)
{
	if (m_WaveLogger.CreateDispatch(_T("WaveLogger2.Application")) == FALSE) {
		return FALSE;
	}
	m_WaveLogger.Initialize();
	m_WaveLogger.SetVisible(TRUE);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	ReleaseWaveLogger
//	日本語名：	WAVE LOGGER解放
//	説    明：	WAVE LOGGER PROを解放する
//	引    数：	なし
//	戻 り 値：	なし
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::ReleaseWaveLogger()
{
	if (m_WaveLogger.m_lpDispatch != NULL) {
		m_WaveLogger.ReleaseDispatch();
	}
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	CheckFile
//	日本語名：	ファイル存在チェック
//	説    明：	指定ファイルが存在するかどうかチェックする
//	引    数：	lpszFilePath		ファイルパス
//	戻 り 値：	TRUE：存在する、FALSE：存在しない
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
BOOL CSample3Dlg::CheckFile(LPCTSTR lpszFilePath)
{
	// ファイルパスのチェック
	if (lpszFilePath == NULL) {
		return FALSE;
	}
	DWORD dwAttr = ::GetFileAttributes(lpszFilePath);
	if (dwAttr == 0xFFFFFFFF) {
		// 不正なパス（存在しない）
		return FALSE;
	}
	else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
		// 不正なパス（フォルダ）
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	GetMFCDataArray
//	日本語名：	測定値取得
//	説    明：	ユニット１のチャンネル１～最後までのデータを取得、
//				FlowOut(ch1-0)とMFMOut(ch1-2)のデータを作成する
//	引    数：	pXdtDoc		ドキュメント操作用クラスへのポインタ
//				vntArray	波形データ取得用のSAFEARRAY
//				flowOut	測定値(ch1-0)
//				mfmOut  測定値(ch1-2)
//	戻 り 値：	取得データ数、エラーの場合は負数
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
long CSample3Dlg::GetMFCDataArray(CXdtDocument2* pXdtDoc, VARIANT& vntArray, float* flowOut, float* mfmOut)
{
	// ユニット１のチャンネル１～のデータを1024個ずつ取得
	BOOL abValidChannel[HA_CHNUM_MAX];
	long lDataMax = min(MAX_ARRAY_SIZE, pXdtDoc->GetDataCount());
	long lDataCnt = 0;
	for (lDataCnt = 0; lDataCnt < lDataMax; ) {
		// ユニット１のチャンネル１～のデータを取得
		long lGetCount = -1;
		for (long lChannel = 0; lChannel < HA_CHNUM_MAX; lChannel++) {
			long lResult = pXdtDoc->GetArrayData(1, lChannel, lDataCnt, GET_CHDATA_MAX,
				vntArray, GET_CHDATA_MAX * lChannel);
			if (lResult > 0) {
				abValidChannel[lChannel] = TRUE;
				lGetCount = lResult;
			}
			else {
				abValidChannel[lChannel] = FALSE;
			}
		}
		if (lGetCount < 0) {
			return -1;
		}

		// FlowOut(0), MFMOut(2) 
		float* pfData;
		SafeArrayAccessData(*vntArray.pparray, (void**)&pfData);
		for (long lIndex = 0; lIndex < lGetCount; lIndex++, lDataCnt++) {
			flowOut[lDataCnt] = 0.;
			for (long lChannel = 0; lChannel < HA_CHNUM_MAX; lChannel++) {
				// flowOut
				if (lChannel == 0)
				{
					// 無効値は飛ばす
					if (abValidChannel[lChannel] && pfData[GET_CHDATA_MAX * lChannel + lIndex] != FLT_MAX) {
						flowOut[lDataCnt] = pfData[GET_CHDATA_MAX * lChannel + lIndex];
					}
				}

				// mfmOut
				if (lChannel == 2)
				{
					// 無効値は飛ばす
					if (abValidChannel[lChannel] && pfData[GET_CHDATA_MAX * lChannel + lIndex] != FLT_MAX) {
						mfmOut[lDataCnt] = pfData[GET_CHDATA_MAX * lChannel + lIndex];
					}
				}
			}
		}

		SafeArrayUnaccessData(*vntArray.pparray);

		if (lGetCount < GET_CHDATA_MAX) {
			break;
		}
	} // for (long lDataCnt = 0; lDataCnt < lDataMax; )

	return lDataCnt;
}

/// <summary>
/// 非ステップ応答時間の出力関数
/// </summary>
/// <param name="flowOut"></param>
/// <param name="mfmOut"></param>
/// <param name="lDataCnt"></param>
/// <param name="bShowError"></param>
/// <returns></returns>
long CSample3Dlg::NonStepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError /*= FALSE*/)
{
	long lResult = ERROR_SUCCESS;
	bool isCount = false;
	bool isUp = false, isDwn = false;
	bool isNoise = false;
	int thresholdCnt = 0;
	int fiftyCheckCnt = 0;
	int dataCnt = 0;
	int millSec = 0;
	int pastMSec = 0;
	float upperLim = 0.0f;
	float lowerLim = 0.0f;

	// flowOut, mfmOut値書き込み
	TCHAR szWork[32];
	for (long lIndex = 0; lIndex < lDataCnt; lIndex++) {
		// !EOF
		if (lIndex != lDataCnt - 1)
		{
			// flowOutで判定
			float flowSub = 0;
			flowSub = flowOut[lIndex + 1] - flowOut[lIndex];

			// 上昇カウント開始
			if (flowSub > SUBSTRUCT_THRESHOLD && !isDwn && !isCount)
			{
				if (isNoise)
				{
					isNoise = false;
					if (dataCnt % 2 == 1)
					{
						dataCnt--;
					}
				}
				// flowOutのノイズ(スパイク)判定
				else if ((flowOut[lIndex + ARRAY_OFFSET] <= threshold_NonStep.volt[thresholdCnt] + threshold_NonStep.threshold[thresholdCnt]
					&& flowOut[lIndex + ARRAY_OFFSET] >= threshold_NonStep.volt[thresholdCnt] - threshold_NonStep.threshold[thresholdCnt]))
				{
					isDwn = false;
					isUp = true;
					isCount = true;
					pastMSec = 0;
					millSec = 0;
					upperLim = threshold_NonStep.volt[thresholdCnt] + threshold_NonStep.threshold[thresholdCnt];
					lowerLim = threshold_NonStep.volt[thresholdCnt] - threshold_NonStep.threshold[thresholdCnt];
					fiftyCheckCnt = 0;
				}
				else
				{
					isNoise = true;
				}
			}
			// 下降カウント開始
			// dataCnt % 2 == 1は下降スパイク判定
			else if (flowSub < -SUBSTRUCT_THRESHOLD && !isUp && !isCount)
			{
				if (isNoise)
				{
					isNoise = false;
					if (dataCnt % 2 == 1)
					{
						dataCnt--;
					}
				}
				// mfmOutのノイズ(スパイク)判定
				else if ((flowOut[lIndex - ARRAY_OFFSET] <= threshold_NonStep.volt[thresholdCnt] + threshold_NonStep.threshold[thresholdCnt]
					&& flowOut[lIndex - ARRAY_OFFSET] >= threshold_NonStep.volt[thresholdCnt] - threshold_NonStep.threshold[thresholdCnt]))
				{
					isUp = false;
					isDwn = true;
					isCount = true;
					pastMSec = 0;
					millSec = 0;
					upperLim = threshold_NonStep.volt[PARAM_SIZE] + threshold_NonStep.threshold[PARAM_SIZE];
					lowerLim = threshold_NonStep.volt[PARAM_SIZE] - threshold_NonStep.threshold[PARAM_SIZE];
					fiftyCheckCnt = 0;
				}
				else
				{
					isNoise = true;
				}
			}

			// カウント処理
			if (isCount)
			{
				// 計測時間経過
				pastMSec++;

				// 上昇時
				if (isUp && !isDwn)
				{
					// 閾値範囲に入ったらカウントしない
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 閾値範囲外ならmsecカウント
					else
					{
						fiftyCheckCnt++;
						// 50msec判定でNGならカウント
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 5%~100%且つ4000msec経過したらカウント終了
					if (dataCnt < (PARAM_SIZE - 1) * 2 && pastMSec > SAMPLE_INTERVAL)
					{
						isUp = false;
						isCount = false;
						nonStepResult[dataCnt] = millSec;
						dataCnt++;
						fiftyCheckCnt = 0;
					}
					// 2%且つ10000msec経過したらカウント終了
					else if (dataCnt == (PARAM_SIZE - 1) * 2 && pastMSec > LONG_SAMPLE_INTERVAL)
					{
						isUp = false;
						isCount = false;
						nonStepResult[dataCnt] = millSec;
						dataCnt++;
						fiftyCheckCnt = 0;
					}
				}
				// 下昇時
				else if (!isUp && isDwn)
				{
					// 閾値範囲に入ったらカウントしない
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 閾値範囲外ならmsecカウント
					else
					{
						fiftyCheckCnt++;
						// 50msec判定でNGならカウント
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 4000msec経過したらカウント終了
					if (pastMSec > SAMPLE_INTERVAL)
					{
						isDwn = false;
						isCount = false;
						nonStepResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt++;
						fiftyCheckCnt = 0;
					}
				}
			}
		}
		else
		{
			// 何もしない
		}
	}

	return lResult;
}

/// <summary>
/// ステップ応答時間の出力関数
/// </summary>
/// <param name="flowOut"></param>
/// <param name="mfmOut"></param>
/// <param name="lDataCnt"></param>
/// <param name="bShowError"></param>
/// <returns></returns>
long CSample3Dlg::StepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError /*= FALSE*/)
{
	long lResult = ERROR_SUCCESS;
	bool isCount = false;
	bool isUp, isDwn = false;
	int thresholdCnt_Up = 0, thresholdCnt_Dwn = 0;
	int fiftyCheckCnt = 0;
	int dataCnt = 0;
	int millSec = 0;
	int pastMSec = 0;
	float upperLim = 0.0f;
	float lowerLim = 0.0f;

	// flowOut, mfmOut値書き込み
	TCHAR szWork[32];
	for (long lIndex = 0; lIndex < lDataCnt; lIndex++) {
		// !EOF
		if (lIndex != lDataCnt - 1)
		{
			// flowOutで判定
			float flowSub = 0;
			flowSub = flowOut[lIndex + 1] - flowOut[lIndex];

			// 上昇カウント開始
			if (flowSub > SUBSTRUCT_THRESHOLD && !isDwn && !isCount)
			{
				isDwn = false;
				isUp = true;
				isCount = true;
				pastMSec = 0;
				millSec = 0;
				upperLim = threshold_Step.volt_Up[thresholdCnt_Up] + threshold_Step.threshold_Up[thresholdCnt_Up];
				upperLim = roundTo(upperLim, 6);
				lowerLim = threshold_Step.volt_Up[thresholdCnt_Up] - threshold_Step.threshold_Up[thresholdCnt_Up];
				lowerLim = roundTo(lowerLim, 6);
				fiftyCheckCnt = 0;
			}
			// 下降カウント開始
			else if (flowSub < -SUBSTRUCT_THRESHOLD && !isUp && !isCount)
			{
				isUp = false;
				isDwn = true;
				isCount = true;
				pastMSec = 0;
				millSec = 0;
				upperLim = threshold_Step.volt_Dwn[thresholdCnt_Dwn] + threshold_Step.threshold_Dwn[thresholdCnt_Dwn];
				upperLim = roundTo(upperLim, 6);
				lowerLim = threshold_Step.volt_Dwn[thresholdCnt_Dwn] - threshold_Step.threshold_Dwn[thresholdCnt_Dwn];
				lowerLim = roundTo(lowerLim, 6);
				fiftyCheckCnt = 0;
			}

			// カウント処理
			if (isCount)
			{
				// 計測時間経過
				pastMSec++;

				// 上昇時
				if (isUp && !isDwn)
				{
					// 閾値範囲に入ったらカウントしない
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 閾値範囲外ならmsecカウント
					else
					{
						fiftyCheckCnt++;
						// 50msec判定でNGならカウント
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 5%~100%且つ4000msec経過したらカウント終了
					if (dataCnt != 0 && pastMSec > SAMPLE_INTERVAL)
					{
						isCount = false;
						stepUpResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt_Up++;
						// 12回処理したら上昇完了
						if (dataCnt == 12)
						{
							isUp = false;
							dataCnt = 0;
						}
					}
					// 最初の2%且つ10000msec経過したらカウント終了
					else if (dataCnt == 0 && pastMSec > LONG_SAMPLE_INTERVAL)
					{
						isCount = false;
						stepUpResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt_Up++;
					}
				}
				// 下昇時
				else if (!isUp && isDwn)
				{
					// 閾値範囲に入ったらカウントしない
					//if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					if (roundTo(mfmOut[lIndex], 6) <= upperLim && roundTo(mfmOut[lIndex], 6) >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 閾値範囲外ならmsecカウント
					else
					{
						fiftyCheckCnt++;
						// 50msec判定でNGならカウント
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 4000msec経過したらカウント終了
					if (pastMSec > SAMPLE_INTERVAL)
					{
						isCount = false;
						stepDwnResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt_Dwn++;
					}
				}
			}
		}
		else
		{
			// 何もしない
		}
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////
//
//	関数名称：	SaveCsvFile
//	日本語名：	CSVファイル保存
//	説    明：	平均値の波形データをCSVファイルに保存する
//	引    数：	lpszFilePath	CSVファイルパス
//				flowOut		平均値の配列
//				lDataCnt		平均値の配列数
//				bShowError		エラーメッセージ表示フラグ
//	戻 り 値：	0：正常、その他：異常（Win32エラーコード参照）
//	注意事項：	
//
/////////////////////////////////////////////////////////////////////
long CSample3Dlg::SaveCsvFile(LPCTSTR lpszFilePath, BOOL bShowError /*= FALSE*/)
{
	long lResult = ERROR_SUCCESS;

	try {
		CString strInvalid;
		strInvalid.LoadString(IDS_INVALID_VALUE);

		// CSVファイル作成
		CStdioFile CsvFile(lpszFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);

		//if (flowOut[lIndex] != FLT_MAX) {
		//	_stprintf(szWork, _T("%f,%f\n"), flowOut[lIndex], mfmOut[lIndex]);
		//}
		//else {
		//	_stprintf(szWork, _T("%s\n"), strInvalid);
		//}
		//CsvFile.WriteString(szWork);

	}
	catch (CFileException* pFileEx) {
		if (bShowError) {
			pFileEx->ReportError();
		}
		lResult = pFileEx->m_lOsError;
		pFileEx->Delete();
	}

	return lResult;
}

// 小数点以下n桁で四捨五入
float roundTo(float value, int digits) {
	float factor = std::pow(10.0f, digits);
	return std::round(value * factor) / factor;
}


void CSample3Dlg::OnBnClickedRadio1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CSample3Dlg::OnBnClickedRadio2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


/// <summary>
/// 実行ボタンクリック時の操作
/// 実行前の設定が正しくなされているかをチェックする
/// </summary>
void CSample3Dlg::OnBnClickedOk()
{
	// 出力モードのラジオボタンローカルポインタ
	pRadio_NonStep = (CButton*)GetDlgItem(IDC_RADIO1);
	pRadio_Step = (CButton*)GetDlgItem(IDC_RADIO2);
	m_listOutput_NonStep = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT1);
	m_listOutput_StepUp = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT2);
	m_listOutput_StepDwn = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT3);

	// 出力モード選択チェック
	if (!pRadio_NonStep->GetCheck() && !pRadio_Step->GetCheck())
	{
		AfxMessageBox(_T("ラジオボタンから出力モードを選択してください。"));
	}
	else
	{
		// リストコントロール1の初期化
		if (pRadio_NonStep->GetCheck())
		{
			m_listOutput_NonStep->DeleteColumn(0);
			m_listOutput_NonStep->DeleteColumn(1);

			// 列追加
			m_listOutput_NonStep->InsertColumn(0, _T("出力設定(%)"), LVCFMT_LEFT, 80);
			m_listOutput_NonStep->InsertColumn(1, _T("出力時間(msec)"), LVCFMT_LEFT, 100);
		}
		// リストコントロール2の初期化
		else if (pRadio_Step->GetCheck())
		{
			m_listOutput_StepUp->DeleteColumn(0);
			m_listOutput_StepUp->DeleteColumn(1);
			m_listOutput_StepDwn->DeleteColumn(0);
			m_listOutput_StepDwn->DeleteColumn(1);

			// 列追加
			m_listOutput_StepUp->InsertColumn(0, _T("出力設定(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepUp->InsertColumn(1, _T("出力時間(msec)"), LVCFMT_LEFT, 100);
			m_listOutput_StepDwn->InsertColumn(0, _T("出力設定(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepDwn->InsertColumn(1, _T("出力時間(msec)"), LVCFMT_LEFT, 100);
		}

		CSample3Dlg::OnOK();
	}
}
