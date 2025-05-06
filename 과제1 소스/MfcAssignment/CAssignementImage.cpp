// CAssignementImage.cpp: 구현 파일
//

#include "pch.h"
#include "MfcAssignment.h"
#include "afxdialogex.h"
#include "CAssignementImage.h"
#include <stdexcept>
#include <chrono>

using namespace std::chrono;

// CAssignementImage 대화 상자

IMPLEMENT_DYNAMIC(CAssignementImage, CDialogEx)

CAssignementImage::CAssignementImage(int pointRadius, int circleThickness, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAssignementImage, pParent),
	m_nPointRadius(pointRadius),
	m_nCirclethickness(circleThickness)
{
	m_pParent = pParent;
}

CAssignementImage::~CAssignementImage()
{
}

void CAssignementImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAssignementImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("Origin Image"));
	srand(static_cast<unsigned int>(time(NULL)));
	MoveWindow(0, 0, 640, 480);
	InitImage();
	return TRUE;
}

void CAssignementImage::InitImage()
{
	m_image.Create(m_nImageWidth, -m_nImageHeight, m_nImageBpp);
	if (m_nImageBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	m_nImagePitch = m_image.GetPitch();
	m_cFm = (unsigned char*)m_image.GetBits();

	memset(m_cFm, 0xff, m_nImageWidth * m_nImageHeight);
	DisplayImage();	
}

void CAssignementImage::DrawPoint(CPoint point)
{
	for (int y = point.y - m_nPointRadius; y <= point.y + m_nPointRadius; y++)
	{
		for (int x = point.x - m_nPointRadius; x <= point.x + m_nPointRadius; x++)
		{
			if (!(0 < x && x < m_nImageWidth))  continue;
			if (!(0 < y && y < m_nImageHeight)) continue;

			int dx = x - point.x;
			int dy = y - point.y;

			if (dx * dx + dy * dy <= m_nPointRadius * m_nPointRadius)
			{
				int index = y * m_nImagePitch + x;
				m_cFm[index] = 0;
			}
		}
	}

	DisplayImage();
}

void CAssignementImage::DrawCircle()
{
	CPoint p1 = m_clickPoints[0];
	CPoint p2 = m_clickPoints[1];
	CPoint p3 = m_clickPoints[2];

	int x1 = p1.x, x2 = p2.x, x3 = p3.x;
	int y1 = p1.y, y2 = p2.y, y3 = p3.y;

	double D = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

	if (D == 0.0)
	{
		DrawLine(p1, p2);
		DisplayImage();
		return;
	}

	double cx = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	double cy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;
	double r = sqrt((cx - x1) * (cx - x1) + (cy - y1) * (cy - y1));

	// 선 두께 설정	
	int rMin = r - m_nCirclethickness / 2;
	int rMax = r + m_nCirclethickness / 2;

	for (int y = static_cast<int>(cy - rMax); y <= static_cast<int>(cy + rMax); y++)
	{
		for (int x = static_cast<int>(cx - rMax); x <= static_cast<int>(cx + rMax); x++)
		{
			if (!(0 < x && x < m_nImageWidth))  continue;
			if (!(0 < y && y < m_nImageHeight)) continue;

			double dx = x - cx;
			double dy = y - cy;
			double dist2 = dx * dx + dy * dy;

			if (dist2 >= rMin * rMin && dist2 <= rMax * rMax)
			{
				int index = y * m_nImagePitch + x;
				m_cFm[index] = 0;
			}
		}
	}

	DisplayImage();
}

void CAssignementImage::DrawLine(CPoint p1, CPoint p2)
{
	int dx = abs(p2.x - p1.x), dy = abs(p2.y - p1.y);
	int sx = (p1.x < p2.x) ? 1 : -1;
	int sy = (p1.y < p2.y) ? 1 : -1;
	int err = dx - dy;

	int x = p1.x;
	int y = p1.y;

	while (true)
	{
		if (x > 0 && x < m_nImageWidth && y > 0 && y < m_nImageHeight)
		{
			int index = y * m_nImagePitch + x;
			m_cFm[index] = 0;
		}

		if (x == p2.x && y == p2.y) break;

		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x += sx; }
		if (e2 < dx) { err += dx; y += sy; }
	}
}

void CAssignementImage::DisplayImage()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
	Invalidate(FALSE);
}

void CAssignementImage::ResetImage()
{
	m_clickPoints.clear();
	memset(m_cFm, 0xff, m_nImageWidth * m_nImageHeight);	
	DisplayImage();
}

void CAssignementImage::MoveRandomPoints()
{
	if (m_clickPoints.size() != 3) return;

	// 목표 위치 설정
	m_clickPoints.clear();
	unsigned seed = static_cast<unsigned>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
	srand(seed);
	for (int i = 0; i < 3; ++i)
	{
		int tx = rand() % m_nImageWidth;
		int ty = rand() % m_nImageHeight;
		m_clickPoints.push_back(CPoint(tx, ty));
	}

	memset(m_cFm, 0xff, m_nImageWidth * m_nImageHeight);
	for (const auto& point : m_clickPoints)
	{
		DrawPoint(point);
	}
	DrawCircle();	
}

bool CAssignementImage::SetCircleThickness(int thickness)
{
	if (!(nMinCircleThickness <= thickness && thickness <= nMaxCircleThickness))
	{
		CString msg;
		msg.Format(_T("원의 크기의 범위는 (%d ~ %d) 입니다."), nMinCircleThickness, nMaxCircleThickness);
		throw std::out_of_range(CT2A(msg));
	}

	m_nCirclethickness = thickness;
}

bool CAssignementImage::SetPointRadius(int radius)
{
	if (!(nMinPointRadius <= radius && radius <= nMaxPointRadius))
	{		
		CString msg;
		msg.Format(_T("점의 크기의 범위는 (%d ~ %d) 입니다."), nMinPointRadius, nMaxPointRadius);
		throw std::out_of_range(CT2A(msg));
		
	}

	m_nPointRadius = radius;
}

void CAssignementImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CAssignementImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pParent) m_pParent->SetFocus();

	// 세 번째 점이 찍히기 전
	if (m_clickPoints.size() < 2)
	{
		m_clickPoints.push_back(point); // 클릭한 좌표 저장
		DrawPoint(point);
	}
	// 세 번째 점이 찍힌다면
	else if (m_clickPoints.size() == 2)
	{
		m_clickPoints.push_back(point); // 클릭한 좌표 저장
		DrawPoint(point);
		DrawCircle();
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			// 점 드래그하여 그림 다시 그리기
			CPoint p = m_clickPoints[i];
			if ((abs(p.x - point.x) <= m_nPointRadius) && (abs(p.y - point.y) <= m_nPointRadius))
			{
				m_dragIndex = i;
				m_isDragging = true;
				SetCapture();
				return;
			}
		}
	}
}

void CAssignementImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDragging)
	{
		m_isDragging = false;
		m_dragIndex = -1;
		ReleaseCapture();
	}
}

void CAssignementImage::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_isDragging) return;	

	if (0 <= m_dragIndex && m_dragIndex < 3)
	{
		// 점 위치 실시간 업데이트
		CPoint& clickedPoint = m_clickPoints[m_dragIndex];
		clickedPoint.x = point.x;
		clickedPoint.y = point.y;

		memset(m_cFm, 0xff, m_nImageWidth * m_nImageHeight);
		for (const auto& point : m_clickPoints)
		{
			DrawPoint(point);
		}
		DrawCircle();
	}
}

void CAssignementImage::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(CAssignementImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CAssignementImage 메시지 처리기
