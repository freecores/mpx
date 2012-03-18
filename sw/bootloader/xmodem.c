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
#include <string.h>
#include "xmodem.h"
#include "timer.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define XMODEM_HDR_SIZE		6

// Control character codes
#define SOH			0x01
#define STX			0x02
#define EOT			0x04
#define ACK			0x06
#define NAK			0x15
#define CAN			0x18
#define CTRLZ		0x1A

#define XMODEM_CRC_REQ 'C'

//-----------------------------------------------------------------
// Locals
//-----------------------------------------------------------------

// Serial IO function pointers
static int (*_serial_putc)(char c) = 0;
static int (*_serial_getc)(void) = 0;

// Xmodem buffer
static unsigned char xbuffer[XMODEM_BUFFER_SIZE+XMODEM_HDR_SIZE];

//-----------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------
static unsigned short calc_crc_16(unsigned char *data, int length);
static unsigned char  calc_checksum(unsigned char *data, int length);
static int xmodem_verify_checksum(unsigned char *buffer, int size, int useCRC);
static int xmodem_getc(unsigned int timeout);
static void xmodem_flush(void);

//-----------------------------------------------------------------
// xmodem_init: Initialise X-Modem module
//-----------------------------------------------------------------
void xmodem_init(int (*sputc)(char c), int (*sgetc)(void))
{
    _serial_putc = sputc;
    _serial_getc = sgetc;
}
//-----------------------------------------------------------------
// xmodem_receive: Receive a X-Modem transfer
//-----------------------------------------------------------------
int xmodem_receive( int (*write)(unsigned char* buffer, int size) )
{
    unsigned char seq_num = 1;
    unsigned short packet_length = 128;
    unsigned char response;
    char retry = XMODEM_RETRY_LIMIT;
    unsigned char useCrc = 0;
    int totalbytes = 0;
    int i, c;

    // No I/O functions? No transfer!
    if (!_serial_putc || !_serial_getc)
        return -1;

    // Start by requesting CRC transfer
    response = XMODEM_CRC_REQ;

    while(retry > 0)
    {
        // solicit a connection/packet
        _serial_putc(response);

        // wait for start of packet
        if( (c = xmodem_getc(XMODEM_TIMEOUT_DELAY)) >= 0)
        {
            switch(c)
            {
            // Start of transfer (128 bytes)
            case SOH:
                packet_length = 128;
                break;

            // Start of transfer (1024 bytes)
            #if(XMODEM_BUFFER_SIZE>=1024)
            case STX:
                packet_length = 1024;
                break;
            #endif

            // End of transfer 
            case EOT:
                xmodem_flush();
                _serial_putc(ACK);
                // Inform app layer of end of transfer
                write(0, 0); 
                return totalbytes;

            // Cancel transfer
            case CAN:
                if((c = xmodem_getc(XMODEM_TIMEOUT_DELAY)) == CAN)
                {
                    xmodem_flush();
                    _serial_putc(ACK);
                    return -1;
                }
            default:
                break;
            }
        }
        // No response, retry
        else
        {
            retry--;
            continue;
        }

        // Using CRC mode (as requested)
        if(response == 'C') 
            useCrc = 1;

        // Add header character to buffer (SOH/STX)
        xbuffer[0] = c;
        
        // Wait for rest of packet
        for(i=0; i<(packet_length+useCrc+4-1); i++)
        {
            // Get a byte of data
            if((c = xmodem_getc(XMODEM_TIMEOUT_DELAY)) >= 0)
                xbuffer[1+i] = c;
            // Timeout while expecting data
            else
            {
                retry--;
                xmodem_flush();
                response = NAK;
                break;
            }
        }

        // If timeout, retry
        if( i < (packet_length+useCrc+4-1) )
            continue;
        // Packet received, check checksum & sequence number
        else if( (xbuffer[1] == (unsigned char)(~xbuffer[2])) && xmodem_verify_checksum(&xbuffer[3], packet_length, useCrc))
        {
            // Is this the sequence number waited on?
            if(xbuffer[1] == seq_num)
            {
                // Pass data to user call-back
                write(&xbuffer[3], packet_length);
                
                // Increment receive count
                totalbytes += packet_length;

                // Next sequence number
                seq_num++;

                // Reset retries
                retry = XMODEM_RETRY_LIMIT;

                // ACK packet
                response = ACK;

                continue;
            }
            // Last packet resent?
            else if(xbuffer[1] == (unsigned char)(seq_num-1))
            {
                // Just ACK it
                response = ACK;
                continue;
            }
            // Some other failure!
            else
            {
                xmodem_flush();
                _serial_putc(CAN);
                _serial_putc(CAN);
                _serial_putc(CAN);

                return -1;
            }
        }
        // Checksum / sequence number check byte are wrong
        else
        {
            // Send NAK
            retry--;
            xmodem_flush();
            response = NAK;
            continue;
        }
    }

    // Retries count exceeded
    xmodem_flush();
    _serial_putc(CAN);
    _serial_putc(CAN);
    _serial_putc(CAN);

    return -1;
}
//-----------------------------------------------------------------
// calc_crc_16: Calculate 16 bit CRC used by XModem(CRC)
//				Polynomial x^16 + x^12 + x^5 + 1 (0x1021)
//-----------------------------------------------------------------
static unsigned short calc_crc_16(unsigned char *data, int length)
{
    unsigned char c;
    unsigned short crc = 0;
    int i;

    while (length)
    {
        c = *data++;
        crc = crc ^ ((unsigned short)c << 8);
        for (i=0; i<8; i++)
        {
            if(crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }

        length--;
    }

    return crc;
}
//-----------------------------------------------------------------
// calc_checksum: Calculate standard (summation) checksum used by
// older XModem protocol versions
//-----------------------------------------------------------------
static unsigned char calc_checksum(unsigned char *data, int length)
{
    unsigned char sum = 0;
    
    while (length)	
    {
        sum += *data++;
        length--;
    }

    return sum;
}
//-----------------------------------------------------------------
// xmodem_verify_checksum: Verify the checksum for the received packet
//-----------------------------------------------------------------
static int xmodem_verify_checksum(unsigned char *buffer, int size, int useCRC)
{
    int res = 0;

    // CRC-16
    if(useCRC)
    {
        unsigned short crc = (buffer[size]<<8) + buffer[size+1];
        
        if(calc_crc_16((unsigned char*)buffer, size) == crc)
            res = 1;
    }
    // Old sum checksum
    else
    {
        // check checksum against packet
        if(calc_checksum(buffer, size) == buffer[size])
            res = 1;
    }

    return res;
}
//-----------------------------------------------------------------
// xmodem_getc: Get a character from serial port within timeout
//-----------------------------------------------------------------
static int xmodem_getc(unsigned int timeout)
{
    int c = -1;
    t_time startTime = timer_now();

    while ( (c = _serial_getc()) < 0 ) 
        if (timer_diff(timer_now(), startTime) >= timeout)
            break;

    return c;
}
//-----------------------------------------------------------------
// xmodem_flush: Flush serial port
//-----------------------------------------------------------------
static void xmodem_flush(void)
{
    while(xmodem_getc(XMODEM_TIMEOUT_DELAY) >= 0);
}
