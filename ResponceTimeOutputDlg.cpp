/////////////////////////////////////////////////////////////////////////////////
//
//  ̧�ٖ��́F	Sample3Dlg.cpp
//  ���@�@���F	�I�[�g���[�V�����T�[�o�@�\�T���v��3�_�C�A���O�N���X�̓����`
//  ���ӎ����F	
//
//  Copyright(C) 2003-2005 KEYENCE CORPORATION All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResponceTimeOutput.h"
#include "ResponceTimeOutputDlg.h"
#include <cmath>
#include <vector>
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
#define SUBSTRUCT_THRESHOLD		0.05f
#define PARAM_SIZE				12		// param��
#define SAMPLE_INTERVAL			4000	// �ʏ�T���v������
#define LONG_SAMPLE_INTERVAL	10000	// 2%�㏸���T���v������
#define FIFTY_CHECK_INTERVAL	50		// 50msec�J�E���g�p
#define ARRAY_OFFSET			10		// FlowOut�̌��݈ʒu����̃I�t�Z�b�g(�����オ�莞�ɂ̂ݗ��p���邪�A�傫���ɒ���)
#define HSIZE					950		// ���T�C�Y臒l
#define VSIZE					550		// �c�T�C�Y臒l

#define UNIT_NUM				3		// �f�[�^���K�[�̃��j�b�g��
#define CH_NUM					10		// �f�[�^���K�[�̃`�����l����

int nonStepResult[PARAM_SIZE * 2]; // 12*2�ʂ�̃f�[�^��
int stepUpResult[PARAM_SIZE]; // �X�e�b�v�㏸���̃f�[�^
int stepDwnResult[PARAM_SIZE]; // �X�e�b�v�������̃f�[�^

// �eUI�I�u�W�F�N�g
CListCtrl m_listOutput_NonStep;
CListCtrl m_listOutput_StepUp;
CListCtrl m_listOutput_StepDwn;
CButton pRadio_NonStep;
CButton pRadio_Step;
CButton btn_Cp_Nonstep;
CButton btn_Cp_StepUp;
CButton btn_Cp_StepDwn;
CComboBox comboFlowUnit;
CComboBox comboFlowCh;
CComboBox comboMFMUnit;
CComboBox comboMFMCh;
CStatic nonstepResultPath;
CStatic stepResultPath;

const int hSize = HSIZE;
const int vSize = VSIZE;

