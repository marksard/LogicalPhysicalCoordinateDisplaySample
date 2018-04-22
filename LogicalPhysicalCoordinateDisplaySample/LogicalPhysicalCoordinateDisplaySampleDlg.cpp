
// LogicalPhysicalCoordinateDisplaySampleDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LogicalPhysicalCoordinateDisplaySample.h"
#include "LogicalPhysicalCoordinateDisplaySampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogicalPhysicalCoordinateDisplaySampleDlg ダイアログ



CLogicalPhysicalCoordinateDisplaySampleDlg::CLogicalPhysicalCoordinateDisplaySampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGICALPHYSICALCOORDINATEDISPLAYSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogicalPhysicalCoordinateDisplaySampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVING()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SIZING()
END_MESSAGE_MAP()

const PWCHAR c_strAware = L"DPI aware";
const PWCHAR c_strAwareM = L"DPI aware as monitor";
const PWCHAR c_strUnaware = L"DPI unaware";

// CLogicalPhysicalCoordinateDisplaySampleDlg メッセージ ハンドラー

BOOL CLogicalPhysicalCoordinateDisplaySampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	if (IsProcessDPIAware())
	{
		PROCESS_DPI_AWARENESS dpi;
		GetProcessDpiAwareness(GetCurrentProcess(), &dpi);
		if (dpi == PROCESS_PER_MONITOR_DPI_AWARE)
		{
			SetWindowTextW(c_strAwareM);
		}
		else
		{
			SetWindowTextW(c_strAware);
		}
	}
	else
	{
		SetWindowTextW(c_strUnaware);
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CLogicalPhysicalCoordinateDisplaySampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CLogicalPhysicalCoordinateDisplaySampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLogicalPhysicalCoordinateDisplaySampleDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	HWND hwnd = this->GetSafeHwnd();
	if (nHitTest == HTCAPTION)
	{
		InitMove(hwnd);

		RECT rect;
		::GetWindowRect(hwnd, &rect);
		Output(&rect, &rect);
	}

	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CLogicalPhysicalCoordinateDisplaySampleDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	HWND hwnd = this->GetSafeHwnd();

	RECT rectStart = *pRect;
	RECT rectEnd = *pRect;
	BeginDWMOffset(hwnd, &rectEnd);
	PreMove(hwnd, &rectEnd);
	EndDWMOffset(hwnd, &rectEnd);

	Output(pRect, &rectStart);
}


