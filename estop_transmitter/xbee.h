/// \file xbee.h
/// \brief Defines the xbee class.
#ifndef xbee_h
#define xbee_h

#include <Arduino.h>    // Include Arduino.h to enroll class h/cpp file in compilation.

namespace estop {

/// \brief xbee A class for interfacing with the E-Stop Transmitter's XBee module.
class xbee
{
public:
  // CONSTRUCTORS

  // METHODS
  /// \brief set_node_identifier Sets the Node Identifier name of the XBee.
  /// \param identifier The new Node Identifier name to set.
  /// \param length The length of the new Node Identifier name to set.
  /// \returns TRUE if the command succeeded, otherwise FALSE.
  bool set_node_identifier(const char* identifier, uint16_t length);
  /// \brief get_node_identifier Gets the Node Identifier name of the XBee.
  /// \param identifier An empty character array to store the received Node Identifier name in.
  /// \param length An empty variable to store the recieved Node Identifier name length in.
  /// \returns TRUE if the command succeeded, otherwise FALSE.
  bool get_node_identifier(char*& identifier, uint16_t& length);
  /// \brief set_team_name Sets the team name assigned to the XBee.
  /// \param team_name The new team_name to set.
  /// \param length The length of the new team name to set.
  /// \returns TRUE if the command succeeded, otherwise FALSE.
  bool set_team_name(const char* team_name, uint16_t length);
  /// \brief set_encryption_key Sets the encryption key of the XBee.
  /// \param identifier The new encryption key to set.
  /// \param length The length of the new encryption key to set.
  /// \returns TRUE if the command succeeded, otherwise FALSE.
  bool set_encryption_key(const char* encryption_key, uint16_t length);
  /// \brief save_configuration Issues an ATWR command to save the current XBee configuration to it's non-volatile memory.
  /// \returns TRUE if the command succeeded, otherwise FALSE.
  bool save_configuration();
  /// \brief broadcast_estop Sends a single broadcast e-stop command directly to the XBee.
  void broadcast_estop();
  /// \brief broadcast_test_packet Sends a single broadcast test packet directly to the XBee.
  void broadcast_test_packet();

  /// \brief extract_device_name Extracts the device name as a string from the XBee's node identifier string.
  /// \param node_identifier The XBee's node identifier, in the form of device_name-team_name.
  /// \param ni_length The length of the node identifier char array.
  /// \param device_name An empty character array to store the extracted device name in.
  /// \param dn_length An empty variable to store the extracted device name length in.
  void extract_device_name(const char* node_identifier, uint16_t ni_length, char*& device_name, uint16_t& dn_length);
  /// \brief extract_device_name Extracts the team name as a string from the XBee's node identifier string.
  /// \param node_identifier The XBee's node identifier, in the form of device_name-team_name.
  /// \param ni_length The length of the node identifier char array.
  /// \param team_name An empty character array to store the extracted team name in.
  /// \param tn_length An empty variable to store the extracted team name length in.
  void extract_team_name(const char* node_identifier, uint16_t ni_length, char*& team_name, uint16_t& tn_length);

private:
  /// \brief send_message Sends a new message to the XBee via serial.
  /// \param data The data to be sent. This method takes ownership of the pointer.
  /// \param length The length of the data in bytes.
  /// \details This method inserts the appropriate header, length, and checksum bytes.
  void send_message(uint8_t* data, uint16_t length);
  /// \brief read_response Reads a response message from the XBee via serial.
  /// \param frame_type The specific frame message type to receive.
  /// \param frame An empty byte array to store the received frame within.
  /// \param frame_length An empty variable to store the recieved frame length within.
  /// \returns TRUE if a response was received, otherwise FALSE.
  bool read_response(uint8_t frame_type, uint8_t*& frame, uint16_t &frame_length);
  /// \brief checksum Calculates the checksum of a message in frame packet form.
  /// \param message The frame packet to calculate the checksum for.
  /// \param length The length of the frame packet in bytes.
  /// \returns The checksum for the frame packet data.
  uint8_t checksum(const uint8_t* message, uint32_t length);
  /// \brief at_ack Listens for an ACK response from the XBee for a given AT command.
  /// \param at1 The first character of the AT Command being acknowledged.
  /// \param at2 The second character of the AT Command being acknowledged.
  /// \returns TRUE if the command was acknowledged by the XBee, otherwise FALSE.
  bool at_ack(uint8_t at1, uint8_t at2);
};

}

#endif
