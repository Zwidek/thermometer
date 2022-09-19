#include <msp430x14x.h>
#include "LCD.h"
#include "portyLcd.h"
#define  INTERVAL  50000  //okres licznika 0,5s
#define  _5s       10     //okres pomiarów

int temp;
unsigned int cntr;
void show(void);
void InitSpecial() {
  LCD_cmd(CG_RAM_ADDR);
  int spec[8] = {0, 7, 5, 7, 0, 0, 0 ,0}; //stopieñ
                    
  for(int j = 0; j < 8; j++) {
    LCD_char(spec[j]);
  }
  
}
main (void)
{
 unsigned int k;
 WDTCTL = WDTPW+WDTHOLD; 
 InitPortsLcd();                   // inicjalizacja portów LCD
 ini_display();                   //inicjalizacja LCD
 InitSpecial();
 ADC12CTL0=ADC12ON|REFON|SHT0_15; //w³. rdzenia, w³.gen. nap. odniesienia, wybór nap. odniesienia
 ADC12CTL1=SHP|CSTARTADD_0;       //próbkowanie impulsowe, wynik sk³adany w ADC12MEM0
 ADC12MCTL0=INCH_10|SREF_1;       //kana³ 10, Ÿród³o nap. odniesienia - wew. generator (1,5V)
 for(k=0;k<0x3600;k++);           //czas na ustabilizowanie generatora nap. odniesienia
 
 CCR0=INTERVAL;                   //ustala nowy okres licznika
 TACTL=TASSEL_2|ID_3|MC_1;        //Ÿród³o taktowania SMCLK, dzielone przez 8,tryb UP
 CCTL0=CCIE;                      //uaktywnienie przerwania od TACCR0 CCIFG
 _BIS_SR(GIE);                    //w³aczenie przerwañ
 
 ADC12CTL0 |= ENC;                //uaktywnienie konwersji
 
 while(1)
  {
   P2OUT^=BIT1;
   ADC12CTL0 |= ADC12SC;             //start konwersji
   while((ADC12CTL1&ADC12BUSY)==1);  //czekanie na koniec konwersji
   temp=ADC12MEM0*1.0318-2777.4647;  //wartoœæ temperatury z dok. 1-miejsce po przecinku
   show();                           //wyœwietla na LCD
   _BIS_SR(LPM0_bits);               //wejœcie w tryb oszczêdny
   }
}

void show(void)
{
 int cyfra, waga=10;
 LCD_cmd(CLR_LCD);
 wait (_2ms);
 LCD_char('T');
 LCD_char('E');
 LCD_char('M');
 LCD_char('P');
 LCD_char('E');
 LCD_char('R');
 LCD_char('A');
 LCD_char('T');
 LCD_char('U');
 LCD_char('R');
 LCD_char('A');
 LCD_char(':');
 LCD_cmd(DD_RAM_ADDR2);
 if(temp<0)
  {
    LCD_char('-');
    temp*=-1;
   }
 if(temp<10)
    LCD_char('0');   
 
 if (temp >= 1000)
 {
  LCD_char('?');
  return;
  }
 while (waga <= temp)
 {
  waga*=10;
  }
 while ((waga/=10)>1)
 {
  cyfra = temp / waga;
  LCD_char((int)('0'+cyfra));
  temp-=cyfra*waga;
  }
 LCD_char('.');
 LCD_char((int)('0'+temp));
 LCD_char(8);
 LCD_char('C');
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
 if(++cntr==_5s)
 { 
  _BIC_SR_IRQ(LPM0_bits);            //wyjœcie z trybu oszczednego
  cntr=0;
  }
}
