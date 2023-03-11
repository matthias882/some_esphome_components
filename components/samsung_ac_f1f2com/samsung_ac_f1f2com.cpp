/* 
One Datapackage consists of:
<start> <src> <dst> <cmd> <data: 8 bytes> <chksum> <end>

With:
Byte   Identifier   Comments
----------------------------
0      Start  : start of message (0x32)
1      Src    : Source address (00 01 02 03 = indoor units; C8 = outdoor unit; AD = ?; F0 = ?)
2      Dst    : Destination address
3      Cmd    : Command byte
4-11   Data   : Data is always 8 bytes in length, unused bytes will be zero
12     Chksum : Checksum of message which is the XOR of bytes 2-12
13     End    : end of message (0x34)

*/

#include "esphome/core/log.h"
#include "samsung_ac_f1f2com.h"
#include <vector>

namespace esphome {
namespace samsung_ac_f1f2com {

static const char *TAG = "samsung_ac_f1f2com";

static const uint8_t DATA_SRC = 1;
static const uint8_t DATA_DST = 2;
static const uint8_t DATA_CMD = 3;
static const uint8_t DATA_BYTE1 = 4;
static const uint8_t DATA_BYTE2 = 5;
static const uint8_t DATA_BYTE3 = 6;
static const uint8_t DATA_BYTE4 = 7;
static const uint8_t DATA_BYTE5 = 8;
static const uint8_t DATA_BYTE6 = 9;
static const uint8_t DATA_BYTE7 = 10;
static const uint8_t DATA_BYTE8 = 11;

static const uint8_t ADDR_INDOOR_UNIT_1 = 0x00;
static const uint8_t ADDR_INDOOR_UNIT_2 = 0x01;
static const uint8_t ADDR_INDOOR_UNIT_3 = 0x02;
static const uint8_t ADDR_INDOOR_UNIT_4 = 0x03;
static const uint8_t ADDR_OUTDOOR_UNIT_1 = 0xC8;

climate::ClimateTraits Samsung_AC_F1F2comComponent::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(this->sensor_ != nullptr);
  traits.set_supports_auto_mode(true);
  traits.set_supports_cool_mode(this->supports_cool_);
  traits.set_supports_heat_mode(this->supports_heat_);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_supports_away(false);
  traits.set_visual_min_temperature(YASHIMA_TEMP_MIN);
  traits.set_visual_max_temperature(YASHIMA_TEMP_MAX);
  traits.set_visual_temperature_step(1);
  return traits;
}

void Samsung_AC_F1F2comComponent::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value())
    this->mode = *call.get_mode();
  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();

  this->transmit_state_();
  this->publish_state();
}


void Samsung_AC_F1F2comComponent::setup() {}

void Samsung_AC_F1F2comComponent::dump_config(){
  ESP_LOGCONFIG(TAG, "Samsung_AC_F1F2com:");
  ESP_LOGCONFIG(TAG, "dataline debug enabled?: %s", this->dataline_debug_ ? "true" : "false");
  this->check_uart_settings(2400, 1, esphome::uart::UART_CONFIG_PARITY_EVEN, 8);
}

