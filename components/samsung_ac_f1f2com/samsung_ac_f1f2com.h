#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

namespace esphome {
namespace samsung_ac_f1f2com {

class Samsung_AC_F1F2comComponent : public PollingComponent, public uart::UARTDevice, public climate::Climate {
 public:
 Samsung_AC_F1F2comComponent() = default;

  // SENSORS
  void set_indoor1_room_temp_sensor(sensor::Sensor *indoor1_room_temp_sensor) { indoor1_room_temp_sensor_ = indoor1_room_temp_sensor; }
  void set_indoor1_set_temp_sensor(sensor::Sensor *indoor1_set_temp_sensor) { indoor1_set_temp_sensor_ = indoor1_set_temp_sensor; }
  void set_indoor1_pipe_in_temp_sensor(sensor::Sensor *indoor1_pipe_in_temp_sensor) { indoor1_pipe_in_temp_sensor_ = indoor1_pipe_in_temp_sensor; }
  void set_indoor1_pipe_out_temp_sensor(sensor::Sensor *indoor1_pipe_out_temp_sensor) { indoor1_pipe_out_temp_sensor_ = indoor1_pipe_out_temp_sensor; }
  void set_indoor1_fanspeed_sensor(sensor::Sensor *indoor1_fanspeed_sensor) { indoor1_fanspeed_sensor_ = indoor1_fanspeed_sensor; }
  void set_indoor1_mode_sensor(sensor::Sensor *indoor1_mode_sensor) { indoor1_mode_sensor_ = indoor1_mode_sensor; }
  void set_indoor1_capacity_sensor(sensor::Sensor *indoor1_capacity_sensor) { indoor1_capacity_sensor_ = indoor1_capacity_sensor; }
  void set_indoor1_delta_q_sensor(sensor::Sensor *indoor1_delta_q_sensor) { indoor1_delta_q_sensor_ = indoor1_delta_q_sensor; }

  void set_indoor2_room_temp_sensor(sensor::Sensor *indoor2_room_temp_sensor) { indoor2_room_temp_sensor_ = indoor2_room_temp_sensor; }
  void set_indoor2_set_temp_sensor(sensor::Sensor *indoor2_set_temp_sensor) { indoor2_set_temp_sensor_ = indoor2_set_temp_sensor; }
  void set_indoor2_pipe_in_temp_sensor(sensor::Sensor *indoor2_pipe_in_temp_sensor) { indoor2_pipe_in_temp_sensor_ = indoor2_pipe_in_temp_sensor; }
  void set_indoor2_pipe_out_temp_sensor(sensor::Sensor *indoor2_pipe_out_temp_sensor) { indoor2_pipe_out_temp_sensor_ = indoor2_pipe_out_temp_sensor; }
  void set_indoor2_fanspeed_sensor(sensor::Sensor *indoor2_fanspeed_sensor) { indoor2_fanspeed_sensor_ = indoor2_fanspeed_sensor; }
  void set_indoor2_mode_sensor(sensor::Sensor *indoor2_mode_sensor) { indoor2_mode_sensor_ = indoor2_mode_sensor; }
  void set_indoor2_capacity_sensor(sensor::Sensor *indoor2_capacity_sensor) { indoor2_capacity_sensor_ = indoor2_capacity_sensor; }
  void set_indoor2_delta_q_sensor(sensor::Sensor *indoor2_delta_q_sensor) { indoor2_delta_q_sensor_ = indoor2_delta_q_sensor; }

  //BINARY SENSORS
  void set_indoor1_operating_binary_sensor(binary_sensor::BinarySensor *indoor1_operating_binary_sensor) { indoor1_operating_binary_sensor_ = indoor1_operating_binary_sensor; }
  void set_indoor1_bladeswing_binary_sensor(binary_sensor::BinarySensor *indoor1_bladeswing_binary_sensor) { indoor1_bladeswing_binary_sensor_ = indoor1_bladeswing_binary_sensor; }
  void set_indoor2_operating_binary_sensor(binary_sensor::BinarySensor *indoor2_operating_binary_sensor) { indoor2_operating_binary_sensor_ = indoor2_operating_binary_sensor; }
  void set_indoor2_bladeswing_binary_sensor(binary_sensor::BinarySensor *indoor2_bladeswing_binary_sensor) { indoor2_bladeswing_binary_sensor_ = indoor2_bladeswing_binary_sensor; }
 
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;

  void set_dataline_debug(bool dataline_debug) { dataline_debug_ = dataline_debug; };
 
 float get_setup_priority() const override;
 
 protected:
  bool check_data_() const;
  void parse_data_();
  int8_t byte_to_temperature_(uint8_t databyte);
 
  sensor::Sensor *indoor1_room_temp_sensor_{nullptr};
  sensor::Sensor *indoor1_set_temp_sensor_{nullptr};
  sensor::Sensor *indoor1_pipe_in_temp_sensor_{nullptr};
  sensor::Sensor *indoor1_pipe_out_temp_sensor_{nullptr};
  sensor::Sensor *indoor1_fanspeed_sensor_{nullptr};
  sensor::Sensor *indoor1_mode_sensor_{nullptr};
  sensor::Sensor *indoor1_capacity_sensor_{nullptr};
  sensor::Sensor *indoor1_delta_q_sensor_{nullptr};

  sensor::Sensor *indoor2_room_temp_sensor_{nullptr};
  sensor::Sensor *indoor2_set_temp_sensor_{nullptr};
  sensor::Sensor *indoor2_pipe_in_temp_sensor_{nullptr};
  sensor::Sensor *indoor2_pipe_out_temp_sensor_{nullptr};
  sensor::Sensor *indoor2_fanspeed_sensor_{nullptr};
  sensor::Sensor *indoor2_mode_sensor_{nullptr};
  sensor::Sensor *indoor2_capacity_sensor_{nullptr};
  sensor::Sensor *indoor2_delta_q_sensor_{nullptr};

  binary_sensor::BinarySensor *indoor1_operating_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *indoor1_bladeswing_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *indoor2_operating_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *indoor2_bladeswing_binary_sensor_{nullptr};
 
  std::vector<uint8_t> data_;
  bool receiving_{false};
  uint32_t last_transmission_{0};

  //settings from yaml
  bool dataline_debug_{false};

  //Indoor unit 1
  int8_t indoor1_room_temp_;
  int8_t indoor1_set_temp_;
  int8_t indoor1_pipe_in_temp_;
  int8_t indoor1_pipe_out_temp_;
  uint8_t indoor1_fanspeed_;
  uint8_t indoor1_mode_;
  bool indoor1_bladeswing_;
  bool indoor1_operation_;
  float indoor1_capacity_;
  int8_t indoor1_delta_q_;

  //Indoor unit 2
  int8_t indoor2_room_temp_;
  int8_t indoor2_set_temp_;
  int8_t indoor2_pipe_in_temp_;
  int8_t indoor2_pipe_out_temp_;
  uint8_t indoor2_fanspeed_;
  uint8_t indoor2_mode_;
  bool indoor2_bladeswing_;
  bool indoor2_operation_;
  float indoor2_capacity_;
  int8_t indoor2_delta_q_;

};

}  // namespace samsung_ac_f1f2com
}  // namespace esphome
