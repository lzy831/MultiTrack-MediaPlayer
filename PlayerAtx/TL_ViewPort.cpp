// TL_ViewPort.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"


#include "PlayerAtx.h"
#include "TL_ViewPort.h"
#include "PlayerCoreDLL/PlayerCoreInterface.h"
#include "TL_Message.h"
#include "TL_Color.h"

// TL_ViewPort �Ի���

IMPLEMENT_DYNAMIC(TL_ViewPort, CDialogEx)

TL_ViewPort::TL_ViewPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(TL_ViewPort::IDD, pParent)
	, m_hBitmap(NULL)
	, m_hBitmapOrg(NULL)
	, m_pSrcBuf(NULL)
	, m_pSrcBufOrg(NULL)
	, m_hMemDC()
	, m_iWidth(0)
	, m_iHeight(0)
	//, m_show_wnd(0)
	, m_hdc(0)
	, m_Index(-1)
{
	memset(&m_cs, 0, sizeof(m_cs));
	InitializeCriticalSection(&m_cs);
	m_bk_brush.CreateSolidBrush(RGB(0,255,0));

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //ɾ���ļ�����ֻ���·��
	m_AppPath = szFilePath;

	for(int i = 0; i < 8; i++)
	{
		m_src_data[i] = NULL;
		m_src_linesize[i] = 0;
	}
}

TL_ViewPort::~TL_ViewPort()
{
	if(m_src_data[0]!= NULL)
	{
		free(m_src_data[0]);
		m_src_data[0] = NULL;
	}
}

void TL_ViewPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TL_ViewPort, CDialogEx)
//	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void TL_ViewPort::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CString strMsg(">>>>>>>>>>>>>>>>>>>>>>>>> TL_ViewPort %d OnPaint\n", m_Index);
	OutputDebugString(strMsg);

	if(m_hBitmap!=NULL)
	{
		UpdateDisplay();
	}
}

//HBRUSH TL_ViewPort::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	if( nCtlColor == CTLCOLOR_DLG)
//	{
//		return m_bk_brush;   //������ɫˢ��
//	}
//
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
//}


// TL_ViewPort ��Ϣ�������


/************************************************************************************************************************************************/
/*  �Լ����ӵĺ���                                                                     */
/************************************************************************************************************************************************/
void SaveFrame( int flag, UINT8** data, int* linesize, int width, int height)
{
	FILE *pFile ;
	char szFilename[256];
	int  y ;

	static int fcount = 0;

	sprintf_s(szFilename , 256, "ff_%d_%d.ppm", flag, fcount++);
	int ret = fopen_s( &pFile, szFilename, "wb");
	if(ret != 0)
	{
		return;
	}

	fprintf(pFile , "P6\n%d %d\n255\n", width , height);
	for(y =0; y < height; y ++)
		fwrite( data[0] + y * linesize[0], 1, width*3, pFile);
	fclose(pFile );
	return ;
}

int TL_ViewPort::FrameDataInit(int size, int width, int height)
{
	if(m_src_data[0]==NULL)
	{
		m_src_w = width;
		m_src_h = height;
		m_src_data[0] = (unsigned char*)malloc(size);
		ASSERT(m_src_data[0]!=NULL);
	}
	return 0;
}

