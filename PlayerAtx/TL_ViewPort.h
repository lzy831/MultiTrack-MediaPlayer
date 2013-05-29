#pragma once

#include <string>
// TL_ViewPort 对话框

class TL_ViewPort : public CDialogEx
{
	DECLARE_DYNAMIC(TL_ViewPort)

public:
	TL_ViewPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TL_ViewPort();

// 对话框数据
	enum { IDD = IDD_TL_VIEWPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	int FrameDataInit(int size, int width, int height);

	// 给该ViewPort设置新的位图数据
	int SetRGBData( int tid, long pts, UINT8* data[], int linesize[], int src_w, int src_h);

	// 更新ViewPort的显示
	int UpdateDisplay();
	int ClearDisplay();
	int SaveImage();
	void SetIndex(DWORD index) { m_Index = index;}

private:
	int  CreateBmp(int w,int h);	//创建位图
	void DestroyBmp();				//销毁位图
	

private:
	CRITICAL_SECTION	m_cs;

	HBITMAP				m_hBitmap;		//用于显示的位图句柄，位图大小随着窗口大小而变化
	BYTE *				m_pSrcBuf;		//指向位图的数据区域

	HBITMAP				m_hBitmapOrg;	//用于保存的位图句柄，位图大小与视频原始大小一致
	BYTE *				m_pSrcBufOrg;	//指向位图的数据区域

	HDC					m_hMemDC;	//内存DC句柄
	int					m_iWidth;	//当前位图宽度
	int					m_iHeight;	//当前位图的高度

	//HWND				m_show_wnd;
	HDC					m_hdc;

	CBrush				m_bk_brush;
	DWORD				m_Index;

	int					m_VideoTrackID;
	long				m_pts;

	CString				m_AppPath;
	/*
	* 保存每次更新的视频帧数据
	*/
	UINT8*				m_src_data[8];
	int					m_src_linesize[8];
	int					m_src_size;
	int					m_src_w;
	int					m_src_h;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
