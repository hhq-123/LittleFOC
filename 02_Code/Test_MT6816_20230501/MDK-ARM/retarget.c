#include <stdio.h>
#include <rt_misc.h>
 
//#pragma import(__use_no_semihosting_swi)//ARM Compiler6不兼容import，改为下面的内联函数
__asm(".global __use_no_semihosting");
 
extern int  sendchar(int ch);  /* in Serial.c */
//extern int  getkey(void);      /* in Serial.c */
extern long timeval;           /* in Time.c   */
 
 
//struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
 
int fputc(int ch, FILE *f) {
  return (sendchar(ch));
}
 
//int fgetc(FILE *f) {
// return (sendchar(getkey()));  
//}
 
 
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}
 
 
void _ttywrch(int ch) {
  sendchar (ch);
}
 
 
void _sys_exit(int return_code) {
  while (1);    /* endless loop */
}