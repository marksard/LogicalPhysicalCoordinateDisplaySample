
// LogicalPhysicalCoordinateDisplaySampleDlg.h : ヘッダー ファイル
//

#pragma once


// CLogicalPhysicalCoordinateDisplaySampleDlg ダイアログ
class CLogicalPhysicalCoordinateDisplaySampleDlg : public CDialogEx
{
// コンストラクション
public:
	CLogicalPhysicalCoordinateDisplaySampleDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGICALPHYSICALCOORDINATEDISPLAYSAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
private:
	void Output(LPRECT pRect);
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString strEdit1;
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
