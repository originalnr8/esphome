#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

class UartReadLineSensor : public Component, public UARTDevice, public TextSensor {
 public:
  UartReadLineSensor(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    // nothing to do here
  }

  int readline(int readch, char *buffer, int len) {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case '\n':  // Ignore new-lines
          break;
        case '\r':  // Return on CR
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          return rpos;
        default:
          if (pos < len - 1) {
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
  }

  void loop() override {
    const int max_line_length = 80;
    static char buffer[max_line_length];
    while (available()) {
      if (readline(read(), buffer, max_line_length) > 0) {
        publish_state(buffer);
      }
    }
  }
};

namespace esphome {
namespace cc_envir {

class CCEnvirComponent : public PollingComponent, public uart::UARTDevice {
 public:
  CCEnvirComponent() = default;

  /* Sensors */
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_power_sensor_1(sensor::Sensor *power_sensor_1) { power_sensor_1_ = power_sensor_1; }
  void set_power_sensor_2(sensor::Sensor *power_sensor_2) { power_sensor_1_ = power_sensor_2; }
  void set_power_sensor_3(sensor::Sensor *power_sensor_3) { power_sensor_1_ = power_sensor_3; }
  void set_power_sensor_4(sensor::Sensor *power_sensor_4) { power_sensor_1_ = power_sensor_4; }
  void set_power_sensor_5(sensor::Sensor *power_sensor_5) { power_sensor_1_ = power_sensor_5; }
  void set_power_sensor_6(sensor::Sensor *power_sensor_6) { power_sensor_1_ = power_sensor_6; }
  void set_power_sensor_7(sensor::Sensor *power_sensor_7) { power_sensor_1_ = power_sensor_7; }
  void set_power_sensor_8(sensor::Sensor *power_sensor_8) { power_sensor_1_ = power_sensor_8; }
  void set_power_sensor_9(sensor::Sensor *power_sensor_9) { power_sensor_1_ = power_sensor_9; }

  void setup() override;
  void dump_config() override;
  void update() override;

 protected:
  void request_data_();
  void decode_data_();
  int read_xml_(int readch, char *buffer, int len);
  char *substring(char *arr, int begin, int len);
  int indexOf(char *buffer, char find);

  sensor::Sensor *temperature_sensor{nullptr};
  sensor::Sensor *power_sensor_1{nullptr};
  sensor::Sensor *power_sensor_2{nullptr};
  sensor::Sensor *power_sensor_3{nullptr};
  sensor::Sensor *power_sensor_4{nullptr};
  sensor::Sensor *power_sensor_5{nullptr};
  sensor::Sensor *power_sensor_6{nullptr};
  sensor::Sensor *power_sensor_7{nullptr};
  sensor::Sensor *power_sensor_8{nullptr};
  sensor::Sensor *power_sensor_9{nullptr};
}
}  // namespace cc_envir
}  // namespace esphome
