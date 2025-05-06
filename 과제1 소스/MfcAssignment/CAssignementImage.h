#pragma once
#include "afxdialogex.h"
#include <vector>


// CAssignementImage 대화 상자

class CAssignementImage : public CDialogEx
{
	DECLARE_DYNAMIC(CAssignementImage)
private:
	const double m_dPi = 3.1415926535897932384626433832795;	
	const int m_nVertex = 365;

	int m_dragIndex = -1;
	bool m_isDragging = false;

	int m_nImageWidth = 640;
	int m_nImageHeight = 480;
	int m_nImageBpp = 8;
	int m_nImagePitch;
	unsigned char* m_cFm;

	std::vector<CPoint> m_targetPoints; // 랜던이동에 필요한 변수

public:
	CAssignementImage(int pointRadius, int circleThickness, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAssignementImage();

	CWnd* m_pParent;
	CImage m_image;
	std::vector<CPoint> m_clickPoints;

	static const int nMaxCircleThickness = 15;
	static const int nMinCircleThickness = 3;
	static const int nMaxPointRadius = 15;
	static const int nMinPointRadius = 3;

	int m_nPointRadius;
	int m_nCirclethickness;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAssignementImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ResetImage();
	void MoveRandomPoints();
	bool SetCircleThickness(int thickness);
	bool SetPointRadius(int radius);
	
private:
	void InitImage();
	void DrawPoint(CPoint point);
	void DrawCircle();
	void DrawLine(CPoint point1, CPoint point2);
	void DisplayImage();	
	void PostNcDestroy();

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
