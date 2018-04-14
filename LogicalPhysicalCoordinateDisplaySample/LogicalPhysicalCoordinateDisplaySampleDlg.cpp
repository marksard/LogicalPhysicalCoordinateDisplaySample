
// LogicalPhysicalCoordinateDisplaySampleDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LogicalPhysicalCoordinateDisplaySample.h"
#include "LogicalPhysicalCoordinateDisplaySampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogicalPhysicalCoordinateDisplaySampleDlg �_�C�A���O



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
const PWCHAR c_strUnaware = L"DPI unaware";

// CLogicalPhysicalCoordinateDisplaySampleDlg ���b�Z�[�W �n���h���[

BOOL CLogicalPhysicalCoordinateDisplaySampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	if (IsProcessDPIAware())
	{
		SetWindowTextW(c_strAware);
	}
	else
	{
		SetWindowTextW(c_strUnaware);
	}

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CLogicalPhysicalCoordinateDisplaySampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CLogicalPhysicalCoordinateDisplaySampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLogicalPhysicalCoordinateDisplaySampleDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	HWND hwnd = this->GetSafeHwnd();

	RECT rectStart = *pRect;
	RECT rectEnd = *pRect;
	BeginDWMOffset(hwnd, &rectEnd);
	PreMove(hwnd, &rectEnd);
	EndDWMOffset(hwnd, &rectEnd);

	Output(pRect, &rectStart);
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


void CLogicalPhysicalCoordinateDisplaySampleDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
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


void CLogicalPhysicalCoordinateDisplaySampleDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::CalcDWMOffset(HWND hwnd)
{
	RECT rectThis;
	RECT rectDwm;
	::GetWindowRect(hwnd, &rectThis);

	if (IsProcessDPIAware() == FALSE)
	{
		RECT rectBefore = rectThis;
		LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&rectThis);
		LogicalToPhysicalPointForPerMonitorDPI(hwnd, (LPPOINT)&rectThis + 1);

		m_dsizeScale.cx = (double)(rectThis.right - rectThis.left) / (double)(rectBefore.right - rectBefore.left);
		m_dsizeScale.cy = (double)(rectThis.bottom - rectThis.top) / (double)(rectBefore.bottom - rectBefore.top);

		HWND hwnd = this->GetSafeHwnd();
		HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monInfo;
		monInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hmon, &monInfo);
		m_sizeDeskOffset.x = monInfo.rcMonitor.left;
		m_sizeDeskOffset.y = monInfo.rcMonitor.top;
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
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, &posCursor);
	DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rectThis, sizeof(RECT));

	// �T�C�Y�̌v�Z
	m_sizeThis.cx = rectThis.right - rectThis.left;
	m_sizeThis.cy = rectThis.bottom - rectThis.top;
	// �E�B���h�E���W�ƃ}�E�X���W�̋����v�Z
	m_sizeDistance.cx = posCursor.x - rectThis.left;
	m_sizeDistance.cy = posCursor.y - rectThis.top;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::PreMove(HWND hwnd, LPRECT lprectThis)
{
	POINT	posCursor;
	GetCursorPos(&posCursor);
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, &posCursor);
	// WM_MOVING��Rect�A�h���X���i�[
	m_lprectThis = lprectThis;
	// �}�E�X���W����E�B���h�E���W�֕ϊ�
	m_rectThisTemp.left = posCursor.x - m_sizeDistance.cx;
	m_rectThisTemp.top = posCursor.y - m_sizeDistance.cy;
	m_rectThisTemp.right = m_rectThisTemp.left + m_sizeThis.cx;
	m_rectThisTemp.bottom = m_rectThisTemp.top + m_sizeThis.cy;
	// �ϊ������̂��i�[
	*m_lprectThis = m_rectThisTemp;
}

void CLogicalPhysicalCoordinateDisplaySampleDlg::BeginDWMOffset(HWND hwnd, LPRECT lprectThis)
{
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

	RECT temp;
	temp.left = (lprectThis->left - m_sizeDeskOffset.x) / m_dsizeScale.cx;
	temp.top = (lprectThis->top - m_sizeDeskOffset.y) / m_dsizeScale.cy;
	temp.right = (lprectThis->right - m_sizeDeskOffset.x) / m_dsizeScale.cx;
	temp.bottom = (lprectThis->bottom - m_sizeDeskOffset.y) / m_dsizeScale.cy;
	lprectThis->left = temp.left + m_sizeDeskOffset.x;
	lprectThis->top = temp.top + m_sizeDeskOffset.y;
	lprectThis->right = temp.right + m_sizeDeskOffset.x;
	lprectThis->bottom = temp.bottom + m_sizeDeskOffset.y;
}