int m_nScrollPosX; // ���X�N���[���ʒu
int m_nScrollRangeX; // �R���e���c�S�̂̕�
int m_nScrollPageX; // ���X�N���[���y�[�W�T�C�Y�i�\���̈�̕��j
int m_nScrollPosY; // �c�X�N���[���ʒu
int m_nScrollRangeY; // �c�X�N���[���͈�
int m_nScrollPageY; // �c�X�N���[���y�[�W�T�C�Y�i�\���̈�̍����j

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
	ON_BN_CLICKED(IDOK, &CSample3Dlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OUTPUT2, &CSample3Dlg::OnLvnItemchangedListOutput2)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CP_NONSTEP, &CSample3Dlg::OnBnClickedButtonCpNonstep)
	ON_BN_CLICKED(IDC_BUTTON_CP_STEP_UP, &CSample3Dlg::OnBnClickedButtonCpStepUp)
	ON_BN_CLICKED(IDC_BUTTON_CP_STEP_DWN, &CSample3Dlg::OnBnClickedButtonCpStepDwn)
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
	strSummary += _T("\n");
	strSummary += strRemarks;
	SetDlgItemText(IDC_SUMMARY_STA, strSummary);

	// WAVE LOGGER �N��
	if (RunWaveLogger() == FALSE) {
		AfxMessageBox(IDS_ERR_RUN_WAVELOGGER, MB_ICONSTOP);
		EndDialog(IDABORT);
		return FALSE;
	}

	// �X�N���[���ʒu������
	m_nScrollPosY = 0;

	// �S�̂̃R���e���c�̕��A�����i�X�N���[���\�ȁj���Z�b�g
	m_nScrollRangeX = hSize;
	m_nScrollRangeY = vSize; // �Ⴆ��1000�s�N�Z���i���ۂ̓��e�T�C�Y�ɍ��킹��j

	// �\���̈�̍������擾���ăy�[�W�T�C�Y�ݒ�
	CRect rect;
	GetClientRect(&rect);
	m_nScrollPageX = rect.Width();
	m_nScrollPageY = rect.Height();

	// �����X�N���[��
	if (m_nScrollRangeY > m_nScrollPageY)
	{
		SetScrollRange(SB_VERT, 0, m_nScrollRangeY - m_nScrollPageY, FALSE);
		SetScrollPos(SB_VERT, 0, TRUE);
	}

	// �����X�N���[��
	if (m_nScrollRangeX > m_nScrollPageX)
	{
		SetScrollRange(SB_HORZ, 0, m_nScrollRangeX - m_nScrollPageX, FALSE);
		SetScrollPos(SB_HORZ, 0, TRUE);
	}
	// �eUI�I�u�W�F�N�g�̃T�u�N���X��
	pRadio_NonStep.SubclassDlgItem(IDC_RADIO1, this);
	pRadio_NonStep.SetCheck(1);
	pRadio_Step.SubclassDlgItem(IDC_RADIO2, this);
	btn_Cp_Nonstep.SubclassDlgItem(IDC_BUTTON_CP_NONSTEP, this);
	btn_Cp_StepUp.SubclassDlgItem(IDC_BUTTON_CP_STEP_UP, this);
	btn_Cp_StepDwn.SubclassDlgItem(IDC_BUTTON_CP_STEP_DWN, this);
	m_listOutput_NonStep.SubclassDlgItem(IDC_LIST_OUTPUT1, this);
	m_listOutput_StepUp.SubclassDlgItem(IDC_LIST_OUTPUT2, this);
	m_listOutput_StepDwn.SubclassDlgItem(IDC_LIST_OUTPUT3, this);
	comboFlowUnit.SubclassDlgItem(IDC_COMBO_UNIT_FLOWOUT, this);
	comboFlowCh.SubclassDlgItem(IDC_COMBO_CH_FLOWOUT, this);
	comboMFMUnit.SubclassDlgItem(IDC_COMBO_UNIT_MFMOUT, this);
	comboMFMCh.SubclassDlgItem(IDC_COMBO_CH_MFMOUT, this);
	nonstepResultPath.SubclassDlgItem(IDC_STATIC_NonStepResultPath, this);
	stepResultPath.SubclassDlgItem(IDC_STATIC_StepResultPath, this);
	// ���j�b�g�A�`�����l���l��ǉ�
	for (int i = 1; i <= UNIT_NUM; ++i) {
		CString str;
		str.Format(_T("%d"), i);
		comboFlowUnit.AddString(str);
		comboMFMUnit.AddString(str);
	}
	for (int i = 0; i <= CH_NUM; ++i) {
		CString str;
		str.Format(_T("%d"), i);
		comboFlowCh.AddString(str);
		comboMFMCh.AddString(str);
	}

	// TODO JSON or XML�ŕۑ����ǂݍ��݂�����
	// ���j�b�g�A�`�����l���l��ݒ�
	comboFlowUnit.SetCurSel(0);
	comboFlowCh.SetCurSel(0);
	comboMFMUnit.SetCurSel(0);
	comboMFMCh.SetCurSel(2);

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

	// �e�o�b�t�@�m��<vector>
	// -�����Ń������Ǘ������
	// -��O���������Ă��m���Ƀ�������������
	std::vector<float> flowOut(MAX_ARRAY_SIZE);
	std::vector<float> mfmOut(MAX_ARRAY_SIZE);

	VARIANT vntArray;
	vntArray.vt = VT_ARRAY | VT_R4 | VT_BYREF;
	vntArray.pparray = &psa;

	// �Ώۃf�[�^�擾
	long lDataCnt = GetMFCDataArray(&XdtDoc, vntArray, flowOut.data(), mfmOut.data());
	if (lDataCnt == -1) return;
	XdtDoc.CloseFile();

	// �g�`�f�[�^�z��擾�p�o�b�t�@���
	SafeArrayDestroy(psa);

	// ��X�e�b�v���[�h
	if (pRadio_NonStep.GetCheck())
	{
		if (lDataCnt > 0) {
			// �������ԃf�[�^�쐬
			NonStepResonseTImeOutput(flowOut.data(), mfmOut.data(), lDataCnt, TRUE);

			// ���X�g�R���g���[��1�ɏo��
			for (int i = 0; i < sizeof(nonStepResult) / sizeof(nonStepResult[0]); i++)
			{
				CString str;
				if (i % 2 == 0)
				{
					str.Format(_T("%d"), threshold_NonStep.settingPercentage[i / 2]);
				}
				else
				{
					str.Format(_T("%d"), 0);
				}
				int index = m_listOutput_NonStep.InsertItem(i, str); // i�Ԗڂɑ}��

				CString strValue;
				strValue.Format(_T("%.3f"), (float)(nonStepResult[i])/1000);  // �����_3���܂ŕ\���i�K�v�ɉ����Ē����j

				m_listOutput_NonStep.SetItemText(index, 1, strValue);
			}
			// �f�[�^�R�s�[�{�^����L����
			if (!btn_Cp_Nonstep.IsWindowEnabled())
			{
				btn_Cp_Nonstep.EnableWindow(TRUE);				
			}
			// �ǂݍ��񂾃p�X�̕\��
			nonstepResultPath.SetWindowTextA(strFilePath);
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
	else if (pRadio_Step.GetCheck())
	{
		if (lDataCnt > 0) {
			// �������ԃf�[�^�쐬
			StepResonseTImeOutput(flowOut.data(), mfmOut.data(), lDataCnt, TRUE);

			CString str;
			CString strValue;
			// ���X�g�R���g���[��2�ɗ����オ��o��
			for (int i = 0; i < sizeof(stepUpResult) / sizeof(stepUpResult[0]); i++)
			{				
				str.Format(_T("%d"), threshold_Step.settingUpPercentage[i]);
				// �C���f�N�X�����X�gi�Ԗڂɑ}��
				int index = m_listOutput_StepUp.InsertItem(i, str);
				// �����_3���܂ŕ\��
				strValue.Format(_T("%.3f"), (float)(stepUpResult[i]) / 1000);
				m_listOutput_StepUp.SetItemText(index, 1, strValue);
			}
			// �f�[�^�R�s�[�{�^����L����
			if (!btn_Cp_StepUp.IsWindowEnabled())
			{
				btn_Cp_StepUp.EnableWindow(TRUE);
			}

			// ���X�g�R���g���[��3�ɗ���������o��
			for (int i = 0; i < sizeof(stepDwnResult) / sizeof(stepDwnResult[0]); i++)
			{
				str.Format(_T("%d"), threshold_Step.settingDwnPercentage[i]);
				// �C���f�N�X�����X�gi�Ԗڂɑ}��
				int index = m_listOutput_StepDwn.InsertItem(i, str);
				// �����_3���܂ŕ\��
				strValue.Format(_T("%.3f"), (float)(stepDwnResult[i]) / 1000);
				m_listOutput_StepDwn.SetItemText(index, 1, strValue);
			}
			// �f�[�^�R�s�[�{�^����L����
			if (!btn_Cp_StepDwn.IsWindowEnabled())
			{
				btn_Cp_StepDwn.EnableWindow(TRUE);				
			}
			// �ǂݍ��񂾃p�X�̕\��
			stepResultPath.SetWindowTextA(strFilePath);
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

	// �R���{�{�b�N�X��UNIT�I���C���f�N�X�擾
	// �C���f�N�X���當����擾�A������𐔒l�ɕϊ�
	CString flowUnit;
	CString mfmUnit;
	int flowUnitNo = comboFlowUnit.GetCurSel();
	int mfmUnitNo = comboMFMUnit.GetCurSel();	
	if (flowUnitNo != CB_ERR) {
		comboFlowUnit.GetLBText(flowUnitNo, flowUnit);
		flowUnitNo = _ttoi(flowUnit);
	}
	if (mfmUnitNo != CB_ERR) {
		comboMFMUnit.GetLBText(mfmUnitNo, mfmUnit);
		mfmUnitNo = _ttoi(mfmUnit);
	}

	// �R���{�{�b�N�X��Ch�I���C���f�N�X�擾
	// �C���f�N�X���當����擾�A������𐔒l�ɕϊ�
	CString flowCh;
	CString mfmCh;
	int flowChNo = comboFlowCh.GetCurSel();
	int mfmChNo = comboMFMCh.GetCurSel();
	if (flowChNo != CB_ERR) {
		comboFlowCh.GetLBText(flowChNo, flowCh);
		flowChNo = _ttoi(flowCh);
	}
	if (mfmChNo != CB_ERR) {
		comboMFMCh.GetLBText(mfmChNo, mfmCh);
		mfmChNo = _ttoi(mfmCh);
	}

	// ���j�b�gNo����������Ch�������ꍇ�͏������f
	if ((flowUnitNo == mfmUnitNo) && (flowChNo == mfmChNo))
	{
		AfxMessageBox(IDS_ERR_SAME_CH, MB_ICONEXCLAMATION);
		return -1;
	}

	int fCnt = 0, mCnt = 0;

	// FLowOut, MFMOut�̃��j�b�g�������ꍇ
	if (flowUnitNo == mfmUnitNo)
	{
		lDataCnt = GetChannelData(pXdtDoc,vntArray,flowUnitNo,flowOut,flowChNo,
			[]() -> int {
				AfxMessageBox(IDS_NODATA_UNIT, MB_ICONEXCLAMATION);
				return -1;
			}, &fCnt);

		if (lDataCnt != -1)
		{
			GetChannelData(pXdtDoc, vntArray, mfmUnitNo, mfmOut, mfmChNo,
				[]() -> int {
					AfxMessageBox(IDS_NODATA_UNIT, MB_ICONEXCLAMATION);
					return -1;
				}, &mCnt);
		}
		else
		{
			return lDataCnt;
		}
	}
	// FLowOut, MFMOut�̃��j�b�g���قȂ�ꍇ
	else
	{
		int fDataCnt = GetChannelData(pXdtDoc, vntArray, flowUnitNo, flowOut, flowChNo,
			[]() -> int {
				CString msg;
				CString noDataMsg;
				noDataMsg.LoadString(IDS_NODATA_UNIT);
				msg.Format(_T("FlowOut��%s"), noDataMsg);
				AfxMessageBox(msg, MB_ICONEXCLAMATION);
				return -1;
			}, &fCnt);
		if (fDataCnt != -1)
		{
			int mDataCnt = GetChannelData(pXdtDoc, vntArray, mfmUnitNo, mfmOut, mfmChNo,
				[]() -> int {
					CString msg;
					CString noDataMsg;
					noDataMsg.LoadString(IDS_NODATA_UNIT);
					msg.Format(_T("mfmOut��%s"), noDataMsg);
					AfxMessageBox(msg, MB_ICONEXCLAMATION);
					return -1;
				}, &mCnt);
			if (mDataCnt == -1)
			{
				return -1;
			}
			else
			{
				lDataCnt = fDataCnt + mDataCnt;
			}
		}
		else
		{
			return -1;
		}
	}

	// ����I���FFlowOut�`�����l����MFMOut�`�����l���̃f�[�^�����݂���
	if (fCnt != 0 && mCnt != 0)
	{
		return lDataCnt;
	}
	else
	{
		CString msg;
		CString noDataMsg;
		noDataMsg.LoadString(IDS_NO_CHDATA);

		// FlowOut�`�����l���̃f�[�^��0
		if (fCnt == 0 && mCnt != 0)
		{
			msg.Format(_T("FlowOut %s %s"), flowCh, noDataMsg);
			AfxMessageBox(msg, MB_ICONEXCLAMATION);
		}
		// MFMOut�`�����l���̃f�[�^��0
		else if (fCnt != 0 && mCnt == 0)
		{
			msg.Format(_T("MFMOut %s %s"), mfmCh, noDataMsg);
			AfxMessageBox(msg, MB_ICONEXCLAMATION);
		}
		// FlowOut�`�����l����MFMOut�`�����l���̃f�[�^��0
		else
		{
			msg.Format(_T("FlowOut %s CH��MFMOut %s %s"), flowCh, mfmCh, noDataMsg);
			AfxMessageBox(msg, MB_ICONEXCLAMATION);
		}
		
		return -1;
	}
}

/// <summary>
/// �w�肳�ꂽ���j�b�g�̎w�肳�ꂽ�`�����l������f�[�^���擾����
/// </summary>
/// <param name="pXdtDoc"></param>
/// <param name="vntArray"></param>
/// <param name="unitNo"></param>
/// <param name="out"></param>
/// <param name="chNo"></param>
/// <param name="onError"></param>
/// <param name="cnt"></param>
/// <returns></returns>
int CSample3Dlg::GetChannelData(CXdtDocument2* pXdtDoc, VARIANT& vntArray, int unitNo, float* out, int chNo, std::function<int()> onError, int* cnt)
{
	// ���j�b�gn�̃`�����l���P�`�̃f�[�^��1024���擾
	BOOL abValidChannel[HA_CHNUM_MAX];
	long lDataMax = min(MAX_ARRAY_SIZE, pXdtDoc->GetDataCount());
	long lDataCnt = 0;

	for (lDataCnt = 0; lDataCnt < lDataMax; ) {
		// ���j�b�gn�̃`�����l��0�`�̃f�[�^���擾
		long lGetCount = -1;
		for (long lChannel = 0; lChannel < HA_CHNUM_MAX; lChannel++) {
			long lResult = pXdtDoc->GetArrayData(unitNo, lChannel, lDataCnt, GET_CHDATA_MAX,
				vntArray, GET_CHDATA_MAX * lChannel);
			if (lResult > 0) {
				abValidChannel[lChannel] = TRUE;
				lGetCount = lResult;
			}
			else {
				abValidChannel[lChannel] = FALSE;
			}
		}
		// ���j�b�g�Ƀf�[�^�����݂��Ȃ�
		if (lGetCount < 0) {
			return onError();
		}

		// FlowOut(0), MFMOut(2) 
		// pfDat��SAFEARRAY���Ǘ����Ă���o�b�t�@�ւ̒��ڃ|�C���^�Ȃ̂ŁA����s�v
		float* pfData;
		SafeArrayAccessData(*vntArray.pparray, (void**)&pfData);
		for (long lIndex = 0; lIndex < lGetCount; lIndex++, lDataCnt++) {
			out[lDataCnt] = 0.;
			for (long lChannel = 0; lChannel < HA_CHNUM_MAX; lChannel++) {
				// flowOut
				if (lChannel == chNo)
				{
					// �����l�͔�΂�
					if (abValidChannel[lChannel] && pfData[GET_CHDATA_MAX * lChannel + lIndex] != FLT_MAX) {
						out[lDataCnt] = pfData[GET_CHDATA_MAX * lChannel + lIndex];
						(*cnt)++;
					}
				}
			}
		}

		//delete pfData;
		SafeArrayUnaccessData(*vntArray.pparray);

		if (lGetCount < GET_CHDATA_MAX) {
			break;
		}
	}

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
					upperLim = roundTo(upperLim, 6);
					lowerLim = threshold_NonStep.volt[thresholdCnt] - threshold_NonStep.threshold[thresholdCnt];
					lowerLim = roundTo(lowerLim, 6);
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
					upperLim = roundTo(upperLim, 6);
					lowerLim = threshold_NonStep.volt[PARAM_SIZE] - threshold_NonStep.threshold[PARAM_SIZE];
					lowerLim = roundTo(lowerLim, 6);
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
//	��    ���F	�f�[�^��CSV�t�@�C���ɕۑ�����
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

/// <summary>
/// ���s�{�^���N���b�N���̑���
/// ���s�O�̐ݒ肪�������Ȃ���Ă��邩���`�F�b�N����
/// </summary>
void CSample3Dlg::OnBnClickedOk()
{
	// �o�̓��[�h�I���`�F�b�N
	if (!pRadio_NonStep.GetCheck() && !pRadio_Step.GetCheck())
	{
		AfxMessageBox(_T("���W�I�{�^������o�̓��[�h��I�����Ă��������B"));
	}
	else
	{
		// ���X�g�R���g���[��1�̏�����
		if (pRadio_NonStep.GetCheck())
		{
			// �����f�[�^������ꍇ�A���ׂč폜����
			int nonStepDataCnt = m_listOutput_NonStep.GetItemCount();
			if (nonStepDataCnt > 0)
			{
				for (int i = 0; i < nonStepDataCnt; i++)
				{
					// ���X�g�̐擪�폜���J��Ԃ����ƂőS�č폜
					// (i)�Ƃ���Ə������ƂɃI�t�Z�b�g�����̂őS�ď����Ȃ�
					m_listOutput_NonStep.DeleteItem(0);
				}
			}

			// 2�񂠂�A�������[1]��[0]�ɂȂ邽�ߘA������0�������Ă���
			int columnCount = m_listOutput_NonStep.GetHeaderCtrl()->GetItemCount();
			for (int i = 0; i < columnCount; i++)
			{
				m_listOutput_NonStep.DeleteColumn(0);
			}

			// �f�[�^�p�X���󗓂�
			nonstepResultPath.SetWindowTextA("");

			// ��ǉ�
			m_listOutput_NonStep.InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_NonStep.InsertColumn(1, _T("�o�͎���(sec)"), LVCFMT_LEFT, 100);
		}
		// ���X�g�R���g���[��2�̏�����
		else if (pRadio_Step.GetCheck())
		{
			// Up(�����オ��)���X�g������
			// �����f�[�^������ꍇ�A���ׂč폜����
			int stepUpDataCnt = m_listOutput_StepUp.GetItemCount();
			if (stepUpDataCnt > 0)
			{
				for (int i = 0; i < stepUpDataCnt; i++)
				{
					m_listOutput_StepUp.DeleteItem(0);
				}
			}
			int columnCount = m_listOutput_StepUp.GetHeaderCtrl()->GetItemCount();
			for (int i = 0; i < columnCount; i++)
			{
				m_listOutput_StepUp.DeleteColumn(0);
			}

			// Down(����������)���X�g������
			// �����f�[�^������ꍇ�A���ׂč폜����
			int stepDwnDataCnt = m_listOutput_StepDwn.GetItemCount();
			if (stepDwnDataCnt > 0)
			{
				for (int i = 0; i < stepDwnDataCnt; i++)
				{
					m_listOutput_StepDwn.DeleteItem(0);
				}
			}
			columnCount = m_listOutput_StepDwn.GetHeaderCtrl()->GetItemCount();
			for (int i = 0; i < columnCount; i++)
			{
				m_listOutput_StepDwn.DeleteColumn(0);
			}

			// �f�[�^�p�X���󗓂�
			stepResultPath.SetWindowTextA("");

			// ��ǉ�
			m_listOutput_StepUp.InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepUp.InsertColumn(1, _T("�o�͎���(sec)"), LVCFMT_LEFT, 100);
			m_listOutput_StepDwn.InsertColumn(0, _T("�o�͐ݒ�(%)"), LVCFMT_LEFT, 80);
			m_listOutput_StepDwn.InsertColumn(1, _T("�o�͎���(sec)"), LVCFMT_LEFT, 100);
		}

		CSample3Dlg::OnOK();
	}
}

/// <summary>
/// �񃊃j�A�@���X�g�{�b�N�X��̑I���s���R�s�[����
/// </summary>
void CSample3Dlg::CopySelectedItemToClipboard()
{
	POSITION pos_NonStep = 0;
	POSITION pos_StepUp = 0;
	POSITION pos_StepDwn = 0;

	pos_NonStep = m_listOutput_NonStep.GetFirstSelectedItemPosition();
	pos_StepUp = m_listOutput_StepUp.GetFirstSelectedItemPosition();
	pos_StepDwn = m_listOutput_StepDwn.GetFirstSelectedItemPosition();
	if (!pos_NonStep && !pos_StepUp && !pos_StepDwn) return;

	CString strText_NonStep, strText_StepUp, strText_StepDwn;

	// �e�L�X�g�f�[�^�擾
	// ��X�e�b�v
	if (pos_NonStep)
	{
		GetCopyTarget(&m_listOutput_NonStep, pos_NonStep, strText_NonStep);
	}
	// �X�e�b�v�����オ��
	else if (pos_StepUp)
	{
		GetCopyTarget(&m_listOutput_StepUp, pos_StepUp, strText_StepUp);
	}
	// �X�e�b�v����������
	else if (pos_StepDwn)
	{
		GetCopyTarget(&m_listOutput_StepDwn, pos_StepDwn, strText_StepDwn);
	}

	// �N���b�v�{�[�h�ɃR�s�[
	if (OpenClipboard())
	{
		// ��X�e�b�v
		if (strText_NonStep.GetLength() > 0)
		{
			m_listOutput_NonStep.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_NonStep);
		}
		// �X�e�b�v�����オ��
		else if (strText_StepUp.GetLength() > 0)
		{
			m_listOutput_StepUp.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_StepUp);
		}
		// �X�e�b�v����������
		else if (strText_StepDwn.GetLength() > 0)
		{
			m_listOutput_StepDwn.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_StepDwn);
		}
	}
}

/// <summary>
/// ���X�g�R���g���[������e�L�X�g���擾����
/// </summary>
void CSample3Dlg::GetCopyTarget(CListCtrl* cCtr, POSITION pos, CString& str)
{
	while (pos)
	{
		int index = cCtr->GetNextSelectedItem(pos);

		// �e��̒l���^�u��؂�Ŏ擾
		for (int col = 0; col < cCtr->GetHeaderCtrl()->GetItemCount(); ++col)
		{
			str += cCtr->GetItemText(index, col);
			if (col < cCtr->GetHeaderCtrl()->GetItemCount() - 1)
				str += _T("\t");
		}
		str += _T("\r\n");
	}
}

/// <summary>
/// �N���b�v�{�[�h�����������A�e�L�X�g���R�s�[����
/// </summary>
void CSample3Dlg::SetClipbored(CString str)
{
	EmptyClipboard();
	HGLOBAL hGlob;
	m_listOutput_StepUp.SetItemState(-1, 0, LVIS_SELECTED);
	hGlob = GlobalAlloc(GMEM_MOVEABLE, (str.GetLength() + 1) * sizeof(TCHAR));
	if (hGlob)
	{
		LPTSTR p = (LPTSTR)GlobalLock(hGlob);
		_tcscpy_s(p, str.GetLength() + 1, str);
		GlobalUnlock(hGlob);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, hGlob);
#else
		SetClipboardData(CF_TEXT, hGlob);
#endif
	}

	CloseClipboard();
}

