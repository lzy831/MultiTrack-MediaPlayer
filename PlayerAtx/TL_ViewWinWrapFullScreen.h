#pragma once


// TL_ViewWinWrapFullScreen 对话框
#include "TL_ViewWindow.h"
class CPlayerAtxCtrl;

class TL_ViewWinWrapFullScreen : public CDialogEx
{
	DECLARE_DYNAMIC(TL_ViewWinWrapFullScreen)

public:
	TL_ViewWinWrapFullScreen(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TL_ViewWinWrapFullScreen();

// 对话框数据
	enum { IDD = IDD_VIEW_WIN_WRAP_FULLSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	afx_msg HRESULT OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam);

	
protected:
	void MyOnSize(UINT nType, int cx, int cy);
	
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
	
	void SetViewWinPos();
	void SetPlayerAtxCtrl(CPlayerAtxCtrl* p) { m_pPlayerAtxCtrl = p; }

private:
	TL_ViewWindow		m_ViewWindow;
	int					m_inited;
	CPlayerAtxCtrl*		m_pPlayerAtxCtrl;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
