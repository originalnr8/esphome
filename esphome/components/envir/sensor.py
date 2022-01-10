import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_TEMPERATURE,
    CONF_POWER_1,
    CONF_POWER_2,
    CONF_POWER_3,
    CONF_POWER_4,
    CONF_POWER_5,
    CONF_POWER_6,
    CONF_POWER_7,
    CONF_POWER_8,
    CONF_POWER_9,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_WATT,
    ICON_COUNTER,
    ICON_THERMOMETER,
)
from . import CCEnvirComponent, CONF_CC_ENVIR_ID

TYPES = [
    CONF_TEMPERATURE,
    CONF_POWER_1,
    CONF_POWER_2,
    CONF_POWER_3,
    CONF_POWER_4,
    CONF_POWER_5,
    CONF_POWER_6,
    CONF_POWER_7,
    CONF_POWER_8,
    CONF_POWER_9,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_CC_ENVIR_ID): cv.use_id(DalyBmsComponent),

            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                UNIT_CELSIUS,
                ICON_THERMOMETER,
                1,
                DEVICE_CLASS_TEMPERATURE,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_1): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_2): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_3): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_4): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_5): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_6): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_7): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_8): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER_9): sensor.sensor_schema(
                UNIT_WATT,
                ICON_COUNTER,
                1,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)

async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        sens = await sensor.new_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_sensor")(sens))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_CC_ENVIR_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
