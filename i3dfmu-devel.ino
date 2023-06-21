#include <i3dfmu.h>

uart_comm_ns::uart_comm_ovidius AGENT;
i3dfmu_ns::i3dfmu UNIT;

void setup() {
  Serial.begin(USB_BAUD1); // Opem port to PC for real-time data plotting
  Serial2.begin(IDDH_BAUD1); // OPen port for comm with IDDH (Intermediate data handler for comm with robot Master)
  
  UNIT.setup_unit(); // setup the 3d force sensor(establish pins for I/O, set calibration factors)

  // Unit waits until is pinged from master
  // ... next to ne coded ... 
  
  // Initialize timing vars for state-machie logic
  UNIT.last_usb_plot_ms    = 0;
  UNIT.last_data_sample_ms = 0;
}

void loop() {
  // Checks if it is time to sample data
  if (millis()-UNIT.last_data_sample_ms > Tsample_i3dfmu_ms) {
    UNIT.sample_data_kg();
    UNIT.last_data_sample_ms = millis();
  }

  // Checks if it is time to print data
  if (millis()-UNIT.last_usb_plot_ms > Tusb_i3dfmu_ms) {
    UNIT.print_data_usb(Serial);
    UNIT.last_usb_plot_ms = millis();
  }

  // Always checks if IDDH has sent a request
  if ( Serial2.available() ) {
    UNIT.execute_request(i3dfmu2iddh_serial);
  }

}
