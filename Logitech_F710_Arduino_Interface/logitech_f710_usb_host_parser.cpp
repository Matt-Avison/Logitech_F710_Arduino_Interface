#include "logitech_f710_usb_host_parser.h"

Logitech_F710_Parser::Logitech_F710_Parser(){}

const D_Pad_Values d_Pad_Values = {};


// USB packet parsing function for the Logitech F710 that populates the gamepad_status struct
void Logitech_F710_Parser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf){

  // Ensure that the packet length matches the expected size
  if (len != LOGITECH_F710_PACKET_LENGTH) return; 

  // Copy of the current packet buffer
  for(int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
    gamepad_status.packet_buffer[i] = buf[i];
  }

  // Mapped Gamepad Status Data
  gamepad_status.hid_report_id         = buf[0];               // Byte 0 contains reserved bits for the HID packet report
  gamepad_status.left_joystick_x       = buf[1];               // Byte 1 contains left joystick x-direction data (0 - 255) (0: Fully pushed LEFT, 128: Centered, 255: Fully pushed RIGHT)
  gamepad_status.left_joystick_y       = buf[2];               // Byte 2 contains left joystick y-direction data (0 - 255) (0: Fully pushed UP, 128: Centered, 255: Fully pushed RIGHT)
  gamepad_status.right_joystick_x      = buf[3];               // Byte 3 contains right joystick x-direction data (0 - 255) (0: Fully pushed LEFT, 128: Centered, 255: Fully pushed RIGHT)
  gamepad_status.right_joystick_y      = buf[4];               // Byte 4 contains right joystick y-direction data (0 - 255) (0: Fully pushed UP, 128: Centered, 255: Fully pushed RIGHT)
  gamepad_status.d_pad                 = buf[5] & 0b00001111;  // D_Pad bitmask - Lower 4 bits of Byte 5 hold a value representing the D-Pad status (Reference the "D_Pad_Values" struct in the header file)
  gamepad_status.x_button              = buf[5] & 0b00010000;  // X_Button bitmask on Byte 5 (1: Pressed, 0: Not pressed)
  gamepad_status.a_button              = buf[5] & 0b00100000;  // A_Button bitmask on Byte 5  (1: Pressed, 0: Not pressed)
  gamepad_status.b_button              = buf[5] & 0b01000000;  // B_Button bitmask on Byte 5  (1: Pressed, 0: Not pressed)
  gamepad_status.y_button              = buf[5] & 0b10000000;  // Y_Button bitmask on Byte 5  (1: Pressed, 0: Not pressed)
  gamepad_status.left_bumper           = buf[6] & 0b00000001;  // Left_Bumper bitmask on Byte 6 (1: Pressed, 0: Not pressed) 
  gamepad_status.right_bumper          = buf[6] & 0b00000010;  // Right_Bumper bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.left_trigger          = buf[6] & 0b00000100;  // Left_Trigger bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.right_trigger         = buf[6] & 0b00001000;  // Right_Trigger bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.back_button           = buf[6] & 0b00010000;  // Back_Button bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.start_button          = buf[6] & 0b00100000;  // Start_Button bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.left_joystick_button  = buf[6] & 0b01000000;  // Left_Joystick_Button bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.right_joystick_button = buf[6] & 0b10000000;  // Right_Joystick_Button bitmask on Byte 6 (1: Pressed, 0: Not pressed)
  gamepad_status.mode_toggle           = buf[7] & 0b00001000;  // Mode_Toggle button bitmask on Byte 7 (Bit value persists after button released, and toggles on next button press)
  gamepad_status.vibrate_toggle        = buf[7] & 0b00100000;  // Vibrate_Toggle button bitmask on Byte 7 (Bit value persists after button released, and toggles on next button press)
  // All other bits are reserved or unused
}


/* Useful debug function for printing raw USB packets to the Serial Monitor 
    - Prints the contents of the packet_buffer from the gamepad_status struct
    - "Print_mode" can be set to binary (BIN), decimal (DEC) or hexidecimal (HEX). Defaults to binary.
*/
void Logitech_F710_Parser::print_raw_packet(int print_mode = BIN){
  switch(print_mode){

    case BIN:
      Serial.print("Raw Packet (Binary):    ");
      for (int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
        Serial.print(gamepad_status.packet_buffer[i], BIN);
        Serial.print(" ");
      }
      Serial.println();
      break;

    case DEC:
      Serial.print("Raw Packet (Decimal):   ");
      for (int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
        Serial.print(gamepad_status.packet_buffer[i]);
        Serial.print(" ");
      }
      Serial.println();
      break;

    case HEX:
      Serial.print("Raw Packet (HEX):       ");
      for (int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
          Serial.print(gamepad_status.packet_buffer[i], HEX);
          Serial.print(" ");
      }
      Serial.println();
      break;

    default:
      Serial.println("Logitech_F710_Parser::print_raw_packet() received invalid parameter. (Use 'BIN', 'DEC' or 'HEX'");
      break;
  }
}


// Copies the contents of the first Gamepad_Status_Data struct into the second Gamepad_Status_Data struct
Gamepad_Status_Data Logitech_F710_Parser::copy_packet_struct(Gamepad_Status_Data original_packet){
  Gamepad_Status_Data copy_packet;
  for(int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
    copy_packet.packet_buffer[i] = original_packet.packet_buffer[i];
  }
  copy_packet.hid_report_id         = original_packet.hid_report_id;
  copy_packet.left_joystick_x       = original_packet.left_joystick_x; 
  copy_packet.left_joystick_y       = original_packet.left_joystick_y;
  copy_packet.right_joystick_x      = original_packet.right_joystick_x;
  copy_packet.right_joystick_y      = original_packet.right_joystick_y;
  copy_packet.d_pad                 = original_packet.d_pad;
  copy_packet.x_button              = original_packet.x_button;
  copy_packet.a_button              = original_packet.a_button;
  copy_packet.b_button              = original_packet.b_button;
  copy_packet.y_button              = original_packet.y_button;
  copy_packet.left_bumper           = original_packet.left_bumper;
  copy_packet.right_bumper          = original_packet.right_bumper;
  copy_packet.left_trigger          = original_packet.left_trigger;
  copy_packet.right_trigger         = original_packet.right_trigger;
  copy_packet.back_button           = original_packet.back_button;
  copy_packet.start_button          = original_packet.start_button;
  copy_packet.left_joystick_button  = original_packet.left_joystick_button;
  copy_packet.right_joystick_button = original_packet.right_joystick_button;
  copy_packet.mode_toggle           = original_packet.mode_toggle;
  copy_packet.vibrate_toggle        = original_packet.vibrate_toggle;
  return copy_packet;
}


/* Compares two Gamepad_Status_Data structs for equivalence. Useful for determining if the latest parsed packet contains new data relative to a previously parsed packet
    - Returns 0 if the packets are equivalent, 1 if not
*/
int Logitech_F710_Parser::compare_packet_structs(Gamepad_Status_Data packet_1, Gamepad_Status_Data packet_2){
  int compare_result = 0;
  for(int i = 0; i < LOGITECH_F710_PACKET_LENGTH; i++){
    if(packet_1.packet_buffer[i] != packet_2.packet_buffer[i]){
      compare_result = 1;
    } 
  }
  return compare_result;
}

