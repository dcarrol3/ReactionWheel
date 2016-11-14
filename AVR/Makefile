# Makefile for SER-456
# Doug Carroll
# SER-456 Fall 2016
# Setup for accel, serial and motorDrive

FILENAME = main

CC = avr-gcc
COPY = avr-objcopy
COPYFLAGS = -j .text -j .data -O ihex
CFLAGS = -Os -g -std=gnu99 -Wall
CHIP = atmega168

# Set target as file name for readability.
TARGET = $(FILENAME)

all: $(FILENAME).hex

$(FILENAME).hex: $(FILENAME).elf
	$(COPY) $(COPYFLAGS) $< $(TARGET).hex

$(FILENAME).elf: $(FILENAME).o serial.o motorDrive.o pid.o accel.o
	$(CC) -g -mmcu=$(CHIP) -o $@ $< serial.o motorDrive.o pid.o accel.o

$(FILENAME).o: $(FILENAME).c
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<
	
accel.o: accel.c accel.h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<
	
serial.o: serial.c serial.h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<

motorDrive.o: motorDrive.c motorDrive.h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<
	
pid.o: pid.c pid.h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o *.~ *.elf *.hex

send:
	avrdude -c usbtiny -p $(CHIP) -U flash:w:$(FILENAME).hex