void CLogicalPhysicalCoordinateDisplaySampleDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	HWND hwnd = this->GetSafeHwnd();

	RECT rectStart = *pRect;
	RECT rectEnd = *pRect;
	BeginDWMOffset(hwnd, &rectEnd);
	PreSize(hwnd, &rectEnd, fwSide);
	EndDWMOffset(hwnd, &rectEnd);

	Output(pRect, &rectStart);
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::LogicalToPhysicalPointForPerMonitorDPI(LPPOINT lppoint)
{
	POINT temp;
	temp.x = (lppoint->x - m_sizeDeskOffset.x) * m_dsizeScale.cx;
	temp.y = (lppoint->y - m_sizeDeskOffset.y) * m_dsizeScale.cy;
	lppoint->x = temp.x + m_sizeDeskOffset.x;
	lppoint->y = temp.y + m_sizeDeskOffset.y;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::PhysicalToLogicalPointForPerMonitorDPI(LPPOINT lppoint)
{
	POINT temp;
	temp.x = (lppoint->x - m_sizeDeskOffset.x) / m_dsizeScale.cx;
	temp.y = (lppoint->y - m_sizeDeskOffset.y) / m_dsizeScale.cy;
	lppoint->x = temp.x + m_sizeDeskOffset.x;
	lppoint->y = temp.y + m_sizeDeskOffset.y;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::CalcDWMOffset(HWND hwnd)
{
	RECT rectThis;
	RECT rectDwm;
	::GetWindowRect(hwnd, &rectThis);

	if (IsProcessDPIAware() == FALSE)
	{
		RECT rectBefore = rectThis;
		::LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&rectThis);
		::LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&rectThis + 1);

		m_dsizeScale.cx = (double)(rectThis.right - rectThis.left) / (double)(rectBefore.right - rectBefore.left);
		m_dsizeScale.cy = (double)(rectThis.bottom - rectThis.top) / (double)(rectBefore.bottom - rectBefore.top);

		HWND hwnd = this->GetSafeHwnd();
		HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monInfo;
		monInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hmon, &monInfo);
		//m_sizeDeskOffset.x = monInfo.rcMonitor.left;
		//m_sizeDeskOffset.y = monInfo.rcMonitor.top;
		//m_sizeDeskOffset.x = (LONG)(monInfo.rcMonitor.left * m_dsizeScale.cx);
		//m_sizeDeskOffset.y = (LONG)(monInfo.rcMonitor.top * m_dsizeScale.cy);
	}

	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rectDwm, sizeof(RECT));

	m_rectDWMOffset.left = rectThis.left - rectDwm.left;
	m_rectDWMOffset.top = rectThis.top - rectDwm.top;
	m_rectDWMOffset.right = rectDwm.right - rectThis.right;
	m_rectDWMOffset.bottom = rectDwm.bottom - rectThis.bottom;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::InitMove(HWND hwnd)
{
	CalcDWMOffset(hwnd);

	POINT	posCursor;
	RECT	rectThis;
	GetCursorPos(&posCursor);
	::LogicalToPhysicalPointForPerMonitorDPI(hwnd, &posCursor);
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rectThis, sizeof(RECT));

	// サイズの計算
	m_sizeThis.cx = rectThis.right - rectThis.left;
	m_sizeThis.cy = rectThis.bottom - rectThis.top;
	// ウィンドウ座標とマウス座標の距離計算
	m_sizeDistance.cx = posCursor.x - rectThis.left;
	m_sizeDistance.cy = posCursor.y - rectThis.top;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::PreMove(HWND hwnd, LPRECT lprectThis)
{
	POINT	posCursor;
	GetCursorPos(&posCursor);
	::LogicalToPhysicalPointForPerMonitorDPI(hwnd, &posCursor);
	// WM_MOVINGのRectアドレスを格納
	m_lprectThis = lprectThis;
	// マウス座標からウィンドウ座標へ変換
	m_rectThisTemp.left = posCursor.x - m_sizeDistance.cx;
	m_rectThisTemp.top = posCursor.y - m_sizeDistance.cy;
	m_rectThisTemp.right = m_rectThisTemp.left + m_sizeThis.cx;
	m_rectThisTemp.bottom = m_rectThisTemp.top + m_sizeThis.cy;
	// 変換したのを格納
	*m_lprectThis = m_rectThisTemp;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::PreSize(HWND hwnd, LPRECT lprectThis, UINT nSide)
{
	// WM_SIZINGのRectアドレスを格納
	m_lprectThis = lprectThis;
	// 作業Rectに格納
	m_rectThisTemp = *m_lprectThis;

	//m_nSide = nSide;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::BeginDWMOffset(HWND hwnd, LPRECT lprectThis)
{

	if (IsProcessDPIAware() == FALSE)
	{
		this->LogicalToPhysicalPointForPerMonitorDPI((LPPOINT)lprectThis);
		this->LogicalToPhysicalPointForPerMonitorDPI((LPPOINT)lprectThis + 1);
	}

	lprectThis->left -= m_rectDWMOffset.left;
	lprectThis->top -= m_rectDWMOffset.top;
	lprectThis->right += m_rectDWMOffset.right;
	lprectThis->bottom += m_rectDWMOffset.bottom;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::EndDWMOffset(HWND hwnd, LPRECT lprectThis)
{
	lprectThis->left += m_rectDWMOffset.left;
	lprectThis->top += m_rectDWMOffset.top;
	lprectThis->right -= m_rectDWMOffset.right;
	lprectThis->bottom -= m_rectDWMOffset.bottom;

	if (IsProcessDPIAware() == FALSE)
	{
		this->PhysicalToLogicalPointForPerMonitorDPI((LPPOINT)lprectThis);
		this->PhysicalToLogicalPointForPerMonitorDPI((LPPOINT)lprectThis + 1);
	}
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::Output(LPRECT pRect, LPRECT pRectStart)
{
	HWND hwnd = this->GetSafeHwnd();
	HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hmon, &monInfo);

	UINT rawX = 0, rawY = 0;
	UINT effX = 0, effY = 0;
	GetDpiForMonitor(hmon, MDT_RAW_DPI, &rawX, &rawY);
	GetDpiForMonitor(hmon, MDT_EFFECTIVE_DPI, &effX, &effY);

	POINT curPos;
	GetCursorPos(&curPos);

	POINT curPosWithL2P;
	GetCursorPos(&curPosWithL2P);
	::LogicalToPhysicalPointForPerMonitorDPI(hwnd, &curPosWithL2P);

	POINT physicalCurPos;
	GetPhysicalCursorPos(&physicalCurPos);

	RECT pRectWithL2P = *pRect;
	::LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&pRectWithL2P);
	::LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&pRectWithL2P + 1);

	RECT dwmRect;
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &dwmRect, sizeof(RECT));

	RECT dwmRectWithP2L;
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &dwmRectWithP2L, sizeof(RECT));
	::PhysicalToLogicalPointForPerMonitorDPI(hwnd, (LPPOINT)&dwmRectWithP2L);
	::PhysicalToLogicalPointForPerMonitorDPI(hwnd, (LPPOINT)&dwmRectWithP2L + 1);

	this->strEdit1 = L"";
	CString strTemp;
	strTemp.Format(L"Desktop\t\t[%04d, %04d, %04d, %04d (%04d, %04d)]\r\nMDT_RAW_DPI\t[%04d, %04d]\r\nMDT_EFFECTIVE_DPI[%04d, %04d (%03d%%)]\r\nGetCursorPos\t[%04d, %04d] ->L2P[%04d, %04d]\r\nGetPhysicalCursorPos[%04d, %04d]\r\n",
		monInfo.rcMonitor.left, monInfo.rcMonitor.top, monInfo.rcMonitor.right, monInfo.rcMonitor.bottom, monInfo.rcMonitor.right - monInfo.rcMonitor.left, monInfo.rcMonitor.bottom - monInfo.rcMonitor.top,
		rawX, rawY,
		effX, effY, (int)((double)effX / 96.0 * 100.0),
		curPos.x, curPos.y,
		curPosWithL2P.x, curPosWithL2P.y,
		physicalCurPos.x, physicalCurPos.y
	);
	this->strEdit1.Append(strTemp);
	strTemp.Format(L"OnMovingArg\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\n->L2P\t\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\nDwmGetWindowA...\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\n->P2L\t\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\n",
		pRect->left, pRect->top, pRect->right, pRect->bottom, pRect->right - pRect->left, pRect->bottom - pRect->top,
		pRectWithL2P.left, pRectWithL2P.top, pRectWithL2P.right, pRectWithL2P.bottom, pRectWithL2P.right - pRectWithL2P.left, pRectWithL2P.bottom - pRectWithL2P.top,
		dwmRect.left, dwmRect.top, dwmRect.right, dwmRect.bottom, dwmRect.right - dwmRect.left, dwmRect.bottom - dwmRect.top,
		dwmRectWithP2L.left, dwmRectWithP2L.top, dwmRectWithP2L.right, dwmRectWithP2L.bottom, dwmRectWithP2L.right - dwmRectWithP2L.left, dwmRectWithP2L.bottom - dwmRectWithP2L.top
	);
	this->strEdit1.Append(strTemp);

	if (m_lprectThis)
	{
		strTemp.Format(L"This\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\nThisTemp\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\nDWMOffet\t[%04d, %04d, %04d, %04d]\r\nDwmSize\t[%04d - %04d] Dist[%04d - %04d] scale[%02d - %02d]\r\npRectStart\t[%04d, %04d, %04d, %04d (%04d - %04d)]\r\n",
			m_lprectThis->left, m_lprectThis->top, m_lprectThis->right, m_lprectThis->bottom, m_lprectThis->right - m_lprectThis->left, m_lprectThis->bottom - m_lprectThis->top,
			m_rectThisTemp.left, m_rectThisTemp.top, m_rectThisTemp.right, m_rectThisTemp.bottom, m_rectThisTemp.right - m_rectThisTemp.left, m_rectThisTemp.bottom - m_rectThisTemp.top,
			m_rectDWMOffset.left, m_rectDWMOffset.top, m_rectDWMOffset.right, m_rectDWMOffset.bottom,
			m_sizeThis.cx, m_sizeThis.cy,
			m_sizeDistance.cx, m_sizeDistance.cy,
			(LONG)(m_dsizeScale.cx * 10), (LONG)(m_dsizeScale.cy * 10),
			pRectStart->left, pRectStart->top, pRectStart->right, pRectStart->bottom, pRectStart->right - pRectStart->left, pRectStart->bottom - pRectStart->top
		);
		this->strEdit1.Append(strTemp);
	}

	GetDlgItem(IDC_EDIT1)->SetWindowTextW(this->strEdit1);
	this->Invalidate();
}
