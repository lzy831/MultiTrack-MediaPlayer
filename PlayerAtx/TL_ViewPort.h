#pragma once

#include <string>
// TL_ViewPort �Ի���

class TL_ViewPort : public CDialogEx
{
	DECLARE_DYNAMIC(TL_ViewPort)

public:
	TL_ViewPort(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TL_ViewPort();

// �Ի�������
	enum { IDD = IDD_TL_VIEWPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	int FrameDataInit(int size, int width, int height);

	// ����ViewPort�����µ�λͼ����
	int SetRGBData( int tid, long pts, UINT8* data[], int linesize[], int src_w, int src_h);

	// ����ViewPort����ʾ
	int UpdateDisplay();
	int ClearDisplay();
	int SaveImage();
	void SetIndex(DWORD index) { m_Index = index;}

private:
	int  CreateBmp(int w,int h);	//����λͼ
	void DestroyBmp();				//����λͼ
	

private:
	CRITICAL_SECTION	m_cs;

	HBITMAP				m_hBitmap;		//������ʾ��λͼ�����λͼ��С���Ŵ��ڴ�С���仯
	BYTE *				m_pSrcBuf;		//ָ��λͼ����������

	HBITMAP				m_hBitmapOrg;	//���ڱ����λͼ�����λͼ��С����Ƶԭʼ��Сһ��
	BYTE *				m_pSrcBufOrg;	//ָ��λͼ����������

	HDC					m_hMemDC;	//�ڴ�DC���
	int					m_iWidth;	//��ǰλͼ���
	int					m_iHeight;	//��ǰλͼ�ĸ߶�

	//HWND				m_show_wnd;
	HDC					m_hdc;

	CBrush				m_bk_brush;
	DWORD				m_Index;

	int					m_VideoTrackID;
	long				m_pts;

	CString				m_AppPath;
	/*
	* ����ÿ�θ��µ���Ƶ֡����
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
