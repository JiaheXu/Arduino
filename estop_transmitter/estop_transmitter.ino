#include "battery_monitor.h"
#include "xbee.h"
#include "estop_controller.h"
#include "serial_manager.h"
#include "oled_display.h"

estop::battery_monitor* battery_monitor;
estop::xbee* xbee;
estop::estop_controller* estop_controller;
estop::serial_manager* serial_manager;
estop::oled_display* oled;

void setup()
{
  // Open serial ports and set timeouts.
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial1.begin(9600);
  Serial1.setTimeout(50);

  // Initialize components.  
  battery_monitor = new estop::battery_monitor();
  xbee = new estop::xbee();
  estop_controller = new estop::estop_controller(battery_monitor, xbee);
  serial_manager = new estop::serial_manager(xbee);
  oled = new estop::oled_display(battery_monitor, xbee, estop_controller, serial_manager);

  // Display loading splash, and wait for XBee to come online.
  oled->splash();
  delay(500);
  // Probe XBee for device/team names and display.
  oled->update_names();
}

void loop()
{
  // Spin components.
  estop_controller->spin_once();
  serial_manager->spin_once();
  oled->spin_once();
  
  // Sleep for 25ms before looping.
  delay(25);
}
