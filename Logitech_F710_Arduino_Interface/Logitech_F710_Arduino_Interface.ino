/* 
  Demo of a simple USB Parser for the Logitech F710 Gamepad using a USB Host Shield peripheral device for Arduino
  - Place the logitech_f710_usb_host_parser.h and logitech_f710_usb_host_parser.cpp files in the same directory as this .ino file for this demo
*/

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <SPI.h>
#include "logitech_f710_usb_host_parser.h"

#define SERIAL_BAUD_RATE  115200    

USB                             Usb;
USBHub                          Hub(&Usb);
HIDUniversal                    Hid(&Usb);
Logitech_F710_Parser            gamepad_parser;


void setup(){
  Serial.begin(SERIAL_BAUD_RATE);

  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #if !defined(__MIPSEL__)
    while (!Serial); 
  #endif

  // Initialize the USB class instance, and print the result to Serial Monitor to verify whether the USB Host Shield is working
  if (Usb.Init() == -1) Serial.println("USB Host Shield MAX3421E On-Chip Support Controller (OSC) did not initialize.");
  else Serial.println("USB Host Shield MAX3421E On-Chip Support Controller (OSC) initialized.");

  // Pass an instance of the Logitech_F710_Gamepad parser to the USB HID library as the handler for processing incoming USB packets (HID calls the overridden Parse() function from this class)
  if (!Hid.SetReportParser(0, &gamepad_parser))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);

  if (Hid.isReady()){
    Serial.println("HID Receiver connected.");
  } else { 
      Serial.println("No HID device detected.");
  }
}


Gamepad_Status_Data previous_status;  // Holds packet data from the most recent parsed packet
void loop(){

  // Get the latest HID report from the connected USB device, and process any incoming/outgoing USB packets
  Usb.Task();

  // Only perform the following prints to the Serial Monitor when the latest packet contains unique data
  if(gamepad_parser.compare_packet_structs(previous_status, gamepad_parser.gamepad_status) != 0){

    // Update the previous_packet struct
    previous_status = gamepad_parser.copy_packet_struct(gamepad_parser.gamepad_status);

    /* Prints the content of the most recent processed USB packet to Serial Monitor
        - Change the function parameter to print in binary ("BIN"), decimal ("DEC") or hexidecimal ("HEX")
    */
    gamepad_parser.print_raw_packet(BIN);

    /* Example of how to reference an input on the gamepad from the gamepad_status struct
      - Check the gamepad_status struct in logitech_f710_usb_host_parser.h for the full list of gamepad inputs that can be referenced
    */
    if(gamepad_parser.gamepad_status.a_button != 0){
      Serial.println("A Pressed!");
    }

    /* Example of how to reference the D_Pad status using the D_Pad_Values struct (Uses)
        - Reference the D_Pad_Values struct in logitech_f710_usb_host_parser.h 
    */
    if(gamepad_parser.gamepad_status.d_pad == d_Pad_Values.d_pad_up){
      Serial.println("D_Pad pushed UP!");
    }
  }
}

