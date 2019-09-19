
#ifndef _EPH1660_H_
#define _EPH1660_H_



#ifndef _IN_EPH1660
extern void Delayms(uint16 ms);
extern void ResetEPH1660(void);
extern uint8 GenerateCRC(uint8 *Packet,int size);
extern void MasterSPIInitiEPH1660(void);
extern void MasterSPI_Send_Eph1660_Command(uint8 *Databuf,uint8 l);
extern void EPH1660Manager(void);



extern uint8 xdata Num;
extern uint8 xdata MCUSndDataLength,MCUSndDataPosition;	//数据长度,数据位置
extern xdata uint8 code *MCUSndDataBuff;	//数据指针
extern uint8 xdata SPIRecData;	//接收数据缓存

extern uint8 bdata flag;
extern bit RecFramFinishFlag;	//接收完一帧数据
extern bit MCUSndFlag;		//单片机发送标志
extern bit MCUSndCmdFinishFlag;	//一帧命令发送完成标志
extern bit VerifyingFlag;	//正在计算校验和标志

//----------------------------------------------------------------------------------------------
extern uint8 code Recognition_Mode[]; //设置汉字识别模式
extern uint8 code GraphicMode[];	//设置为图片模式
extern uint8 code Set_Area[];    //设置手写区域       

extern uint8 code SetInkingOff[];		//设置轨迹关闭
extern uint8 code WakeUpEPH1660[];
extern uint8 code TestCode[];
extern uint8 code SetEnaTapWake[];	//无输入16S后进入省电模式
extern uint8 code SetNomalMode[];	//不进入省电模
extern uint8 code ExitPowerSavingMode[];	//退出省电模式
extern uint8 code SetStrokeOverWaitingTime[];//设置画笔等待时间
extern uint8 code SoftwareReset[];	//软件复位
//----------------------------------------------------------------------------------------------

extern uint8   xdata  Rec_Data[8];//接收数据缓冲区


//CRC-8 8bits checksum table
extern uint8 code CRCTable[];
#endif


#endif