// ����ViewPort�����µ�λͼ����
int TL_ViewPort::SetRGBData( int tid, long pts, UINT8* src_data[], int src_linesize[], int src_w, int src_h)
{
	/* ����һ��Ҫע��һ�£�
	*  ���MediaPlayer�Ļ������ڻص������Ƶ֡�󣬲��ٱ�����֡������VideoQueue��backward_queue��
	*  ��ô��SaveImage()��ʹ�õ�m_src_data[]��m_src_linesize[]�����ݺܿ��ܾͲ��ǵ�ǰViewPort�ж�Ӧ����֡��Ƶ
	*  ��Ϊ�����뿪�ú���֮�󣬵�ǰViewPort�е���֡�����Ѿ������ź󱣴���m_hBitmap֮���ˡ�һ��UpdateDisplay()�ǰ�m_hBitmap�����������ʾ��
	*  �����뿪�ú���֮��src_data��src_linesize��Ӧ�ĸ�֡�����Ѿ���MediaPlayer��VideoQueue���գ����ܿ��ܱ�����������Ժ���������Ƶ֡
	*
	*  ����������һ�����⣬
	*  ����SaveImage()ϣ������ԭʼ�ߴ������ViewPort���ڵ�ǰ�ߴ��֡ͼƬ�������m_hBitmap�����ݷŴ�ͼƬ����֮���ܻ�ʧ��
	*  ���������������
	*  (1) �ڸú����н�֡���ݱ�����TL_ViewPort����֮�У����Ǳ���src_data��src_linesize�е�ʵ�����ݣ������ǵ�ַ
	*  (2) ʹMediaPlayer������󣬰�֡������VideoQueue��backward_queue�У�ֻ�е�backward_queue����֮�󣬲ſ�ʼ������ǰ���֡��
	*      �����������Ա�֤��SaveImage()��ʱ��m_src_data��m_src_linesize��Ӧ�����ݾ���m_hBitmap��ԭʼ�ߴ�����
	*  ֮ǰ�ù�(2) 
	*  Ŀǰ�޸�Ϊ��(1)�ַ���
	*/

	EnterCriticalSection( &m_cs );

	m_VideoTrackID = tid;
	m_pts = pts;
	m_src_w = src_w;
	m_src_h = src_h;

	{
		// copy YUV420P������ ��ViewPort
		int copy_size = TL_GetAVPictureSizeInYUV420P(src_w,src_h);
		memcpy(m_src_data[0],src_data[0],copy_size);
		for( int i = 0; i < 8; i++)
		{
			m_src_linesize[i] = src_linesize[i];
			if( i > 0 && m_src_linesize[i] != 0)
			{
				m_src_data[i] = m_src_data[i-1] + ( src_data[i] - src_data[i-1]);
			}
		}
	}


	// ��ȡ��ǰ���ڵĴ�С 
	CRect rc;
	::GetClientRect(m_hWnd,&rc);

	// ���ݵ�ǰ���ڵĴ�С�������ǲ���Ҫ���´���bitmap���
	int Ret = 0;
	int dst_w = rc.Width();
	int dst_h = rc.Height();
	Ret = CreateBmp( dst_w, dst_h);

	// ��Bitmap�ĺ���λ����� ���õ����洴����Bitmap�Ļ���λ����, ׼����������
	UINT8* data[4] = {(UINT8 *)m_pSrcBuf, 0, 0, 0};
	int	linesize[4] = {(m_iWidth*3+3) &~3, 0, 0, 0};

	//SaveFrame( 1, src_data, src_linesize, src_w, src_h);
	//for( int i = 0; i < 4; i++)
	//{
	//	data[i] = src_data[i];
	//	linesize[i] = src_linesize[i];
	//}

	// ����
	// ע��MeidaPlayer�Ѿ��޸�Ϊֱ�ӱ���������ԭʼYUV420P������
	//Ret = TL_SwsSscale_RGB24_to_BGR24( m_src_data, m_src_linesize, m_src_w, m_src_h, data, linesize, dst_w, dst_h);
	Ret = TL_SwsSscale_YUV420P_to_BGR24( m_src_data, m_src_linesize, m_src_w, m_src_h, data, linesize, dst_w, dst_h);

	LeaveCriticalSection( &m_cs );
	return 0;
}

// ����ViewPort����ʾ
int TL_ViewPort::UpdateDisplay()
{
	//SaveFrame( 2, data, linesize, dst_w, dst_h);
	if( m_hdc == 0)
	{
		m_hdc = ::GetDC(m_hWnd);
	}
	if( m_hdc == 0)
	{
		ASSERT(0);
	}

	//static int debug_interval = 0;
	//if( debug_interval++ % 71 == 0)
	//{
	//	CRect Rect;
	//	GetClientRect(&Rect);
	//	CString strMsg;
	//	strMsg.Format(_T(">>>>> UpdateDisplay %d < %d %d >\n"), m_Index, (int)Rect.top, (int)Rect.left);
	//	OutputDebugString(strMsg);
	//}

	BOOL Ret = BitBlt( m_hdc, 0, 0, m_iWidth, m_iHeight, m_hMemDC, 0, 0, SRCCOPY) == TRUE ? 0 : -1;
	if( Ret != TRUE)
	{
		int i = 0;
	}

	//ReleaseDC((CDC*)m_hdc);
	return Ret;
}

int TL_ViewPort::ClearDisplay()
{
	//UpdateWindow();
	//return 0;
	
	CDC* pcd = GetDC();
	CBrush b;
	CRect winRect;
	GetClientRect(&winRect);
	b.CreateSolidBrush(RGB(0,0,0));
	pcd->FillRect(winRect, &b);
	DestroyBmp();
	UpdateWindow();


	if(m_src_data[0]!= NULL)
	{
		free(m_src_data[0]);
		m_src_data[0] = NULL;
	}
	return 0;
}

