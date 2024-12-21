#if !defined(__HIDGAMEPADRPTPARSER_H__)
#define __HIDGAMEPADRPTPARSER_H__

#include <usbhid.h>

// Logitech F710 Gamepad sends 8-byte packets via USB
#define LOGITECH_F710_PACKET_LENGTH	8


// Input data mapping struct that is populated by the Parse() function, containing the parsed packet data mapped to each button, as well as a copy of the raw packet buffer
struct Gamepad_Status_Data{
  // Copy of the raw USB packet buffer content as an array of bytes
  uint8_t packet_buffer[LOGITECH_F710_PACKET_LENGTH];
  // Mapped gamepad input status fields
  uint8_t hid_report_id;
  uint8_t left_joystick_x;
  uint8_t left_joystick_y;
  uint8_t right_joystick_x;
  uint8_t right_joystick_y;
  uint8_t d_pad;
  uint8_t x_button;
  uint8_t a_button;
  uint8_t b_button;
  uint8_t y_button;
  uint8_t left_bumper;
  uint8_t right_bumper;
  uint8_t left_trigger;
  uint8_t right_trigger;
  uint8_t back_button;
  uint8_t start_button;
  uint8_t left_joystick_button;
  uint8_t right_joystick_button;
  uint8_t mode_toggle;
  uint8_t vibrate_toggle;
};


// D_Pad input value map (F710 uses a 4-bit number to represent each of 8 possible states instead of a bitmask to avoid UP + DOWN or LEFT + RIGHT states)
struct D_Pad_Values{
  const uint8_t d_pad_up          = 0;
  const uint8_t d_pad_up_right    = 1;
  const uint8_t d_pad_right       = 2; 
  const uint8_t d_pad_down_right  = 3;
  const uint8_t d_pad_down        = 4;
  const uint8_t d_pad_down_left   = 5;
  const uint8_t d_pad_left        = 6;
  const uint8_t d_pad_up_left     = 7;
  const uint8_t d_pad_neutral     = 8;
};
extern const D_Pad_Values d_Pad_Values;


class Logitech_F710_Parser : public HIDReportParser{

  public:
    Logitech_F710_Parser();

    // Input data mapping struct that is populated by the Parse() function
    Gamepad_Status_Data gamepad_status;

    /* USB packet parsing function for the Logitech F710 that populates the gamepad_status struct
        - This function is called when an instance of Logitech_F710_Parse is passed to the SetReportParser() function of the USB HID library
        - Polymorphism/Override of Parse() function in HIDReportParser 
    */
    virtual void Logitech_F710_Parser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);

    /* Useful debugging function for printing raw USB packets to the Serial Monitor 
       - Prints the contents of the packet_buffer from the gamepad_status struct
       - "Print_mode" can be set to binary (BIN), decimal (DEC) or hexidecimal (HEX). Defaults to binary.
    */
    void Logitech_F710_Parser::print_raw_packet(int print_mode = BIN);

    // Copies the contents of the first Gamepad_Status_Data struct into the second Gamepad_Status_Data struct
    Gamepad_Status_Data Logitech_F710_Parser::copy_packet_struct(Gamepad_Status_Data original_packet);

    /* Compares two Gamepad_Status_Data structs' packet buffers for equivalence. Useful for determining if the latest parsed packet contains new data relative to a previously parsed packet
        - Returns 0 if the packets are equivalent
        - Returns 1 if the packets are different
    */
    int Logitech_F710_Parser::compare_packet_structs(Gamepad_Status_Data packet_1, Gamepad_Status_Data packet_2);
};

#endif // __HIDGAMEPADRPTPARSER_H__
