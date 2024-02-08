/// \file serial_manager.h
/// \brief Defines the serial_manager class.
#ifndef serial_manager_h
#define serial_manager_h

#include <Arduino.h>              // Include Arduino.h to enroll class h/cpp file in compilation.
#include <SerialCommunicator.h>   // Adds serial communication functionality.

#include "xbee.h"

namespace estop {

/// \brief serial_manager A class for managing serial communications onboard the transmitter.
class serial_manager
{
public:
  /// \brief serial_manager Creates a new serial_manager instance and initializes it.
  serial_manager(estop::xbee* xb);
  
  /// \brief spin_once Performs a single iteration of the serial_manager's regular duties.
  void spin_once();

  bool is_forwarding();
  bool team_updated();
  
private:  
  enum class operating_mode
  {
    normal = 0,
    forwarding = 1
  };
  enum class message_id
  {
    set_team = 0x1001,
    set_forwarding_mode = 0x1002
  };
  
  SC::Communicator* communicator;
  estop::xbee* xbee;

  operating_mode current_mode;
  bool f_team_updated;

  /// \brief forward_data Forwards all current data from one serial port to another.
  /// \param from The serial port to forward data from.
  /// \param to The serial port to forward data to.
  void forward_data(Stream& from, Stream& to);

  void handle_messages();
  void handle_set_team(const SC::Message* message);  
  void handle_set_forwarding_mode();
};

}

#endif
