#include <i3dfmu.h>
#include <uart_comm_ovidius.h>
#include <SoftwareSerial.h>

SoftwareSerial i3dfmu2iddh_serial(I3DFMU_RxPin, I3DFMU_TxPin);

uart_comm_ns::uart_comm_ovidius AGENT;
i3dfmu_ns::i3dfmu UNIT;

void setup() {
  Serial.begin(USB_BAUD1); // Opem port to PC for real-time data plotting
  i3dfmu2iddh_serial.begin(USB_BAUD1); // OPen port for comm with IDDH (Intermediate data handler for comm with robot Master)
  
  UNIT.setup_unit(); // setup the 3d force sensor(establish pins for I/O, set calibration factors)
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
  if (i3dfmu2iddh_serial.available()) {
    UNIT.cmd_received = i3dfmu2iddh_serial.read();
  }
  

}
