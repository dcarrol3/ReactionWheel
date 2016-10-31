# Makefile for SER-456
# Doug Carroll
# SER-456 Fall 2016
# Setup for accel and serial

FILENAME = accel

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

$(FILENAME).elf: $(FILENAME).o serial.o 
	$(CC) -g -mmcu=$(CHIP) -o $@ $< serial.o

$(FILENAME).o: $(FILENAME).c $(FILENAME).h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<
	
serial.o: serial.c serial.h
	$(CC) $(CFLAGS) -mmcu=$(CHIP) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o *.~ *.elf *.hex

send:
	avrdude -c usbtiny -p $(CHIP) -U flash:w:$(FILENAME).hex
