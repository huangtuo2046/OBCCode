#include "includes.h"
#include "Includes.h"
#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <malloc.h>

extern OS_EVENT * Lock_Q_TTC;	
extern OS_EVENT * CAN_Q_REC_TTC;			//TTC数据报接收标志
extern OS_EVENT * PC_Q;
extern OS_EVENT * PC_UQ;
extern OS_EVENT * PCQFlag;
extern OS_EVENT * RS422_Q_Rec;
extern OS_EVENT * sendMsg;

extern INT8U LockFirst; 
extern int GpsOnOff;
extern INT8U StarFlag;


extern OS_EVENT *SHARE_PRINTF;//共享控制
extern OS_EVENT *SHARE_SENDDOWN;
extern OS_EVENT *SHARE_CAN;  //CAN

extern INT8U ERR_SHARE_PRINTF;
extern INT8U ERR_SHARE_SENDDOWN;
extern INT8U ERR_SHARE_CAN;

extern INT32U PCSsecond,PCFsecond;
extern INT16U PCSweek,PCFweek;

INT8U StarOFF[] = {0xEB,0x90,0xDC,0x32,0x0F,0x2C,0x2C,0x2C,0x2C,0x2C,0x2C,0x56,0x16,
        0x54,0x00,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};
INT8U StarON[] = {0xEB,0x90,0xDC,0x32,0x0F,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x5a,0x26,
        0x64,0x40,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};

INT8U MEMSGryoOFF[] = {0xEB,0x90,0xDC,0x32,0x0F,0x24,0x24,0x24,0x24,0x24,0x24,0x45,0x15,
        0x14,0x00,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};
INT8U MEMSGryoON[] = {0xEB,0x90,0xDC,0x32,0x0F,0x25,0x25,0x25,0x25,0x25,0x25,0x49,0x25,
        0x24,0x40,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};

INT8U ADCSOFF[] = {0xEB,0x90,0xDC,0x32,0x0F,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x22,0x15,
        0x54,0x14,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};
INT8U ADCSON[] = {0xEB,0x90,0xDC,0x32,0x0F,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x26,0x25,
        0x64,0x54,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};

INT8U MWON[] = {0xEB,0x90,0xDC,0x32,0x0F,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x25,0x41,
        0x90,0x14,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};
INT8U MWOFF[] = {0xEB,0x90,0xDC,0x32,0x0F,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x29,0x51,
        0xa0,0x54,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0xDD,0x09,0xD7};

INT8U BDGPSON[8] = {0xF1,0x2, 0x06, 0xFF, 0x00, 0x00, 0x07, 0xF1};
INT8U BDGPSOFF[8] = {0xF1,0x2, 0x06, 0x00, 0x00, 0x00, 0x08, 0xF1};

/*
 *------------------------------------------------------------*/
extern OS_EVENT * CAN_Q_Rec;			//TTC数据报接收标志

INT8U RF_Telemitry[8] =    {0xEB,0x90,0xBB,0x01,0x00,0x08,0x90,0x1D};
INT8U RF_Photograph[8] =   {0xEB,0x90,0xBB,0x02,0x00,0x08,0x90,0x1D};
INT8U RF_Take_To_OBC[10] = {0xEB,0x90,0xBB,0x03,0x00,0x0A,0x00,0x00,0x90,0x1D};

INT32U addr = PC_DATA;
extern INT32U PC_Cycle;
extern INT16U gps_week;
extern INT32U gps_second;
void RF();


/*00000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*00000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
INT8U MCU2_Cycle[9]         = {0x02,0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x06};
INT8U GYRO_OFF[9]           = {0x01,0x01,0x01,0x02,0x03,0x00,0x00,0x00,0x05};
INT8U GYRO_ON[9]            = {0x01,0x01,0x01,0x02,0x03,0xff,0x00,0x00,0x04};
INT8U GYRO_Telemitry[9]     = {0x02,0x01,0x01,0x04,0x55,0x01,0x00,0x00,0x5a};

INT8U BDGPSbuf[160];
INT8U EquId;
extern INT8U STATE;
INT16U all_photolen =0;
INT32U NumCount,PCNumCount;
void RS422_Data_Send( INT8U type , INT8U *data , INT8U lenH, INT8U lenL);
INT8U GetBDData(int FrameCount);
/*------------------------------------------------------------------------------*/

