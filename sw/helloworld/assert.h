#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "serial.h"

#define assert(exp)		do { if (!(exp)) { serial_putstr(#exp); while (1) ; } } while (0)
#define panic(reason)	do { serial_putstr(#reason); while (1) ; } while (0)

#endif