int TL_ViewPort::CreateBmp(int w,int h)///����λͼ
{
	EnterCriticalSection( &m_cs );
	if( w == m_iWidth && h == m_iHeight)
	{
		if( m_hMemDC != NULL || m_pSrcBuf != NULL )
		{
			LeaveCriticalSection( &m_cs);
			return 0;	//already setup
		}
	}
	if(!m_hWnd)//
	{
		LeaveCriticalSection( &m_cs);
		return -1;//failed
	}

	DestroyBmp();

	m_iWidth = w;
	m_iHeight = h;

	HDC					hdc;
	BYTE				buffer[sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD)];
	BITMAPINFO*			pBMI    = (BITMAPINFO*) buffer;
	BITMAPINFOHEADER*	pHeader = &pBMI->bmiHeader;
	DWORD*				pColors = (DWORD*)&pBMI->bmiColors;   
	int					bpp = 24;

	pHeader->biSize            = sizeof(BITMAPINFOHEADER);
	pHeader->biWidth           = m_iWidth;
	pHeader->biHeight          = -1*m_iHeight;
	pHeader->biPlanes          = 1;
	pHeader->biBitCount        = bpp;
	pHeader->biCompression     = BI_RGB;//BI_BITFIELDS;
	pHeader->biSizeImage       = (m_iWidth * m_iHeight * bpp ) / 8;
	pHeader->biXPelsPerMeter   = 0;
	pHeader->biYPelsPerMeter   = 0;
	pHeader->biClrUsed         = 0;
	pHeader->biClrImportant    = 0;

	hdc=::GetDC(m_hWnd);
	if(!hdc) 
	{
		LeaveCriticalSection( &m_cs);
		return -1;
	}
	m_hMemDC = CreateCompatibleDC( hdc/*pDestDC->m_hDC*/ );
	if( m_hMemDC != NULL )
	{
		m_hBitmap = CreateDIBSection( 0, pBMI, DIB_RGB_COLORS, (void**)&(m_pSrcBuf), 0, 0 );
		if(m_hBitmap)
		{
			::SelectObject(m_hMemDC, m_hBitmap);
		}
		else
		{
			LeaveCriticalSection( &m_cs);
			return -1;
		}
	}
	else
	{
		LeaveCriticalSection( &m_cs);
		return -1;
	}
	::ReleaseDC(m_hWnd,hdc);
	LeaveCriticalSection( &m_cs);

	return 0;
}

void TL_ViewPort::DestroyBmp()
{
	if( m_hBitmap )
	{
		m_pSrcBufOrg;
		::DeleteObject(m_hBitmap);
		m_pSrcBufOrg;
		m_hBitmap = NULL;
	}
	if( m_hMemDC )
	{
		::DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}
	m_pSrcBuf = NULL;
}


int TL_ViewPort::SaveImage()
{
	/*
	*
	* ע�⿴һ�� SetRGBData()�е�ע�ͣ��ǹ����ڸú�����ʹ��֮ǰ�����m_src_dataָ�����ݵ�
	*
	*/

	//if(m_Index==1)
	{
		//SaveFrame( 1, m_src_data, m_src_linesize, m_src_w, m_src_h);

		// ��һ�ν�ͼ��ʱ�򣬴������ڱ����Bitmap�ṹ
		//if( m_hBitmapOrg == NULL)
		//{
			BYTE				buffer[sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD)];
			BITMAPINFO*			pBMI    = (BITMAPINFO*) buffer;
			BITMAPINFOHEADER*	pHeader = &pBMI->bmiHeader;
			DWORD*				pColors = (DWORD*)&pBMI->bmiColors;
			int					bpp = 24;

			pHeader->biSize            = sizeof(BITMAPINFOHEADER);
			pHeader->biWidth           = m_src_w;
			pHeader->biHeight          = -1*m_src_h;
			pHeader->biPlanes          = 1;
			pHeader->biBitCount        = bpp;
			pHeader->biCompression     = BI_RGB;//BI_BITFIELDS;
			pHeader->biSizeImage       = (m_src_w * m_src_h * bpp ) / 8;
			pHeader->biXPelsPerMeter   = 0;
			pHeader->biYPelsPerMeter   = 0;
			pHeader->biClrUsed         = 0;
			pHeader->biClrImportant    = 0;

			m_hBitmapOrg = CreateDIBSection( 0, pBMI, DIB_RGB_COLORS, (void**)&(m_pSrcBufOrg), 0, 0 );
		//}
		if( m_hBitmapOrg != NULL)
		{
			ATL::CImage img;

			//CBitmap bitmap;
			//bitmap.CreateBitmap(m_src_w,m_src_h,1,24,m_src_data);

			EnterCriticalSection( &m_cs );

			UINT8* data[4] = {(UINT8 *)m_pSrcBufOrg, 0, 0, 0};
			int	linesize[4] = {(m_src_w*3+3) &~3, 0, 0, 0};
			TL_SwsSscale_YUV420P_to_BGR24( m_src_data, m_src_linesize, m_src_w, m_src_h, data, linesize, m_src_w, m_src_h);

			m_pSrcBufOrg;
			CString filename;
			img.Attach(m_hBitmapOrg);
			filename.Format(_T("\\TrackID-%d_PTS-%d.jpeg"),m_VideoTrackID, m_pts);
			img.Save(m_AppPath+filename,Gdiplus::ImageFormatJPEG);
			LeaveCriticalSection( &m_cs);
		}
	}
	return 0;
}


void TL_ViewPort::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CString strMsg;
	strMsg.Format(_T("TL_ViewPort::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	OutputDebugString(strMsg);

	if(m_Index!=-1)
	{
		::SendMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_VIEW_PORT_ON_DCLICK,0,m_Index);
	}
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}



