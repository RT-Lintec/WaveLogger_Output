/////////////////////////////////////////////////////////////////////////////////
//
//  ̧�ٖ��́F	ResponceTimeOutput.h
//  ���@�@���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�A�v���P�[�V�����N���X�̒�`
//  ���ӎ����F	
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

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSample3App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Sample3.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

//////////////////////////////////////////////////////////////////////
//
//	�N���X���F	CSample3App
//	���{�ꖼ�F	�A�v���P�[�V�����N���X
//	��    ���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�A�v���P�[�V�����N���X
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
class CSample3App : public CWinApp
{
public:
	CSample3App();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSample3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CSample3App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SAMPLE3_H__25F22AB9_BF8F_43F5_87AE_C9390D814D8B__INCLUDED_)
