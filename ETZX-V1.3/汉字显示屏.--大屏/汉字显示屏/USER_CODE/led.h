#ifndef _LED_H_
#define _LED_H_

//λ�õ㶨��
typedef struct Point{
	INT16U x;
	INT16U y;
}POINT;
//������
typedef struct Rect{
	POINT p1;
	POINT p2;
} RECT;
//֡����
typedef struct{ 
	INT8U buf[128*32/8];
}FRAMEBUFFER;
//��Ļ���Ʋ���
typedef struct Screen {
	INT16U width;   //��Ļ���
	INT16U height;  //��Ļ�߶�
	INT16U BUF_width;  //�ڴ���
	INT16U BUF_height; //�ڴ�߶�
	RECT DISP_area; //��ǰ��Ч����ʾ����
	INT16U DISP_x; //��ǰ��ʾλ��x
	INT16U DISP_y; //��ǰ��ʾλ��y
	RECT OP_area;  //��ǰ������Ч����
	POINT OP_point; //��ǰ����λ��
	FRAMEBUFFER frame[2]; //��ʾ������
	INT8U DISP_frame; //��ǰ��ʾ
	INT8U RunTime_frame; //����ִ����ʾ
	INT8U OP_frame; //��ǰ����
	INT8U DISP_first; //�ǵڼ���
}SCREEN;

//���嶨��
#define FONT_DEFAULT 0
#define FONT_ARIAL 1
#define FONT_ROMAN 2

extern SCREEN screen; //��Ļ����
extern const INT8U Eter_logo[128/8*32];
//����ͼ��
extern const INT8U Icon_ok[8];
//����ͼ��
extern const INT8U Icon_error[8];
//��������
extern const INT8U Icon_failure[8];
//���ߴ���
extern const INT8U Icon_buserror[8];

extern void LED_Init(void);
extern void SetArea(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void DrawLine(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void DrawRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void SetPixel(INT8U x,INT8U y, INT8U data);
extern void PutPixelDatas(INT8U *buf,INT16U len);
extern void TextOut(INT16S x,INT16S y,INT8U * txt,INT8U font, INT8U size);
extern void ShowNextLine(void); //��ʾ��ǰ������
extern void PutDataToSSP(void);
extern void ClsScreen(void);
extern INT16U GetTextShowLength(INT8U * txt,INT8U size);
extern void SetDisplayFrame(INT8U index);
extern void SetOperateFrame(INT8U index);
extern INT8U GetNextDisplayFrame(void);
extern void ClsScreenForLine(INT8U start, INT8U end);
extern void FullScreen(void);
#endif


