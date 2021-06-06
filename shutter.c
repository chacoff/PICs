/****************************************************/
 /* Compilador PCW C Compiler IDE
 * Programador MR
 * - single cycle ok
 * - Multiple cycle ok
 * - Confirmation of open and close ok
 * - pulse width for open and close ok
 * - abort ok
/******************************************************/


#include "C:\PICC\Devices\16f873a.h"
#include <stdlib.h>

#use delay (clock=20000000)
#use rs232(baud=9600,xmit=pin_c6,rcv=pin_c7,stream=HOSTPC)


#fuses HS,NOWDT,NOPROTECT,NOLVP

#define GP0 PIN_A0
#define GP1 PIN_A1
#define GP2 PIN_A2
#define GP3 PIN_A3
#define GP4 PIN_A4
#define GP5 PIN_A5

#define AUX0 PIN_B0
#define AUX1 PIN_B1
#define AUX2 PIN_B2

//************************************
//  Print Menu
//************************************
void menu(void)
   {
   printf("\n\r             Commands\n\n\r");
   printf("I.................: Do one cycle\n\r");
   printf("Onn...............: Open pulse width: 1-255\n\r");
   printf("Snn...............: Open stop pulse width: 1-255\n\r");
   printf("Cnn...............: Close pulse width: 1-255\n\r");
   printf("Tnn...............: Close stop pulse width: 1-255\n\r");
   printf("Hnn...............: Home pulse width: 1-255\n\r");
   printf("Vnn...............: Print Current values\n\r");
   //printf("Dnn...............: Do nn cycles: 1-64000\n\r");
   printf("Gnn...............: Do nn cycles full: 1-64000\n\r");
   printf("X.................: Abort series\n\r");
   printf("?.................: Prints this Menu\n\n\r");
   printf("Command:");
   }

//************************************
//  Others
//************************************
 void delay_seconds(int n)
   {
   for (;n!=0; n--)
   delay_ms( 1000 );
   }

void Open(int ms)
   {
   output_high (GP0);      // pulse on
   output_low (aux0);      // led 1 on
   delay_ms (ms);          // pulse width
   output_low (GP0);       // Pulse off
   output_high (aux0);     // led 1 off
   }

void Close(int ms)
   {
   output_high (GP1);      // pulse on
   output_low (aux2);      // led 1 on
   delay_ms (ms);          // pulse width
   output_low (GP1);       // Pulse off
   output_high (aux2);     // led 1 off
   }

void get_string(char* s, int max)
   {
   int len;
   char c;

   --max;
   len=0;
   do {
     c=getc();
     if(c==8) {  // Backspace
        if(len>0) {
          len--;
          putc(c);
          putc(' ');
          putc(c);
        }
     } else if ((c>=' ')&&(c<='~'))
       if(len<max) {
         s[len++]=c;
         putc(c);
       }
   } while(c!=13);
   s[len]=0;
   }

void startimer(int val)
   {

   }

/***********************************/
//           MAIN
/***********************************/

