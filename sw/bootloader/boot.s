   .text
   .align 2
   .global entry
   .extern cpu_handle_irq
   
#-------------------------------------------------------------
# entry:
#-------------------------------------------------------------	   
   .ent	entry
entry:
   .set noreorder

   la    $gp, _gp           # initialize global pointer
   la    $5, _edata			# $5 = .sbss_start
   la    $4, _end           # $2 = .bss_end
   la    $sp, _sp			# initialize stack pointer
   
$BSS_CLEAR:
   sw    $0, 0($5)			# Write 0x00 to mem[$5]
   slt   $3, $5, $4			# $3 = $5 < $4
   bnez  $3, $BSS_CLEAR		# If $3 != 0, jump to BSS_CLEAR
   addiu $5, $5, 4			# $5 += 4

   jal   main
   nop
$L1:
   j $L1

   .end entry
   
#-------------------------------------------------------------
# asm_handle_irq:
#-------------------------------------------------------------	  
   .global asm_handle_irq
   .ent asm_handle_irq	
   
asm_handle_irq:
   .set noreorder
   
    nop
    nop       
   
   # Get cause into first arg register
   mfc0 $a0, $13        # C0_CAUSE
   
   # Jump to irq handling function
   jal   cpu_handle_irq
   nop   
   
   .set reorder  
   .end asm_handle_irq	
	
