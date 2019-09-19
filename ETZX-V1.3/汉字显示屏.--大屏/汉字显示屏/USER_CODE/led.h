#ifndef _LED_H_
#define _LED_H_

//位置点定义
typedef struct Point{
	INT16U x;
	INT16U y;
}POINT;
//区域定义
typedef struct Rect{
	POINT p1;
	POINT p2;
} RECT;
//帧缓冲
typedef struct{ 
	INT8U buf[128*32/8];
}FRAMEBUFFER;
//屏幕控制参数
typedef struct Screen {
	INT16U width;   //屏幕宽度
	INT16U height;  //屏幕高度
	INT16U BUF_width;  //内存宽度
	INT16U BUF_height; //内存高度
	RECT DISP_area; //当前有效的显示区域
	INT16U DISP_x; //当前显示位置x
	INT16U DISP_y; //当前显示位置y
	RECT OP_area;  //当前操作有效区域
	POINT OP_point; //当前操作位置
	FRAMEBUFFER frame[2]; //显示缓冲区
	INT8U DISP_frame; //当前显示
	INT8U RunTime_frame; //正在执行显示
	INT8U OP_frame; //当前操作
	INT8U DISP_first; //是第几屏
}SCREEN;

//字体定义
#define FONT_DEFAULT 0
#define FONT_ARIAL 1
#define FONT_ROMAN 2

extern SCREEN screen; //屏幕定义
extern const INT8U Eter_logo[128/8*32];
//正常图标
extern const INT8U Icon_ok[8];
//错误图标
extern const INT8U Icon_error[8];
//本机故障
extern const INT8U Icon_failure[8];
//总线错误
extern const INT8U Icon_buserror[8];

extern void LED_Init(void);
extern void SetArea(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void DrawLine(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void DrawRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void SetPixel(INT8U x,INT8U y, INT8U data);
extern void PutPixelDatas(INT8U *buf,INT16U len);
extern void TextOut(INT16S x,INT16S y,INT8U * txt,INT8U font, INT8U size);
extern void ShowNextLine(void); //显示当前行内容
extern void PutDataToSSP(void);
extern void ClsScreen(void);
extern INT16U GetTextShowLength(INT8U * txt,INT8U size);
extern void SetDisplayFrame(INT8U index);
extern void SetOperateFrame(INT8U index);
extern INT8U GetNextDisplayFrame(void);
extern void ClsScreenForLine(INT8U start, INT8U end);
extern void FullScreen(void);
#endif


