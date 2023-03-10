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

    