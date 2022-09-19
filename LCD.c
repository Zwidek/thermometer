#include <msp430x14x.h>
#define set_bit(var,bitNo)   ((var)|=1<<(bitNo))     //ustawienie bitu na pozycji bitNo
#define clr_bit(var,bitNo)   ((var)&=~(1<<(bitNo)))  //czyszczenie bitu na pozycji bitNo
#define E                    3        //P2.3
#define RS                   2        //P2.2
#define DISPLAY              P2OUT    // wyswietlacz pod³aczony do portu P2
#define _50ms   2000
#define _5ms     200
#define _50us      2
#define _2ms      80
#define _200us     8

//polecenia steruj¹ce wyœwietlaczem

#define ON_LCD               0x0c   // wlaczenie wyswietlania
#define CLR_LCD              0x01   // czyszczenie wyswietlacza


void ini_display(void);
void LCD_char (unsigned char c);
void LCD_cmd(unsigned char cmd);
void toggle_E(void);
void wait_25us(void);
void wait (unsigned int n);

void ini_display(void)
{
 P2DIR=~BIT0;                  //linia P2.O  pozostaje jako wejœcie
 wait(_50ms);             // czekaj na ustabilizowanie napiêcia
 DISPLAY=0x30;            // procedura inicjalizacyjna 
 toggle_E();
 wait(_5ms);
 toggle_E();
 wait(_200us);
 toggle_E();
 wait(_200us);              //koniec inicjalizacji
 DISPLAY&=~BIT4;            //interfejs 4-bitowy
 toggle_E();
 LCD_cmd(ON_LCD);           
 LCD_cmd(CLR_LCD); 
 wait(_2ms);      
 }
 
 void LCD_char (unsigned char c)
 {
  unsigned char temp;
  
      //wysylanie starszego polbajtu
    
  temp=c&0xf0;
  DISPLAY&=0x0f;
  DISPLAY|=temp;
  set_bit(DISPLAY,RS);     //wysylane beda dane
  toggle_E();
  //wait(_0,5us)           - stosowac przy wiekszych czestotliwosciach zegara
  
     //wysylanie mlodszej polowki bajtu
  
  temp=c&0x0f;
  temp=temp<<4;
  DISPLAY&=0x0f;
  DISPLAY|=temp;
  toggle_E();
  //wait(_05us);
  wait (_50us); 
 }
 
 void LCD_cmd(unsigned char cmd)
 {
  unsigned char temp;
  temp=cmd&0xf0;
  DISPLAY&=0x0f;
  DISPLAY|=temp;
  clr_bit(DISPLAY,RS);
  toggle_E();
  //wait(_0.5us);
  temp=cmd&0x0f;
  temp=temp<<4;
  DISPLAY&=0x0f;
  DISPLAY|=temp;
  toggle_E();
  // wait(_0.5us);
  wait (_50us);
 }
 
 void toggle_E(void)
 {
  set_bit (DISPLAY,E);
  wait_25us ();          //min 0,25us
  clr_bit(DISPLAY,E);
 }
 
 void wait_25us(void)   // 2 cykle (przekazanie arg.)+5(call)+1(i=1)+3(i<n)+3(i++)+3(i<n)+3(ret)=20cykli
 {                      // 20*1/800kHz=25us
  unsigned int i;
  for(i=0;i<1;i++);
 }

 void wait(unsigned int n)
 {
  unsigned int i;
  for(i=0;i<n;i++)
  wait_25us();
 }

 void LCD_number(unsigned int liczba)
{
 int cyfra, waga=10;
 if (liczba >= 100000)
 {
  LCD_char('?');
  return;
  }
 while (waga <= liczba)
 {
  waga*=10;
  }
 while ((waga/=10)>1)
 {
  cyfra = liczba / waga;
  LCD_char((int)('0'+cyfra));
  liczba-=cyfra*waga;
  }
 LCD_char((int)('0'+liczba));
}

 
 
  
  
  
  
  
 
