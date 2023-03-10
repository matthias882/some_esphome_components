import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, climate
from esphome.const import CONF_ID

CODEOWNERS = ["matthias882"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

CONF_SAMSUNG_AC_F1F2COM_ID = "samsung_ac_f1f2com_id"

samsung_ac_f1f2com = cg.esphome_ns.namespace("samsung_ac_f1f2com")
Samsung_AC_F1F2comComponent = samsung_ac_f1f2com.class_(
    "Samsung_AC_F1F2comComponent", cg.PollingComponent, uart.UARTDevice, climate.Climate
)

CONF_DATALINE_DEBUG = "dataline_debug"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Samsung_AC_F1F2comComponent),
            cv.Optional(CONF_DATALINE_DEBUG, default=False): cv.boolean,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(climate.CLIMATE_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_dataline_debug(config[CONF_DATALINE_DEBUG]))
