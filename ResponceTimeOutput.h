/////////////////////////////////////////////////////////////////////////////////
//
//  ﾌｧｲﾙ名称：	ResponceTimeOutput.h
//  説　　明：	オートメーションサーバ機能サンプル3アプリケーションクラスの定義
//  注意事項：	
//
//  Copyright(C) 2003-2005 KEYENCE CORPORATION All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLE3_H__25F22AB9_BF8F_43F5_87AE_C9390D814D8B__INCLUDED_)
#define AFX_SAMPLE3_H__25F22AB9_BF8F_43F5_87AE_C9390D814D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSample3App:
// このクラスの動作の定義に関しては Sample3.cpp ファイルを参照してください。
//

//////////////////////////////////////////////////////////////////////
//
//	クラス名：	CSample3App
//	日本語名：	アプリケーションクラス
//	説    明：	オートメーションサーバ機能サンプル3アプリケーションクラス
//	注意事項：	
//
//////////////////////////////////////////////////////////////////////
class CSample3App : public CWinApp
{
public:
	CSample3App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSample3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSample3App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLE3_H__25F22AB9_BF8F_43F5_87AE_C9390D814D8B__INCLUDED_)
