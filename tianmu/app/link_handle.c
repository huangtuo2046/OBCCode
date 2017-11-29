/*******************************************************

  link_handle.c

  coded by zhao wenjun 

  2017-3-9

 *********************************************************/

#include "includes.h"
#include "Includes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



extern OS_EVENT * GPSQ;
extern OS_EVENT * StarSQ;
extern OS_EVENT * SpiQ;
extern OS_EVENT * SendQ;
extern OS_EVENT * Send_rfQ;
extern OS_EVENT * Dsp_rfQ;

extern INT8U  rf_flag;
static INT8U dsp_flag;


void dispatch(void *jdata)
{
        INT8U temp,err, len , num, *rf;
        INT16U k;
        INT32U *addr;
        INT8U scc3buf[200], time_flag;
        enum myevent event_type;
        INT8U i , *sendmsg, sendbuf[255], n, id;
        char buf0[30] = {'a','a','a','a','a','a','a','a','a','a','a','a','a','a'};
        const char * buf1 = "bbbbbbbbbb";
        INT32U  timedata, j;

        j=0;
        time_flag = 0 ;

        for(;;)
        {
                /*
                if (time_flag == 0){
                        OSTimeDly (2000);
                        time_flag = 1;
                }
                */
                
                if (dsp_flag == 0) {
                        rf = (INT8U *)OSQPend (Dsp_rfQ, 0,  &err);
                }

                //printf (" ^_^       ^_^    egse  egse   egse  start \n");


                scc3_app_init ();
                OSSchedLock();

                //for (j = 0 ; j < 10 ; j++){
                //       scc3_serial_putc (buf0[j]);
                //      scc3_serial_putc (buf1[j]);
                //}

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[0] = scc3_serial_getc();
                if (scc3buf[0] != 0xeb) {
                        printf (" ********* continue  \n");
                        OSSchedUnlock();
                        continue;
                }

                //if (scc3buf[0]!=0xEB) continue;	

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[1] = scc3_serial_getc();
                //if (scc3buf[1]!=0x90) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[2] = scc3_serial_getc();
                //if (scc3buf[2]!=0xb4) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[3] = scc3_serial_getc();
                //if (scc3buf[3] !=0x1c) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[4] = scc3_serial_getc();
                if (scc3buf[4] == 0){
                        printf ("fang shi  zi  wei  :scc3buf[4] = 0x00\n");
                        OSSchedUnlock();
                        continue;
                }
                //if (scc3buf[4] !=0x20 || scc3buf[4] != 0x21) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[5] = scc3_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[6] = scc3_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[7] = scc3_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[8] = scc3_serial_getc();

                if (scc3buf[4] == 0x21) 
                {
                        scc3buf[9] = scc3_serial_getc();
                        scc3buf[10] = scc3_serial_getc();

                        for (j=0; j<scc3buf[10]; j++)
                                scc3buf[11+j] = scc3_serial_getc();
                }
                else 
                {
                        for (j=0; j<scc3buf[8]; j++)
                        {
                                //timedata = 0;
                                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                                //if (timedata>=100000*5) continue;
                                //while (!scc3_serial_tstc());
                                scc3buf[9+j] = scc3_serial_getc();

                        }

                }
                OSSchedUnlock();









               /* 
                if (scc3buf[4] == 0x21) {
                        printf("egse up data is\n");
                        for(j = 0; j < (scc3buf[10] + 11); j++)
                                printf("egse up[%d]   =  %02x\n",j, scc3buf[j]);
                }
                else{
                        printf("egse up data is\n");
                        for(j = 0; j < (scc3buf[8] + 9); j++)
                                printf("egse up[%d]   =  %02x\n",j, scc3buf[j]);
                }
                */


                id = scc3buf[7];
                //printf ("  dispish    id :  %02x\n", id);


                if ((scc3buf[4] == 0x22) && (scc3buf[9] == 0x01)){
                        //printf ("dsp_flag 5= %d \n", dsp_flag);
                        dsp_flag = 0;
                        //printf ("dsp_flag 6= %d \n", dsp_flag);
                        OSQPost(Send_rfQ,scc3buf);
                        continue;
                }

                /*
                if ((scc3buf[4] == 0x22) && (scc3buf[9] == 0x00)){
                        //printf ("dsp_flag 1= %d \n", dsp_flag);
                        dsp_flag = 1;
                        //printf ("dsp_flag 2= %d \n", dsp_flag);
                        OSQPost(Dsp_rfQ,scc3buf);
                }
                */

                else if (scc3buf[0] == 0xeb && scc3buf[1] == 0x90 && scc3buf[2] == 0xb4 && scc3buf[3] == 0x1c && (scc3buf[4] == 0x21 || scc3buf[4] == 0x20) && scc3buf[5] == 0x00){
                        rf_flag = 0;
                        if (id == 0x50)
                                OSQPost(GPSQ,scc3buf);
                        else if (id == 0x51 || id == 0x52)
                                OSQPost(StarSQ,scc3buf);
                        else if (id == 0x10 || id == 0x12 ||  id == 0x13 || id == 0x14 || id == 0x15 || id == 0x16 || id == 0x17 || id == 0x18 || id == 0x1d || id == 0x20 || id == 0x21 || id == 0x33 || id == 0x34 || id == 0x35 || id == 0x36 || id == 0x37 || id == 0x38 || id == 0x39 || id == 0x3a || id == 0x3b || id == 0x3c || id == 0x40 || id == 0x41 || id == 0x43 || id == 0x4b || id == 0x53 || id == 0x42)
                                OSQPost(SpiQ,scc3buf);
                        else{
                                continue;
                        }
                }
                else{
                        continue;
                }

                sendmsg = (INT8U *)OSQPend (SendQ,0,  &err);

                len = (* (sendmsg + 8));
                //printf ("link_handler    len == %x\n", len);

                num = len + 9 + 2 + 30;

                n =0;
                for (;;) {
                        if (n > num)
                                break;
                        sendbuf[n] = *(sendmsg + n);
                        n ++;

                }

                //for (n = 0; n < num; n ++)
                //       printf ("link send down  sendbuf[%d] = %x\n", n, sendbuf[n]);

                //printf ("________send data num   = %d\n",num );
                OSSchedLock();

                /*
                n =0;
                for (;;){
                        if (n >= num)
                                break;

                        scc3_serial_putc (sendbuf[n]);
                        printf ("egse send [%d] : %02x\n",n,sendbuf[n]);
                        n ++;
                }

                */

                n =0;
                for (;;){
                        if (n >= num)
                                break;
                        scc3_serial_putc (sendbuf[n]);
                        //printf ("egse send [%d] = %02x\n", n, sendbuf[n]);
                        //for (k = 0; k < 1000; k++);
                        n ++;
                }




                //printf ("end    num   =   %d \n", num);
                OSSchedUnlock();



        }
}










