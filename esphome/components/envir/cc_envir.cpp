#include "cc_envir.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cc_envir {

static const char *const TAG = "cc_envir";

void CCEnvirComponent::setup() {}

void CCEnvirComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "CC Envir:");
  this->check_uart_settings(57600);
}

void CCEnvirComponent::update() {
  // Fetch data
  decode_data_();
}

void CCEnvirComponent::decode_data_() {
  // Baud 57600 Parity=N Data=8 stop=1
  /*
  <msg>
  <src>CC128 - v1 .29</src>
  <uid>61A582015AA09B</uid>
  <dsb>00089</dsb>
  <time>13:02:39</time>
  <tmpr>18.7</tmpr>
  <tmprF>72.3</tmprF>
  <sensor>1</sensor>
  <id>01234</id>
  <type>1</type>
  <ch1><watts>00345</watts></ch1>
  <ch2><watts>02151</watts></ch2>
  <ch3><watts>00000</watts></ch3>
  </msg>
  */

  const int max_line_length = 256;
  static char buffer[max_line_length];
  while (available()) {
    if (read_xml_(read(), buffer, max_line_length) > 0) {
      ESP_LOGD(TAG, buffer);
    }
  }
  // How much is in the buffer?
  int lengthXML = strlen(buffer);

  /*
  *  Fetch Temperature
  */
  int idxOfLetter = indexOf(buffer, "p");
  ESP_LOGD(TAG, "Incidence of P in XML:" + idxOfLetter);

  float fltTemperatureC = 0.0;
  if (idxOfLetter > 0) {
    // pr>18.7</tmpr>
    char *temperatureC = substring(buffer, idxOfLetter + 3, 4);
    ESP_LOGD(TAG, "Temperature extracted details:" + temperatureC);

    fltTemperatureC = (float) temperatureC;
    ESP_LOGD(TAG, "Temperature cast:" + temperatureC);
  }
  if (this->temperature_sensor_) {
    this->temperature_sensor_->publish_state(fltTemperatureC);
  }

  // Truncate buffer into newbuffer then swap back
  int toChomp = idxOfLetter + 3 + 4 + 7;
  ESP_LOGD(TAG, "Index at which to truncate XML:" + toChomp);
  char *newbuffer = substring(buffer, toChomp, lengthXML - toChomp);
  ESP_LOGD(TAG, "XML after beiung truncated:" + newbuffer);
  buffer = newbuffer;
  lengthXML = strlen(newbuffer);
  ESP_LOGD(TAG, "New XML length:" + lengthXML);

  /*
  * Fetch Power values into array for later
  */
  int index = 0;
  char *wattage[9] = {
      "00000",
      "00000",
      "00000",
      "00000", 
      "00000", 
      "00000", 
      "00000", 
      "00000", 
      "00000"
  };
  while (lengthXML > 6) {
    idxOfLetter = indexOf(buffer, "c");
    if (idxOfLetter > 0) {
      // ch1><watts>00345</watts></ch1>
      wattage[index] = substring(buffer, idxOfLetter + 11, 5);
      ESP_LOGD(TAG, "Wattage extracted details:" + wattage[index]);
    }

    // Truncate buffer into newbuffer
    toChomp = idxOfLetter + 11 + 5 + 14;
    ESP_LOGD(TAG, "Index at which to truncate XML:" + toChomp);
    *newbuffer = substring(buffer, toChomp, lengthXML - toChomp);
    ESP_LOGD(TAG, "XML after being truncated:" + newbuffer);
    buffer = newbuffer;
    ESP_LOGD(TAG, "XML after being swapped:" + buffer);
    lengthXML = strlen(buffer);
    ESP_LOGD(TAG, "New XML length:" + lengthXML);
    i++;
  }

  if (wattage[0] != "00000") {
    if (this->power_sensor_1_) {
      this->power_sensor_1_->publish_state((int) wattage[0]);
    }
  }
  if (wattage[1] != "00000") {
    if (this->power_sensor_2_) {
      this->power_sensor_2_->publish_state((int) wattage[1]);
    }
  }
  if (wattage[2] != "00000") {
    if (this->power_sensor_3_) {
      this->power_sensor_3_->publish_state((int) wattage[2]);
    }
  }
  if (wattage[3] != "00000") {
    if (this->power_sensor_4_) {
      this->power_sensor_4_->publish_state((int) wattage[3]);
    }
  }
  if (wattage[4] != "00000") {
    if (this->power_sensor_5_) {
      this->power_sensor_5_->publish_state((int) wattage[4]);
    }
  }
  if (wattage[5] != "00000") {
    if (this->power_sensor_6_) {
      this->power_sensor_6_->publish_state((int) wattage[5]);
    }
  }
  if (wattage[6] != "00000") {
    if (this->power_sensor_7_) {
      this->power_sensor_7_->publish_state((int) wattage[6]);
    }
  }
  if (wattage[7] != "00000") {
    if (this->power_sensor_8_) {
      this->power_sensor_8_->publish_state((int) wattage[7]);
    }
  }
  if (wattage[8] != "00000") {
    if (this->power_sensor_9_) {
      this->power_sensor_9_->publish_state((int) wattage[8]);
    }
  }

}

int read_xml_(int readch, char *buffer, int len) {
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

char *substring(char *arr, int begin, int len) {
  char *res = new char[len + 1];
  for (int i = 0; i < len; i++) {
    res[i] = *(arr + begin + i);
  }
  res[len] = 0;
  return res;
}

int indexOf(char* buffer, char find) {
  int index = -1;
  const char *ptr = strchr(buffer, find);
  if (ptr) {
    index = ptr - buffer;
  }
  return index;
}


}
}
