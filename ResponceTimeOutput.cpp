/////////////////////////////////////////////////////////////////////////////////
//
//  ̧�ٖ��́F	Sample3.cpp
//  ���@�@���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�A�v���P�[�V�����N���X�̓����`
//  ���ӎ����F	
//
//  Copyright(C) 2003-2005 KEYENCE CORPORATION All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResponceTimeOutput.h"
#include "ResponceTimeOutputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSample3App

//////////////////////////////////////////////////////////////////////
//
//	���b�Z�[�W�}�b�v
//
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSample3App, CWinApp)
	//{{AFX_MSG_MAP(CSample3App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample3App �N���X�̍\�z

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	CSample3App
//	���{�ꖼ�F	�R���X�g���N�^
//	��    ���F	�R���X�g���N�^
//	��    ���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
CSample3App::CSample3App()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSample3App �I�u�W�F�N�g

CSample3App theApp;

/////////////////////////////////////////////////////////////////////////////
// CSample3App �N���X�̏�����

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	InitInstance
//	���{�ꖼ�F	�C���X�^���X������
//	��    ���F	�A�v���P�[�V�����̏��������s��
//	��    ���F	�Ȃ�
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
BOOL CSample3App::InitInstance()
{
	// OLE DLL ��������
	if (AfxOleInit() == FALSE) {
		AfxMessageBox(IDS_ERR_OLE_INIT, MB_ICONSTOP);
		return FALSE;
	}

	AfxEnableControlContainer();

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	CSample3Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
