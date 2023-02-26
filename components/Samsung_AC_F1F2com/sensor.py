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



Samsung_AC_F1F2com_ns = cg.esphome_ns.namespace('Samsung_AC_F1F2com')
Samsung_AC_F1F2comComponent = Samsung_AC_F1F2com_ns.class_('Samsung_AC_F1F2comComponent', uart.UARTDevice, cg.PollingComponent)

CONF_ROOM_TEMP_1 = "room_temp_1"

TYPES = [
    CONF_ROOM_TEMP_1,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.use_id(Samsung_AC_F1F2comComponent),

            cv.Optional(CONF_ROOM_TEMP_1): sensor.sensor_schema(
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
