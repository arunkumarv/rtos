MCU=atmega328p
SRC=../src
INC=../inc
OPTIONS=-g -Wall
DEVICE=/dev/ttyACM0

all: main.hex main.lst main.map

main.o: $(SRC)/main.c
	avr-gcc $(OPTIONS) -mmcu=$(MCU) -I$(INC) -c $(SRC)/main.c -o main.o

util.o: $(SRC)/util.c
	avr-gcc $(OPTIONS) -mmcu=$(MCU) -I$(INC) -c $(SRC)/util.c -o util.o

task.o: $(SRC)/task.c
	avr-gcc $(OPTIONS) -mmcu=$(MCU) -I$(INC) -c $(SRC)/task.c -o task.o

main.elf: main.o util.o task.o 
	avr-gcc -g -mmcu=$(MCU) -o main.elf main.o util.o task.o

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex 

main.lst: main.elf
	avr-objdump -h -S main.elf > main.lst
 
main.map: main.elf
	avr-gcc -g -mmcu=$(MCU) -Wl,-Map,main.map -o main.elf main.o util.o task.o
	avr-size main.elf

burn:
	avrdude -F -V -c arduino -p $(MCU) -P $(DEVICE) -b 115200 -U flash:w:main.hex

clean:
	rm -rf main.elf main.hex main.lst main.map *.o
