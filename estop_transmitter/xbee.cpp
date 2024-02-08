#include "xbee.h"

using namespace estop;

// PUBLIC METHODS
bool xbee::set_node_identifier(const char* identifier, uint16_t length)
{
  // Create a data packet to set NI.
  uint16_t data_length = 4 + length;
  uint8_t* data = new uint8_t[data_length];

  // Write fields.
  data[0] = 0x08;               // Frame Type: 0x08 AT Command
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x4E;               // AT Command: NI = 0x4E49
  data[3] = 0x49;               // AT Command: NI = 0x4E49
  // Write identifier string.
  for(uint16_t i = 0; i < length; i++)
  {
    data[4+i] = identifier[i];
  }

  // Send the packet.
  xbee::send_message(data, data_length);
  
  // Get ACK from XBee.
  return xbee::at_ack('N', 'I');
}

bool xbee::get_node_identifier(char*& identifier, uint16_t& length)
{
  // Create data packet to query NI.
  uint16_t data_length = 4;
  uint8_t* data = new uint8_t[data_length];

  // Write fields.
  data[0] = 0x08;               // Frame Type: 0x08 AT Command
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x4E;               // AT Command: NI = 0x4E49
  data[3] = 0x49;               // AT Command: NI = 0x4E49

  // Send the query packet.
  xbee::send_message(data, data_length);

  // Get response from XBee.
  uint8_t* response = NULL;
  uint16_t response_length;
  if(xbee::read_response(0x88, response, response_length))
  {
    // Calculate string length.
    length = response_length - 9;
    // Copy the name bytes into the output buffer.
    identifier = new char[length];
    for(uint16_t i = 0; i < length; i++)
    {
      identifier[i] = response[8+i];
    }

    // Clean up response.
    delete [] response;
    
    return true;
  }
  else
  {
    // Did not get a response from the XBee.
    return false;
  }
}

bool xbee::set_team_name(const char* team_name, uint16_t length)
{
  // XBee node identifier will always be in the following format: device_name-team_name.
  // First, get the current node identifier to extract the device name.
  char* current_node_identifier = NULL;
  uint16_t current_node_identifier_length = 0;
  if(!xbee::get_node_identifier(current_node_identifier, current_node_identifier_length))
  {
    delete [] current_node_identifier;
    return false;
  }

  // Extract the device name.
  char* device_name = NULL;
  uint16_t device_name_length = 0;
  xbee::extract_device_name(current_node_identifier, current_node_identifier_length, device_name, device_name_length);

  // Create a new string for the new node identifier.
  // Calculate length of new node identifier
  uint16_t new_node_identifier_length = device_name_length + 1 + length;
  char* new_node_identifier = new char[new_node_identifier_length];
  
  // Populate the string.
  uint16_t i = 0;
  for(uint16_t j = 0; j < device_name_length; j++)
  {
    new_node_identifier[i++] = device_name[j];
  }
  new_node_identifier[i++] = '-';
  for(uint16_t j = 0; j < length; j++)
  {
    new_node_identifier[i++] = team_name[j];
  }

  // Set the new node identifier.
  bool success = xbee::set_node_identifier(new_node_identifier, new_node_identifier_length);

  // Delete strings.
  delete [] new_node_identifier;
  delete [] device_name;
  delete [] current_node_identifier;

  return success;
}

bool xbee::set_encryption_key(const char* encryption_key, uint16_t length)
{
  // Create a data packet to set NI.
  uint16_t data_length = 4 + length;
  uint8_t* data = new uint8_t[data_length];

  // Write fields.
  data[0] = 0x08;               // Frame Type: 0x08 AT Command
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x4B;               // AT Command: KY = 0x4B59
  data[3] = 0x59;               // AT Command: KY = 0x4B59
  // Write identifier string.
  for(uint16_t i = 0; i < length; i++)
  {
    data[4+i] = encryption_key[i];
  }

  // Send the packet.
  xbee::send_message(data, data_length);
  
  // Get command ACK from XBee.
  return xbee::at_ack('K', 'Y');
}

