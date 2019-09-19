#ifndef _EXTINT_H_
#define _EXTINT_H_



#ifndef _IN_EXTINT_
extern void eint0Init (void);
extern void eint1Init (void);
extern void eint2Init (void);
extern void eint3Init (void);
extern int32_t zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio);

#endif



#endif
