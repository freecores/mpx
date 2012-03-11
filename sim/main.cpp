//-----------------------------------------------------------------
//                MPX 32-bit Soft-Core Processor
//                            V0.1
//                     Ultra-Embedded.com
//                   Copyright 2011 - 2012
//
//               Email: admin@ultra-embedded.com
//
//                       License: LGPL
//
// If you would like a version with a different license for use 
// in commercial projects please contact the above email address 
// for more details.
//-----------------------------------------------------------------
//
// Copyright (C) 2011 - 2012 Ultra-Embedded.com
//
// This source file may be used and distributed without         
// restriction provided that this copyright statement is not    
// removed from the file and that any derivative work contains  
// the original copyright notice and the associated disclaimer. 
//
// This source file is free software; you can redistribute it   
// and/or modify it under the terms of the GNU Lesser General   
// Public License as published by the Free Software Foundation; 
// either version 2.1 of the License, or (at your option) any   
// later version.                                               
//
// This source is distributed in the hope that it will be       
// useful, but WITHOUT ANY WARRANTY; without even the implied   
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      
// PURPOSE.  See the GNU Lesser General Public License for more 
// details.                                                     
//
// You should have received a copy of the GNU Lesser General    
// Public License along with this source; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
// Boston, MA  02111-1307  USA              
//-----------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>

#include "mpx_top.h"
#include "verilated.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Locals
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// main
//-----------------------------------------------------------------
int main(int argc, char **argv, char **env) 
{
	FILE *f;
	char *filename;
	int err;

	Verilated::commandArgs(argc, argv);

	mpx_test_init();

	if (argc <= 1)
	{
		argc = 2;
		filename = (char*)".\\test.bin";
	}
	else
		filename = argv[1];

	printf("Opening %s\n", filename);
	f = fopen(filename, "rb");
	if (f)
	{
		long size;
		char *buf;

		// Get size
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		rewind(f);

		buf = (char*)malloc(size+1);
		if (buf)
		{
			unsigned int addr;

			// Read file data in
			int len = fread(buf, 1, size, f);
			buf[len] = 0;

			for (addr=0;addr<len;addr++)
				mpx_test_load(addr, buf[addr]);

			free(buf);
			fclose(f);
		}
	}
	else
	{
		printf("No file to execute!\n");
		return 0;
	}

	// Run
	err = mpx_test_run(-1);

	if (err == MPX_RUN_RES_FAULT)
		printf("FAULT PC %x!\n", mpx_test_getpc());

	mpx_test_done();

	printf("Exit\n");
	exit(0);
}
