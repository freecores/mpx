//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                         MPX 32-bit Soft-Core Processor
//                                    V0.1
//                              Ultra-Embedded.com
//                            Copyright 2011 - 2012
//
//                         Email: admin@ultra-embedded.com
//
//                                License: GPL
//   If you would like a version with a more permissive license for use in
//   closed source commercial applications please contact me for details.
//-----------------------------------------------------------------------------
//
// This file is part of MPX 32-bit Soft-Core Processor.
//
// MPX 32-bit Soft-Core Processor is free software; you can redistribute it and/or 
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// MPX 32-bit Soft-Core Processor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MPX 32-bit Soft-Core Processor; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "exception.h"
#include "assert.h"
#include "printf.h"

//-----------------------------------------------------------------
// Globals:
//-----------------------------------------------------------------
volatile unsigned long * _currentTCB;

//-----------------------------------------------------------------
// Locals:
//-----------------------------------------------------------------
static void (*func_fault)(void) = 0;
static void (*func_break)(void) = 0;
static void (*func_extint)(void) = 0;
static void (*func_syscall)(void) = 0;
static volatile int in_interrupt = 0;

//-----------------------------------------------------------------
// exception_register:
//-----------------------------------------------------------------
void exception_register_fault_handler(void (*func)(void))	{ func_fault = func; }
void exception_register_break_handler(void (*func)(void))	{ func_break = func; }
void exception_register_extint_handler(void (*func)(void))	{ func_extint = func; }
void exception_register_syscall_handler(void (*func)(void))	{ func_syscall = func; }

//-----------------------------------------------------------------
// cpu_handle_irq:
//-----------------------------------------------------------------
void cpu_handle_irq(unsigned cause)
{
    int recursive_int = (in_interrupt != 0);

    in_interrupt = 1;

    switch (cause)
    {
    case EXCEPTION_SYSCALL: 
        if (func_syscall)
            func_syscall();
        else
            panic("SYSCALL!");
        break;

    case EXCEPTION_BREAK: 
        if (func_break)
            func_break();
        else
            panic("BREAK!");
        break;

    case EXCEPTION_EXTINT: 
        if (func_extint)
            func_extint();
        else
            panic("EXT_INT!");
        break;

    case EXCEPTION_FAULT: 
        if (func_fault)
            func_fault();
        else
            panic("FAULT");
        break;

    case EXCEPTION_MULT: 
        panic("MULT!");
        break;

    case EXCEPTION_UMULT: 
        panic("UMULT!");
        break;

    case EXCEPTION_DIV: 
        panic("DIV!");
        break;

    case EXCEPTION_UDIV:
        panic("UDIV!");
        break;

    default:
        panic("UNKNOWN");
        break;
    }

    assert(recursive_int == 0);

    in_interrupt = 0;
}
