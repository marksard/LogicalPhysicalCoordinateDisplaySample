
// LogicalPhysicalCoordinateDisplaySample.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CLogicalPhysicalCoordinateDisplaySampleApp:
// このクラスの実装については、LogicalPhysicalCoordinateDisplaySample.cpp を参照してください
//

class CLogicalPhysicalCoordinateDisplaySampleApp : public CWinApp
{
public:
	CLogicalPhysicalCoordinateDisplaySampleApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CLogicalPhysicalCoordinateDisplaySampleApp theApp;
