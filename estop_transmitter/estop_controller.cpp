#include "estop_controller.h"

using namespace estop;

estop_controller::estop_controller(estop::battery_monitor* bm, estop::xbee* xb)
{
  // Store pointers to battery monitor and xbee.
  estop_controller::battery_monitor = bm;
  estop_controller::xbee = xb;
  
  // Set up the switch input pin.
  pinMode(estop_controller::switch_pin, INPUT);

  // Initialize the e-stop state.
  estop_controller::estop_state = false;
  // Initialize state action timestamp to zero to force immediate state action.
  estop_controller::state_action_timestamp = 0;
}

void estop_controller::spin_once()
{
  // Check for a state change.
  bool new_state = digitalRead(estop_controller::switch_pin) == HIGH;
  if(new_state && !estop_controller::estop_state)
  {
    // State transition from false to true.
    // Check if battery monitor indicates good power before transitioning.
    if(estop_controller::battery_monitor->power_good())
    {
      // Power is good.  Transition to estop state.
      estop_controller::update_state(new_state);
    }
  }
  else if(!new_state && estop_controller::estop_state)
  {
    // State transition from true to false.
    estop_controller::update_state(new_state);
  }
  // Else no change in state.

  // Perform state activities.
  if(estop_controller::estop_state)
  {
    // Currently in e-stop state.  Broadcast e-stops at specified rate.
    // Check if enough time has elapsed since last state action.
    if(estop_controller::estop_broadcast_rate * 1000 < (millis() - estop_controller::state_action_timestamp))
    {
      // Enough time has passed since last state action to meet specified action rate.
      // Update last action timestamp first, since it will take time to execute the broadcast.    
      estop_controller::state_action_timestamp = millis();
      // Perform state action.
      estop_controller::xbee->broadcast_estop();
      //estop_controller::xbee->broadcast_test_packet();
    }
  }
}

bool estop_controller::current_estop_state()
{
  return estop_controller::estop_state;
}

void estop_controller::update_state(bool new_estop_state)
{
  // Check if state needs to be updated.
  if(new_estop_state != estop_controller::estop_state)
  {
    // Update internal state.
    estop_controller::estop_state = new_estop_state;

    // Reset last action timestamp.
    estop_controller::state_action_timestamp = 0;
  }
}
