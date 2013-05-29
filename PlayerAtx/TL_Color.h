#pragma once

#if 0
// 用于调试，区分窗口，方便调整窗口位置，大小，设置布局的配色方案
#define PlayTimeFontBkColor		RGB(0,0,255)		// 播放时间 文字背景颜色
#define PlayTimeFontColor		RGB(255,0,255)		// 播放时间	文字颜色
#define PlayTimeBkColor			RGB(0,0,0)			// 播放时间 CStatic背景颜色
#define PlayStatusBkColor		RGB(0,0,0)			// 播放状态 CStatic背景颜色
#define ViewPortBkColor			RGB(128,128,128)
#define ViewWinBkColor			RGB(150,150,150)
#define ViewWinWrapBkColor		RGB(0,255,0)
#define PlayerAtxBkColor		RGB(0,0,0)
#else
// 用于发布
#define PlayTimeFontBkColor		RGB(0,0,0)			// 播放时间 文字背景颜色
#define PlayTimeFontColor		RGB(255,255,255)	// 播放时间	文字颜色
#define PlayTimeBkColor			RGB(0,0,0)			// 播放时间 CStatic背景颜色
#define PlayStatusBkColor		RGB(0,0,0)			// 播放状态 CStatic背景颜色
#define ViewPortBkColor			RGB(0,0,0)
#define ViewWinBkColor			RGB(0,0,0)
#define ViewWinWrapBkColor		RGB(0,0,0)
#define PlayerAtxBkColor		RGB(0,0,0)
#endif