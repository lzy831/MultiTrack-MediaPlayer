// TL_ViewPort.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"


#include "PlayerAtx.h"
#include "TL_ViewPort.h"
#include "PlayerCoreDLL/PlayerCoreInterface.h"
#include "TL_Message.h"
#include "TL_Color.h"

// TL_ViewPort 对话框

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
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

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
//	// TODO:  在此更改 DC 的任何特性
//	if( nCtlColor == CTLCOLOR_DLG)
//	{
//		return m_bk_brush;   //返加绿色刷子
//	}
//
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}


// TL_ViewPort 消息处理程序


/************************************************************************************************************************************************/
/*  自己增加的函数                                                                     */
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

// 给该ViewPort设置新的位图数据
int TL_ViewPort::SetRGBData( int tid, long pts, UINT8* src_data[], int src_linesize[], int src_w, int src_h)
{
	/* 这里一定要注意一下，
	*  如果MediaPlayer的机制是在回调输出视频帧后，不再保留该帧数据在VideoQueue的backward_queue中
	*  那么在SaveImage()中使用的m_src_data[]和m_src_linesize[]的数据很可能就不是当前ViewPort中对应的那帧视频
	*  因为，在离开该函数之后，当前ViewPort中的这帧数据已经被缩放后保存在m_hBitmap之中了。一会UpdateDisplay()是把m_hBitmap的数据输出显示。
	*  而在离开该函数之后，src_data和src_linesize对应的该帧数据已经被MediaPlayer的VideoQueue回收，并很可能被立即分配给以后解码出的视频帧
	*
	*  所以这里有一个问题，
	*  就是SaveImage()希望保存原始尺寸而不是ViewPort窗口当前尺寸的帧图片，如果拿m_hBitmap的数据放大图片保存之可能会失真
	*  有两个方法解决：
	*  (1) 在该函数中将帧数据保存在TL_ViewPort对象之中，就是保存src_data和src_linesize中的实际数据，而不是地址
	*  (2) 使MediaPlayer在输出后，把帧保存在VideoQueue的backward_queue中，只有当backward_queue满了之后，才开始丢弃最前面的帧，
	*      这样基本可以保证在SaveImage()的时候，m_src_data和m_src_linesize对应的数据就是m_hBitmap的原始尺寸数据
	*  之前用过(2) 
	*  目前修改为第(1)种方法
	*/

	EnterCriticalSection( &m_cs );

	m_VideoTrackID = tid;
	m_pts = pts;
	m_src_w = src_w;
	m_src_h = src_h;

	{
		// copy YUV420P的数据 到ViewPort
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


	// 获取当前窗口的大小 
	CRect rc;
	::GetClientRect(m_hWnd,&rc);

	// 根据当前窗口的大小，看看是不是要重新创建bitmap句柄
	int Ret = 0;
	int dst_w = rc.Width();
	int dst_h = rc.Height();
	Ret = CreateBmp( dst_w, dst_h);

	// 将Bitmap的横纵位置针对 设置到上面创建的Bitmap的缓存位置上, 准备进行缩放
	UINT8* data[4] = {(UINT8 *)m_pSrcBuf, 0, 0, 0};
	int	linesize[4] = {(m_iWidth*3+3) &~3, 0, 0, 0};

	//SaveFrame( 1, src_data, src_linesize, src_w, src_h);
	//for( int i = 0; i < 4; i++)
	//{
	//	data[i] = src_data[i];
	//	linesize[i] = src_linesize[i];
	//}

	// 缩放
	// 注意MeidaPlayer已经修改为直接保存解码出的原始YUV420P的数据
	//Ret = TL_SwsSscale_RGB24_to_BGR24( m_src_data, m_src_linesize, m_src_w, m_src_h, data, linesize, dst_w, dst_h);
	Ret = TL_SwsSscale_YUV420P_to_BGR24( m_src_data, m_src_linesize, m_src_w, m_src_h, data, linesize, dst_w, dst_h);

	LeaveCriticalSection( &m_cs );
	return 0;
}

// 更新ViewPort的显示
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

int TL_ViewPort::CreateBmp(int w,int h)///创建位图
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
	* 注意看一下 SetRGBData()中的注释，是关于在该函数中使用之前保存的m_src_data指针数据的
	*
	*/

	//if(m_Index==1)
	{
		//SaveFrame( 1, m_src_data, m_src_linesize, m_src_w, m_src_h);

		// 第一次截图的时候，创建用于保存的Bitmap结构
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strMsg;
	strMsg.Format(_T("TL_ViewPort::OnLButtonDblClk 0x%x < %d %d >\n"),this, (int)point.x, (int)point.y);
	OutputDebugString(strMsg);

	if(m_Index!=-1)
	{
		::SendMessage(this->GetParent()->m_hWnd,WM_TL_PLAY_VIEW_PORT_ON_DCLICK,0,m_Index);
	}
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}



