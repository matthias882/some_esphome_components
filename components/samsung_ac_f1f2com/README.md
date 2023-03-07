# Samsung Air Conditioning F1/F2 Bus

Warning: This Component is still in developement on early beta-state. Use on your own Risk and don't be sad if something does not work ;)

Special thanks to DannyDeGaspari and his [Samsung-HVAC-buscontrol](https://github.com/DannyDeGaspari/Samsung-HVAC-buscontrol) project. (His subject ist the dataline between indoor-unit and wired-controller on Line F3/F4, but his reverse-engineering was a great start for my project)

This component is able to read out all the data from SAMSUNG FJM+ (multisplit) climate devices.
It reads the data from dataline F1 F2. This is the databus between the outdoor-unit and the indoor-units.
Thatswhy you can get the data from alle indoor-units and the outdoor-unit with only one ESPHome-device on a single point somewhere on this bus.
On the other hand it is possible to controll al the indoor-Units from one single point. !!The controll-functionality ist not implemented yet!!!

In my setup the esp8266-controller ist placed in one of the indoor-units. You can grab 12VDC from the indoor-unit controller board for generating the supply-voltage for the ESP.

## My tested setup:
### Climate-devices
Outdoor-unit:   MH070FXEA4A
2x indoor-unit: MH035FPEA

### Controller: WEMOS D1 mini
RS485 <-> TTL converter (3.3V)

### Connections between RS485-converter and Bus:
- RS485-A-line <-> F1
- RS485-B-line <-> F2

### Connections between WEMOS and RS485-converter:
- RS485-converter RX <-> WEMOS D1 (soft-uart TX)
- RS485-converter TX <-> WEMOS D2(soft-uart RX)
(- RS485-converter DE/!RE <-> WEMOS D0) (switching data-direction not supported yet, use rs485-converter with automatic direction control)


## Implement the Component to your ESPHome-yaml:

### Include the external component:
```yaml
external_components:
  source: github://matthias882/some_esphome_components@main
  components: [samsung_ac_f1f2com]
```

### A configured uart component is required. Settings are 2400 8E1
```yaml
uart:
  tx_pin: D1
  rx_pin: D2
  baud_rate: 2400
  parity: EVEN
```

### Set up the component:
the "dataline_debug" property is used to enable or disable printing all the received data-packages on F1 F2 bus to the log as debug-messages. Log-level must be set minimum on DEBUG
```yaml
samsung_ac_f1f2com:
  update_interval: 10s
  dataline_debug: false
```

### Following Sensor-values are currently available:
```yaml
sensor:
  - platform: samsung_ac_f1f2com
    #indoor unit 1
    indoor1_room_temp:
      name: "Sleepingroom room temperature"
    indoor1_set_temp:
      name: "Sleepingroom set temperature"
    indoor1_pipe_in_temp:
      name: "Sleepingroom temperature EVA in"
    indoor1_pipe_out_temp:
      name: "Sleepingroom temperature EVA out"
    indoor1_fanspeed:
      name: "Sleepingroom fanspeed"
    indoor1_mode:
      name: "Sleepingroom mode"
    indoor1_capacity:
      name: "Sleepingroom thermal capacity"
    indoor1_delta_q:
      name: "Sleepingroom delta Q"
    #indoor unit 2
    indoor2_room_temp:
      name: "Livingroom room temperature"
    indoor2_set_temp:
      name: "Livingroom set temperature"
    indoor2_pipe_in_temp:
      name: "Livingroom temperature EVA in"
    indoor2_pipe_out_temp:
      name: "Livingroom temperature EVA out"
    indoor2_fanspeed:
      name: "Livingroom fanspeed"
    indoor2_mode:
      name: "Livingroom mode"
    indoor2_capacity:
      name: "Livingroom thermal capacity"
    indoor2_delta_q:
      name: "Livingroom delta Q"

binary_sensor:
  - platform: samsung_ac_f1f2com
    indoor1_operating:
      name: "Sleepingroom operating"
    indoor1_bladeswing:
      name: "Sleepingroom bladeswing"
    indoor2_operating:
      name: "Livingroom operating"
    indoor2_bladeswing:
      name: "Livingroom bladeswing"
```


## ToDos and currently in Pipeline:
- currently you can read two indoor-units on bus-adress 0x00 and 0x01 -> up to 4 units with free configurable adresses should come soon
- RS485 direction control (DE/!RE) with WEMOS-Pin to be able to use cheaper max485 bus-converters
- read many values from outdoor-unit (messages are still decoded, not yet time to implement it)
- control the indoor-devices (mode, temp, swing and fanspeed) (already testet but not implemented yet
- documentation of all decoded data-packages