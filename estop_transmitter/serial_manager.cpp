#include "serial_manager.h"

using namespace estop;

serial_manager::serial_manager(estop::xbee* xb)
{
  // Initialize operating mode.
  serial_manager::current_mode = serial_manager::operating_mode::normal;

  // Create a new communicator.
  serial_manager::communicator = new SC::Communicator(Serial);

  // Store reference to the XBee class.
  serial_manager::xbee = xb;

  // Initialize flags.
  serial_manager::f_team_updated = false;
}

void serial_manager::spin_once()
{
  // Check the current operating mode.
  switch(serial_manager::current_mode)
  {
    case serial_manager::operating_mode::normal:
    {
      // Spin the communicator once.
      serial_manager::communicator->Spin();
      // Handle any received messages.
      serial_manager::handle_messages();
      break;
    }
    case serial_manager::operating_mode::forwarding:
    {
      // Forward bytes bidirectionally between USB and XBee serial ports.
      serial_manager::forward_data(Serial, Serial1);
      serial_manager::forward_data(Serial1, Serial);
      break;
    }
  }
}

bool serial_manager::is_forwarding()
{
  return serial_manager::current_mode == serial_manager::operating_mode::forwarding;
}
bool serial_manager::team_updated()
{
  // Store the current value of the flag.
  bool output = serial_manager::f_team_updated;

  // Reset flag to false to clear it upon read.
  serial_manager::f_team_updated = false;

  // Return the original state of the flag.
  return output;
}

void serial_manager::handle_messages()
{
  // Handle all messages currently in the RX queue.
  for(uint16_t i = 0; i < serial_manager::communicator->MessagesAvailable(); i++)
  {
    // Recieve message from RX queue.
    const SC::Message* message = serial_manager::communicator->Receive();

    // Handle message based on ID.
    if(message)
    {
      switch(static_cast<serial_manager::message_id>(message->pID()))
      {
        case serial_manager::message_id::set_team:
        {
          serial_manager::handle_set_team(message);
          break;
        }
        case serial_manager::message_id::set_forwarding_mode:
        {
          serial_manager::handle_set_forwarding_mode();
          break;
        }
        default:
        {
          // Do nothing.
        }
      }
    }

    // Clean up the message.
    delete message;
  }
}
void serial_manager::handle_set_team(const SC::Message* message)
{
  // Parse the message data, whose structure is team_length, key_length, team_string, key_string.
  uint16_t address = 0;
  // Read the length bytes for each string.
  uint16_t tn_length = message->GetData<uint8_t>(address++);
  uint16_t ek_length = message->GetData<uint8_t>(address++);
  // Create new strings for the team_name and encryption_key.
  char* team_name = new char[tn_length];
  char* encryption_key = new char[ek_length];
  // Read each string.
  for(uint8_t i = 0; i < tn_length; i++)
  {
    team_name[i] = message->GetData<char>(address++);
  }
  for(uint8_t i = 0; i < ek_length; i++)
  {
    encryption_key[i] = message->GetData<char>(address++);
  }

  // Update the encryption key first.
  if(serial_manager::xbee->set_encryption_key(encryption_key, ek_length))
  {
    // If encryption key was updated successfully, update the team name.
    if(serial_manager::xbee->set_team_name(team_name, tn_length))
    {
      // Both encryption key and team name have been updated.
      // Save settings to the XBee memory.
      serial_manager::xbee->save_configuration();
      
      // Mark flag.
      serial_manager::f_team_updated = true;
    }
  }

  // Clean up strings.
  delete [] team_name;
  delete [] encryption_key;
}
void serial_manager::handle_set_forwarding_mode()
{
  // Update forwarding mode.
  serial_manager::current_mode = serial_manager::operating_mode::forwarding;

  // Spin the communicator a few more times to ensure that acknowledgments get returned.
  for(uint16_t i = 0; i < 5; i++)
  {
    serial_manager::communicator->Spin();
  }
}

void serial_manager::forward_data(Stream& from, Stream& to)
{
  // Check bytes available at FROM port.
  uint16_t n_bytes = from.available();
  // Create buffer for storing data being forwarded.
  uint8_t* data = new uint8_t[n_bytes];
  // Read data from FROM port into buffer.
  from.readBytes(data, n_bytes);
  // Send data to TO port.
  to.write(data, n_bytes);
  // Clean up data buffer.
  delete [] data;
}
