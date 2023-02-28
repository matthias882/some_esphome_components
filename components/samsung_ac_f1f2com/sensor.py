import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_VOLTAGE,
    CONF_CURRENT,
    CONF_BATTERY_LEVEL,
    CONF_MAX_TEMPERATURE,
    CONF_MIN_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_PERCENT,
    UNIT_CELSIUS,
    ICON_FLASH,
    ICON_PERCENT,
    ICON_COUNTER,
    ICON_THERMOMETER,
    ICON_GAUGE,
)
from . import Samsung_AC_F1F2comComponent, CONF_SAMSUNG_AC_F1F2COM_ID

CONF_INDOOR1_ROOM_TEMP = "indoor1_room_temp"
CONF_INDOOR1_SET_TEMP = "indoor1_set_temp"
CONF_INDOOR1_PIPE_IN_TEMP = "indoor1_pipe_in_temp"
CONF_INDOOR1_PIPE_OUT_TEMP = "indoor1_pipe_out_temp"

CONF_INDOOR2_ROOM_TEMP = "indoor2_room_temp"
CONF_INDOOR2_SET_TEMP = "indoor2_set_temp"
CONF_INDOOR2_PIPE_IN_TEMP = "indoor2_pipe_in_temp"
CONF_INDOOR2_PIPE_OUT_TEMP = "indoor2_pipe_out_temp"

TYPES = [
    CONF_INDOOR1_ROOM_TEMP,
    CONF_INDOOR1_SET_TEMP,
    CONF_INDOOR1_PIPE_IN_TEMP,
    CONF_INDOOR1_PIPE_OUT_TEMP,
    CONF_INDOOR2_ROOM_TEMP,
    CONF_INDOOR2_SET_TEMP,
    CONF_INDOOR2_PIPE_IN_TEMP,
    CONF_INDOOR2_PIPE_OUT_TEMP,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.use_id(Samsung_AC_F1F2comComponent),

            cv.Optional(CONF_INDOOR1_ROOM_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR1_SET_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR1_PIPE_IN_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR1_PIPE_OUT_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR2_ROOM_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR2_SET_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR2_PIPE_IN_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INDOOR2_PIPE_OUT_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        sens = await sensor.new_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_sensor")(sens))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_SAMSUNG_AC_F1F2COM_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