void main()
{

int cmd=0,pwo=130,pwc=130,test=0,j=0;          // default pulse width values
int pwos=10,pwcs=10,pwh=10,timeout=0;  // default pulse width values
int16 i,x;
char string[5];

setup_adc_ports(NO_ANALOGS);
setup_adc(ADC_OFF);
setup_spi(FALSE);
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
setup_timer_1(T1_DISABLED);
setup_timer_2(T2_DISABLED,0,1);

output_low (GP0);                   // Init Pulse 0 off
output_low (GP1);                   // Init Pulse 1 off

menu();                             // print menu

While(true)                         // get command
   {
   if(kbhit())
      cmd = getc();                 // print command letter
      putc(cmd);

switch (cmd)                     // do command
         {
case '?':                  // HELP - menu
         {
         menu();
         cmd=0;
         break;
         }

case 'O':                  // ***** set open pulse width
         {
         get_string(string,5);
         x = atol(string);
         if(x>=255)
            printf("\n\rError 1-255: %lu\n\r",x);  // check max value 255
         else
            {
            pwo=x;
            printf("\n\rNew Open Pw %u\n\r",pwo);  // print accepted value
            }

         printf("Command:");
         cmd=0;
         break;
         }

case 'C':                  // ***** set close pulse width
         {
         get_string(string,5);
         x = atoi(string);
         if(x>=255)
            printf("\n\rError 1-255: %lu\n\r",x);  // check max value 255
         else
            {
            pwc=x;
            printf("\n\rNew Close Pw %u\n\r",pwc); // print accepted value
            }

         printf("Command:");
         cmd=0;
         break;
         }
         
case 'S':                  // **** set stop pulse width
         {
         get_string(string,5);
         x = atoi(string);
         if(x>=255)
            printf("\n\rError 1-255: %lu\n\r",x);  // check max value 255
         else
            {
            pwos=x;
            printf("\n\rNew Open stop Pw %u\n\r",pwos); // print accepted value
            }

         printf("Command:");
         cmd=0;
         break;
         }
case 'T':                  // **** set stop pulse width
         {
         get_string(string,5);
         x = atoi(string);
         if(x>=255)
            printf("\n\rError 1-255: %lu\n\r",x);  // check max value 255
         else
            {
            pwcs=x;
            printf("\n\rNew Close stop Pw %u\n\r",pwcs); // print accepted value
            }

         printf("Command:");
         cmd=0;
         break;
         }

case 'V':                  // **** set stop pulse width
         {
         printf("\n\rCurrent Values: Po %u Pc %u Ps %u Pt %u Ph %u\n\r",pwo,pwc,pwos,pwcs,pwh);

         printf("Command:");
         cmd=0;
         break;
         }
case 'H':                  // ***** set Home pulse width
         {
         get_string(string,5);
         x = atoi(string);
         if(x>=255)
            printf("\n\rError 1-255: %lu\n\r",x);  // check max value 255
         else
            {
            pwh=x;
            printf("\n\rNew Home Pw %u\n\r",pwh); // print accepted value
            }

         printf("Command:");
         cmd=0;
         break;
         }

case 'I':                 // **** do single pulse full
         {
         printf("\n\rOpen\n\r");

         Open(pwo);                 // open shutter with set pulse width
         while(input(GP4));         // wait for open signal

         Close(pwos);
         Open(pwh);
         delay_seconds(1);

         for(j=1;j<=10;j++)
               test+=input(GP4);

            if(test>=8)
               {
               printf("\n\rError... open shutter fail..\n\r");
               break;
               }


         printf("Wait...\n\r");
         delay_seconds(1);

         Close(pwc);

         while(input(GP5)); // wait for open signal
         Open(pwcs);
         Close(pwh);
         delay_seconds(1);

         for(j=1;j<=10;j++)
               test+=input(GP5);

            if(test>=8)
               {
               printf("\n\rError... close shutter fail...\n\r");
               break;
               }


         printf("Command:");
         cmd=0;
         }
         break;


case 'G':                        // ****  go n cycles full
         {
         get_string(string,5);
         x = atol(string);
         printf("\n\rStart Full Nr:%lu Po %u Pc %u Ps %u Pt %u Ph %u\n\r",x,pwo,pwc,pwos,pwcs,pwh);
         delay_seconds(1);

         for(i=1;i<=x;i++)
            {
            printf("%lu\r",i);
            
// Open procedure
            Open(pwo);              // open pulse

            while(input(GP4));      // wait for switch detection

            Close(pwos);            // stop pulse
            Open(pwh);              // home pulse
            delay_seconds(1);       // open time first half
            
            test=0;
            for(j=1;j<=10;j++)
               test+=input(GP4);

            if(test>=8)
               {
               printf("\n\rError... open shutter fail...Aborted in cycle %lu\n\r",i);
               break;
               }

            delay_seconds(1);       // open time second half
            
// close procedure

            Close(pwc);             // close pulse

            while(input(GP5));      // wait for switch detection 
            Open(pwcs);             // stop pulse
            Close(pwh);             // home pulse
            delay_seconds(1);
            
            test=0;
            for(j=1;j<=10;j++)      // filter end switch status
               test+=input(GP5);

            if(test>=8)
               {
               printf("\n\rError... close shutter fail...Aborted in cycle %lu\n\r",i);
               break;
               }

            delay_seconds(1);

            if(kbhit())
               {
               cmd = getc();
               If(cmd == 'X')
                  {
                  printf("\n\rAborted by user\n\r");
                  break;
                  }
               }
            }

         printf("\n\rReady\n\n\r");
         printf("Command:");
         cmd=0;
         }
         break;

default:
         {
         if (cmd!=0)
            {
            printf("\n\rError\n\r");
            printf("Command:");
            }
         cmd=0;
         }
         break;
         }


   }

}

