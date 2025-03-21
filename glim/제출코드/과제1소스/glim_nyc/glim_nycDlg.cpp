
// glim_nycDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "glim_nyc.h"
#include "glim_nycDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
UINT ThreadForRandom(LPVOID param);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cglim_nycDlg 대화 상자



Cglim_nycDlg::Cglim_nycDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cglim_nycDlg::IDD, pParent)
	, m_bDraw(false)
	, m_nSelPoint(0)
	, m_pThread(NULL)
	, m_isWorkingThread(false)
{
	m_nPoint = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cglim_nycDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_cPointRadius);
	DDX_Control(pDX, IDC_EDIT_RADIUS2, m_cPointLine);
	DDX_Control(pDX, IDC_EDIT_INFO, m_cPointInfo);
	DDX_Control(pDX, IDC_EDIT_RGB, m_cRGB);
}

BEGIN_MESSAGE_MAP(Cglim_nycDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CRIMAGE, &Cglim_nycDlg::OnBnClickedCreateImage)	
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &Cglim_nycDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &Cglim_nycDlg::OnBnClickedButtonRandom)
END_MESSAGE_MAP()


// Cglim_nycDlg 메시지 처리기

BOOL Cglim_nycDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_cPointRadius.SetWindowTextW(_T("5"));
	m_cPointLine.SetWindowTextW(_T("3"));
	m_cRGB.SetWindowTextW(_T("0"));

	srand(time(NULL));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cglim_nycDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cglim_nycDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cglim_nycDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cglim_nycDlg::OnCreateImage()
{

	if (m_image.IsNull())
	{

		int nWidth = 640;
		int nHeight = 480;
		int nBpp = 8;

		m_image.Create(nWidth, -nHeight, nBpp);
		if (nBpp == 8) {
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);
		}

		int nPitch = m_image.GetPitch();
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		memset(fm, 0xff, nWidth*nHeight);

		UpdateDisplay();
	}
}

void Cglim_nycDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	
	int nCenterX = x ;
	int nCenterY = y ;
	int nPitch = m_image.GetPitch();

	for (int j = y - nRadius ; j < y + nRadius ; j++)
	{
		if (j < 0 || j >= m_image.GetHeight()) continue;
		for (int i = x - nRadius ; i < x + nRadius ; i++)
		{
			if (i < 0 || i >= m_image.GetWidth()) continue;
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
			{
				fm[j * nPitch + i] = nGray;
			}
		}
	}
}

void Cglim_nycDlg::OnCircleDraw()
{
	OnInitDraw();

	int nGray = OnGetIntFromID(IDC_EDIT_RGB);
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	CPoint getCirclePoint = GetCircleCenter(m_cPoint[0], m_cPoint[1], m_cPoint[2]);

	_Point pCircle;
	_Point pDot;

	pCircle.x = (double)getCirclePoint.x;
	pCircle.y = (double)getCirclePoint.y;
	pDot.x = (double)m_cPoint[1].x;
	pDot.y = (double)m_cPoint[1].y;

	int nCircleRadius = (int)GetDistance(pCircle, pDot);

	int nLine = OnGetIntFromID(IDC_EDIT_RADIUS2);

	drawCircle(fm, getCirclePoint.x, getCirclePoint.y, nCircleRadius + nLine / 2, nGray);
	drawCircle(fm, getCirclePoint.x, getCirclePoint.y, nCircleRadius - nLine / 2, 255);
	//SetPointText(getCirclePoint);
}


void Cglim_nycDlg::OnInitDraw()
{
	// 지우고 새로 그리기
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();
	for (int j = 0; j < m_image.GetHeight(); j++)
	{
		for (int i = 0; i < m_image.GetWidth(); i++)
		{
			{
				fm[j * nPitch + i] = 255;
			}
		}
	}

	UpdateDisplay();
}




void Cglim_nycDlg::OnBnClickedCreateImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCreateImage();

}

void Cglim_nycDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);

}

bool Cglim_nycDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius*nRadius) {
		bRet = true;
	}

	return bRet;
}

CPoint Cglim_nycDlg::GetCircleCenter(CPoint pt1, CPoint pt2, CPoint pt3)
{
	double A = pt2.x - pt1.x;
	double B = pt2.y - pt1.y;
	double C = pt3.x - pt1.x;
	double D = pt3.y - pt1.y;
	double E = A * (pt1.x + pt2.x) + B * (pt1.y + pt2.y);
	double F = C * (pt1.x + pt3.x) + D * (pt1.y + pt3.y);
	double G = 2.0 * (A * (pt3.y - pt2.y) - B * (pt3.x - pt2.x));
	if (G == 0)
		return pt1;

	CPoint ptCenter;
	ptCenter.x = (D * E - B * F) / G;
	ptCenter.y = (A * F - C * E) / G;

	return ptCenter;
}

