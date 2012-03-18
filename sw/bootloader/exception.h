#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------
void exception_register_fault_handler(void (*func)(void));
void exception_register_break_handler(void (*func)(void));
void exception_register_extint_handler(void (*func)(void));
void exception_register_syscall_handler(void (*func)(void));

#endif // __EXCEPTION_H__
