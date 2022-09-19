#include <msp430x14x.h>
#include "LCD.h"
#define  INTERVAL  50000  //okres licznika 0,5s
#define  _1s       2     //okres pomiarów co jedna sekunda

unsigned int volt;
unsigned int cntr;
void show(void);
main (void)
{
 unsigned int value, k;
 WDTCTL = WDTPW+WDTHOLD; 
 ini_display();                            //inicjalizacja LCD
 P6SEL|=0x01;                              // zmiana funkcji pinu 
 ADC12CTL0=ADC12ON|REF2_5V|SHT0_15|MSH|REFON;  //w³. rdzenia, gen. nap. od. 2,5V, wybór nap. odniesienia, w³.gen. nap. odniesienia,
 ADC12CTL1=SHP|CSTARTADD_0|CONSEQ_1;       //próbkowanie impulsowe, pierwszy wynik sk³adany w ADC12MEM0,tryb jednorazowa sekwencja
 ADC12MCTL0=INCH_0|SREF_1;                //kana³ 0, Ÿród³o nap. odniesienia - wew. generator (2,5V)
 ADC12MCTL1=INCH_0|SREF_1; 
 ADC12MCTL2=INCH_0|SREF_1; 
 ADC12MCTL3=EOS|INCH_0|SREF_1;            //ostatni wynik
 for(k=0;k<0x3600;k++);                    //czas na ustabilizowanie generatora nap. odniesienia
 
 CCR0=INTERVAL;                   //ustala nowy okres licznika
 TACTL=TASSEL_2|ID_3|MC_1;        //Ÿród³o taktowania SMCLK, dzielone przez 8,tryb UP
 CCTL0=CCIE;                      //uaktywnienie przerwania od TACCR0 CCIFG
 _BIS_SR(GIE);                    //w³aczenie przerwañ
 
 ADC12CTL0 |= ENC;                //uaktywnienie konwersji
 while(1)
  {
   
   ADC12CTL0 |= ADC12SC;                          //start konwersji
   while(ADC12CTL0&ADC12SC);                      //czekanie na koniec konwersji
   value=ADC12MEM0+ADC12MEM0+ADC12MEM0+ADC12MEM0;
   value >>=2;
   volt=value*0.006105;
   show();                                         //wyœwietla na LCD
   _BIS_SR(LPM0_bits);                             //wejœcie w tryb oszczêdny
   }
}

void show(void)
{
 int cyfra, waga=10;
 LCD_cmd(CLR_LCD);
 wait (_2ms);
 if (volt >= 100)
 {
  LCD_char('?');
  return;
  }
 if(volt<10)
    LCD_char('0');   
 
 while (waga <= volt)
 {
  waga*=10;
  }
 while ((waga/=10)>1)
 {
  cyfra = volt / waga;
  LCD_char((int)('0'+cyfra));
  volt-=cyfra*waga;
  }
 LCD_char('.');
 LCD_char((int)('0'+volt));
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
 if(++cntr==_1s)
 { 
  _BIC_SR_IRQ(LPM0_bits);            //wyjœcie z trybu oszczednego
  cntr=0;
  }
}
