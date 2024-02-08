#include "oled_display.h"

using namespace estop;

// CONSTRUCTORS
oled_display::oled_display(estop::battery_monitor* battery_monitor, estop::xbee* xbee, estop::estop_controller* estop_controller, estop::serial_manager* serial_manager)
    // Initialize the display member.
    : m_display(128, 32, &Wire, 7)
{
  // Store reference to battery monitor and xbee.
  oled_display::m_battery_monitor = battery_monitor;
  oled_display::m_xbee = xbee;
  oled_display::m_estop_controller = estop_controller;
  oled_display::m_serial_manager = serial_manager;
  
  // Set up the display, instructing it to use internal regulator and I2C address 0x3C.
  oled_display::m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear anything that may have been left on the display.
  oled_display::m_display.clearDisplay();

  // Rotate the display by 180 degrees to account for inverse mounting.
  oled_display::m_display.setRotation(2);

  // Set text color for SSD1306.
  oled_display::m_display.setTextColor(SSD1306_WHITE);

  // Set empty names for device/team.
  oled_display::m_device_name = NULL;
  oled_display::m_team_name = NULL;
  oled_display::m_dn_length = 0;
  oled_display::m_tn_length = 0;
}


// PUBLIC METHODS
void oled_display::update_names()
{
  // Clean up old names.
  if(oled_display::m_device_name)
  {
    delete [] oled_display::m_device_name;
    oled_display::m_dn_length = 0;
  }
  if(oled_display::m_team_name)
  {
    delete [] oled_display::m_team_name;
    oled_display::m_tn_length = 0;
  }
  
  // Get an updated set up team/device names from the Xbee.
  char* ni;
  uint16_t ni_length;
  if(oled_display::m_xbee->get_node_identifier(ni, ni_length))
  {    
    // Extract names into local variables.
    oled_display::m_xbee->extract_device_name(ni, ni_length, oled_display::m_device_name, oled_display::m_dn_length);
    oled_display::m_xbee->extract_team_name(ni, ni_length, oled_display::m_team_name, oled_display::m_tn_length);

    // Clean up ni string.
    delete [] ni;

    // Signal immediate display update.
    oled_display::redraw();
  }
}
void oled_display::spin_once()
{
  // Check if the team name has been updated.
  if(oled_display::m_serial_manager->team_updated())
  {
    oled_display::update_names();
  }
  
  // Redraw the display.
  oled_display::redraw();
}
void oled_display::splash()
{
  // Clear the display.
  oled_display::m_display.clearDisplay();

  // Display splash screen.
  oled_display::draw_text(0, 8, String("LOADING..."), 2);

  // Update drawn display.
  oled_display::m_display.display();
}


// PRIVATE METHODS
void oled_display::draw_text(uint16_t x, uint16_t y, const char* text, uint16_t text_length, uint16_t scale = 1)
{
  oled_display::m_display.setTextSize(scale);
  oled_display::m_display.setCursor(x,y);
  for(uint16_t i = 0; i < text_length; i++)
  {
    oled_display::m_display.print(text[i]);
  }
}
void oled_display::draw_text(uint16_t x, uint16_t y, String text, uint16_t scale = 1)
{
  oled_display::m_display.setTextSize(scale);
  oled_display::m_display.setCursor(x,y);
  oled_display::m_display.println(text);
}
void oled_display::redraw()
{ 
  // Clear the display.
  oled_display::m_display.clearDisplay();

  // Draw the device name in top left corner.
  oled_display::draw_text(0, 0, oled_display::m_device_name, oled_display::m_dn_length, 1);

  // Draw the battery percentage if the top right corner.
  String battery_percent = String(oled_display::m_battery_monitor->filtered_percentage());
  battery_percent.concat("%");
  oled_display::draw_text(100, 0, battery_percent, 1);

  // Draw 'F' if in forwarding mode.
  if(oled_display::m_serial_manager->is_forwarding())
  {
    oled_display::draw_text(80, 0, String("F"), 1);
  }

  // Draw 'E' if in e-stop broadcasting mode.
  if(oled_display::m_estop_controller->current_estop_state())
  {
    oled_display::draw_text(70, 0, String("E"), 1);
  }

  // Draw the team name on the bottom.
  oled_display::draw_text(0, 16, oled_display::m_team_name, oled_display::m_tn_length, 2);

  // Update drawn display.
  oled_display::m_display.display();
}
