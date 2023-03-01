import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from . import Samsung_AC_F1F2comComponent, CONF_SAMSUNG_AC_F1F2COM_ID

CONF_INDOOR1_OPERATING = "indoor1_operating"
CONF_INDOOR2_OPERATING = "indoor1_operating"

TYPES = [
    CONF_INDOOR1_OPERATING,
    CONF_INDOOR2_OPERATING,
    
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_SAMSUNG_AC_F1F2COM_ID): cv.use_id(DalyBmsComponent),
            cv.Optional(
                CONF_INDOOR1_OPERATING
            ): binary_sensor.binary_sensor_schema(),
            cv.Optional(
                CONF_INDOOR2_OPERATING
            ): binary_sensor.binary_sensor_schema(),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        var = await binary_sensor.new_binary_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_binary_sensor")(var))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_SAMSUNG_AC_F1F2COM_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
