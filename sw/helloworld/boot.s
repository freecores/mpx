   .text
   .align 2
   .global entry
   .extern cpu_handle_irq
   .extern _currentTCB
   
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
# asm_save_context:
#-------------------------------------------------------------	
   .global asm_save_context
   .ent asm_save_context	
   
asm_save_context:
   .set noreorder
   .set noat
   
    nop
    nop
    
    # Adjust SP
    addi  $sp, $sp, -144	    
        
    # Save register set to stack    
	sw $ra, 136($sp)
	sw $fp, 132($sp)
	# Ignore $gp as it will not be changing
	#sw $gp, 128($sp)
	# Don't bother saving k0 & k1	
	#sw $27, 124($sp)
	#sw $26, 120($sp)
	sw $25, 116($sp)
	sw $24, 112($sp)
	sw $23, 108($sp)
	sw $22, 104($sp)
	sw $21, 100($sp)
	sw $20, 96($sp)
	sw $19, 92($sp)
	sw $18, 88($sp)
	sw $17, 84($sp)		
	sw $16, 80($sp)
	sw $15, 76($sp)
	sw $14, 72($sp)
	sw $13, 68($sp)
	sw $12, 64($sp)
	sw $11, 60($sp)
	sw $10, 56($sp)
	sw $9,  52($sp)
	sw $8,  48($sp)
	sw $7,  44($sp)
	sw $6,  40($sp)
	sw $5,  36($sp)
	sw $4,  32($sp)
	sw $3,  28($sp)
	sw $2,  24($sp)
	sw $1,  20($sp)
	
	# Save LO & HI
	mfhi    $t0
	sw $t0,  16($sp)	
	mflo    $t0
	sw $t0,  12($sp)		
	
	# Save EPC (PC of non-exception code)
	mfc0 $t0, $14        # C0_EPC
    sw $t0, 0($sp)    
	
	# Save stack pointer to TCB
	la $t0, _currentTCB
	sw $sp, ($t0)
	
   .set reorder
   .end asm_save_context		
   
#-------------------------------------------------------------
# asm_handle_irq:
#-------------------------------------------------------------	  
   .global asm_handle_irq
   .ent asm_handle_irq	
   
asm_handle_irq:
   .set noreorder
   
   # Get cause into first arg register
   mfc0 $a0, $13        # C0_CAUSE
   
   # Jump to irq handling function
   jal   cpu_handle_irq
   nop   
   
   .set reorder  
   .end asm_handle_irq	
	
#-------------------------------------------------------------
# asm_load_context:
#-------------------------------------------------------------		
   .global asm_load_context
   .ent asm_load_context	
   
asm_load_context:
   .set noreorder
   .set noat
   
	# TODO: restore sp? 

	# Restore EPC (PC of non-exception code) to $k0
	lw $k0,  0($sp)
	# Load delay slot
	nop 
	mtc0 $k0, $14        # C0_EPC
	
	# Restore LO & HI
	lw $t0,  12($sp)
	# Load delay slot
	nop 	
	mtlo    $t0
	lw $t0,  16($sp)
	# Load delay slot
	nop 	
	mthi    $t0	
	
	# Restore register set
	lw $1,  20($sp)
	lw $2,  24($sp)
	lw $3,  28($sp)
	lw $4,  32($sp)
	lw $5,  36($sp)
	lw $6,  40($sp)
	lw $7,  44($sp)
	lw $8,  48($sp)
	lw $9,  52($sp)
	lw $10, 56($sp)
	lw $11, 60($sp)
	lw $12, 64($sp)
	lw $13, 68($sp)
	lw $14, 72($sp)
	lw $15, 76($sp)
	lw $16, 80($sp)
	lw $17, 84($sp)
	lw $18, 88($sp)
	lw $19, 92($sp)
	lw $20, 96($sp)
	lw $21, 100($sp)
	lw $22, 104($sp)
	lw $23, 108($sp)
	lw $24, 112($sp)
	lw $25, 116($sp)
	# Don't bother restoring k0 & k1		
	#lw $26, 120($sp)
	#lw $27, 124($sp)		
	# Ignore $gp as it will not be changing
	#lw $gp, 128($sp)	
	lw $fp, 132($sp)
	lw $ra, 136($sp)
	
	# Adjust SP past register set
	addi  $sp, $sp, +144 
	
	# Leave external interrupts disabled
	la $k1,  0		
	# Load delay slot
	nop 		
	
	# Return from exception
    jr    $26
    mtc0  $k1, $12        # $k1 -> status register (ints enable / disable)
    nop	
	
   .set reorder
   .end asm_load_context	
