#ifndef __SERIAL_H__
#define __SERIAL_H__

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------
void serial_init (void);
int serial_putchar(char ch);
int serial_getchar(void);
int serial_haschar();
void serial_putstr(char *str);
void serial_putnum( int n );
void serial_printstrnum(char *str1, unsigned int hexnum, char *str2);

#endif // __SERIAL_H__
