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
#include "serial.h"
#include "boot_serial.h"
#include "xmodem.h"

//-----------------------------------------------------------------
// Defines:
//-----------------------------------------------------------------
#define FLASH_SECTOR_SIZE		128

//-----------------------------------------------------------------
// Locals:
//-----------------------------------------------------------------
static unsigned long xfer_offset = 0;
static unsigned long xfer_base;
static unsigned int xfer_sector_offset = 0;
static unsigned char flash_write_buffer[FLASH_SECTOR_SIZE];

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------
static int xmodem_write(unsigned char* buffer, int size);

//-----------------------------------------------------------------
// boot_serial:
//-----------------------------------------------------------------
void boot_serial(unsigned long target)
{
    int res;

    // Load target memory address
    xfer_base = target;

    // Init X-Modem transfer
    xmodem_init(serial_putchar, serial_getchar);

    do
    {
        res = xmodem_receive( xmodem_write );

        // Reset
        xfer_offset = 0;
        xfer_sector_offset = 0;
    }
    while (res < 0);

    // Normal boot
    {
        int (*app_start)();
        app_start = (unsigned long *)(xfer_base);
        app_start();

        while (1) 
            ;
    }
}
//-----------------------------------------------------------------
// xmodem_write:
//-----------------------------------------------------------------
static int xmodem_write(unsigned char* buffer, int size)
{
    // Write to flash
    int i;
    int flush = 0;

    // Flush final block
    if (size == 0)
        flush = 1;

    // Write final (and already done!)
    if (flush && xfer_sector_offset == 0)
        return 0;

    // We are relying on the Flash sector size to be a multiple
    // of Xmodem transfer sizes (128 or 1024)...

    // Copy to write buffer
    for (i=0;i<size;i++)
        flash_write_buffer[xfer_sector_offset + i] = buffer[i];

    // Have we got enough data to write a sector?
    if ((xfer_sector_offset + size) >= FLASH_SECTOR_SIZE || flush)
    {
        // Write to memory 
        unsigned char *ptr = (unsigned char *)(xfer_base + xfer_offset);

        for (i=0;i<FLASH_SECTOR_SIZE;i++)
            *ptr++ = flash_write_buffer[i];

        // Reset to start of sector
        xfer_sector_offset = 0;
    }
    else
        xfer_sector_offset += size;

    // Increment end point to include new data
    xfer_offset += size;
    return 0;
}
