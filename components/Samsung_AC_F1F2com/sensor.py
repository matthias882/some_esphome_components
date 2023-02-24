import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY

DEPENDENCIES = ['uart']

Samsung_AC_F1F2com_ns = cg.esphome_ns.namespace('Samsung_AC_F1F2com')
Samsung_AC_F1F2comComponent = Samsung_AC_F1F2com_ns.class_('Samsung_AC_F1F2comComponent', cg.PollingComponent, uart.UARTDevice)

CONF_ROOM_TEMP_1 = "room_temp"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Samsung_AC_F1F2comComponent),
    cv.Optional(CONF_ROOM_TEMP_1): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 0),
}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    yield uart.register_uart_device(var, config)
    
    if CONF_ROOM_TEMP_1 in config:
        sens = await sensor.new_sensor(config[CONF_ROOM_TEMP_1])
        cg.add(var.set_room_temp_sensor(sens))

