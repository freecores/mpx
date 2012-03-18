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

volatile unsigned long * _currentTCB;

//-----------------------------------------------------------------
// cpu_handle_irq:
//-----------------------------------------------------------------
void cpu_handle_irq(unsigned cause)
{
    switch (cause)
    {
    case 0x01: 
        panic("SYSCALL!");
        break;
    case 0x02: 
        panic("BREAK!");
        break;
    case 0x03: 
        panic("EXT_INT!");
        break;
    case 0x04: 
        panic("FAULT!");
        break;
    case 0x05: 
    case 0x06: 
        panic("MULT!");
        break;
    case 0x07: 
    case 0x08:
        panic("DIV!");
        break;
    }
}