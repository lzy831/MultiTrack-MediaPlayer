#pragma once

class TL_ViewPort;

/*
* ViewPort��ʾ����ģʽ
*/
enum DISPLAY_MODE
{
	DMODE_NONE,
	DMODE_4_3,
	DMODE_16_9,
};

/*
* ViewWindow��ViewPort����ģʽ
*/
enum LAYOUT_MODE
{
	LMODE_NONE,
	LMODE_1,	// С�����󣬴����� 
	LMODE_2,	// ������С������ 
};

enum PLAY_STATUS
{
	PS_OPENING_MEDIA,
	PS_PLAYING,
	PS_PAUSED,
	PS_STOPED,
};



enum 
{
	MAX_VIEWPORT_COUNT = 4,
};


class TL_ViewWindow : public CDialogEx
{
	DECLARE_DYNAMIC(TL_ViewWindow)

public:
	TL_ViewWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TL_ViewWindow();

// �Ի�������
	enum { IDD = IDD_TL_VIEWWINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HRESULT OnMsgViewPortDClick(WPARAM wParam, LPARAM lParam);

public:
	

	// ����ViewWindow�ϵ������ʾ�ؼ�
	int	CreateViewPort();

	// ���ViewWindow��ʾ
	int ClearDisplay();

	int ScreenShot();

	int ResetViewPortCount(int count);
	int ResetDisplayMode(DISPLAY_MODE dmode);
	int ResetLayoutMode(LAYOUT_MODE lmode);

	int FrameDataInit(int size, int width, int height);
	// ����ViewPort����ʾ
	int UpdateViewPortData(int tid, long pts, int index, UINT8* data[], int linesize[], int src_w, int src_h);
	int UpdateDisplay(int index);

	// ���²���ʱ����ʾ
	int UpdatePlayTime(int cur_time, int total_time);
	int UpdatePlayStatus(PLAY_STATUS status);


private:
	void OnConstruct();
	void OnDestroy();

	void MyOnSize(UINT nType, int cx, int cy);

	int  SetViewPortPos(int index, CRect& VPRect, int hide = 0);
	int  SetViewPortPos(int index, int x, int y, int w, int h);

	void ResetLayout();
	void ResetViewPortLayout(CRect& Rect);
	void ResetStatusBarLayout(CRect& Rect);

	int PrepareViewPortRect( CRect& ViewPortRect, int count, DISPLAY_MODE dmode, LAYOUT_MODE lmode, CRect* WinRect);

private:
	void GetScaledSize(DWORD* width, DWORD* height, DISPLAY_MODE mode);

private:
	WORD			m_ViewPortCreated;
	WORD			m_MaxViewPortCount;

	WORD			m_CurViewPortCount;		// ��ǰViewPort������
	DISPLAY_MODE	m_CurDisplayMode;		// ��ǰViewPort����ʾ����ģʽ
	LAYOUT_MODE		m_CurLayoutMode;		// ��ǰViewWindow�Ĳ���ģʽ

	CStatic			m_StaticPlayTime;
	CBrush			m_StaticPlayTimeBrush;

	CStatic			m_StaticPlayStatus;
	CBrush			m_StaticPlayStatusBrush;

	TL_ViewPort*	m_pViewPortArray[MAX_VIEWPORT_COUNT];
	CStatic			m_StaticViewPortTag[MAX_VIEWPORT_COUNT];	

	//CBrush			m_PlayTimeStaticBkBrush;

	
	//CBrush			m_bk_brush;
	//CPlayerCoreDlg*	m_pPlayerDlg;	// ������ָ��
	

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
