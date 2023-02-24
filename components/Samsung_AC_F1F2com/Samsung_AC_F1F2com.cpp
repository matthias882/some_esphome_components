/* 
One Datapackage consists of:
<start> <src> <dst> <cmd> <data: 8 bytes> <chksum> <end>

With:
Byte   Identifier   Comments
----------------------------
1      Start  : start of message (0x32)
2      Src    : Source address
3      Dst    : Destination address
4      Cmd    : Command byte
5-12   Data   : Data is always 8 bytes in length, unused bytes will be zero
13     Chksum : Checksum of message which is the XOR of bytes 2-12
14     End    : end of message (0x34)

*/

#include "esphome/core/log.h"
#include "Samsung_AC_F1F2com.h"

namespace esphome {
namespace Samsung_AC_F1F2com {

static const char *TAG = "Samsung_AC_F1F2com.sensor";

void Samsung_AC_F1F2com::setup() {

}

void Samsung_AC_F1F2com::update() {

}

void Samsung_AC_F1F2com::loop() {
  const uint32_t now = millis();
  if (receiving_ && (now - last_transmission_ >= 500)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago. Reset RX index.");
    data_.clear();
    receiving_ = false;
  }
    
  if (!available())
    // nothing in uart-input-buffer, end here
    return;
  
  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (!receiving_) {
      if (c != 0x34)    //Wenn nicht End-Marker empfangen (0x34) dann weiter empfangen
        continue;
      receiving_ = true;
      continue;
    }
    data_.push_back(c);
    if (data_.size == 14) {
      if (check_data_())
        parse_data_();
      data_.clear();
      receiving_ = false;
    }
  }
}

bool ESmart3Component::check_data_() const {
    if (data_[0] != 0x32) {
        ESP_LOGW(TAG, "unexpected start byte (not 0x32): %d", data_[0]);
        return false;
    }
    //crc berechnen: xor data_[1] bis data_[11]
    int i;
    int crc = data_[1];
    for (i = 1; i <= 11; i++) {
        crc = crc ^ data[i];
    }
    bool result = false;
    if (crc == data_[12]) crc = true;
    if (!result)
        ESP_LOGW(TAG, "data checksum failed");
    return result;
}
  
void ESmart3Component::parse_data_() {
  ESP_LOGD(TAG, "Raw: %02X %02x %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
           data_[0], data_[1], data_[2], data_[3], data_[4], data_[5], data_[6], data_[7], data_[8], data_[9], data_[10], data_[11], data_[12], data_[13], data_[14], data_[15]);
}

void Samsung_AC_F1F2com::dump_config(){
  ESP_LOGCONFIG(TAG, "Samsung_AC_F1F2com:");
  this->check_uart_settings(2400, 1, UART_CONFIG_PARITY_EVEN, 8);
}

}  // namespace Samsung_AC_F1F2com
}  // namespace esphome
