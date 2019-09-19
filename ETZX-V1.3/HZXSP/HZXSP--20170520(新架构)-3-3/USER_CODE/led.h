#ifndef _LED_H_
#define _LED_H_

#define RED 1
#define GREEN 2
#define YELLOW	3


#define LED_WIDTH 96   	//��Ļ���
#define LED_HEIGHT 16	//��Ļ�߶�

#define BUF_WIDTH	96   //�ڴ���
#define BUF_HEIGHT	16   //�ڴ�߶�

#define BYTES_LINE	(LED_WIDTH/8)	  			//   ÿ��12���ֽ�
#define BYTES_SCREEN (LED_WIDTH/8*LED_HEIGHT)  	//   ÿ��192���ֽ�


#define COLOR_RED	1
#define COLOR_GREEN	2
#define COLOR_ORANGE	3



#define A8BaseAdd	0x00000000
#define D8BaseAdd	0x000005f0		//����
#define C16BaseAdd 	0x00000690



#define SYS_INFO_ADD 		0x003c0000		//ϵͳ��Ϣ��ʼ��ַ
#define SYS_INFO_ADD_BAK 	0x003D0000		//������ϵͳ��Ϣ��ʼ��ַ
#define FRAME_TEST_ADD (0x003e0000)


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
/*typedef struct{ 
	INT8U buf[128*32/8];
}FRAMEBUFFER;*/

typedef struct{ 
	INT8U buf[96*32/8];	//һ��6������
}FRAMEBUFFER;

//��Ļ���Ʋ���
typedef struct Screen {
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
#define FONT_BOLD	3

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



extern INT8U first;

extern const unsigned char FontDots16[3][34];




extern void ClsScreen(INT8U color);
extern void ClsScreenForLine(INT8U start, INT8U end,INT8U color);
extern void FullScreen(INT8U color);
extern void ClearRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color);

extern void LED_Init(void);
extern void SetArea(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void DrawLine(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color);
extern void DrawRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color);
extern void SetPixel(INT8U x,INT8U y, INT8U data,INT8U color);
extern void PutPixelDatas(INT8U *buf,INT16U len,INT8U color);
extern void TextOutRed(INT16S x,INT16S y,INT8U * txt,INT8U font, INT8U size);
extern void TextOutGreen(INT16S x,INT16S y,INT8U * txt,INT8U font, INT8U size);


extern void TextOutGreen_DotData(INT16S x,INT16S y,INT8U * txt,INT8U font,INT8U size);


extern void ShowNextLine(void); //��ʾ��ǰ������
extern void PutDataToSSP(void);
extern INT16U GetTextShowLength(INT8U * txt,INT8U size);
extern void SetDisplayFrame(INT8U index);
extern void SetOperateFrame(INT8U index);
extern INT8U GetNextDisplayFrame(void);
#endif


