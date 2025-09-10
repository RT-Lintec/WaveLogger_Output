/////////////////////////////////////////////////////////////////////////////////
//
//  ̧�ٖ��́F	Sample3Dlg.cpp
//  ���@�@���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�_�C�A���O�N���X�̓����`
//  ���ӎ����F	
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


#define UNITID_NULL				(0x00)	// �����l
#define UNITID_TH				(0x81)	// TH���j�b�g
#define UNITID_HA				(0x82)	// HA���j�b�g
#define UNITID_ST				(0x83)	// ST���j�b�g

#define	MSR_UNITNUM_MIN			1		// �v�����j�b�g�ԍ��ŏ��l
#define	MSR_UNITNUM_MAX			8		// �v�����j�b�g�ԍ��ő�l

#define	TH_CHNUM_MAX			8		// TH���j�b�g�ő�`�����l����
#define	HA_CHNUM_MAX			12		// HA���j�b�g�ő�`�����l����
#define	ST_CHNUM_MAX			4		// ST���j�b�g�ő�`�����l����

#define GET_CHDATA_MAX			1024	// 1�`�����l���̔g�`�f�[�^�擾�ő吔

#define MAX_ARRAY_SIZE			1048576 // �f�[�^�ő吔
#define SUBSTRUCT_THRESHOLD	0.05f
#define PARAM_SIZE				12		// param��
#define SAMPLE_INTERVAL			4000	// �ʏ�T���v������
#define LONG_SAMPLE_INTERVAL	10000	// 2%�㏸���T���v������
#define FIFTY_CHECK_INTERVAL	50		// 50msec�J�E���g�p
#define ARRAY_OFFSET			10		// FlowOut�̌��݈ʒu����̃I�t�Z�b�g(�����オ�莞�ɂ̂ݗ��p���邪�A�傫���ɒ���)

int nonStepResult[PARAM_SIZE * 2]; // 12*2�ʂ�̃f�[�^��
int stepUpResult[PARAM_SIZE]; // �X�e�b�v�㏸���̃f�[�^
int stepDwnResult[PARAM_SIZE]; // �X�e�b�v�������̃f�[�^

CListCtrl* m_listOutput_NonStep;
CListCtrl* m_listOutput_StepUp;
CListCtrl* m_listOutput_StepDwn;
// �o�̓��[�h�̃��W�I�{�^�����[�J���|�C���^
CButton* pRadio_NonStep;
CButton* pRadio_Step;

float roundTo(float value, int digits);

/// <summary>
/// ��X�e�b�v�p�����[�^
/// </summary>
struct thresholdSt_NonStep
{
	float volt[13] = { 5.0f, 4.5f, 4.0f, 3.5f, 3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.25f, 0.1f, 0.0f };
	float threshold[13] = { 0.1f ,0.09f ,0.08f ,0.07f ,0.06f ,0.05f ,0.04f ,0.03f ,0.025f ,0.025f ,0.025f ,0.025f, 0.025f };
	int settingPercentage[13] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 5, 2, 0};
};
thresholdSt_NonStep threshold_NonStep;

/// <summary>
/// �X�e�b�v�p�����[�^
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
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

