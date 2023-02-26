import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["matthias882"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

CONF_SAMSUNG_F1F2COM_ID = "samsung_f1f2com_id"

samsung_f1f2com = cg.esphome_ns.namespace("samsung_f1f2com")
Samsung_AC_F1F2comComponent = samsung_f1f2com.class_(
    "Samsung_AC_F1F2comComponent", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(Samsung_AC_F1F2comComponent)})
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

