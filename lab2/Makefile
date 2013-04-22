DEVICE = atmega1284p
MCU = atmega1284p
AVRDUDE_DEVICE = m1284p
PORT ?= /dev/ttyACM0
DEVICE ?= atmega168
MCU ?= atmega168
AVRDUDE_DEVICE ?= m168

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) -Os
CXXFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) -Os
CC=avr-gcc
CXX=avr-g++
OBJ2HEX=avr-objcopy 
LDFLAGS=-Wl,-gc-sections -lpololu_$(DEVICE) -Wl,-relax

PORT ?= /dev/ttyACM0
AVRDUDE=avrdude

TARGET=lab2
OBJECT_FILES=main.o motor.o encoder.o serial.o logger.o pid.o cmd_processor.o interpolator.o timer.o

all: $(TARGET).hex

assembly: $(OBJECT_FILES:.o=.asm)
	
clean:
	rm -f *.o *.hex *.obj *.hex *.asm

%.asm: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -Wa,-a,-ad $< > $@

%.asm: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c -Wa,-a,-ad $< > $@

%.hex: $(TARGET).obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CXX) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex
