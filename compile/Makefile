MCU=atmega328p
SRC=../src
INC=../inc

all: main.hex main.lst main.map

main.o: $(SRC)/main.c
	avr-gcc -g -mmcu=$(MCU) -I$(INC) -c $(SRC)/main.c -o main.o

user.o: $(SRC)/user.c
	avr-gcc -g -mmcu=$(MCU) -I$(INC) -c $(SRC)/user.c -o user.o
	
print.o: $(SRC)/print.c
	avr-gcc -g -mmcu=$(MCU) -c $(SRC)/print.c -o print.o

main.elf: main.o print.o user.o
	avr-gcc -g -mmcu=$(MCU) -o main.elf main.o print.o user.o

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex 

main.lst: main.elf
	avr-objdump -h -S main.elf > main.lst
 
main.map: main.elf
	avr-gcc -g -mmcu=$(MCU) -Wl,-Map,main.map -o main.elf main.o print.o user.o
	avr-size main.elf

burn:
	avrdude -F -V -c arduino -p $(MCU) -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex


clean:
	rm -rf main.elf main.hex main.lst main.map main.o print.o user.o
