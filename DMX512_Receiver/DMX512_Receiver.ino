//
// #################################################
//
//    HTLAB.NET M5Stack DMX512 Receiver
//      https://htlab.net/electronics/
//
//    Copyright (C) 2022
//      Hideto Kikuchi / PJ (@pcjpnet) - http://pc-jp.net/
//      Tsukuba Science Inc. - http://www.tsukuba-kagaku.co.jp/
//
// #################################################
//

//
// ########## Compatible Boards ##########
//    - M5Stack
//    - M5Stack Core2
//

//
// ########## Require Libraries ##########
//    - esp_dmx 1.1.3
//    - M5Unified 0.0.7
//    - M5GFX 0.0.20
//



#include <M5Unified.h>
#include <esp_dmx.h>

dmx_port_t dmxPort = 2;
byte data[DMX_MAX_PACKET_SIZE];
QueueHandle_t queue;



void setup() {
  M5.begin();
  M5.Display.setBrightness(128);
  M5.Display.setTextSize(4);
  M5.Display.startWrite();
  M5.Display.print("DMX Receiver");
  M5.Display.setCursor(0, 50);
  M5.Display.print(" Waiting..");
  M5.Display.endWrite();

  // DMX
  dmx_config_t dmxConfig = DMX_DEFAULT_CONFIG;
  dmx_param_config(dmxPort, &dmxConfig);
  switch (M5.getBoard()) {
    case m5::board_t::board_M5Stack:
        // HIGH = Receive
       pinMode(12, OUTPUT);
       digitalWrite(12, HIGH);
       dmx_set_pin(dmxPort, 15, 13, -1);
      break;
    case m5::board_t::board_M5StackCore2:
        // HIGH = Receive
       pinMode(27, OUTPUT);
       digitalWrite(27, HIGH);
       dmx_set_pin(dmxPort, 2, 19, -1);
      break;
  }
  int queueSize = 1;
  int interruptPriority = 1;
  dmx_driver_install(dmxPort, DMX_MAX_PACKET_SIZE, queueSize, &queue, interruptPriority);

}



void dispay_dmx_value_hex(uint8_t value) {
  if (value < 0x10) {
    M5.Display.print("0x0");
  } else {
    M5.Display.print("0x");
  }
  M5.Display.println(value, HEX);
}



void loop() {
  vTaskDelay(1);
  M5.update();

  dmx_event_t packet;
  if (xQueueReceive(queue, &packet, DMX_RX_PACKET_TOUT_TICK)) {
    if (packet.status == DMX_OK) {
      dmx_read_packet(dmxPort, data, packet.size);

      M5.Display.startWrite();
      M5.Display.setCursor(0, 50);
      M5.Display.print("  Ch1:");
      dispay_dmx_value_hex(data[1]);
      M5.Display.print("  Ch2:");
      dispay_dmx_value_hex(data[2]);
      M5.Display.print("  Ch3:");
      dispay_dmx_value_hex(data[3]);
      M5.Display.print("  Ch4:");
      dispay_dmx_value_hex(data[4]);
      M5.Display.print("  Ch5:");
      dispay_dmx_value_hex(data[5]);
      M5.Display.print("  Ch6:");
      dispay_dmx_value_hex(data[6]);
      M5.Display.endWrite();

    }
  }

}