void Samsung_AC_F1F2comComponent::update() {
  //publish values for indoor unit 1
  if (this->indoor1_set_temp_sensor_)
    this->indoor1_set_temp_sensor_->publish_state(indoor1_set_temp_);
  if (this->indoor1_room_temp_sensor_)
    this->indoor1_room_temp_sensor_->publish_state(indoor1_room_temp_);
  if (this->indoor1_pipe_in_temp_sensor_)
    this->indoor1_pipe_in_temp_sensor_->publish_state(indoor1_pipe_in_temp_);
  if (this->indoor1_pipe_out_temp_sensor_)
    this->indoor1_pipe_out_temp_sensor_->publish_state(indoor1_pipe_out_temp_);
  if (this->indoor1_fanspeed_sensor_)
    this->indoor1_fanspeed_sensor_->publish_state(indoor1_fanspeed_);
  if (this->indoor1_mode_sensor_)
    this->indoor1_mode_sensor_->publish_state(indoor1_mode_);
  if (this->indoor1_capacity_sensor_)
    this->indoor1_capacity_sensor_->publish_state(indoor1_capacity_);
  if (this->indoor1_delta_q_sensor_)
    this->indoor1_delta_q_sensor_->publish_state(indoor1_delta_q_);
  if (this->indoor1_bladeswing_binary_sensor_)
    this->indoor1_bladeswing_binary_sensor_->publish_state(indoor1_bladeswing_);
  if (this->indoor1_operating_binary_sensor_)
    this->indoor1_operating_binary_sensor_->publish_state(indoor1_operation_);
  
  //publish values for indoor unit 2
  if (this->indoor2_set_temp_sensor_)
    this->indoor2_set_temp_sensor_->publish_state(indoor2_set_temp_);
  if (this->indoor2_room_temp_sensor_)
    this->indoor2_room_temp_sensor_->publish_state(indoor2_room_temp_);
  if (this->indoor2_pipe_in_temp_sensor_)
    this->indoor2_pipe_in_temp_sensor_->publish_state(indoor2_pipe_in_temp_);
  if (this->indoor2_pipe_out_temp_sensor_)
    this->indoor2_pipe_out_temp_sensor_->publish_state(indoor2_pipe_out_temp_);
  if (this->indoor2_fanspeed_sensor_)
    this->indoor2_fanspeed_sensor_->publish_state(indoor2_fanspeed_);
  if (this->indoor2_mode_sensor_)
    this->indoor2_mode_sensor_->publish_state(indoor2_mode_);
  if (this->indoor2_capacity_sensor_)
    this->indoor2_capacity_sensor_->publish_state(indoor2_capacity_);
  if (this->indoor2_delta_q_sensor_)
    this->indoor2_delta_q_sensor_->publish_state(indoor2_delta_q_);
  if (this->indoor2_bladeswing_binary_sensor_)
    this->indoor2_bladeswing_binary_sensor_->publish_state(indoor2_bladeswing_);
  if (this->indoor2_operating_binary_sensor_)
    this->indoor2_operating_binary_sensor_->publish_state(indoor2_operation_);
}

void Samsung_AC_F1F2comComponent::loop() {
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
    if (c == 0x32 && !receiving_) {//start-byte found
      receiving_ = true;
      data_.clear();
    }
    if (receiving_) {//reading datablock of 14 bytes in progress
      data_.push_back(c);
      if (data_.size() == 14 && c == 0x34) {//endbyte found and lenght of datablock is correct
        receiving_ = false;
        if (check_data_())
        parse_data_();
      }
      else if (data_.size() >= 14) {//no endbyte found at 14th byte
        ESP_LOGW(TAG, "received datablock to long (> 14): %02X %02x %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                 data_[0], data_[1], data_[2], data_[3], data_[4], data_[5], data_[6], data_[7], data_[8], data_[9], data_[10], data_[11], data_[12], data_[13]);
        receiving_ = false;
      }
    }
  }
}

float Samsung_AC_F1F2comComponent::get_setup_priority() const { return setup_priority::DATA; }

bool Samsung_AC_F1F2comComponent::check_data_() const {
  if (dataline_debug_ == true) {
    //print all communication packages when dataline debug is set true
    ESP_LOGD(TAG, "%d: %02X %02x %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
             millis(), data_[0], data_[1], data_[2], data_[3], data_[4], data_[5], data_[6], data_[7], data_[8], data_[9], data_[10], data_[11], data_[12], data_[13]);
  }
  if (data_[0] != 0x32) {
    ESP_LOGW(TAG, "unexpected start byte (not 0x32): %d", data_[0]);
    return false;
  }
  //calculate crc: xor data_[1] to data_[11]
  int i;
  uint8_t crc = data_[1];
  for (i = 2; i <= 11; i++) {
    crc = crc xor data_[i];
  }
  bool result = false;
  if (crc == data_[12]) result = true;
  if (!result)
    ESP_LOGW(TAG, "data checksum failed! calculated: %02x but received: %02x", crc, data_[12]);
  return result;
}
  
