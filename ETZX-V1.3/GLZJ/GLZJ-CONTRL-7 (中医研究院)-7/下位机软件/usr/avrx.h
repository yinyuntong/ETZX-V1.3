#ifndef AVRXCHEADER
#define AVRXCHEADER

/*
avrx.h*/


#ifndef BV
  #define BV(A) (1<<A)
#endif



#define NAKEDFUNC(A)  void A(void)
#define CTASKFUNC(A)  void A(void)


/* Transform GCC input and output routines into direct access for IAR */

#define outp(A,B) 	(B=(A))
#define inp(A) A
#define cbi(P, B) 	P&=~(BV(B))
#define sbi(P, B) 	P|=BV(B)
#define BeginCritical() OS_ENTER_CRITICAL()
#define EndCritical()   OS_EXIT_CRITICAL()
#define _SFR_IO_ADDR(A) A
#define _SFR_MEM_ADDR(A) A


#endif





