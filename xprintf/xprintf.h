/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* 
    %с	Символ типа char
    %d	Десятичное число целого типа со знаком
    %i	Десятичное число целого типа со знаком
    %е	Научная нотация (е нижнего регистра)
    %Е	Научная нотация (Е верхнего регистра)
    %f	Десятичное число с плавающей точкой
    %g	Использует код %е или %f — тот из них, который короче (при использовании %g используется е нижнего регистра)
    %G	Использует код %Е или %f — тот из них, который короче (при использовании %G используется Е верхнего регистра)
    %о	Восьмеричное целое число без знака
    %s	Строка символов
    %u	Десятичное число целого типа без знака
    %х	Шестнадцатиричное целое число без знака (буквы нижнего регистра)
    %Х	Шестнадцатиричное целое число без знака (буквы верхнего регистра)
    %р	Выводит на экран значение указателя
    %n	Ассоциированный аргумент — это указатель на переменную целого типа, в которую помещено количество символов, записанных на данный момент
*/
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	1	/* 1: Use input function */
#define	_LINE_ECHO		1	/* 1: Echo back input chars in xgets function */


#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
extern void (*xfunc_out)(unsigned char);
void xputc (char c);
void xputs (const char* str);
void xfputs (void (*func)(unsigned char), const char* str);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void xfprintf (void (*func)(unsigned char), const char*	fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (unsigned char(*)(void))(func)
extern unsigned char (*xfunc_in)(void);
int xgets (char* buff, int len);
int xfgets (unsigned char (*func)(void), char* buff, int len);
int xatoi (char** str, long* res);
#endif

#endif
