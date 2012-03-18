#ifndef __XMODEM_H__
#define __XMODEM_H__

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

// Support 128 byte transfers
#define XMODEM_BUFFER_SIZE		128

// Support 1024 byte transfers
//#define XMODEM_BUFFER_SIZE		1024

// xmodem timeout/retry parameters
#define XMODEM_TIMEOUT_DELAY	1000
#define XMODEM_RETRY_LIMIT		16

//-----------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------
void	xmodem_init(int (*sputc)(char c), int (*sgetc)(void));
int		xmodem_receive( int (*write)(unsigned char* buffer, int size) );

#endif  // __XMODEM_H__

