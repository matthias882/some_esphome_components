#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace Samsung_AC_F1F2com {

class Samsung_AC_F1F2com : public PollingComponent, public uart::UARTDevice {
 public:
  void set_room_temp_sensor(sensor::Sensor *room_temp_sensor) { room_temp_sensor_ = room_temp_sensor; }
 
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
 
 protected:
  boolean check_data_() const;
  void parse_data_();
 
 sensor::Sensor *room_temp_sensor_{nullptr};
 
  std::vector<uint8_t> data_;
  bool receiving_{false};
  uint8_t data_count_;
  uint32_t last_transmission_{0};
};

}  // namespace Samsung_AC_F1F2com
}  // namespace esphome
