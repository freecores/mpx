MPX RTL Simulation
------------------

This directory contains the Verilator RTL simulation model.

Requirements:
- verilator-3.831 or newer

To run applications:
./run_mpx image.bin

Where image.bin is your compiled source which has a starting address of 0x00002000.

Example Image:
./run_mpx test_image.bin

Expected Output:

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
Correct comparision
Total test clock cycles: 1180442
DONE!
