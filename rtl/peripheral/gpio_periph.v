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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
`include "gpio_defs.v"

//-----------------------------------------------------------------
// Module:
//-----------------------------------------------------------------
module gpio_periph
( 
    // General
    clk_i, 
    rst_i, 
    intr_o,
    
    // Ports
    gpio_i,
    gpio_o,
    gpio_out_en_o,
    
    // Peripheral bus
    addr_i, 
    data_o, 
    data_i, 
    wr_i, 
    rd_i
);

//-----------------------------------------------------------------
// Params
//-----------------------------------------------------------------
parameter               GPIO_WIDTH             = 8;
    
//-----------------------------------------------------------------
// I/O
//-----------------------------------------------------------------     
input                   clk_i /*verilator public*/;
input                   rst_i /*verilator public*/;
output                  intr_o /*verilator public*/;
input [GPIO_WIDTH-1:0]  gpio_i /*verilator public*/;
output [GPIO_WIDTH-1:0] gpio_o /*verilator public*/;
output [GPIO_WIDTH-1:0] gpio_out_en_o /*verilator public*/;
input [7:0]             addr_i /*verilator public*/;
output [31:0]           data_o /*verilator public*/;
input [31:0]            data_i /*verilator public*/;
input [3:0]             wr_i /*verilator public*/;
input                   rd_i /*verilator public*/;

//-----------------------------------------------------------------
// Registers / Wires
//-----------------------------------------------------------------

reg [31:0]              data_o;
wire[GPIO_WIDTH-1:0]    gpio_o;
wire[GPIO_WIDTH-1:0]    gpio_out_en_o;
reg                     intr_o;

reg [GPIO_WIDTH-1:0]    gpio_out;
reg [GPIO_WIDTH-1:0]    gpio_in;
reg [GPIO_WIDTH-1:0]    gpio_in_last;
reg [GPIO_WIDTH-1:0]    gpio_in_sync;
reg [GPIO_WIDTH-1:0]    gpio_out_en;
reg [GPIO_WIDTH-1:0]    gpio_int_mask;
reg [GPIO_WIDTH-1:0]    gpio_int_polarity;
reg [GPIO_WIDTH-1:0]    gpio_int_edge;

//-----------------------------------------------------------------
// Resync inputs
//-----------------------------------------------------------------  
always @ (posedge rst_i or posedge clk_i )
begin 
   if (rst_i == 1'b1) 
   begin 
        gpio_in_sync    <= {(GPIO_WIDTH){1'b0}};
        gpio_in         <= {(GPIO_WIDTH){1'b0}};
   end
   else 
   begin 
        gpio_in_sync    <= gpio_i;
        gpio_in         <= gpio_in_sync;
   end
end

//-----------------------------------------------------------------
// GPIO interrupt handling
//-----------------------------------------------------------------  
reg v_gpio_in;
reg v_intr_out;

always @ (posedge rst_i or posedge clk_i )
begin 
   if (rst_i == 1'b1) 
   begin 
        gpio_in_last    <= {(GPIO_WIDTH){1'b0}};
        intr_o          <= 1'b0;
   end
   else 
   begin
        gpio_in_last    <= gpio_in;
       
        v_intr_out      = 1'b0; 
       
        // GPIO interrupt?
        begin : ext_ints_loop
           integer i;
           for (i=0; i< GPIO_WIDTH; i=i+1) 
           begin 
           
                // Input & interrupt enabled on this input
                if (gpio_out_en[i] == 1'b0 && gpio_int_mask[i] == 1'b1)
                begin
                    // Level sensitive interrupt
                    if (gpio_int_edge[i] == 1'b0)
                    begin
                        v_gpio_in = gpio_in[i];
                        
                        // Invert polarity for active low interrupts?
                        if (gpio_int_polarity[i] == 1'b0)  
                            v_gpio_in = !v_gpio_in;
                        
                        // Interrupt active?
                        if (v_gpio_in == 1'b1)
                            v_intr_out = 1'b1;
                    end
                    // Edge sensitive interrupt
                    else
                    begin
                    
                        // Falling edge triggered
                        if (gpio_int_polarity[i] == 1'b0)
                        begin 
                            
                            // Pin H -> L
                            if (gpio_in_last[i] == 1'b1 && gpio_in[i] == 1'b0)
                                v_intr_out = 1'b1;

                        end    
                        // Rising edge triggered
                        else
                        begin
                            
                            // Pin L -> H
                            if (gpio_in_last[i] == 1'b0 && gpio_in[i] == 1'b1)
                                v_intr_out = 1'b1;
                        
                        end                  
                    end
                end
                
           end
        end

        intr_o   <= v_intr_out;
   end
end

//-----------------------------------------------------------------
// Peripheral Register Write
//-----------------------------------------------------------------   
always @ (posedge rst_i or posedge clk_i )
begin 
   if (rst_i == 1'b1) 
   begin 
        gpio_out            <= {(GPIO_WIDTH){1'b0}}; 
        gpio_out_en         <= {(GPIO_WIDTH){1'b1}}; 
        gpio_int_mask       <= {(GPIO_WIDTH){1'b0}}; 
        gpio_int_polarity   <= {(GPIO_WIDTH){1'b0}}; 
        gpio_int_edge       <= {(GPIO_WIDTH){1'b0}};      
   end
   else 
   begin 
   
       // Write Cycle
       if (wr_i != 4'b0000)
       begin
           case (addr_i)
           
           `GPIO_OUT : 
                gpio_out            <= data_i[GPIO_WIDTH-1:0];
                
           `GPIO_DIR : 
                gpio_out_en         <= data_i[GPIO_WIDTH-1:0];                
                
           `GPIO_INT_MASK : 
                gpio_int_mask       <= data_i[GPIO_WIDTH-1:0];                
                
           `GPIO_INT_POLARITY : 
                gpio_int_polarity   <= data_i[GPIO_WIDTH-1:0];                  
                
           `GPIO_INT_EDGE : 
                gpio_int_edge       <= data_i[GPIO_WIDTH-1:0];                 
                  
           default : 
               ;
           endcase
        end
   end
end

//-----------------------------------------------------------------
// Peripheral Register Read
//----------------------------------------------------------------- 
always @ (posedge rst_i or posedge clk_i )
begin 
   if (rst_i == 1'b1) 
   begin 
       data_o       <= 32'h00000000;
   end
   else 
   begin 
       // Read cycle?
       if (rd_i == 1'b1)
       begin
           case (addr_i[7:0])
                
           `GPIO_OUT :
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_out};

           `GPIO_DIR : 
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_out_en};
                
           `GPIO_IN : 
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_in};              
                
           `GPIO_INT_MASK : 
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_int_mask};                   
                
           `GPIO_INT_POLARITY : 
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_int_polarity};                
                
           `GPIO_INT_EDGE : 
                data_o <= {{(32-GPIO_WIDTH){1'b0}}, gpio_int_edge}; 
                             
           default : 
                data_o <= 32'h00000000;
           endcase
        end
   end
end
      
//-----------------------------------------------------------------
// Assignments
//-----------------------------------------------------------------     
assign gpio_o           = gpio_out;
assign gpio_out_en_o    = gpio_out_en;

endmodule
