/// \file oled_display.h
/// \brief Defines the oled_display class.
#ifndef oled_display_h
#define oled_display_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "battery_monitor.h"
#include "xbee.h"
#include "estop_controller.h"
#include "serial_manager.h"

namespace estop {

/// \brief oled_display A class for managing the e-stop transmitter's 128x32 OLED display.
class oled_display
{
public:
  // CONSTRUCTORS
  /// \brief oled_display Initializes a new instance of the oled_display class.
  /// \param battery_monitor A pointer to the e-stop's battery_monitor instance.
  /// \param xbee A pointer to the e-stop's xbee instance.
  /// \param estop_controller A pointer to the e-stop's estop_controller instance.
  /// \param serial_manager A pointer to the e-stop's serial_manager instance.
  oled_display(estop::battery_monitor* battery_monitor, estop::xbee* xbee, estop::estop_controller* estop_controller, estop::serial_manager* serial_manager);

  // METHODS
  /// \brief update_names Probes the XBee for the currently set device and team names, and updates the display accordingly.
  void update_names();
  /// \brief spin_once Performs a single iteration of the oled_display's duties.
  /// \details This essentially redraws the latest information onto the display.
  void spin_once();
  /// \brief splash Displays a "LOADING..." splash screen on the display.
  void splash();
  
private:
  // VARIABLES - INTERNAL CONTROL
  /// \brief m_display The member for internal control of the display through I2C.
  Adafruit_SSD1306 m_display;
  
  // VARIABLES - ESTOP CLASS INSTANCES
  /// \brief m_battery_monitor A pointer to the e-stop's battery_monitor instance.
  estop::battery_monitor* m_battery_monitor;
  /// \brief m_xbee A pointer to the e-stop's xbee instance.
  estop::xbee* m_xbee;
  /// \brief m_estop_controller A pointer to the e-stop's estop_controller instance.
  estop::estop_controller* m_estop_controller;
  /// \brief m_serial_manager A pointer to the e-stop's serial_manager instance.
  estop::serial_manager* m_serial_manager;
  
  // VARIABLES - INTERNAL MEMBERS
  /// \brief m_device_name Stores the most currently grabbed device name from the XBee.
  char* m_device_name;
  /// \brief m_team_name Stores the most currently grabbed team name from the XBee.
  char* m_team_name;
  /// \brief m_dn_length Stores the length of the current m_device_name.
  uint16_t m_dn_length;
  /// \brief m_tn_length Stores the length of the current m_team_name.
  uint16_t m_tn_length;

  // METHODS - DRAWING
  /// \brief draw_text Draws a c-style array of text to an image for display.
  /// \param x The x coordinate to draw the text at.
  /// \param y The y coordinate to draw the text at.
  /// \param text The c-style array of characters to draw.
  /// \param text_length The length of the text array, in characters.
  /// \param scale OPTIONAL The scale to draw the text at.  Default = 1.
  void draw_text(uint16_t x, uint16_t y, const char* text, uint16_t text_length, uint16_t scale = 1);
  /// \brief draw_text Draws a string of text to an image for display.
  /// \param x The x coordinate to draw the text at.
  /// \param y The y coordinate to draw the text at.
  /// \param text The string of characters to draw.
  /// \param scale OPTIONAL The scale to draw the text at.  Default = 1.
  void draw_text(uint16_t x, uint16_t y, String text, uint16_t scale = 1);
  /// \brief redraw Updates the display with the most recent information.
  void redraw();
};

}

#endif
