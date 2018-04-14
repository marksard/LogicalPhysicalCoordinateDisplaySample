
// LogicalPhysicalCoordinateDisplaySampleDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

typedef struct DSIZE
{
	double cx;
	double cy;
}DSIZE, *LPDSIZE;

// CLogicalPhysicalCoordinateDisplaySampleDlg �_�C�A���O
class CLogicalPhysicalCoordinateDisplaySampleDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CLogicalPhysicalCoordinateDisplaySampleDlg(CWnd* pParent = nullptr);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGICALPHYSICALCOORDINATEDISPLAYSAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
private:
	LPRECT m_lprectThis;
	RECT m_rectThisTemp;
	RECT m_rectDWMOffset;
	SIZE m_sizeThis;
	SIZE m_sizeDistance;
	POINT m_sizeDeskOffset;
	DSIZE m_dsizeScale;
	void Output(LPRECT pRect, LPRECT pRectStart);
	void CalcDWMOffset(HWND hwnd);
	void InitMove(HWND hwnd);
	void PreMove(HWND hwnd, LPRECT lprectThis);
	void BeginDWMOffset(HWND hwnd, LPRECT lprectThis);
	void EndDWMOffset(HWND hwnd, LPRECT lprectThis);

protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString strEdit1;
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