void ProgramControl(void *jdata) 
{
        int i, j, k, id, len, SId, iStar = 0;
        INT8U *hdlcmsg, *PCMsg,SendFrameFlag;
        INT8U msg,err, down_frame[8], DataCount, datarequest[20], sum, Count;
        INT8U *FrameBack,Data = 0x67;
        INT16U  timedata, PollCount;
        /*-----------------------------------------*/
        //INT32U addr, StarCount;
        INT32U StarCount,SmallAddr;
        INT8U * gryomsg;
        INT32U  zwj_num = 0;
        /*-----------------------------------------*/
        INT8U  BCFrame[8],scc3buf[150], ErrTimes, _isSuccess;
        INT8U StarOrder[4] = {0x74,0xA1,0xFF,0x00};
        INT8U CMD[6] = {0x10,0x00,0xDD,0xDD,0x10,0x03};
        INT8U CANData[9] = {0x01, 0x01, 0x01, 0x02, 0x06, 0xFF, 0x00, 0x00, 0x07};
        INT32U  tx_va;
        INT32U  tx_mode;
        can_flow data_flow;

        ////printf("PC Start!");

        for(;;)
        { 
                NumCount = 0;
                OSQFlush(PC_Q); 
                OSQFlush(PCQFlag); 
                GpsOnOff = 1;
                hdlcmsg = (INT8U *)OSQPend(PC_Q,0,&err);
                SId = *(hdlcmsg-7)+ (*(hdlcmsg-8))*0x100; 
                ////printf("SId = %d",SId);
                ////printf ("|||||||||||||||||||||||||||||||||||||\n");
                if(SId == 0x0E)
                {
                        //shu ju xia chuan

                        /*0000000000000000000000000000000000000000000000000000000000000*/
                        down_frame[0] = 0x0E;
                        down_frame[1] = EquId;
                        if (down_frame[1] == 0x04) {
                                down_frame[2] = all_photolen;
                                down_frame[3] = all_photolen >> 8;
                                all_photolen = 0;
                        }
                        else{
                                down_frame[2] = PCNumCount;
                                down_frame[3] = PCNumCount >> 8;
                        }
                        /*000000000000000000000000000000000000000000000000000000000000000*/

                        ////printf("PCNumCount = %d\n",PCNumCount);
                        OSQPost(sendMsg,down_frame);
                }
                /*
                 * --------------------------------------------------*/
                /*000000000000000000000000000000000000000000000000000000000000000*/
                /*000000000000000000000000000000000000000000000000000000000000000*/
                /*  赵文军 完成 程控 周期的 设置  操作 */
                else if (SId == 0x00) {
                        PC_Cycle = *hdlcmsg;
                        PC_Cycle = PC_Cycle << 8;
                        PC_Cycle += *(hdlcmsg+1);
                        ////printf ("PC_Cycle = %04x \n", PC_Cycle);
                }
#if 0
                else if (SId == 0x00) {
                        NumCount = *hdlcmsg;
                        NumCount = NumCount << 8;
                        NumCount += *(hdlcmsg+1);
                        ////printf ("------------------------------------------\n");
                        ////printf ("NumCount = %d \n", NumCount);
                        ////printf ("------------------------------------------\n");
                }
#endif
                /*000000000000000000000000000000000000000000000000000000000000000*/
                /*000000000000000000000000000000000000000000000000000000000000000*/
                /*-------------------------------------------------------------*/
                else
                {
                        PCSweek = (*hdlcmsg)*0x100+(*(hdlcmsg+1));/*起始时间*/ 
                        PCSsecond=(*(hdlcmsg+2))*0x10000+(*(hdlcmsg+3))*0x100+(*(hdlcmsg+4));

                        PCFweek = (*(hdlcmsg+5))*0x100+(*(hdlcmsg+6));/*结束时间*/  
                        PCFsecond=(*(hdlcmsg+7))*0x10000+(*(hdlcmsg+8))*0x100+(*(hdlcmsg+9));

                        ////printf("PCTime = %x %x %x %x\n",PCSweek, PCSsecond, PCFweek, PCFsecond);
                        /*
                         * --------------------------------------------------*/
                        /*000000000000000000000000000000000000000000000000000000000000000*/
                        /*000000000000000000000000000000000000000000000000000000000000000*/
#if 1
                        if(PCSweek == PCFweek)
                                NumCount = (PCFsecond - PCSsecond) / PC_Cycle ;
                        else
                                NumCount = (604800 - PCFsecond + PCSsecond) / PC_Cycle ;
#endif
                        /*000000000000000000000000000000000000000000000000000000000000000*/
                        /*000000000000000000000000000000000000000000000000000000000000000*/
                        /*-------------------------------------------------------------*/

                        if((PCSweek > PCFweek)
                                        || (PCSsecond >= PCFsecond)
                                        || ((gps_week >= PCSweek) && (gps_second >= PCSsecond))
                                        || (NumCount == 0) )
                                continue;

                        EquId = SId;
                        ////printf ("SId  :  %x \n",SId);
                        iStar = 0;

                        PCMsg = (INT8U *)OSQPend(PCQFlag,0,&err);
                        msg = *PCMsg;
                        ////printf("msg = %d\n",msg);
                        if(msg == 0x01)//kai wai she
                        {
                                zwj_num = 0;
                                ////printf("PCMsg = 1;\n");
                                /*---------------------------------------*/
                                addr = PC_DATA;
                                /*------------------------------------------*/
                                id=0x00;
                                len=29;
                                data_flow.id=ID_TTC1;
                                data_flow.f=TTC_TC_DATASTREAM;
                                data_flow.i=id>>8;
                                data_flow.i2=id&0xff;
                                data_flow.l=len>>8;
                                data_flow.l2=len&0xff;
                                switch(SId)
                                {
                                        /*------------------------------------*/
                                        //*******
                                        //704_GYRO
                                        //*******			
                                        case 0x0A:
                                                PCNumCount = 0;
                                                ////printf ("704_GYRO kai\n");
                                                /*0000000000000000000000000000000000000000000*/
                                                /*0000000000000000000000000000000000000000000*/
                                                OSSchedLock();
                                                can_datagram_send(MCU2_Cycle,1,1);
                                                OSSchedUnlock();
                                                OSTimeDly(1800);
                                                can_datagram_send(GYRO_ON,1,1);
                                                STATE |= 0x04; 
                                                ////printf ("STATE   :   %02x", STATE);
                                                /*000000000000000000000000000000000000000000*/
                                                /*0000000000000000000000000000000000000000000*/
                                                break;

                                                /*----------------------------------*/
                                                //*******
                                                //Star
                                                //*******			
                                        case 0x05:
                                                PCNumCount = 0;
                                                for(i = 0; i < 29; i++)
                                                        data_flow.d[i] = StarON[i];

                                                can_datagram_send((INT8U *)&data_flow,3,CANPUT);
                                                OSTimeDly(1000);
                                                ////printf("Star Open!");
                                                break;

                                                //*******
                                                //MEMSGryo
                                                //*******
                                        case 0x06:
                                                PCNumCount = 0;
                                                for(i = 0; i < 29; i++)
                                                        data_flow.d[i] = MEMSGryoON[i];


                                                can_datagram_send((INT8U *)&data_flow,3,CANPUT);
                                                OSTimeDly(1000);
                                                break;

                                }
                        }

                        for (;;) {
                                ////printf ("zwj_num : %d\n", zwj_num);
                                if (zwj_num > NumCount)
                                        break;
                                zwj_num += 1;
                                /*-------------------------------------------------------------*/
                                if (addr > (495*1024+PC_DATA))
                                        addr = PC_DATA;
                                /*-------------------------------------------------------------*/
                                ////printf("PCMsg = 2;SId = %d\n",SId);
                                switch(SId)
                                {
                                        /* ----------------------------------------------------------*/
                                        //*******
                                        //704_GYRO
                                        //*******			
                                        case 0x0A:
                                                ////printf ("704_GYRO  yao shu\n");
                                                can_datagram_send(GYRO_Telemitry,1,1);
                                                gryomsg = (INT8U *)OSQPend(CAN_Q_Rec,500,&err);

                                                if (err == OS_NO_ERR) {
                                                        for (i = 0 ; i < 22; i++) {
                                                                *(volatile INT8U *) (addr+i) = *(gryomsg + i );
                                                                ////printf ("%02x\n",*(gryomsg + i));
                                                        }
                                                        addr = addr + 22;
                                                        PCNumCount++;
                                                }

                                                OSTimeDly(1000);
                                                break;

                                                /* -------------------------------------------------*/

                                                //*******
                                                //STAR
                                                //*******
                                        case 0x05:

                                                RS422_Data_Send( 2 , StarOrder, 0 , 4); 
                                                FrameBack = (INT8U *)OSQPend(RS422_Q_Rec,300,&err);	
                                                if(err==OS_NO_ERR)
                                                {
                                                        //////printf("Star:\n");
                                                        for(j = 0; j < 27; j++)
                                                        {
                                                                //	////printf("%02x ",*(FrameBack+j+2));
                                                                *(volatile INT8U *)(addr+j) = *(FrameBack+j+2);
                                                        }
                                                        addr += 27;
                                                        PCNumCount++;
                                                        ////printf("\nStarCount = %d\n",StarCount++);
                                                }
                                                else
                                                {
                                                        ////printf("Star Sensor Task could not work StarCount = %d.\n ",StarCount);
                                                }
                                                OSTimeDly(900);

                                                break;

                                                //*******
                                                //MEMSGryo
                                                //*******
                                        case 0x06:						

                                                RS422_Data_Send( 2 , &Data, 0 , 1); 
                                                FrameBack=(INT8U *)OSQPend(RS422_Q_Rec,500,&err);//2000个时钟节拍超时
                                                if(err==OS_NO_ERR)	
                                                {
                                                        for(i = 0; i < 21; i++)
                                                        {
                                                                //	////printf("Gyro[%d] %02x  ",i, *(FrameBack+i));
                                                                *(volatile INT8U *)(addr+i) = *(FrameBack+i+2);
                                                        }
                                                        addr += 21;
                                                        PCNumCount++;
                                                        ////printf("MEMSGryo!!!!!!!!!!!!!PCNumCount = %d\n",PCNumCount);
                                                }
                                                else
                                                {
                                                        ////printf("MEMSGryo!!!!!!!!!!!!!!!!!Wrong!");
                                                }

                                                OSTimeDly(1000);
                                                break;

                                        default:

                                                break;

                                }
                        }
                        ////printf("PCMsg = 3;\n");
                        addr = PC_DATA;
                        GpsOnOff = 1;
                        switch(SId)
                        {/* ------------------------------------------------------*/
                                //*******
                                //704_GYRO
                                //*******			
                                case 0x0A:
                                        ////printf ("704  guan \n");
                                        can_datagram_send(GYRO_OFF,1,1);
                                        /*0000000000000000000000000000000000000000000000*/
                                        /*0000000000000000000000000000000000000000000000*/
                                        /*0000000000000000000000000000000000000000000000*/
                                        /*0000000000000000000000000000000000000000000000*/
                                        STATE &= 0xfb; 
                                        ////printf ("STATE : %02x \n", STATE);
                                        break;

                                        /* --------------------------------------------------------*/
                                        //*******
                                        //STAR
                                        //*******
                                case 0x05:

                                        datarequest[0] = 0x74;
                                        datarequest[1] = 0xA0;
                                        datarequest[2] = 0xFF;
                                        datarequest[3] = 0x01;
                                        datarequest[4] = 0x21;
                                        datarequest[5] = 0x21;

                                        _isSuccess=0;
                                        ErrTimes=3;
                                        while(ErrTimes>0 && _isSuccess==0)
                                        {
                                                RS422_Data_Send( 2 ,(INT8U *)datarequest, 0 , 6); 

                                                ////printf("\n ");

                                                FrameBack=(INT8U *)OSQPend(RS422_Q_Rec,100,&err); //得到星敏的图像数据

                                                if(err==OS_NO_ERR )
                                                {
                                                        ErrTimes=3;
                                                        _isSuccess=1;

                                                }
                                                else
                                                        ErrTimes--;
                                        }
                                        if(_isSuccess != 1)
                                        {
                                                ////printf("ST task can't connect well with ST\n");

                                                break;
                                        }

                                        OSTimeDly(5000);

                                        LockFirst = 3;

                                        datarequest[0] = 0x74;
                                        datarequest[1] = 0xA2;
                                        datarequest[2] = 0xFF;
                                        datarequest[3] = 0x02;
                                        for(j = 0; j < 8192; j++)//循环发起【请求数据帧】命令
                                        {
                                                datarequest[4] = (INT8U)( j>>8 );  //数据段高字节
                                                datarequest[5] = (INT8U)(j&0x00FF); //数据段低字节
                                                datarequest[6] = (INT8U)( (datarequest[4]+datarequest[5])&0x00FF);  //校验和
                                                _isSuccess=0;
                                                ErrTimes=3;
                                                while(ErrTimes>0 && _isSuccess==0)
                                                {
                                                        RS422_Data_Send( 2 ,(INT8U *)datarequest, 0 , 7); 
                                                        ////printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",datarequest[0],datarequest[1],datarequest[2],datarequest[3],datarequest[4],datarequest[5],datarequest[6],datarequest[7]);

                                                        FrameBack=(INT8U *)OSQPend(RS422_Q_Rec,500,&err); 
                                                        if(err==OS_NO_ERR )
                                                        {
                                                                ErrTimes=3;
                                                                _isSuccess=1;

                                                        }
                                                        else
                                                                ErrTimes--;
                                                }
                                                if(_isSuccess != 1)
                                                {
                                                        ////printf("ST task can't connect well with ST\n");

                                                        break;
                                                }

                                                for(k=0; k < 128; k++)//数据存储到6M数据区
                                                {
                                                        *(volatile INT8U *)(EIS_DATA+128*j+k) = *(FrameBack+8+k); 
                                                }

                                                ////printf("databack: %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x\n",FrameBack[0],FrameBack[1],FrameBack[2],FrameBack[3],FrameBack[4],FrameBack[5],FrameBack[6],FrameBack[7],FrameBack[8],FrameBack[9],FrameBack[10]);


                                        }
                                        if( j < 8192 )
                                        {
                                                LockFirst = 1;

                                                break;
                                        }
                                        SmallAddr = EIS_DATA_SMALL;

                                        for(j = 0; j < 256; j++)
                                        {
                                                for(k = 0; k < 256; k++)
                                                        *(volatile INT8U*)(SmallAddr+j*256+k) = *(volatile INT8U *)(EIS_DATA + 1024*j*4 + k*4);
                                        }
                                        ////printf("Small Photo OK!\n");
                                        StarFlag = 1;	
                                        LockFirst = 1;
                                        for(i = 0; i < 29; i++)
                                                data_flow.d[i] = StarOFF[i];
                                        OSTimeDly(100);
                                        can_datagram_send((INT8U *)&data_flow,3,CANPUT);
                                        break;

                                        //*******
                                        //MEMSGryo
                                        //*******
                                case 0x06:
                                        for(i = 0; i < 29; i++)
                                                data_flow.d[i] = MEMSGryoOFF[i];
                                        OSTimeDly(10);
                                        can_datagram_send((INT8U *)&data_flow,3,CANPUT);
                                        break;

                        }
                }

        }

}


