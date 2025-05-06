
// MfcAssignmentDlg.h: 헤더 파일
//

#pragma once
#include "CAssignementImage.h"


// CMfcAssignmentDlg 대화 상자
class CMfcAssignmentDlg : public CDialogEx
{
private:
	int m_nPrevPointRadius;
	int m_nPrevCircleThickness;
	
// 생성입니다.
public:
	CMfcAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CAssignementImage* m_pImage;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCASSIGNMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int nPointRadius;
	int nCircleThickness;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnReset();	
	afx_msg void OnKillfocusEditCircleThickness();
	afx_msg void OnKillfocusEditPointRadius();
	afx_msg void OnBnClickedBtnRandom();
};
