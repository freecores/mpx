#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

//-----------------------------------------------------------------
// Structures
//-----------------------------------------------------------------

// Exception Fault SRC ID
enum
{
	EXCEPTION_SYSCALL = 0x01,
	EXCEPTION_BREAK = 0x02,
	EXCEPTION_EXTINT = 0x03,
	EXCEPTION_FAULT = 0x04,
	EXCEPTION_MULT = 0x05,
	EXCEPTION_UMULT = 0x06,
	EXCEPTION_DIV = 0x07,
	EXCEPTION_UDIV = 0x08,
};

//-----------------------------------------------------------------
// Macros:
//-----------------------------------------------------------------
static inline void asm_set_isr_vector(unsigned val)
{
	asm volatile ("mtc0 %0, $15" : /* no outputs */ : "r" (val)  );
}

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------
void exception_register_fault_handler(void (*func)(void));
void exception_register_break_handler(void (*func)(void));
void exception_register_extint_handler(void (*func)(void));
void exception_register_syscall_handler(void (*func)(void));
void asm_save_context(void);

#endif // __EXCEPTION_H__