void dispatch_rf(void *jdata)
{
        INT8U data[MAX_REC_LEN*8],ctr_field,temp,c[1],err, len , num, time_flag;
        INT16U k;
        INT32U *addr;
        INT8U scc3buf[200];
        enum myevent event_type;
        INT8U i , *sendmsg, sendbuf[255], n, id, *rf;
        char buf0[30] = {0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0};
        const char * buf1 = "bbbbbbbbbb";
        INT32U  timedata, j;

        j=0;
        event_type=frame_arrival;
        c[0]=event_type;

        time_flag = 0 ;
        for(;;)
        {
                if (time_flag == 0){
                        OSTimeDly (2000);
                        for (j = 0 ; j < 50000; j ++);
                        time_flag = 1;
                }

                //printf (" ^_^       ^_^   RFRFRFRF  start \n");

                        //printf ("dsp_flag 3= %d \n", dsp_flag);
                /*
                if (dsp_flag == 0) {
         //               printf ("dispatch  rf rf rf rf rf rf rf rf rf \n");
                        //printf ("dsp_flag 4= %d \n", dsp_flag);
                        rf = (INT8U *)OSQPend (Dsp_rfQ, 0,  &err);
          //              printf ("_______rf start \n");
                }
                */

                smc1_app_init ();
                OSSchedLock();

                //for (j = 0 ; j < 10 ; j++){
                //       scc3_serial_putc (buf0[j]);
                //      scc3_serial_putc (buf1[j]);
                //}

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[0] = smc1_serial_getc();
                if (scc3buf[0] != 0xeb) {
                        printf (" ********* continue  \n");
                        OSSchedUnlock();
                        continue;
                }
                //if (scc3buf[0] != 0xeb)
                //       continue;
                //if (scc3buf[0]!=0xEB) continue;	

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[1] = smc1_serial_getc();
                //if (scc3buf[1]!=0x90) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[2] = smc1_serial_getc();
                //if (scc3buf[2]!=0xb4) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[3] = smc1_serial_getc();
                //if (scc3buf[3] !=0x1c) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[4] = smc1_serial_getc();
                if (scc3buf[4] == 0) {
                        printf ("scc3buf[4] = 0x00\n");
                        OSSchedUnlock();
                        continue;
                }
                //if (scc3buf[4] !=0x20 || scc3buf[4] != 0x21) continue;

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[5] = smc1_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[6] = smc1_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[7] = smc1_serial_getc();

                //timedata = 0;
                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                //if (timedata>=100000*5) continue;

                scc3buf[8] = smc1_serial_getc();

                //            OSSchedUnlock();

                if (scc3buf[4] == 0x21) {
                        scc3buf[9] = smc1_serial_getc();
                        scc3buf[10] = smc1_serial_getc();
                        for (j=0; j<scc3buf[10]; j++)
                        {
                                //timedata = 0;
                                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                                //if (timedata>=100000*5) continue;
                                //while (!scc3_serial_tstc());
                                scc3buf[11+j] = smc1_serial_getc();

                        }

                }
                else {
                        for (j=0; j<scc3buf[8]; j++)
                        {
                                //timedata = 0;
                                //while ((!scc3_serial_tstc())&&(timedata<100000*5)) timedata++;
                                //if (timedata>=100000*5) continue;
                                //while (!scc3_serial_tstc());
                                scc3buf[9+j] = smc1_serial_getc();

                        }
                }
                OSSchedUnlock();


                /*
                if (scc3buf[4] == 0x21) {
                        printf("egse up data is\n");
                        for(j = 0; j < (scc3buf[10] + 11); j++)
                                printf("egse up[%d]   =  %02x\n",j, scc3buf[j]);
                }
                else{
                        printf("egse up data is\n");
                        for(j = 0; j < (scc3buf[8] + 9); j++)
                                printf("egse up[%d]   =  %02x\n",j, scc3buf[j]);
                }
                */


                id = scc3buf[7];
                //printf ("  dispish  rf   id :  %02x\n", id);


                if ((scc3buf[4] == 0x22) && (scc3buf[9] == 0x00)){
                        //printf ("dsp_flag 1= %d \n", dsp_flag);
                        dsp_flag = 1;
                        //printf ("dsp_flag 2= %d \n", dsp_flag);
                        OSQPost(Dsp_rfQ,scc3buf);
                }

                /*
                if ((scc3buf[4] == 0x22) && (scc3buf[9] == 0x01)){
                        //printf ("dsp_flag 5= %d \n", dsp_flag);
                        dsp_flag = 0;
                        //printf ("dsp_flag 6= %d \n", dsp_flag);
                        OSQPost(SendQ,scc3buf);
                        continue;
                }
                */
                else if (scc3buf[0] == 0xeb && scc3buf[1] == 0x90 && scc3buf[2] == 0xb4 && scc3buf[3] == 0x1c && (scc3buf[4] == 0x21 || scc3buf[4] == 0x20) && scc3buf[5] == 0x00){
                        rf_flag = 1;
                        if (id == 0x50)
                                OSQPost(GPSQ,scc3buf);
                        else if (id == 0x51 || id == 0x52)
                                OSQPost(StarSQ,scc3buf);
                        else if (id == 0x10 || id == 0x12 ||  id == 0x13 || id == 0x14 || id == 0x15 || id == 0x16 || id == 0x17 || id == 0x18 || id == 0x1d || id == 0x20 || id == 0x21 || id == 0x33 || id == 0x34 || id == 0x35 || id == 0x36 || id == 0x37 || id == 0x38 || id == 0x39 || id == 0x3a || id == 0x3b || id == 0x3c || id == 0x40 || id == 0x41 || id == 0x43 || id == 0x4b || id == 0x53 || id == 0x42)
                                OSQPost(SpiQ,scc3buf);
                        else{
                                continue;
                        }
                }
                else{
                        continue;
                }

                sendmsg = (INT8U *)OSQPend (Send_rfQ,0,  &err);

                len = (* (sendmsg + 8));
                //printf ("link_handler    len == %x\n", len);

                num = len + 9 + 2 + 30;

                n =0;
                for (;;) {
                        if (n >= num)
                                break;
                        sendbuf[n] = *(sendmsg + n);
                        n ++;

                }

                //for (n = 0; n < num; n ++)
                //       printf ("link send down  sendbuf[%d] = %x\n", n, sendbuf[n]);


                //printf ("________send data num   = %d\n",num );


                OSSchedLock();
                n =0;
                for (;;){
                        if (n >= num )
                                break;
                        smc1_serial_putc (sendbuf[n]);
                        //printf ("rf send [%d] = %02x\n", n, sendbuf[n]);
                        n ++;
                }

                OSSchedUnlock();


        }
}