double Cglim_nycDlg::GetDistance(const _Point p1, const _Point p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

int Cglim_nycDlg::OnGetIntFromID(int nID)
{
	CString sRadius;
	GetDlgItem(nID)->GetWindowTextW(sRadius);
	return _ttoi(sRadius);
}

void Cglim_nycDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_image.IsNull()) return;
	if (point.x > m_image.GetWidth()) return;
	if (point.y > m_image.GetHeight()) return;
	
	if (m_nPoint < 3)
	{
		
		m_cPoint[m_nPoint++] = point ;
		if (m_nPoint == 3)
		{
			OnCircleDraw();			
		}				
		OnDotDraw();
		
	}
	else 
	{		
		int nRadius = OnGetIntFromID(IDC_EDIT_RADIUS);
		m_nSelPoint = -1;

		// Point Catch
		for (int i = 0; i < 3; ++i)
		{
			if (point.x > m_cPoint[i].x - nRadius &&  point.x < m_cPoint[i].x + nRadius &&
				point.y > m_cPoint[i].y - nRadius &&  point.y < m_cPoint[i].y + nRadius)
			{
				m_nSelPoint = i;
				m_bDraw = true;
				break;
			}
		}
		
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void Cglim_nycDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.	
	if (m_bDraw)
	{
		m_bDraw = false;
		m_nSelPoint = -1;		
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void Cglim_nycDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDraw)
	{
		m_cPoint[m_nSelPoint] = point;
		OnCircleDraw();			
		OnDotDraw();						
	}
	CDialog::OnMouseMove(nFlags, point);
}

void Cglim_nycDlg::OnDotDraw()
{
	
	int nGray = OnGetIntFromID(IDC_EDIT_RGB);
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	
	int nRadius = OnGetIntFromID(IDC_EDIT_RADIUS);
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	
	for (int i = 0; i < m_nPoint; ++i)
	{
		drawCircle(fm, m_cPoint[i].x, m_cPoint[i].y, nRadius, nGray);
		SetPointText(m_cPoint[i]);
	}	
	OnPointInfoWrite();
	UpdateDisplay();
	
}

void Cglim_nycDlg::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_image.IsNull())
	{

		m_isWorkingThread = false;

		OnInitDraw();

		for (int i = 0; i < 3; i++)
		{
			m_cPoint[i].x = 0;
			m_cPoint[i].y = 0;
		}
		m_nPoint = 0;
		m_nSelPoint = -1;	

		OnPointInfoWrite();
	}
}

void Cglim_nycDlg::SetPointText(CPoint point)
{
	// Point Ouput
	CDC* pDC = CDC::FromHandle(m_image.GetDC());
	pDC->SetBkMode(TRANSPARENT);
	CString sPoint; sPoint.Format(_T("(%d,%d)"), point.x, point.y);
	int nGray = OnGetIntFromID(IDC_EDIT_RGB);
	pDC->SetTextColor(RGB(nGray, nGray, nGray));
	pDC->TextOutW(point.x + 10, point.y - 10, sPoint);	
	m_image.ReleaseDC();
}


void Cglim_nycDlg::OnBnClickedButtonRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_isWorkingThread && OnPointCheck())
	{
		m_isWorkingThread = true;
		m_pThread = AfxBeginThread(ThreadForRandom, this);
	}
}

UINT ThreadForRandom(LPVOID param)
{
	Cglim_nycDlg* pMain = (Cglim_nycDlg*)param;
	int nCount = 0;
	while (pMain->m_isWorkingThread)
	{	
		if (pMain->GetImageStatus())
		{
			
			int nXSNumber = 0, nXENumber = pMain->GetImageWidth();
			int nYSNumber = 0, nYENumber = pMain->GetImageHieght();

			for (int i = 0; i < 3; i++) {
				pMain->m_cPoint[i].x = (rand() % (nXENumber - nXSNumber + 1)) + nXSNumber;
				pMain->m_cPoint[i].y = (rand() % (nXENumber - nXSNumber + 1)) + nXSNumber;
				TRACE(_T("Get Random Point %d %d\n"), pMain->m_cPoint[i], pMain->m_cPoint[i]);
				pMain->m_nPoint = i;
			}
			pMain->OnCircleDraw();
			pMain->OnDotDraw();
			Sleep(500);
			if (nCount > 40)
			{
				break;
			}
			nCount++;
		}
		else
		{
			break;
		}
	}

	pMain->m_isWorkingThread = false;

	WaitForSingleObject(pMain->m_pThread->m_hThread, 1000);

	return 0;
}

bool Cglim_nycDlg::GetImageStatus()
{
	if (!m_image.IsNull())
	{
		return true;
	}
	return false;
}

int Cglim_nycDlg::GetImageWidth()
{
	if (!m_image.IsNull())
	{
		return m_image.GetWidth();
	}
	return 0;
}

int Cglim_nycDlg::GetImageHieght()
{
	if (!m_image.IsNull())
	{
		return m_image.GetHeight();
	}
	return 0;
}

void Cglim_nycDlg::OnPointInfoWrite()
{	
	CString sPointInfo;
	sPointInfo.Format(_T("Dot1Point : %d,%d, Dot2Point : %d:%d, Dot3Point : %d:%d"), m_cPoint[0].x, m_cPoint[0].y, m_cPoint[1].x, m_cPoint[1].y, m_cPoint[2].x, m_cPoint[2].y);
	GetDlgItem(IDC_EDIT_INFO)->SetWindowTextW(sPointInfo);
}


bool Cglim_nycDlg::OnPointCheck()
{
	bool bRet = true;
	for (int i = 0; i < 3; ++i)
	{
		if (m_cPoint[i].x == 0)
			bRet = false;
	}

	return bRet;
}