void ProgramControlUart(void *jdata) 
{
        int i, j, k, id, len, SId, iStar = 0;
        INT8U *hdlcmsg, *PCMsg,SendFrameFlag;
        INT8U msg,err, down_frame[8], DataCount, datarequest[20], sum, Count;
        INT8U *FrameBack,Data = 0x67;
        INT16U  timedata, PollCount;
        /*-----------------------------------------*/
        //INT32U addr, StarCount;
        INT32U StarCount,SmallAddr;
        INT8U * gryomsg;
        INT32U  zwj_num = 0;
        /*-----------------------------------------*/
        INT8U  BCFrame[8],scc3buf[150], ErrTimes, _isSuccess;
        INT8U StarOrder[4] = {0x74,0xA1,0xFF,0x00};
        INT8U CMD[6] = {0x10,0x00,0xDD,0xDD,0x10,0x03};
        INT8U CANData[9] = {0x01, 0x01, 0x01, 0x02, 0x06, 0xFF, 0x00, 0x00, 0x07};
        INT32U  tx_va;
        INT32U  tx_mode;
        can_flow data_flow;

        ////printf("PC Uart Start!");

        for(;;)
        { 
                OSQFlush(PC_UQ); 
                OSQFlush(PCQFlag); 
                GpsOnOff = 1;
                hdlcmsg = (INT8U *)OSQPend(PC_UQ,0,&err);
                SId = *(hdlcmsg-7)+ (*(hdlcmsg-8))*0x100; 
                ////printf("Uart SId = %d",SId);
                ////printf ("|||||||||||||||||||||||||||||||||||||\n");

                PCSweek = (*hdlcmsg)*0x100+(*(hdlcmsg+1));/*起始时间*/ 
                PCSsecond=(*(hdlcmsg+2))*0x10000+(*(hdlcmsg+3))*0x100+(*(hdlcmsg+4));

                PCFweek = (*(hdlcmsg+5))*0x100+(*(hdlcmsg+6));/*结束时间*/  
                PCFsecond=(*(hdlcmsg+7))*0x10000+(*(hdlcmsg+8))*0x100+(*(hdlcmsg+9));

                ////printf("PCTime = %x %x %x %x\n",PCSweek, PCSsecond, PCFweek, PCFsecond);
#if 1
                if(PCSweek == PCFweek)
                        NumCount = (PCFsecond - PCSsecond) / PC_Cycle ;
                else
                        NumCount = (604800 - PCFsecond + PCSsecond) / PC_Cycle ;
#endif

                if((PCSweek > PCFweek)
                                || (PCSsecond >= PCFsecond)
                                || ((gps_week >= PCSweek) && (gps_second >= PCSsecond))
                                || (NumCount == 0) )
                        continue;

                EquId = SId;
                ////printf ("SId  :  %x \n",SId);
                iStar = 0;

                PCMsg = (INT8U *)OSQPend(PCQFlag,0,&err);
                msg = *PCMsg;
                ////printf("msg = %d\n",msg);
                if(msg == 0x01)//kai wai she
                {
                        zwj_num = 0;
                        ////printf("PCMsg = 1;\n");
                        /*---------------------------------------*/
                        addr = PC_DATA;
                        /*------------------------------------------*/
                        id=0x00;
                        len=29;
                        data_flow.id=ID_TTC1;
                        data_flow.f=TTC_TC_DATASTREAM;
                        data_flow.i=id>>8;
                        data_flow.i2=id&0xff;
                        data_flow.l=len>>8;
                        data_flow.l2=len&0xff;
                        switch(SId)
                        {
                                //*******
                                //RF
                                //*******			
                                case 0x04:
                                        GpsOnOff = 0;
                                        ////printf("B Star On!\n");
                                        all_photolen = 0;
                                        ////printf ("all_photolen :  %d \n", all_photolen);
                                        break;
                                        /*----------------------------------*/

                                        //*******
                                        //BDGPS
                                        //*******
                                case 0x08:	
                                        ////printf("BDGPS Start!\n");

                                        PCNumCount = 0;
                                        GpsOnOff = 0;
                                        tx_va = 1;
                                        tx_mode = 1;
                                        CANData[5] = 0xFF;
                                        CANData[8] = 0x07;
                                        can_datagram_send(CANData, tx_va, tx_mode);
                                        OSTimeDly(900);

                                        ////printf("BDGPS Open!\n");

                                        //OSTimeDly(500);
                                        OSSchedLock();
                                        changemux(0);
                                        for(j = 0; j < 6; j++)
                                        {
                                                scc3_serial_putc(CMD[j]);
                                                //	////printf("%x \n",CMD[j]);	
                                        }
                                        OSSchedUnlock();

                                        break;

                        }
                }

                for (;;) {
                        ////printf ("zwj_num : %d\n", zwj_num);
                        if (zwj_num > NumCount)
                                break;
                        zwj_num += 1;
                        /*-------------------------------------------------------------*/
                        if (addr > (495*1024+PC_DATA))
                                addr = PC_DATA;
                        /*-------------------------------------------------------------*/
                        ////printf("PCMsg = 2;SId = %d\n",SId);
                        switch(SId)
                        {
                                /* ----------------------------------------------------------*/

                                //*******
                                //RF
                                //*******
                                case 0x04:
                                        ////printf ("RF yao shju \n");
                                        RF();
                                        ////printf ("all_photolen :  %d \n", all_photolen);
                                        OSTimeDly(30000);
                                        OSTimeDly(30000);
                                        OSTimeDly(30000);
                                        break;
                                        /* -------------------------------------------------*/


                                        //*******
                                        //BDGPS
                                        //*******
                                case 0x08:
                                        if((SendFrameFlag = GetBDData(2)) != 0xFF)
                                        {
                                                PCNumCount++;
                                                OSTimeDly(1000);
                                                break;
                                        }
                                        else
                                        {
                                                OSTimeDly(1000);
                                                ////printf("BDGPS jie shou TIMEOUT!\n");
                                        }
                                        break;
                                default:

                                        break;

                        }
                }
                ////printf("PCMsg = 3;\n");
                addr = PC_DATA;
                GpsOnOff = 1;
                switch(SId)
                {
                        /* ------------------------------------------------------*/

                        //*******
                        //RF
                        //*******			
                        case 0x04:
                                ////printf("B Star OFF!\n");
                                ////printf ("all_photolen :  %d \n", all_photolen);
                                break;
                                /* --------------------------------------------------------*/

                                //*******
                                //BDGPS
                                //*******
                        case 0x08:
                                tx_va = 1;
                                tx_mode = 1;
                                CANData[5] = 0x00;
                                CANData[8] = 0x08;
                                can_datagram_send(CANData, tx_va, tx_mode);
                                break;
                        default:

                                break;

                }


        }

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void RF()
{
        INT8U num = 0 , k;
        INT16U PhotoLen;
        INT8U *BackData, Count;
        INT32U timedata, DataCount = 0, i, l,j;
        INT8U scc3buf[550], sum = 0;
        INT8U down_frame[5];

        ////printf("RF task is ready\n");

        // 获取 小星 数据
        for (;;){
                if(DataCount++ > 30) 
                        break;
                memset(scc3buf, 0, 50);
                ////printf ("****************huo qu ji ben zhuang tai xin xi start\n");

                OSSchedLock();
                changemux(3);
                for(j = 0; j < 8; j++)
                        scc3_serial_putc(RF_Telemitry[j]);
                OSSchedUnlock();

                OSTimeDly(1800); // 一秒钟的时钟 节拍 数 是 907 


                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(100000*5))) timedata++;
                if (timedata>=(100000*5)) continue;

                scc3buf[0] = scc3_serial_getc();
                if (scc3buf[0]!=0xEB) continue;	

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[1] = scc3_serial_getc();
                if (scc3buf[1]!=0x90) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[2] = scc3_serial_getc();
                if (scc3buf[2]!=0XBB) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[3] = scc3_serial_getc();
                if (scc3buf[3] !=0x01) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[4] = scc3_serial_getc();
                if (scc3buf[4] !=0x00) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[5] = scc3_serial_getc();
                if (scc3buf[5] !=0x17) continue;

                for (j=0; j<17; j++)
                {
                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<(10000*6)))     timedata++;
                        if (timedata>=(10000*6))        continue;
                        while (!scc3_serial_tstc());
                        scc3buf[6+j] = scc3_serial_getc();
                }

                if (scc3buf[21]!=0x90)
                        continue;		

                if (scc3buf[22]!=0x1D) 
                        continue; 
                else{
                        ////printf ("****************huo qu ji ben zhuang tai xin xi end\n");
                        for(i = 0;i < 20; i++) {
                                sum += scc3buf[i]; 
                        }

                        if (sum == scc3buf[20]) {
                                if (addr > (500*1024 + PC_DATA - 23))  {
                                        addr = PC_DATA;
                                        ////printf ("+++++++++++++++++\n");
                                        ////printf ("yi  chu  \n");
                                }
                                ////printf ("addr = %09x", addr);
                                for(i = 0; i < 23; i++)
                                {
                                        *(volatile INT8U *)(addr+i) = scc3buf[i];
                                        ////printf("BB CC zhuangtai ::%02x \n",scc3buf[i]);
                                }
                                addr += 23;
                                ////printf ("addr = addr + 23 ?  :  %09x", addr);
                                sum = 0;
                                break;
                        }
                        else{ 
                                sum = 0;
                                continue;
                        }
                } // inter if  else  finished  

        } // end  of  for

        ////printf ("*******************************111111111   jeishu\n");

        OSTimeDly(2000);
        for (;;){

                if( ++DataCount > 30)
                        break;
                ////printf ("************************ pai zhao  start\n");
                memset(scc3buf, 0, 50);

                OSSchedLock();
                changemux(3);
                for(j = 0; j < 8; j++)
                        scc3_serial_putc(RF_Photograph[j]);
                OSSchedUnlock();

                OSTimeDly(3500);


                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(100000*5))) timedata++;
                if (timedata>=(100000*5)) continue;

                scc3buf[0] = scc3_serial_getc();
                if (scc3buf[0]!=0xEB) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[1] = scc3_serial_getc();
                if (scc3buf[1]!=0x90) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[2] = scc3_serial_getc();
                if (scc3buf[2]!=0xBB) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[3] = scc3_serial_getc();
                if (scc3buf[3] !=0x02) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[4] = scc3_serial_getc();
                if (scc3buf[4] !=0x00) continue;

                timedata = 0;
                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) timedata++;
                if (timedata>=(10000*6)) continue;

                scc3buf[5] = scc3_serial_getc();
                if (scc3buf[5] !=0x0B) continue;


                for (j=0; j<5; j++)
                {
                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<(10000*6))) 
                                timedata++;
                        if (timedata>=(10000*6)) continue;
                        while (!scc3_serial_tstc());
                        scc3buf[6+j] = scc3_serial_getc();

                }

                if (scc3buf[9]!=0x90)
                        continue;

                if (scc3buf[10]!=0x1D) 
                        continue;
                else{
                        ////printf ("************************ pai zhao  end\n");
                        for(i = 0;i < 8; i++) {
                                sum += scc3buf[i]; 
                        }

                        if (sum == scc3buf[8]) {

                                ////printf("scc3buf: ");
                                for(i = 0; i < 11; i++)
                                {
                                        ////printf("BB CC paizhao :: %02x\n",scc3buf[i]);
                                }
                                /*000000000000000000000000000000000000000000000000000000000000000000000*/
                                /*000000000000000000000000000000000000000000000000000000000000000000000*/
                                PhotoLen = scc3buf[6] ;
                                PhotoLen = PhotoLen << 8;
                                PhotoLen +=  scc3buf[7];
                                all_photolen += PhotoLen;
                                /*000000000000000000000000000000000000000000000000000000000000000000000*/
                                /*000000000000000000000000000000000000000000000000000000000000000000000*/
                                sum = 0;
                                break;

                        }
                        else {

                                sum = 0;
                                continue;
                        }
                } // for  inter  if  else  finished 

        }// end of for 

        ////printf ("****************************2222222222222\n");

        OSTimeDly(2000);

        ////printf ("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        ////printf ("Photolen = %d \n",PhotoLen);
        ////printf ("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        for(Count = 0; Count < PhotoLen; Count++)
        {
                RF_Take_To_OBC[6] = Count >> 8;
                RF_Take_To_OBC[7] = Count;

                DataCount = 0; 
                for (;;) {
                        OSTimeDly(1000);
                        if( ++DataCount > 30) {
                                --Count;
                                break;
                        }
                        memset(scc3buf, 0, 550);
                        ////printf ("******************  dan zu shuju  chuan tu xiang dao OBC start\n");
                        for(j = 0; j < 10; j++)
                                ////printf ("%02x ,  ",RF_Take_To_OBC[j]);
                        ////printf ("\n");

                        OSSchedLock();
                        changemux(3);
                        for(j = 0; j < 10; j++)
                                scc3_serial_putc(RF_Take_To_OBC[j]);
                        OSSchedUnlock();

                        /*-1--------------------------------------------------------------------*/


                        for (i = 0 ; i < 530; i++) {
                                timedata = 0;
                                while ((!scc3_serial_tstc())&&(timedata<(10000*6))) 
                                        timedata++;
                                if (timedata>=(10000*6)) continue;
                                while (!scc3_serial_tstc());
                                scc3buf[i] = scc3_serial_getc();
                        }
                        /*-1--------------------------------------------------------------------*/

                        ////printf ("----------++++++++++++++++++++++------------\n");

                        for (i = 0; i < 10; i++) {
                                ////printf ("%02x\n",scc3buf[i]);
                                if (scc3buf[i] != 0)
                                        break;
                        }

                        for (j = 0; j < 521; j ++) {
                                scc3buf[j] = scc3buf[i++];
                                //////printf ("%d      %02x\n",j,scc3buf[j]);
                        }


                        if (scc3buf[0]!=0xEB) continue;
                        if (scc3buf[1]!=0x90) continue;
                        if (scc3buf[2]!=0xBB) continue;
                        if (scc3buf[3]!=0x03) continue;
                        if (scc3buf[4]!=0x02) continue;
                        if (scc3buf[5]!=0x09) continue;
                        if (scc3buf[519] !=0x90) continue;
                        if (scc3buf[520] !=0x1D) continue;


                        for(i = 0; i < 518; i++)
                        {
                                sum += scc3buf[i];
                        }

                        if(sum == scc3buf[518])
                        {
                                sum = 0;
                                if (addr > (500*1024 + PC_DATA - 520)){ 
                                        addr = PC_DATA;
                                        ////printf ("+++++++++++++++++\n");
                                        ////printf ("yi  chu  \n");
                                }
                                ////printf ("addr = %09x", addr);
                                ////printf ("cheng gong jieshou   shuju \n");
                                for(i = 6; i < 518; i++)
                                        *(volatile INT8U *)(addr + i - 6) = scc3buf[i];
                                addr += 512;
                                ////printf ("addr = addr + 512?  : %09x", addr);
                                ////printf ("PhotoLen  : %d\n", PhotoLen);
                                break;
                        }
                        else {
                                sum = 0;
                                continue; 
                        }
                }//inter  most  for  finished 
                ////printf ("******************dan zu  chuan tu xiang dao OBC end\n");

        }//second  for  finish  ,,,picture  all  be  finished
        ////printf ("tu xiang  shuju   quanbu  obc\n");
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








