/// \file estop_controller.h
/// \brief Defines the estop_controller class.
#ifndef estop_controller_h
#define estop_controller_h

#include <Arduino.h>    // Include Arduino.h to enroll class h/cpp file in compilation.

#include "battery_monitor.h"
#include "xbee.h"

namespace estop {

/// \brief estop_controller A class for controlling emergency stop functionality.
class estop_controller
{
public:
  // CONSTRUCTORS
  /// \brief estop_controller Instantiates and initializes a new estop_controller instance.
  /// \param bm A pointer to the EStop Transmitter's battery_monitor instance.
  /// \param xb A pointer to the EStop Transmitter's xbee instance.
  estop_controller(estop::battery_monitor* bm, estop::xbee* xb);

  // METHODS
  /// \brief spin_once Performs a single iteration of the estop_controller's duties.
  /// \details This monitors for state changes and performs state actions accordingly.
  void spin_once();
  /// \brief current_estop_state Gets the current E-Stop state of the controller.
  /// \returns TRUE if in an E-Stop state, otherwise FALSE.
  bool current_estop_state();
  
private:
  // COMPONENT POINTERS
  /// \brief battery_monitor A pointer to the EStop Transmitter's battery_monitor instance.
  battery_monitor* battery_monitor;
  /// \brief xbee A pointer to the EStop Transmitter's xbee instance.
  xbee* xbee;

  // CONFIGURATION VARIABLES
  /// \brief switch_pin The digital input pin that the toggle switch is connected to.
  const uint16_t switch_pin = 9;
  /// \brief estop_broadcast_rate The rate, in Hz, at which E-Stop commands will be broadcasted during an E-Stop state.
  const float estop_broadcast_rate = 1;

  // VARIABLES
  /// \brief estop_state The current E-Stop state.  TRUE indicates emergency stop mode, otherwise FALSE.
  bool estop_state;
  /// \brief state_action_timestamp The last timestamp, in milliseconds, at which a state action was executed.
  /// \note This is reset back to zero during each state transition.
  uint32_t state_action_timestamp;

  // METHODS
  /// \brief update_state A helper method for updating the current e-stop state and setting the state_action_timestamp.
  /// \param new_estop_state The new state to update to.
  void update_state(bool new_estop_state);
};

}

#endif
