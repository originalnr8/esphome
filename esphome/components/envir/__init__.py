import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@originalnr8"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

CONF_CC_ENVIR_ID = "current_cost_envir_id"

cc_envir_ns = cg.esphome_ns.namespace("cc_envir")
CCEnvirComponent = cc_envir_ns.class_("CCEnvirComponent", cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(CCEnvirComponent)})
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