/*************************************************************** 
  RS422发送函数, 利用双口RAM, 将数据给到c515 (物理发送由c515完成)			
  入口参数说明
  1) type ------- 与c515约定的类型号,中断内部标识位 相机是0x02
  2) data ------- 数据的首字节地址
  3) lenL lenH  ------- 发送给c515的数据长度  
 ***************************************************************/
void RS422_Data_Send( INT8U type , INT8U *data , INT8U lenH, INT8U lenL)
{
        INT16U j;

        OSSchedLock();/*停止任务调度*/

        /* 双口RAM中类型号,中断内部标识位 */
        *(volatile INT8U *)(DPRAM_SEND_422_ADDR)= type;

        /* 数据长度 */
        *(volatile INT8U *)(DPRAM_SEND_422_ADDR+2) = lenH;//高字节
        *(volatile INT8U *)(DPRAM_SEND_422_ADDR+4) = lenL;  //低字节

        /* 双口RAM中数据区 */
        for( j = 0 ; j < (lenL+256*lenH) ; j++ )
                *(volatile INT8U *)(DPRAM_SEND_422_ADDR+6+2*j)=*( data + j ); 

        /* 触发DPRAM中断, 由"写"信号触发 */
        *(volatile INT8U *)(DPRAM_SEND_IRQ_ADDR)=0x01;

        OSSchedUnlock();/*允许任务调度*/
        return;
}




