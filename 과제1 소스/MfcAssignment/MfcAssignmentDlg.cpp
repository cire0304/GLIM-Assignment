
// MfcAssignmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MfcAssignment.h"
#include "MfcAssignmentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <stdexcept>
#include <afxwin.h>
#include <thread>

using namespace std;
using namespace chrono;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMfcAssignmentDlg 대화 상자



CMfcAssignmentDlg::CMfcAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCASSIGNMENT_DIALOG, pParent)
	, nPointRadius(3)
	, nCircleThickness(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINT_RADIUS, nPointRadius);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_THICKNESS, nCircleThickness);	
}

BEGIN_MESSAGE_MAP(CMfcAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RESET, &CMfcAssignmentDlg::OnBnClickedBtnReset)
	ON_EN_KILLFOCUS(IDC_EDIT_CIRCLE_THICKNESS, &CMfcAssignmentDlg::OnKillfocusEditCircleThickness)
	ON_EN_KILLFOCUS(IDC_EDIT_POINT_RADIUS, &CMfcAssignmentDlg::OnKillfocusEditPointRadius)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMfcAssignmentDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CMfcAssignmentDlg 메시지 처리기

BOOL CMfcAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 이전 값 복원 위해 값 저장
	m_nPrevPointRadius = nPointRadius;
	m_nPrevCircleThickness = nCircleThickness;

	m_pImage = new CAssignementImage(nPointRadius, nCircleThickness, this);
	m_pImage->Create(IDD_CAssignementImage, this);	
	m_pImage->ShowWindow(SW_SHOW);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMfcAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMfcAssignmentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMfcAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMfcAssignmentDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (::IsWindow(m_pImage->GetSafeHwnd()))
		m_pImage->DestroyWindow();
}

void CMfcAssignmentDlg::OnBnClickedBtnReset()
{	
	m_pImage->ResetImage();
}

void RandomMoveThreadProcess(CWnd* pParent)
{
	CMfcAssignmentDlg* pWnd = (CMfcAssignmentDlg*)pParent;

	for (int i = 0; i < 10; ++i)
	{
		pWnd->m_pImage->MoveRandomPoints();	
		Sleep(500);
	}
}

void CMfcAssignmentDlg::OnBnClickedBtnRandom()
{
	thread _thread0(RandomMoveThreadProcess, this);
	_thread0.detach();
}

void CMfcAssignmentDlg::OnKillfocusEditCircleThickness()
{
	if (!m_pImage) return;

	try
	{
		UpdateData(TRUE);
		m_pImage->SetCircleThickness(nCircleThickness);
		m_nPrevCircleThickness = nCircleThickness;
		UpdateData(false);
	}
	catch (std::out_of_range& e)
	{
		AfxMessageBox(CString(e.what()));
		SetDlgItemInt(IDC_EDIT_CIRCLE_THICKNESS, m_nPrevCircleThickness);		
	}
}

void CMfcAssignmentDlg::OnKillfocusEditPointRadius()
{
	if (!m_pImage) return;

	try
	{
		UpdateData(TRUE);
		m_pImage->SetPointRadius(nPointRadius);
		m_nPrevPointRadius = nPointRadius;
		UpdateData(false);
	}
	catch (std::out_of_range& e)
	{
		AfxMessageBox(CString(e.what()));	
		SetDlgItemInt(IDC_EDIT_POINT_RADIUS, m_nPrevPointRadius);
	}
}