void Samsung_AC_F1F2comComponent::parse_data_() {
  //data from indoor-unit 1 to outdoor-unit
  if (data_[DATA_SRC] == ADDR_INDOOR_UNIT_1 && data_[DATA_DST] == ADDR_OUTDOOR_UNIT_1) {
    //CMD 0x20
    if (data_[DATA_CMD] == 0x20) {
      //temperatures
      indoor1_set_temp_ = byte_to_temperature_(data_[DATA_BYTE1]);//Set-Temperature: Byte1 in CMD20
      indoor1_room_temp_ = byte_to_temperature_(data_[DATA_BYTE2]);//Room-Temperature: Byte2 in CMD20
      indoor1_pipe_in_temp_ = byte_to_temperature_(data_[DATA_BYTE3]);//Pipe-In-Temperature: Byte3 in CMD20
      indoor1_pipe_out_temp_ = byte_to_temperature_(data_[DATA_BYTE8]);//Pipe-Out-Temperature: Byte8 in CMD20
      //fan
      indoor1_fanspeed_ = data_[DATA_BYTE4] & 0b00001111;// fanspeed: databyte4 Bit 3-0: 0=auto, 2=low, 4=medium, 5=high, 6=fresh
      //swing
      if ((data_[DATA_BYTE4] & 0b11110000) == 0xD0) indoor1_bladeswing_ = true;// bladeswing: databyte4 Bit 7-4: 0=off, D=on
      else indoor1_bladeswing_ = false;
      //power on / off
      if (data_[DATA_BYTE5] & 0b10000000) indoor1_operation_ = true; //bit7 = Power on/off
      else indoor1_operation_ = false;
      //mode
      indoor1_mode_ = data_[DATA_BYTE5] & 0b00111111;//mode: 0x01=heat, 0x02=cool, 0x04=dry, 0x08=fan, 0x21=auto(heat), 0x22=auto(cool)
    }
    //CMD 0x40
    else if (data_[DATA_CMD] == 0x40) {
      //Capacity of indoor unit (byte 6)
      indoor1_capacity_ = float(data_[DATA_BYTE6]) / 10;
      //delta q of indoor unit (byte 8)
      indoor1_delta_q_ = data_[DATA_BYTE8];
    }
  }

  //data from indoor-unit 1 to outdoor-unit
  else if (data_[DATA_SRC] == ADDR_INDOOR_UNIT_2 && data_[DATA_DST] == ADDR_OUTDOOR_UNIT_1) {
    //CMD 0x20  
    if (data_[DATA_CMD] == 0x20) {
      //temperatures
      indoor2_set_temp_ = byte_to_temperature_(data_[DATA_BYTE1]);//Set-Temperature: Byte1 in CMD20
      indoor2_room_temp_ = byte_to_temperature_(data_[DATA_BYTE2]);//Room-Temperature: Byte2 in CMD20
      indoor2_pipe_in_temp_ = byte_to_temperature_(data_[DATA_BYTE3]);//Pipe-In-Temperature: Byte3 in CMD20
      indoor2_pipe_out_temp_ = byte_to_temperature_(data_[DATA_BYTE8]);//Pipe-Out-Temperature: Byte8 in CMD20
      //fan
      indoor2_fanspeed_ = data_[DATA_BYTE4] & 0b00001111;// fanspeed: databyte4 Bit 3-0: 0=auto, 2=low, 4=medium, 5=high, 6=fresh
      //swing
      if ((data_[DATA_BYTE4] & 0b11110000) == 0xD0) indoor2_bladeswing_ = true;// bladeswing: databyte4 Bit 7-4: 0=off, D=on
      else indoor2_bladeswing_ = false;
      //power on / off
      if (data_[DATA_BYTE5] & 0b10000000) indoor2_operation_ = true; //bit7 = Power on/off
      else indoor2_operation_ = false;
      //mode
      indoor2_mode_ = data_[DATA_BYTE5] & 0b00111111;//mode: 0x01=heat, 0x02=cool, 0x04=dry, 0x08=fan, 0x22=auto
    }
    //CMD 0x40
    else if (data_[DATA_CMD] == 0x40) {
      //Capacity of indoor unit (byte 6)
      indoor2_capacity_ = float(data_[DATA_BYTE6]) / 10;
      //delta q of indoor unit (byte 8)
      indoor2_delta_q_ = data_[DATA_BYTE8];
    }
  }
}

int8_t Samsung_AC_F1F2comComponent::byte_to_temperature_(uint8_t databyte) {
  int8_t temperature = databyte - 55;   //0°C = 55 (0x37)
  return temperature;
}

}  // namespace samsung_ac_f1f2com
}  // namespace esphome
