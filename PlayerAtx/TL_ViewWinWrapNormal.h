#pragma once


// TL_ViewWinWrapNormal 对话框
#include "TL_ViewWindow.h"

class TL_ViewWinWrapNormal : public CDialogEx
{
	DECLARE_DYNAMIC(TL_ViewWinWrapNormal)

public:
	TL_ViewWinWrapNormal(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TL_ViewWinWrapNormal();

// 对话框数据
	enum { IDD = IDD_VIEW_WIN_WRAP_NORMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	afx_msg HRESULT OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam);
protected:
	void MyOnSize(UINT nType, int cx, int cy);
	void SetViewWinPos();

public:
	int	CreateViewPort();
	int ClearDisplay();
	int ScreenShot();

	int ResetViewPortCount(int count);
	int ResetDisplayMode(DISPLAY_MODE dmode);
	int ResetLayoutMode(LAYOUT_MODE lmode);
	
	int FrameDataInit(int size, int width, int height);
	int UpdateViewPortData(int tid, long pts, int index, UINT8* data[], int linesize[], int src_w, int src_h);
	int UpdateDisplay(int index);
	int UpdatePlayTime(int cur_time, int total_time);
	int UpdatePlayStatus(PLAY_STATUS status);

private:
	TL_ViewWindow		m_ViewWindow;
	int					m_inited;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
