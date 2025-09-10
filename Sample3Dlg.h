/////////////////////////////////////////////////////////////////////////////////
//
//  ̧�ٖ��́F	Sample3Dlg.h
//  ���@�@���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�_�C�A���O�N���X�̒�`
//  ���ӎ����F	
//
//  Copyright(C) 2003-2005 KEYENCE CORPORATION All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_)
#define AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wavelogger2.h"
/////////////////////////////////////////////////////////////////////////////
// CSample3Dlg �_�C�A���O

//////////////////////////////////////////////////////////////////////
//
//	�N���X���F	CSample3Dlg
//	���{�ꖼ�F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�_�C�A���O�N���X
//	��    ���F	���Z���s���T���v��
//				�E�w��t�@�C�����v���f�[�^��z��ŕ������Ď擾����
//				�E���ϒl�̔g�`�f�[�^���쐬����CSV�t�@�C���ɕۑ�����
//				�E�f�[�^���̏����65536�_�Ƃ���
//	���ӎ����F	
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

	// CSV�o�͊֐�
	long SaveCsvFile(LPCTSTR lpszFilePath, BOOL bShowError = FALSE);

	// ��X�e�b�v�������Ԃ̏o�͊֐�
	long NonStepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError = FALSE);

	// �X�e�b�v�������Ԃ̏o�͊֐�
	long StepResonseTImeOutput(const float* flowOut, const float* mfmOut, long lDataCnt, BOOL bShowError = FALSE);

	//
	void CopySelectedItemToClipboard();

	BOOL PreTranslateMessage(MSG* pMsg);

// �\�z
public:
	CSample3Dlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	virtual ~CSample3Dlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSample3Dlg)
	enum { IDD = IDD_SAMPLE3_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSample3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SAMPLE3DLG_H__1038627C_FDD4_4A61_9BDE_970D73B421A9__INCLUDED_)
