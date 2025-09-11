/////////////////////////////////////////////////////////////////////////////////
//
//  ﾌｧｲﾙ名称：	ResponceTimeOutputDlg.h
//  説　　明：	ダイアログクラスの定義
//  注意事項：	
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_)
#define AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wavelogger2.h"
/////////////////////////////////////////////////////////////////////////////
// CSample3Dlg ダイアログ

//////////////////////////////////////////////////////////////////////
//
//	クラス名：	CSample3Dlg
//	日本語名：	ダイアログクラス
//	説    明：	応答時間(ステップ、非ステップ)を出力するクラス
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
class CSample3Dlg : public CDialog
{
private:
	CWaveLogger2				m_WaveLogger;

protected:
	BOOL RunWaveLogger(COleException* pError = NULL);
	void ReleaseWaveLogger();

	BOOL CheckFile(LPCTSTR lpszFilePath);

	long GetMFCDataArray(CXdtDocument2* pXdtDoc, VARIANT& vntArray, float* flowOut, float* mfmOut);

	// CSV出力関数
	long SaveCsvFile(LPCTSTR lpszFilePath, BOOL bShowError = FALSE);

	// 非ステップ応答時間の出力関数
	long NonStepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError = FALSE);

	// ステップ応答時間の出力関数
	long StepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError = FALSE);

	//
	void GetCopyTarget(CListCtrl* cCtr, POSITION pos, CString& str);
	void CopySelectedItemToClipboard();
	void SetClipbored(CString str);

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int GetChannelData(CXdtDocument2* pXdtDoc, VARIANT& vntArray, int unitNo, float* out, int chNo, std::function<int()> onError, int* cnt);
	//DECLARE_MESSAGE_MAP()

// 構築
public:
	CSample3Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	virtual ~CSample3Dlg();

// ダイアログ データ
	//{{AFX_DATA(CSample3Dlg)
	enum { IDD = IDD_SAMPLE3_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSample3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSample3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowseBtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListOutput2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCpNonstep();
	afx_msg void OnBnClickedButtonCpStepUp();
	afx_msg void OnBnClickedButtonCpStepDwn();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_)
