/*
 * Douglas Carroll
 * 
 * Program to run on the pi zero to
 * receive serial data from AVR and
 * transmit out through Wifi
 * 
 * USB device used:
 * https://www.silabs.com/Support%20Documents/TechnicalDocs/CP2102-9.pdf
 */
 
#ifndef pi_h_
#define pi_h_

void initSerial();
void sendString(char *str);
void sendByte(uint8_t c);
uint8_t receiveByte();

#endif
