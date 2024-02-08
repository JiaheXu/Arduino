/// \file battery_monitor.h
/// \brief Defines the battery_monitor class.
#ifndef battery_monitor_h
#define battery_monitor_h

#include <Arduino.h>    // Include Arduino.h to enroll class h/cpp file in compilation.

namespace estop {

/// \brief battery_monitor A class for monitoring battery status.
class battery_monitor
{
public:
  // CONSTRUCTORS
  /// \brief battery_monitor Creates a new instance of the battery monitor.
  battery_monitor();

  // BATTERY LEVELS
  /// \brief current_voltage Reads the current voltage of the battery.
  /// \returns The current battery voltage level, in volts.
  float current_voltage();
  /// \brief current_percentage Reads the current battery level as a percentage.
  /// \returns The current battery percentage level.
  float current_percentage();
  /// \brief filtered_percentage Gets a filtered version of the current battery level as a percentage.
  /// \returns The current battery percentage level, from 0 to 100.
  uint8_t filtered_percentage();

  // STATUS
  /// \brief power_good Indicates if battery power is in a usable state.
  /// \returns TRUE if the battery is in a usable state, otherwise FALSE> 
  bool power_good();
  
private:
  // CONST PARAMETERS
  /// \brief adc_pin Defines the analog input pin that the battery is connected to on the itsybitsy.
  /// \details The e-stop schematic indicates that the battery voltage divider circuit is connected to pin A3.
  const uint16_t adc_pin = A3;
  /// \brief max_voltage Defines the voltage level that is considered to be fully charged.
  const float max_voltage = 4.15;
  /// \brief min_voltage Defines the voltage level that is considered to be fully discharged.
  const float min_voltage = 3.3;

  // VARIABLES
  /// \brief m_percentage_window A window of the last 5 percentages that have been measured.
  uint8_t m_percentage_window[5];
  /// \brief m_filtered_percentage Stores the most recent filtered percentage.
  uint8_t m_filtered_percentage;
};

}

#endif
