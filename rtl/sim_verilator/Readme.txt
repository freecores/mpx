MPX RTL Simulation
------------------

This directory contains the Verilator RTL simulation model.

Requirements:
- verilator-3.831 or newer
- GTKWave

To run an application binary:
  make TEST_IMAGE=test_image.bin

Where test_image.bin is your compiled source which has a starting address of 0x10000000.

To generate (GTKwave) waveforms;
  make TEST_IMAGE=test_image.bin TRACE=1

To view waveform with GTKWave:
  make view

Test image expected output
--------------------------

Test:
1. Initialised data
2. Multiply
3. Divide
4. Shift left
5. Shift right
6. Shift right arithmetic
7. Signed comparision
8. Word access
9. Byte access
10. SYSCALL return PC check
11. Comparision
Exit