/// <summary>
/// MFC �̑g�ݍ��݃����o�[�֐�(CWnd �N���X�ɒ�`����Ă��鉼�z�֐�)
/// CSample3Dlg->CDialog->CWnd�̏��Ɍp�����Ă���̂Ŏ����\
/// ���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ�����O�Ɉ�x�ʂ鏈��
/// </summary>
BOOL CSample3Dlg::PreTranslateMessage(MSG* pMsg)
{
	// �L�[����
	if (pMsg->message == WM_KEYDOWN)
	{
		// Ctrl + C�Ȃ�R�s�[������
		// GetKeyState(VK_CONTROL) �� Ctrl�L�[�̏�Ԃ�Ԃ��A��ʃr�b�g�������Ă���Ή�����Ă���
		// GetKeyState(VK_XXX) & 0x8000 �� ���̃L�[��������Ă��邩�`�F�b�N
		// pMsg->wParam�łǂ̃L�[�̃��b�Z�[�W������
		if ((GetKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 'C')
		{
			CopySelectedItemToClipboard();
			return TRUE; // �����ς�
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSample3Dlg::OnLvnItemchangedListOutput2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}

/// <summary>
/// �������X�N���[�����̏���
/// </summary>
/// <param name="nSBCode"></param>
/// <param name="nPos"></param>
/// <param name="pScrollBar"></param>
void CSample3Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nNewPos = m_nScrollPosX;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		nNewPos = max(nNewPos - 10, 0);
		break;
	case SB_LINERIGHT:
		nNewPos = min(nNewPos + 10, m_nScrollRangeX - m_nScrollPageX);
		break;
	case SB_PAGELEFT:
		nNewPos = max(nNewPos - m_nScrollPageX, 0);
		break;
	case SB_PAGERIGHT:
		nNewPos = min(nNewPos + m_nScrollPageX, m_nScrollRangeX - m_nScrollPageX);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nNewPos = nPos;
		break;
	default:
		break;
	}

	if (nNewPos != m_nScrollPosX)
	{
		ScrollWindow(m_nScrollPosX - nNewPos, 0); // �� �������X�N���[��
		m_nScrollPosX = nNewPos;
		SetScrollPos(SB_HORZ, m_nScrollPosX, TRUE);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/// <summary>
/// �c�����X�N���[�����̏���
/// </summary>
/// <param name="nSBCode"></param>
/// <param name="nPos"></param>
/// <param name="pScrollBar"></param>
void CSample3Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nNewPos = m_nScrollPosY;

	switch (nSBCode)
	{
	case SB_TOP:
		nNewPos = 0;
		break;
	case SB_BOTTOM:
		nNewPos = m_nScrollRangeY - m_nScrollPageY;
		break;
	case SB_LINEUP:
		nNewPos = max(nNewPos - 10, 0);
		break;
	case SB_LINEDOWN:
		nNewPos = min(nNewPos + 10, m_nScrollRangeY - m_nScrollPageY);
		break;
	case SB_PAGEUP:
		nNewPos = max(nNewPos - m_nScrollPageY, 0);
		break;
	case SB_PAGEDOWN:
		nNewPos = min(nNewPos + m_nScrollPageY, m_nScrollRangeY - m_nScrollPageY);
		break;
	case SB_THUMBTRACK:
		nNewPos = nPos;
		break;
	default:
		break;
	}

	if (nNewPos != m_nScrollPosY)
	{
		ScrollWindow(0, m_nScrollPosY - nNewPos);
		m_nScrollPosY = nNewPos;
		SetScrollPos(SB_VERT, m_nScrollPosY, TRUE);
	}

	// �����̐e�N���X��OnVScroll���Ă�
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

/// <summary>
/// �_�C�A���O�T�C�Y�ύX���̏���
/// </summary>
/// <param name="nType"></param>
/// <param name="cx"></param>
/// <param name="cy"></param>
void CSample3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0) return;

	m_nScrollPageX = cx;
	m_nScrollPageY = cy;	

	// �c�X�N���[���͈�
	int nMaxScrollY = max(0, m_nScrollRangeY - m_nScrollPageY);
	if (m_nScrollPosY > nMaxScrollY)
	{
		ScrollWindow(0, m_nScrollPosY - nMaxScrollY);
		m_nScrollPosY = nMaxScrollY;
	}

	if (nMaxScrollY > 0)
	{
		ShowScrollBar(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, nMaxScrollY, FALSE);
		SetScrollPos(SB_VERT, m_nScrollPosY, TRUE);
	}
	else
	{
		ShowScrollBar(SB_VERT, FALSE);
		m_nScrollPosY = 0;
		SetScrollPos(SB_VERT, 0, TRUE);
	}

	// ���X�N���[���͈�
	int nMaxScrollX = max(0, m_nScrollRangeX - m_nScrollPageX);
	if (m_nScrollPosX > nMaxScrollX)
	{
		ScrollWindow(m_nScrollPosX - nMaxScrollX, 0);
		m_nScrollPosX = nMaxScrollX;
	}

	if (nMaxScrollX > 0)
	{
		ShowScrollBar(SB_HORZ, TRUE);
		SetScrollRange(SB_HORZ, 0, nMaxScrollX, FALSE);
		SetScrollPos(SB_HORZ, m_nScrollPosX, TRUE);
	}
	else
	{
		ShowScrollBar(SB_HORZ, FALSE);
		m_nScrollPosX = 0;
		SetScrollPos(SB_HORZ, 0, TRUE);
	}
}

/// <summary>
/// �f�[�^�R�s�[�{�^��(��X�e�b�v)�������̏���
/// </summary>
void CSample3Dlg::OnBnClickedButtonCpNonstep()
{
	POSITION pos_NonStep = 0;
	CString strText_NonStep;
	int itemCnt =m_listOutput_NonStep.GetItemCount();
	// �S�s��I��������Ԃɂ���
	for (int i = 0; i < itemCnt; ++i)
	{
		m_listOutput_NonStep.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	pos_NonStep = m_listOutput_NonStep.GetFirstSelectedItemPosition();

	// �e�L�X�g�f�[�^�擾
	GetCopyTarget(&m_listOutput_NonStep, pos_NonStep, strText_NonStep);
	
	// �N���b�v�{�[�h�ɃR�s�[
	if (OpenClipboard())
	{
		// ��X�e�b�v
		if (strText_NonStep.GetLength() > 0)
		{
			m_listOutput_NonStep.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_NonStep);
		}
	}
}

/// <summary>
/// �f�[�^�R�s�[�{�^��(�X�e�b�v�����オ��)�������̏���
/// </summary>
void CSample3Dlg::OnBnClickedButtonCpStepUp()
{
	POSITION pos_StepUp = 0;
	CString strText_StepUp;
	int itemCnt = m_listOutput_StepUp.GetItemCount();
	// �S�s��I��������Ԃɂ���
	for (int i = 0; i < itemCnt; ++i)
	{
		m_listOutput_StepUp.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	pos_StepUp = m_listOutput_StepUp.GetFirstSelectedItemPosition();

	// �e�L�X�g�f�[�^�擾
	GetCopyTarget(&m_listOutput_StepUp, pos_StepUp, strText_StepUp);

	// �N���b�v�{�[�h�ɃR�s�[
	if (OpenClipboard())
	{
		// �X�e�b�v(�����オ��)
		if (strText_StepUp.GetLength() > 0)
		{
			m_listOutput_StepUp.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_StepUp);
		}
	}
}

/// <summary>
/// �f�[�^�R�s�[�{�^��(�X�e�b�v����������)�������̏���
/// </summary>
void CSample3Dlg::OnBnClickedButtonCpStepDwn()
{
	POSITION pos_StepDwn = 0;
	CString strText_StepDwn;
	int itemCnt = m_listOutput_StepDwn.GetItemCount();
	// �S�s��I��������Ԃɂ���
	for (int i = 0; i < itemCnt; ++i)
	{
		m_listOutput_StepDwn.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	pos_StepDwn = m_listOutput_StepDwn.GetFirstSelectedItemPosition();

	// �e�L�X�g�f�[�^�擾
	GetCopyTarget(&m_listOutput_StepDwn, pos_StepDwn, strText_StepDwn);

	// �N���b�v�{�[�h�ɃR�s�[
	if (OpenClipboard())
	{
		// �X�e�b�v(����������)
		if (strText_StepDwn.GetLength() > 0)
		{
			m_listOutput_StepDwn.SetItemState(-1, 0, LVIS_SELECTED);
			SetClipbored(strText_StepDwn);
		}
	}
}
