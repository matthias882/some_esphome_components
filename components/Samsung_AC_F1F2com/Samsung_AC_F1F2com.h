#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace Samsung_AC_F1F2com {

class Samsung_AC_F1F2comComponent : public PollingComponent, public uart::UARTDevice {
 public:
  void set_room_temp_sensor_1(sensor::Sensor *room_temp_sensor_1) { room_temp_sensor_1_ = room_temp_sensor_1; }
 
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
 
 protected:
  boolean check_data_() const;
  void parse_data_();
 
 sensor::Sensor *room_temp_sensor_1_{nullptr};
 
  std::vector<uint8_t> data_;
  bool receiving_{false};
  uint8_t data_count_;
  uint32_t last_transmission_{0};
};

}  // namespace Samsung_AC_F1F2com
}  // namespace esphome
