import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

AUTO_LOAD = ["climate"]

from . import Samsung_AC_F1F2comComponent, CONF_SAMSUNG_AC_F1F2COM_ID

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.use_id(Samsung_AC_F1F2comComponent),
    }
)