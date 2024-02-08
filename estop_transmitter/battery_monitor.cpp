#include "battery_monitor.h"

using namespace estop;

// CONSTRUCTORS
battery_monitor::battery_monitor()
{
  // Initialize the filtered percentage value.
  // Grab the raw percentage as an integer.
  uint8_t current_percentage = static_cast<uint8_t>(round(battery_monitor::current_percentage()));
  // Initialize the filter window.
  for(uint8_t i = 0; i < 5; i++)
  {
    battery_monitor::m_percentage_window[i] = current_percentage;
  }
  // Initialize the current filtered value.
  battery_monitor::m_filtered_percentage = current_percentage;
}

// BATTERY LEVELS
float battery_monitor::current_voltage()
{
  // Read analog input pin and convert to voltage.
  // Tests indicated scaling factor of 0.0042v/div.
  return static_cast<float>(analogRead(battery_monitor::adc_pin)) * 0.0042;
}
float battery_monitor::current_percentage()
{
  // Calculate a truncated percentage from 0 to 100, using formula % = (current - min)/(max-min)
  return max(0, min(100, 100.0 * (battery_monitor::current_voltage() - battery_monitor::min_voltage)/(battery_monitor::max_voltage - battery_monitor::min_voltage)));
}
uint8_t battery_monitor::filtered_percentage()
{
  // Perform filtering.
  // Grab an updated value of the raw percentage.
  uint8_t current_percentage = static_cast<uint8_t>(round(battery_monitor::current_percentage()));
  // Insert into the ring buffer.
  // First left shift all elements.
  // Also check if all elements are equal to the new percentage to see if the filtered value should be updated.
  bool update_value = true;
  for(uint8_t i = 0; i < 4; i++)
  {
    battery_monitor::m_percentage_window[i] = battery_monitor::m_percentage_window[i+1];
    update_value &= (battery_monitor::m_percentage_window[i] == current_percentage);
  }
  // Update last element.
  battery_monitor::m_percentage_window[4] = current_percentage;

  // Check for new assigned value.
  if(update_value)
  {
    battery_monitor::m_filtered_percentage = current_percentage;
  }

  return battery_monitor::m_filtered_percentage;
}

// STATUS
bool battery_monitor::power_good()
{
  // Get a debounced comparison of the current voltage to the min voltage.

  // Determine if consecutive reads show that current voltage < min voltage.
  bool output = true;
  for(uint8_t i = 0; i < 5; i++)
  {
    output &= (battery_monitor::current_voltage() < battery_monitor::min_voltage);
  }

  // Return inverse to show that power is good.
  // This ensures that consecutive reads must all be below min voltage before this returns false.
  return !output;
}