bool xbee::save_configuration()
{
  // Create a data packet to send WR command.
  uint16_t data_length = 4;
  uint8_t* data = new uint8_t[data_length];

  // Write fields.
  data[0] = 0x08;               // Frame Type: 0x08 AT Command
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x57;               // AT Command: WR = 0x5752
  data[3] = 0x52;               // AT Command: WR = 0x5752

  // Temporarily increase the XBee serial timeout, since writes can take time.
  uint32_t original_timeout = Serial1.getTimeout();
  Serial1.setTimeout(2000);

  // Send the packet.
  xbee::send_message(data, data_length);
  
  // Get ACK from XBee.
  bool ack = xbee::at_ack('W', 'R');

  // Restore original timeout.
  Serial1.setTimeout(original_timeout);

  // Return ack.
  return ack;
}

void xbee::broadcast_estop()
{
  // Create a data packet.
  uint8_t* data = new uint8_t[16];

  // Write fields.
  data[0] = 0x17;               // Frame Type: 0x17 Remote AT Command
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF (Broadcast)
  data[3] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[4] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[5] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[6] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[7] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[8] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[9] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[10] = 0xFF;              // 16b Address = 0xFF FE (Reserved)
  data[11] = 0xFE;              // 16b Address = 0xFF FE (Reserved)
  data[12] = 0x03;              // Remote CMD Options = 0b11 (Disable ACK, Apply Changes)
  data[13] = 0x44;              // AT Command: D1 = 0x4431
  data[14] = 0x31;              // AT Command: D1 = 0x4431
  data[15] = 0x05;              // Param Value: Digital Output (High) = 0x05

  // Send the data packet.
  xbee::send_message(data, 16);

  // Read out respnses just to keep RX buffer clear.
  // TODO: These responses can be used to figure out how many robots are responding.
  uint8_t* response = NULL;
  uint16_t response_length;
  // Read until timeout.
  while(xbee::read_response(0x97, response, response_length))
  {
    // Clean up any read response.
    delete [] response;
  }
}

void xbee::broadcast_test_packet()
{
  // Create a data packet.
  uint8_t* data = new uint8_t[20];

  data[0] = 0x00;               // Frame Type: 0x00 TX Request
  data[1] = 0x01;               // Frame ID: 1
  data[2] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF (Broadcast)
  data[3] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[4] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[5] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[6] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[7] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[8] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[9] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  data[10] = 0x01;              // Options = 0x01 (Disable ACK)
  data[11] = 't';
  data[12] = 'e';
  data[13] = 's';
  data[14] = 't';
  data[15] = '_';
  data[16] = 'd';
  data[17] = 'a';
  data[18] = 't';
  data[19] = 'a';

  // Send the data packet.
  xbee::send_message(data, 20);

  // Read out respnses just to keep RX buffer clear.
  // TODO: These responses can be used to figure out how many robots are responding.
  uint8_t* response = NULL;
  uint16_t response_length;
  // Read until timeout.
  while(xbee::read_response(0x97, response, response_length))
  {
    // Clean up any read response.
    delete [] response;
  }
}

void xbee::extract_device_name(const char* node_identifier, uint16_t ni_length, char*& device_name, uint16_t& dn_length)
{
  // Find the location of the hyphen.
  dn_length = 0;
  for(uint16_t i = 0; i < ni_length; i++)
  {
    if(node_identifier[i] == '-')
    {
      // Store location as length of the output string.
      dn_length = i;
      break;
    }
  }

  // Check if hyphen was found and device name is at least 1 character.
  if(dn_length > 0)
  {
    // Allocate output array.
    device_name = new char[dn_length];

    for(uint16_t i = 0; i < dn_length; i++)
    {
      device_name[i] = node_identifier[i];
    }
  }
}
void xbee::extract_team_name(const char* node_identifier, uint16_t ni_length, char*& team_name, uint16_t& tn_length)
{
  // Find the location of the hyphen.
  tn_length = 0;
  for(uint16_t i = 0; i < ni_length; i++)
  {
    if(node_identifier[i] == '-')
    {
      // Store location as length of the output string.
      tn_length = ni_length - i - 1;
      break;
    }
  }

  // Check if hyphen was found and team name is at least 1 character.
  if(tn_length > 0)
  {
    // Allocate output array.
    team_name = new char[tn_length];

    for(uint16_t i = 0; i < tn_length; i++)
    {
      team_name[i] = node_identifier[ni_length - tn_length + i];
    }
  }
}