//////////////////////////////////////////////////////////////////////
//
//	�N���X���F	CAboutDlg
//	���{�ꖼ�F	�o�[�W�������_�C�A���O�N���X
//	��    ���F	�o�[�W��������\������
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	CAboutDlg
//	���{�ꖼ�F	�R���X�g���N�^
//	��    ���F	�R���X�g���N�^
//	��    ���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	DoDataExchange
//	���{�ꖼ�F  �_�C�A���O�f�[�^����
//	��    ���F	�_�C�A���O�f�[�^�����Ɛ������]���̂��߂ɌĂяo�����
//	��    ���F	�f�t�H���g
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
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
//	���b�Z�[�W�}�b�v
//
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample3Dlg �_�C�A���O

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	CSample3Dlg
//	���{�ꖼ�F	�R���X�g���N�^
//	��    ���F	�R���X�g���N�^
//	��    ���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
CSample3Dlg::CSample3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSample3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSample3Dlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	~CSample3Dlg
//	���{�ꖼ�F	�f�X�g���N�^
//	��    ���F	�f�X�g���N�^
//	��    ���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
CSample3Dlg::~CSample3Dlg()
{
	ReleaseWaveLogger();
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	DoDataExchange
//	���{�ꖼ�F  �_�C�A���O�f�[�^����
//	��    ���F	�_�C�A���O�f�[�^�����Ɛ������]���̂��߂ɌĂяo�����
//	��    ���F	�f�t�H���g
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
//////////////////////////////////////////////////////////////////////
void CSample3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSample3Dlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////
//
//	���b�Z�[�W�}�b�v
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
// CSample3Dlg ���b�Z�[�W �n���h��

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	OnInitDialog
//	���{�ꖼ�F  �_�C�A���O����������
//	��    ���F	�_�C�A���O���R���g���[�������\��
//	��    ���F	�Ȃ�
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
BOOL CSample3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�

	// ���������[�h
	CString strSummary, strRemarks;
	strSummary.LoadString(IDS_SUMMARY);
	strRemarks.LoadString(IDS_REMARKS);
	strSummary += _T("\n\n");
	strSummary += strRemarks;
	SetDlgItemText(IDC_SUMMARY_STA, strSummary);

	// WAVE LOGGER �N��
	if (RunWaveLogger() == FALSE) {
		AfxMessageBox(IDS_ERR_RUN_WAVELOGGER, MB_ICONSTOP);
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	OnSysCommand
//	���{�ꖼ�F	�V�X�e�����j���[�I��������
//	���@�@���F	�o�[�W�������̕\�������s��
//	���@�@���F	�f�t�H���g
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
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
//	�֐����́F	OnPaint
//	���{�ꖼ�F	�`�揈������
//	���@�@���F	�����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
//				�R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
//				���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B
//	���@�@���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	OnQueryDragIcon
//	���{�ꖼ�F	�ŏ����E�B���h�E�h���b�O������
//	���@�@���F	�V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
//				�J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
//	���@�@���F	�Ȃ�
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
HCURSOR CSample3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	OnBrowseBtn
//	���{�ꖼ�F	�Q�ƃ{�^������������
//	��    ���F	�t�@�C���Q�ƃ_�C�A���O��\������
//	��    ���F	�f�t�H���g
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnBrowseBtn() 
{
	// �t�B���^������쐬
	CString strFilter, strFilterXdt;
	strFilterXdt.LoadString(IDS_FILEFILTER_XDT);
	strFilter.Format(_T("%s||"), strFilterXdt);

	// �t�@�C���Q�ƃ_�C�A���O�\��
	CFileDialog fdlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_FILEMUSTEXIST, strFilter, this);
	if (fdlg.DoModal() == IDOK) {
		SetDlgItemText(IDC_FILE_EDIT, fdlg.GetPathName());
	}
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	OnOK
//	���{�ꖼ�F	���s�{�^������������
//	��    ���F	���ϒl�̔g�`�f�[�^�쐬�������s��
//	��    ���F	�f�t�H���g
//	�� �� �l�F	�f�t�H���g
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
void CSample3Dlg::OnOK() 
{
	CWaitCursor WaitCursor;
	CButton* pRadio_NonStep = (CButton*)GetDlgItem(IDC_RADIO1);
	CButton* pRadio_Step = (CButton*)GetDlgItem(IDC_RADIO2);

	// �t�@�C���p�X�`�F�b�N
	CString strFilePath;
	GetDlgItemText(IDC_FILE_EDIT, strFilePath);
	if (CheckFile(strFilePath) == FALSE) {
		AfxMessageBox(IDS_ERR_FILEPATH, MB_ICONEXCLAMATION);
		return;
	}

	// �h�L�������g�C���^�[�t�F�[�X�擾
	if (m_WaveLogger.OpenFile(strFilePath) != 0) {
		AfxMessageBox(IDS_ERR_OPENFILE, MB_ICONEXCLAMATION);
		return;
	}
	CXdtDocument2 XdtDoc;
	XdtDoc.AttachDispatch(m_WaveLogger.GetActiveFile());

	// �g�`�f�[�^�z��擾�p�o�b�t�@�m��
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = GET_CHDATA_MAX * HA_CHNUM_MAX;
	SAFEARRAY* psa = SafeArrayCreate(VT_R4, 1, rgsabound);
	if (psa == NULL) {
		AfxMessageBox(IDS_ERR_MEMORY, MB_ICONEXCLAMATION);
		return;
	}

	// �e�o�b�t�@�m��
	float* flowOut = new float[MAX_ARRAY_SIZE];
	float* mfmOut = new float[MAX_ARRAY_SIZE];

	VARIANT vntArray;
	vntArray.vt = VT_ARRAY | VT_R4 | VT_BYREF;
	vntArray.pparray = &psa;

	// �Ώۃf�[�^�擾
	long lDataCnt = GetMFCDataArray(&XdtDoc, vntArray, flowOut, mfmOut);
	XdtDoc.CloseFile();

	// �g�`�f�[�^�z��擾�p�o�b�t�@���
	SafeArrayDestroy(psa);

	// ��X�e�b�v���[�h
	if (pRadio_NonStep->GetCheck())
	{
		if (lDataCnt > 0) {
			// �������ԃf�[�^�쐬
			NonStepResonseTImeOutput(flowOut, mfmOut, lDataCnt, TRUE);

			// ���X�g�R���g���[��1�ɏo��
			for (int i = 0; i < sizeof(nonStepResult) / sizeof(nonStepResult[0]); i++)
			{
				CString str;
				str.Format(_T("%d"), i);
				int index = m_listOutput_NonStep->InsertItem(i, str); // i�Ԗڂɑ}��

				CString strValue;
				strValue.Format(_T("%.3f"), (float)(nonStepResult[i])/1000);  // �����_3���܂ŕ\���i�K�v�ɉ����Ē����j

				m_listOutput_NonStep->SetItemText(index, 1, strValue);

				//m_listOutput_NonStep->InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
				//m_listOutput_NonStep->InsertColumn(1, _T("�o�͎���(msec)"), LVCFMT_LEFT, 100);
			}


			//// CSV�t�@�C���ɕۑ�
			//CString strCsvFile(strFilePath, strFilePath.ReverseFind(_T('.')));
			//strCsvFile += _T(".csv");
			//if (SaveCsvFile(strCsvFile, TRUE) == ERROR_SUCCESS) {
			//	// ����I��
			//	AfxMessageBox(IDS_COMPLETE_AVERAGE, MB_ICONINFORMATION);
			//}
		}
		else if (lDataCnt == 0) {
			// �g�`�f�[�^�Ȃ�
			AfxMessageBox(IDS_ERR_NODATA, MB_ICONEXCLAMATION);
		}
		else {
			// �G���[
			AfxMessageBox(IDS_ERR_GETWAVEDATA, MB_ICONEXCLAMATION);
		}
	}
	// �X�e�b�v���[�h
	else if (pRadio_Step->GetCheck())
	{
		if (lDataCnt > 0) {
			// �������ԃf�[�^�쐬
			StepResonseTImeOutput(flowOut, mfmOut, lDataCnt, TRUE);

			// ���X�g�R���g���[��2�ɏo��


			//// CSV�t�@�C���ɕۑ�
			//CString strCsvFile(strFilePath, strFilePath.ReverseFind(_T('.')));
			//strCsvFile += _T(".csv");
			//if (SaveCsvFile(strCsvFile, TRUE) == ERROR_SUCCESS) {
			//	// ����I��
			//	AfxMessageBox(IDS_COMPLETE_AVERAGE, MB_ICONINFORMATION);
			//}
		}
		else if (lDataCnt == 0) {
			// �g�`�f�[�^�Ȃ�
			AfxMessageBox(IDS_ERR_NODATA, MB_ICONEXCLAMATION);
		}
		else {
			// �G���[
			AfxMessageBox(IDS_ERR_GETWAVEDATA, MB_ICONEXCLAMATION);
		}
	}

	// ���ϒl�p�o�b�t�@�o�b�t�@���
	delete [] flowOut;
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	RunWaveLogger
//	���{�ꖼ�F	WAVE LOGGER�N��
//	��    ���F	WAVE LOGGER PRO���N������
//	��    ���F	pError		OLE ��O�I�u�W�F�N�g�ւ̃|�C���^
//	�� �� �l�F	TRUE�F�����AFALSE�F���s
//	���ӎ����F	
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
//	�֐����́F	ReleaseWaveLogger
//	���{�ꖼ�F	WAVE LOGGER���
//	��    ���F	WAVE LOGGER PRO���������
//	��    ���F	�Ȃ�
//	�� �� �l�F	�Ȃ�
//	���ӎ����F	
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
//	�֐����́F	CheckFile
//	���{�ꖼ�F	�t�@�C�����݃`�F�b�N
//	��    ���F	�w��t�@�C�������݂��邩�ǂ����`�F�b�N����
//	��    ���F	lpszFilePath		�t�@�C���p�X
//	�� �� �l�F	TRUE�F���݂���AFALSE�F���݂��Ȃ�
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
BOOL CSample3Dlg::CheckFile(LPCTSTR lpszFilePath)
{
	// �t�@�C���p�X�̃`�F�b�N
	if (lpszFilePath == NULL) {
		return FALSE;
	}
	DWORD dwAttr = ::GetFileAttributes(lpszFilePath);
	if (dwAttr == 0xFFFFFFFF) {
		// �s���ȃp�X�i���݂��Ȃ��j
		return FALSE;
	}
	else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
		// �s���ȃp�X�i�t�H���_�j
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	GetMFCDataArray
//	���{�ꖼ�F	����l�擾
//	��    ���F	���j�b�g�P�̃`�����l���P�`�Ō�܂ł̃f�[�^���擾�A
//				FlowOut(ch1-0)��MFMOut(ch1-2)�̃f�[�^���쐬����
//	��    ���F	pXdtDoc		�h�L�������g����p�N���X�ւ̃|�C���^
//				vntArray	�g�`�f�[�^�擾�p��SAFEARRAY
//				flowOut	����l(ch1-0)
//				mfmOut  ����l(ch1-2)
//	�� �� �l�F	�擾�f�[�^���A�G���[�̏ꍇ�͕���
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
long CSample3Dlg::GetMFCDataArray(CXdtDocument2* pXdtDoc, VARIANT& vntArray, float* flowOut, float* mfmOut)
{
	// ���j�b�g�P�̃`�����l���P�`�̃f�[�^��1024���擾
	BOOL abValidChannel[HA_CHNUM_MAX];
	long lDataMax = min(MAX_ARRAY_SIZE, pXdtDoc->GetDataCount());
	long lDataCnt = 0;
	for (lDataCnt = 0; lDataCnt < lDataMax; ) {
		// ���j�b�g�P�̃`�����l���P�`�̃f�[�^���擾
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
					// �����l�͔�΂�
					if (abValidChannel[lChannel] && pfData[GET_CHDATA_MAX * lChannel + lIndex] != FLT_MAX) {
						flowOut[lDataCnt] = pfData[GET_CHDATA_MAX * lChannel + lIndex];
					}
				}

				// mfmOut
				if (lChannel == 2)
				{
					// �����l�͔�΂�
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
/// ��X�e�b�v�������Ԃ̏o�͊֐�
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

	// flowOut, mfmOut�l��������
	TCHAR szWork[32];
	for (long lIndex = 0; lIndex < lDataCnt; lIndex++) {
		// !EOF
		if (lIndex != lDataCnt - 1)
		{
			// flowOut�Ŕ���
			float flowSub = 0;
			flowSub = flowOut[lIndex + 1] - flowOut[lIndex];

			// �㏸�J�E���g�J�n
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
				// flowOut�̃m�C�Y(�X�p�C�N)����
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
			// ���~�J�E���g�J�n
			// dataCnt % 2 == 1�͉��~�X�p�C�N����
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
				// mfmOut�̃m�C�Y(�X�p�C�N)����
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

			// �J�E���g����
			if (isCount)
			{
				// �v�����Ԍo��
				pastMSec++;

				// �㏸��
				if (isUp && !isDwn)
				{
					// 臒l�͈͂ɓ�������J�E���g���Ȃ�
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 臒l�͈͊O�Ȃ�msec�J�E���g
					else
					{
						fiftyCheckCnt++;
						// 50msec�����NG�Ȃ�J�E���g
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 5%~100%����4000msec�o�߂�����J�E���g�I��
					if (dataCnt < (PARAM_SIZE - 1) * 2 && pastMSec > SAMPLE_INTERVAL)
					{
						isUp = false;
						isCount = false;
						nonStepResult[dataCnt] = millSec;
						dataCnt++;
						fiftyCheckCnt = 0;
					}
					// 2%����10000msec�o�߂�����J�E���g�I��
					else if (dataCnt == (PARAM_SIZE - 1) * 2 && pastMSec > LONG_SAMPLE_INTERVAL)
					{
						isUp = false;
						isCount = false;
						nonStepResult[dataCnt] = millSec;
						dataCnt++;
						fiftyCheckCnt = 0;
					}
				}
				// ������
				else if (!isUp && isDwn)
				{
					// 臒l�͈͂ɓ�������J�E���g���Ȃ�
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 臒l�͈͊O�Ȃ�msec�J�E���g
					else
					{
						fiftyCheckCnt++;
						// 50msec�����NG�Ȃ�J�E���g
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 4000msec�o�߂�����J�E���g�I��
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
			// �������Ȃ�
		}
	}

	return lResult;
}

/// <summary>
/// �X�e�b�v�������Ԃ̏o�͊֐�
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

	// flowOut, mfmOut�l��������
	TCHAR szWork[32];
	for (long lIndex = 0; lIndex < lDataCnt; lIndex++) {
		// !EOF
		if (lIndex != lDataCnt - 1)
		{
			// flowOut�Ŕ���
			float flowSub = 0;
			flowSub = flowOut[lIndex + 1] - flowOut[lIndex];

			// �㏸�J�E���g�J�n
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
			// ���~�J�E���g�J�n
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

			// �J�E���g����
			if (isCount)
			{
				// �v�����Ԍo��
				pastMSec++;

				// �㏸��
				if (isUp && !isDwn)
				{
					// 臒l�͈͂ɓ�������J�E���g���Ȃ�
					if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 臒l�͈͊O�Ȃ�msec�J�E���g
					else
					{
						fiftyCheckCnt++;
						// 50msec�����NG�Ȃ�J�E���g
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 5%~100%����4000msec�o�߂�����J�E���g�I��
					if (dataCnt != 0 && pastMSec > SAMPLE_INTERVAL)
					{
						isCount = false;
						stepUpResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt_Up++;
						// 12�񏈗�������㏸����
						if (dataCnt == 12)
						{
							isUp = false;
							dataCnt = 0;
						}
					}
					// �ŏ���2%����10000msec�o�߂�����J�E���g�I��
					else if (dataCnt == 0 && pastMSec > LONG_SAMPLE_INTERVAL)
					{
						isCount = false;
						stepUpResult[dataCnt] = millSec;
						dataCnt++;
						thresholdCnt_Up++;
					}
				}
				// ������
				else if (!isUp && isDwn)
				{
					// 臒l�͈͂ɓ�������J�E���g���Ȃ�
					//if (mfmOut[lIndex] <= upperLim && mfmOut[lIndex] >= lowerLim)
					if (roundTo(mfmOut[lIndex], 6) <= upperLim && roundTo(mfmOut[lIndex], 6) >= lowerLim)
					{
						fiftyCheckCnt = 0;
					}
					// 臒l�͈͊O�Ȃ�msec�J�E���g
					else
					{
						fiftyCheckCnt++;
						// 50msec�����NG�Ȃ�J�E���g
						if (fiftyCheckCnt >= FIFTY_CHECK_INTERVAL)
						{
							millSec++;
							millSec = max(millSec, pastMSec);
						}
					}

					// 4000msec�o�߂�����J�E���g�I��
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
			// �������Ȃ�
		}
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////
//
//	�֐����́F	SaveCsvFile
//	���{�ꖼ�F	CSV�t�@�C���ۑ�
//	��    ���F	���ϒl�̔g�`�f�[�^��CSV�t�@�C���ɕۑ�����
//	��    ���F	lpszFilePath	CSV�t�@�C���p�X
//				flowOut		���ϒl�̔z��
//				lDataCnt		���ϒl�̔z��
//				bShowError		�G���[���b�Z�[�W�\���t���O
//	�� �� �l�F	0�F����A���̑��F�ُ�iWin32�G���[�R�[�h�Q�Ɓj
//	���ӎ����F	
//
/////////////////////////////////////////////////////////////////////
long CSample3Dlg::SaveCsvFile(LPCTSTR lpszFilePath, BOOL bShowError /*= FALSE*/)
{
	long lResult = ERROR_SUCCESS;

	try {
		CString strInvalid;
		strInvalid.LoadString(IDS_INVALID_VALUE);

		// CSV�t�@�C���쐬
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

// �����_�ȉ�n���Ŏl�̌ܓ�
float roundTo(float value, int digits) {
	float factor = std::pow(10.0f, digits);
	return std::round(value * factor) / factor;
}


void CSample3Dlg::OnBnClickedRadio1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void CSample3Dlg::OnBnClickedRadio2()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/// <summary>
/// ���s�{�^���N���b�N���̑���
/// ���s�O�̐ݒ肪�������Ȃ���Ă��邩���`�F�b�N����
/// </summary>
void CSample3Dlg::OnBnClickedOk()
{
	// �o�̓��[�h�̃��W�I�{�^�����[�J���|�C���^
	pRadio_NonStep = (CButton*)GetDlgItem(IDC_RADIO1);
	pRadio_Step = (CButton*)GetDlgItem(IDC_RADIO2);
	m_listOutput_NonStep = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT1);
	m_listOutput_StepUp = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT2);
	m_listOutput_StepDwn = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT3);

	// �o�̓��[�h�I���`�F�b�N
	if (!pRadio_NonStep->GetCheck() && !pRadio_Step->GetCheck())
	{
		AfxMessageBox(_T("���W�I�{�^������o�̓��[�h��I�����Ă��������B"));
	}
	else
	{
		// ���X�g�R���g���[��1�̏�����
		if (pRadio_NonStep->GetCheck())
		{
			m_listOutput_NonStep->DeleteColumn(0);
			m_listOutput_NonStep->DeleteColumn(1);

			// ��ǉ�
			m_listOutput_NonStep->InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_NonStep->InsertColumn(1, _T("�o�͎���(msec)"), LVCFMT_LEFT, 100);
		}
		// ���X�g�R���g���[��2�̏�����
		else if (pRadio_Step->GetCheck())
		{
			m_listOutput_StepUp->DeleteColumn(0);
			m_listOutput_StepUp->DeleteColumn(1);
			m_listOutput_StepDwn->DeleteColumn(0);
			m_listOutput_StepDwn->DeleteColumn(1);

			// ��ǉ�
			m_listOutput_StepUp->InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepUp->InsertColumn(1, _T("�o�͎���(msec)"), LVCFMT_LEFT, 100);
			m_listOutput_StepDwn->InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepDwn->InsertColumn(1, _T("�o�͎���(msec)"), LVCFMT_LEFT, 100);
		}

		CSample3Dlg::OnOK();
	}
}
