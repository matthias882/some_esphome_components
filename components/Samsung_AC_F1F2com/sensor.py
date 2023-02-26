import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, \
    ICON_POWER, ICON_CURRENT_AC, CONF_TEMPERATURE, ICON_THERMOMETER, UNIT_CELSIUS, \
    UNIT_PERCENT, ICON_PERCENT, UNIT_EMPTY, ICON_EMPTY

DEPENDENCIES = ['uart']

Samsung_AC_F1F2com_ns = cg.esphome_ns.namespace('Samsung_AC_F1F2com')
Samsung_AC_F1F2comComponent = Samsung_AC_F1F2com_ns.class_('Samsung_AC_F1F2comComponent', uart.UARTDevice, cg.PollingComponent)

CONF_ROOM_TEMP_1 = "room_temp_1"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Samsung_AC_F1F2comComponent),

    cv.Optional(CONF_ROOM_TEMP_1): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}).extend(cv.polling_component_schema('60s'))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    
    if CONF_ROOM_TEMP_1 in config:
        sens = yield sensor.new_sensor(config[CONF_ROOM_TEMP_1])
        cg.add(var.set_room_temp_sensor_1(sens))