// PRIVATE METHODS
void xbee::send_message(uint8_t* data, uint16_t length)
{
  // Calculate full frame length, which is data + 4 control bytes.
  uint32_t frame_length = length + 4;
  
  // Create a new buffer to store the full frame packet.
  uint8_t* frame = new uint8_t[frame_length];
  
  // Set frame header.
  frame[0] = 0x7E;
  // Set length bits.
  frame[1] = (length >> 8) & 0xFF;
  frame[2] = length & 0xFF;

  // Copy the data into the frame.
  for(uint16_t i = 0; i < length; i++)
  {
    frame[3+i] = data[i];
  }

  // Write the checksum.
  frame[frame_length-1] = xbee::checksum(frame, frame_length);

  // Write the frame to the XBee via serial.
  Serial1.write(frame, frame_length);

  // Clean up frame and data arrays.
  delete [] data;
  delete [] frame;
}

bool xbee::read_response(uint8_t frame_type, uint8_t*& frame, uint16_t &frame_length)
{
  // Continue searching until frame_type is found or timeout occurs.
  while(true)
  {
    // Read bytes until header is found or timeout.
    uint8_t read_byte = 0;
    while(read_byte != 0x7E)
    {
      if(Serial1.readBytes(&read_byte, 1) == 0)
      {
        return false;
      }
    }
  
    // If this point is reached, header has been found.
    // Read next two bytes to determine length.
    uint8_t length_bytes[2];
    if(Serial1.readBytes(length_bytes, 2) < 2)
    {
      return false;
    }
    frame_length = (static_cast<uint16_t>(length_bytes[0]) << 8) | static_cast<uint16_t>(length_bytes[1]);
    // Add 4 to total frame length to account for header, length, and checksum bytes.
    frame_length += 4;

    // Create a new frame array to receive the response.
    frame = new uint8_t[frame_length];
  
    // Read out the rest of the packet into an array.
    // Fill in header and length bytes.
    frame[0] = 0x7E;
    frame[1] = length_bytes[0];
    frame[2] = length_bytes[1];
    // Read rest from the serial port.
    Serial1.readBytes(&frame[3], frame_length - 3);
  
    // Validate the checksum and check if frame type matches.
    if(xbee::checksum(frame, frame_length) == frame[frame_length-1] && frame[3] == frame_type)
    {
      // Good checksum and frame type.
      return true;
    }
    else
    {
      // Clean up outputs and repeat iteration to continue search.
      delete [] frame;
      frame_length = 0;
    }
  }
}

uint8_t xbee::checksum(const uint8_t* message, uint32_t length)
{
  // Formula: Add all bytes (except for start delimiter and length), keep only the lowest 8 bits, and subtract of 0xFF.
  uint32_t sum = 0;
  for(uint32_t i = 3; i < length - 1; i++)
  {
    sum += message[i];
  }
  return (uint8_t)(255-(sum % 256));
}

bool xbee::at_ack(uint8_t at1, uint8_t at2)
{
  // Read response packet.
  uint8_t* response = NULL;
  uint16_t response_length;
  bool ack = false;
  // Read responses until correct AT command found or times out.
  while(xbee::read_response(0x88, response, response_length))
  {
    // Check if response packet AT matches desired AT.
    if(response[5] == at1 && response[6] == at2)
    {
      // Check that the response packet indicates an ack.
      ack = response[7] == 0x00;
      // Clean up final response.
      delete [] response;
      break;
    }
    else
    {
      // Clean up current response.
      delete [] response;
    }
  }

  return ack;
}
