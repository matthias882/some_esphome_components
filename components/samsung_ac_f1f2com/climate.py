from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_WATT,
)
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, select

from . import Samsung_AC_F1F2comComponent, CONF_SAMSUNG_AC_F1F2COM_ID

AUTO_LOAD = ["sensor", "select"]
DEPENDENCIES = ["uart"]

Samsung_AC_F1F2comSelect = samsung_ac_f1f2com.class_(
    "Samsung_AC_F1F2comSelect", select.Select, cg.Component
)

CONF_VERTICAL_SWING_SELECT = "vertical_swing_select"
CONF_CURRENT_TEMPERATURE_SENSOR = "current_temperature_sensor"

VERTICAL_SWING_OPTIONS = ["on", "off"]

SELECT_SCHEMA = select.SELECT_SCHEMA.extend(
    {cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.declare_id(Samsung_AC_F1F2comSelect)}
)

SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.Optional(CONF_VERTICAL_SWING_SELECT): SELECT_SCHEMA,
    }
).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    if CONF_VERTICAL_SWING_SELECT in config:
        conf = config[CONF_VERTICAL_SWING_SELECT]
        swing_select = await select.new_select(conf, options=VERTICAL_SWING_OPTIONS)
        await cg.register_component(swing_select, conf)
        cg.add(var.set_vertical_swing_select(swing_select))