INT8U GetBDData(int FrameCount)
{
        INT8U i,j;
        INT16U timedata;
        if(FrameCount == 0x01)
        {
                changemux(0);
                for(i=0;i < 255;i++)
                {
                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<1000)) {timedata++;}
                        if (timedata>=1000) 
                        {
                                continue;
                        }


                        BDGPSbuf[0] = scc3_serial_getc();
                        if (BDGPSbuf[0]!=0x10) continue;	

                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<1000)) timedata++;
                        if (timedata>=1000) continue;

                        BDGPSbuf[1] = scc3_serial_getc();
                        if (BDGPSbuf[1]!=0x01) continue;

                        OSSchedLock();
                        for(j = 2; j <= 46; j++)
                        {
                                timedata = 0;
                                while ((!scc3_serial_tstc())&&(timedata<1000)) timedata++;
                                if (timedata>=1000) continue;
                                //while (!scc3_serial_tstc());
                                BDGPSbuf[j] = scc3_serial_getc();

                        }
                        OSSchedUnlock();

                        for(i = 45; i <= 47; i++)
                        {
                                ////printf("BDGPSbuf[%d] = %x ",i,BDGPSbuf[i]);
                        }
                        ////printf("\n");

                        if (BDGPSbuf[45]!=0x10) continue;	
                        if (BDGPSbuf[46]!=0x03) 
                        {continue;}
                        else
                        {
                                ////printf("BDGPSbuf[3] = %d\n",BDGPSbuf[3]);
                                return BDGPSbuf[3];
                        }
                }
                return i;
        }
        else
        {
                changemux(0);
                for(i=0;i < 255;i++)
                {
                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<1000)) {timedata++;}
                        if (timedata>=1000) 
                        {
                                continue;
                        }

                        BDGPSbuf[0] = scc3_serial_getc();
                        if (BDGPSbuf[0]!=0x10) continue;	

                        timedata = 0;
                        while ((!scc3_serial_tstc())&&(timedata<1000)) timedata++;
                        if (timedata>=1000) continue;

                        BDGPSbuf[1] = scc3_serial_getc();
                        if (BDGPSbuf[1]!=0x01) continue;

                        OSSchedLock();
                        for(j = 2; j < 155; j++)
                        {
                                timedata = 0;
                                while ((!scc3_serial_tstc())&&(timedata<1000)) timedata++;
                                if (timedata>=1000) continue;
                                //while (!scc3_serial_tstc());
                                BDGPSbuf[j] = scc3_serial_getc();

                        }
                        OSSchedUnlock();

                        if (BDGPSbuf[45]!=0x10) continue;	
                        if (BDGPSbuf[46]!=0x03) continue;  
                        if (BDGPSbuf[47]!=0x10) continue;	
                        if (BDGPSbuf[48]!=0x02) continue; 
                        if (BDGPSbuf[153]!=0x10) continue;	
                        if (BDGPSbuf[154]!=0x03) 
                        {

                                continue;
                        }
                        else
                        {
                                ////printf("BDGPS get data!\n");
                                OSSchedLock();
                                for(j = 0; j < 155; j++)
                                {
                                        //	////printf("BDGPSbuf[%d] = %x ",j,BDGPSbuf[j]); //调试串口输出
                                        *(volatile INT8U *)(addr+j) = BDGPSbuf[j];
                                }
                                addr += 155;	
                                OSSchedUnlock();
                                return i;
                        }
                }
                return i;
        }

}

