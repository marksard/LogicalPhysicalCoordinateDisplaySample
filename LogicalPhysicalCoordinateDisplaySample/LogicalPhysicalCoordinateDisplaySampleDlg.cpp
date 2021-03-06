
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
END_MESSAGE_MAP()

const PWCHAR c_strAware = L"DPI aware";
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
		SetWindowTextW(c_strAware);
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



void CLogicalPhysicalCoordinateDisplaySampleDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);
	Output(pRect);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CLogicalPhysicalCoordinateDisplaySampleDlg::Output(LPRECT pRect)
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
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, &curPosWithL2P);

	POINT physicalCurPos;
	GetPhysicalCursorPos(&physicalCurPos);

	RECT pRectWithL2P = *pRect;
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&pRectWithL2P);
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&pRectWithL2P + 1);

	RECT dwmRect;
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &dwmRect, sizeof(RECT));

	RECT dwmRectWithP2L;
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &dwmRectWithP2L, sizeof(RECT));
	PhysicalToLogicalPointForPerMonitorDPI(hwnd, (LPPOINT)&dwmRectWithP2L);
	PhysicalToLogicalPointForPerMonitorDPI(hwnd, (LPPOINT)&dwmRectWithP2L + 1);

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

	GetDlgItem(IDC_EDIT1)->SetWindowTextW(this->strEdit1);
	this->Invalidate();
}
