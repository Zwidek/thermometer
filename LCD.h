#define ON_LCD               0x0c   // wlaczenie wyswietlania
#define CLR_LCD              0x01   // czyszczenie wyswietlacza
#define _50ms   2000
#define _2ms      80
#define _50us      2

void ini_display(void);
void LCD_char (unsigned char c);
void LCD_cmd(unsigned char cmd);
void LCD_number(unsigned int liczba);
void toggle_E(void);
void wait (unsigned int n);
