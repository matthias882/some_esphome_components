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
      if (c != 0xaa)    //Wenn nicht End-Marker empfangen (0x34) dann weiter empfangen
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

void Samsung_AC_F1F2com::dump_config(){
    ESP_LOGCONFIG(TAG, "Samsung_AC_F1F2com:");
}

}  // namespace Samsung_AC_F1F2com
}  // namespace esphome
