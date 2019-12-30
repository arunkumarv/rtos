# Task Switching code
This is a simple task switching (An RTOS) system for avr ( specifically Atmega328p ). I was trying to implement a basic scheduler.
Developed Fixed priority pre-emptive scheduling policy.

Here is a screen record - [Youtube](https://www.youtube.com/watch?v=pLknZu1mUhs)

## How to 
- Install avr toolchain, minicom
- For debian distros: sudo apt-get install gcc-avr binutils-avr gdb-avr avr-libc avrdude

- Need an Arduino UNO board.
- Connect the board to the system. Open `./compile/Makefile`. DEVICE= option with the new 'tty' device name populated in /dev folder.

## Compile and burn
cd ./compile
make
sudo make burn

sudo minicom -s
* Select - Serial port setup
* Change settings to the following
  Serial Device      : /dev/ttyACM0 
  Bps/Par/Bits       : 38400 8N1 
  Hardware Flow Control : Yes
  Software Flow Control : No
* Save setup as dfl
* Exit

We can see tasks are switching and getting deleted according to our function_1, 2, 3, 4 defnitions.
Check src/main.c. There are 4 functions, function_1 function_2 function_3 function_4.
Fixed priority pre-emptive scheduling (FPPS) is the scheduling policy. So to switch the task priority of other functions are changing in the function.
FPPS select the highest priority task to run.
They are infinite loops. But every time when the scheduler executes

Note
----
UNO board is not specific. You can have an Atmega328p configured on a breadboard. 
Compile the code and program the chip with a burner of your choice. 
For that change the 'burn' in Makefile according to the burner you have. 
For to see the data printed with printf, use an FTDI breakout board and 
configure minicom properly to view data through breakout board.
Or a blink LED code is enough. For this change the printf statements in main with
blink led code and connect some LEDs to the chip.
