import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
)

AUTO_LOAD = ["climate"]

from . import Samsung_AC_F1F2comComponent, CONF_SAMSUNG_AC_F1F2COM_ID

CONFIG_SCHEMA = climate.CLIMATE.extend(
    {
        cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.use_id(Samsung_AC_F1F2comComponent),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate_ir.register_climate_ir(var, config)    