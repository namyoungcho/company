
// glim_nycDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

typedef struct tagPoint
{
	double x;
	double y;
}_Point;


// Cglim_nycDlg 대화 상자
class Cglim_nycDlg : public CDialog
{
private:
	CImage m_image;
	void OnCreateImage();
	
// 생성입니다.
public:
	Cglim_nycDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GLIM_NYC_DIALOG };

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

	afx_msg void OnBnClickedCreateImage();

	void UpdateDisplay();
	void drawCircle(unsigned char* fm, int i, int j, int nRadius, int nGray);
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);	
	CPoint GetCircleCenter(CPoint pt1, CPoint pt2, CPoint pt3);
	double GetDistance(const _Point p1, const _Point p2);

	afx_msg void OnBnClickedButton3();
	
	CEdit m_cPointRadius;
	CEdit m_cPointLine;
	bool m_bDraw;
	int m_nSelPoint;
	CPoint m_cPoint[3];
	int m_nPoint;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void OnDotDraw();
	void OnCircleDraw();
	void OnInitDraw();
	int OnGetIntFromID(int nID);
	afx_msg void OnBnClickedButtonInit();


	void SetPointText(CPoint point);
	afx_msg void OnBnClickedButtonRandom();
	CWinThread* m_pThread;
	bool m_isWorkingThread;
	bool GetImageStatus();
	int GetImageWidth();
	int GetImageHieght();
	CEdit m_cPointInfo;
	void OnPointInfoWrite();
	bool OnPointCheck();
	CEdit m_cRGB;
};
