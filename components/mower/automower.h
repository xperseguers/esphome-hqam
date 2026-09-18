#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/template/sensor/template_sensor.h"
#include "esphome/components/template/text_sensor/template_text_sensor.h"

#include <algorithm>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace esphome
{
  namespace mower
  {

    class Automower : public PollingComponent, public uart::UARTDevice
    {
    public:
      Automower(uart::UARTComponent *parent, uint32_t update_interval);

      void set_battery_level_sensor(template_::TemplateSensor *s);
      void set_battery_temperature_sensor(template_::TemplateSensor *s);
      void set_battery_used_sensor(template_::TemplateSensor *s);
      void set_battery_voltage_sensor(template_::TemplateSensor *s);
      void set_blade_motor_speed_sensor(template_::TemplateSensor *s);
      void set_charging_time_sensor(template_::TemplateSensor *s);
      void set_mowing_time_sensor(template_::TemplateSensor *s);
      void set_firmware_version_sensor(template_::TemplateSensor *s);
      void set_battery_capacity_sensor(template_::TemplateSensor *s);
      void set_battery_return_sensor(template_::TemplateSensor *s);
      void set_battery_temp_charged_sensor(template_::TemplateSensor *s);
      void set_battery_temp_load_sensor(template_::TemplateSensor *s);
      void set_temp_next_check_sensor(template_::TemplateSensor *s);
      void set_cutting_time_sensor(template_::TemplateSensor *s);
      void set_loop_quality_sensor(template_::TemplateSensor *s);
      void set_square_status_sensor(template_::TemplateSensor *s);
      void set_square_percent_sensor(template_::TemplateSensor *s);
      void set_square_ref_sensor(template_::TemplateSensor *s);
      void set_speed_right_sensor(template_::TemplateSensor *s);
      void set_speed_left_sensor(template_::TemplateSensor *s);
      void set_language_sensor(template_::TemplateSensor *s);

      void set_last_code_received_text_sensor(template_::TemplateTextSensor *s);
      void set_mode_text_sensor(template_::TemplateTextSensor *s);
      void set_status_text_sensor(template_::TemplateTextSensor *s);
      void set_status_message_text_sensor(template_::TemplateTextSensor *s);

      template_::TemplateSensor *get_battery_level_sensor() const;
      template_::TemplateSensor *get_battery_temperature_sensor() const;
      template_::TemplateSensor *get_battery_used_sensor() const;
      template_::TemplateSensor *get_battery_voltage_sensor() const;
      template_::TemplateSensor *get_blade_motor_speed_sensor() const;
      template_::TemplateSensor *get_charging_time_sensor() const;
      template_::TemplateSensor *get_mowing_time_sensor() const;
      template_::TemplateSensor *get_firmware_version_sensor() const;
      template_::TemplateSensor *get_battery_capacity_sensor() const;
      template_::TemplateSensor *get_battery_return_sensor() const;
      template_::TemplateSensor *get_battery_temp_charged_sensor() const;
      template_::TemplateSensor *get_battery_temp_load_sensor() const;
      template_::TemplateSensor *get_temp_next_check_sensor() const;
      template_::TemplateSensor *get_cutting_time_sensor() const;
      template_::TemplateSensor *get_loop_quality_sensor() const;
      template_::TemplateSensor *get_square_status_sensor() const;
      template_::TemplateSensor *get_square_percent_sensor() const;
      template_::TemplateSensor *get_square_ref_sensor() const;
      template_::TemplateSensor *get_speed_right_sensor() const;
      template_::TemplateSensor *get_speed_left_sensor() const;
      template_::TemplateSensor *get_language_sensor() const;

      template_::TemplateTextSensor *get_last_code_received_text_sensor() const;
      template_::TemplateTextSensor *get_mode_text_sensor() const;
      template_::TemplateTextSensor *get_status_text_sensor() const;
      template_::TemplateTextSensor *get_status_message_text_sensor() const;

      // Generic access to the last received value of any register, for the
      // template sensors/numbers defined in yaml. Returns NaN when the register
      // hasn't been read yet, so the lambda can skip publishing (return {}).
      float get_register(uint16_t addr, bool is_signed = false);

      void setup() override;
      void update() override;
      void loop() override;

      void set_mode(const std::string &value);
      void set_stop(bool stop);
      void set_left_motor(int value);
      void set_right_motor(int value);
      void key_back();
      void key_yes();
      void key_num(uint8_t num);

    protected:
      int pollingId_ = 0;
      bool _writable = true;
      bool stopStatus = false;

      // Reply-waiting: only release the bus when the expected address arrives.
      // Prevents keypad traffic from incorrectly releasing the bus.
      bool awaiting_reply_ = false;
      uint16_t expected_addr_ = 0;
      uint32_t last_send_time_ = 0;
      static constexpr uint32_t UART_REPLY_TIMEOUT_MS = 2000;

      // Write queue: user writes are queued and drained by serviceBus()
      // before the next poll. Prevents collisions with poll replies.
      struct Frame { uint8_t b[5]; };
      std::vector<Frame> write_queue_;
      void queueFrame(const uint8_t *data);
      void serviceBus();

      template_::TemplateSensor *battery_current_sensor_ = nullptr;
      template_::TemplateSensor *battery_level_sensor_ = nullptr;
      template_::TemplateSensor *battery_temperature_sensor_ = nullptr;
      template_::TemplateSensor *battery_used_sensor_ = nullptr;
      template_::TemplateSensor *battery_voltage_sensor_ = nullptr;
      template_::TemplateSensor *blade_motor_speed_sensor_ = nullptr;
      template_::TemplateSensor *charging_time_sensor_ = nullptr;
      template_::TemplateSensor *mowing_time_sensor_ = nullptr;
      template_::TemplateSensor *firmware_version_sensor_ = nullptr;
      template_::TemplateSensor *battery_capacity_sensor_ = nullptr;
      template_::TemplateSensor *battery_return_sensor_ = nullptr;
      template_::TemplateSensor *battery_temp_charged_sensor_ = nullptr;
      template_::TemplateSensor *battery_temp_load_sensor_ = nullptr;
      template_::TemplateSensor *temp_next_check_sensor_ = nullptr;
      template_::TemplateSensor *cutting_time_sensor_ = nullptr;
      template_::TemplateSensor *loop_quality_sensor_ = nullptr;
      template_::TemplateSensor *square_status_sensor_ = nullptr;
      template_::TemplateSensor *square_percent_sensor_ = nullptr;
      template_::TemplateSensor *square_ref_sensor_ = nullptr;
      template_::TemplateSensor *speed_right_sensor_ = nullptr;
      template_::TemplateSensor *speed_left_sensor_ = nullptr;
      template_::TemplateSensor *language_sensor_ = nullptr;

      template_::TemplateTextSensor *mode_text_sensor_ = nullptr;
      template_::TemplateTextSensor *status_text_sensor_ = nullptr;
      template_::TemplateTextSensor *status_message_text_sensor_ = nullptr;
      template_::TemplateTextSensor *last_code_received_text_sensor_ = nullptr;

      // Register cache: sorted vector of (address, value) pairs.
      // Replaces std::map for better memory efficiency with ~50 entries.
      std::vector<std::pair<uint16_t, uint16_t>> register_values_;
      void store_register(uint16_t addr, uint16_t val);

      static constexpr uint8_t MAN_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x00};
      static constexpr uint8_t AUTO_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x01};
      static constexpr uint8_t HOME_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x03};
      static constexpr uint8_t DEMO_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x04};
      static constexpr uint8_t STOP_ON_DATA[5] = {0x0F, 0x81, 0x2F, 0x00, 0x02};
      static constexpr uint8_t STOP_OFF_DATA[5] = {0x0F, 0x81, 0x2F, 0x00, 0x00};
      static constexpr uint8_t KEY_BACK[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0F};
      static constexpr uint8_t KEY_YES[5] = {0x0F, 0x80, 0x5F, 0x00, 0x12};
      static constexpr uint8_t getModeCmd[5] = {0x0F, 0x01, 0x2C, 0x00, 0x00};
      static constexpr uint8_t getStatusCode[5] = {0x0F, 0x01, 0xF1, 0x00, 0x00};
      static constexpr uint8_t getChargingTime[5] = {0x0F, 0x01, 0xEC, 0x00, 0x00};
      static constexpr uint8_t getMowingTime[5] = {0x0F, 0x00, 0x56, 0x00, 0x00};
      // static constexpr uint8_t getBatteryCurrent[5] = {0x0F, 0x01, 0xEB, 0x00, 0x00}; don't know 65+
      static constexpr uint8_t getBatteryLevel[5] = {0x0F, 0x01, 0xEF, 0x00, 0x00};
      static constexpr uint8_t getBatteryUsed[5] = {0x0F, 0x2E, 0xE0, 0x00, 0x00};
      static constexpr uint8_t getBatteryVoltage[5] = {0x0F, 0x2E, 0xF4, 0x00, 0x00};
      static constexpr uint8_t getBatteryTemperature[5] = {0x0F, 0x02, 0x33, 0x00, 0x00};
      static constexpr uint8_t getBladeMotorSpeed[5] = {0x0F, 0x2E, 0xEA, 0x00, 0x00};
      static constexpr uint8_t getFirmwareVersion[5] = {0x0F, 0x33, 0x90, 0x00, 0x00};
      static constexpr uint8_t READ_STOP_CMD[5] = {0x0F, 0x01, 0x2F, 0x00, 0x00};
      // Additional registers for sensors
      static constexpr uint8_t getBatteryCapacity[5] = {0x0F, 0x00, 0xB1, 0x00, 0x00};
      static constexpr uint8_t getBatteryReturn[5] = {0x0F, 0x01, 0xF0, 0x00, 0x00};
      static constexpr uint8_t getBatteryTempCharged[5] = {0x0F, 0x02, 0x34, 0x00, 0x00};
      static constexpr uint8_t getBatteryTempLoad[5] = {0x0F, 0x02, 0x35, 0x00, 0x00};
      static constexpr uint8_t getTempNextCheck[5] = {0x0F, 0x02, 0x36, 0x00, 0x00};
      static constexpr uint8_t getCuttingTime[5] = {0x0F, 0x00, 0x38, 0x00, 0x00};
      static constexpr uint8_t getLoopQuality[5] = {0x0F, 0x01, 0xB9, 0x00, 0x00};
      static constexpr uint8_t getSquareStatus[5] = {0x0F, 0x01, 0x38, 0x00, 0x00};
      static constexpr uint8_t getSquarePercent[5] = {0x0F, 0x01, 0x34, 0x00, 0x00};
      static constexpr uint8_t getSquareRef[5] = {0x0F, 0x01, 0x37, 0x00, 0x00};
      static constexpr uint8_t getSpeedRight[5] = {0x0F, 0x24, 0xBF, 0x00, 0x00};
      static constexpr uint8_t getSpeedLeft[5] = {0x0F, 0x24, 0xC0, 0x00, 0x00};
      static constexpr uint8_t getLanguage[5] = {0x0F, 0x3A, 0xC0, 0x00, 0x00};
      static constexpr uint8_t getTimerActive[5] = {0x0F, 0x4A, 0x4E, 0x00, 0x00};
      static constexpr uint8_t getTimer1StartH[5] = {0x0F, 0x4A, 0x38, 0x00, 0x00};
      static constexpr uint8_t getTimer1StartM[5] = {0x0F, 0x4A, 0x39, 0x00, 0x00};
      static constexpr uint8_t getTimer1StopH[5] = {0x0F, 0x4A, 0x3A, 0x00, 0x00};
      static constexpr uint8_t getTimer1StopM[5] = {0x0F, 0x4A, 0x3B, 0x00, 0x00};
      static constexpr uint8_t getTimer2StartH[5] = {0x0F, 0x4A, 0x40, 0x00, 0x00};
      static constexpr uint8_t getTimer2StartM[5] = {0x0F, 0x4A, 0x41, 0x00, 0x00};
      static constexpr uint8_t getTimer2StopH[5] = {0x0F, 0x4A, 0x42, 0x00, 0x00};
      static constexpr uint8_t getTimer2StopM[5] = {0x0F, 0x4A, 0x43, 0x00, 0x00};
      static constexpr uint8_t getClockSec[5] = {0x0F, 0x36, 0xB1, 0x00, 0x00};
      static constexpr uint8_t getClockMin[5] = {0x0F, 0x36, 0xB3, 0x00, 0x00};
      static constexpr uint8_t getClockHour[5] = {0x0F, 0x36, 0xB5, 0x00, 0x00};

      // static constexpr uint8_t getBatteryCapacity[5] = {0x0F, 0x01, 0xEB, 0x00, 0x00};
      // static constexpr uint8_t getBatteryCapacityUsed[5] = {0x0F, 0x2E, 0xE0, 0x00, 0x00};
      // static constexpr uint8_t getBatteryChargingTimeMinute[5] = {0x0F, 0x01, 0xEC, 0x00, 0x00};
      // static constexpr uint8_t getBatteryLastChargeMinute[5] = {0x0F, 0x02, 0x35, 0x00, 0x00};
      // static constexpr uint8_t getBatteryNextTemperatureMeasurementSecond[5] = {0x0F, 0x02, 0x36, 0x00, 0x00};
      // static constexpr uint8_t getBatteryTemperatureCharged[5] = {0x0F, 0x02, 0x35, 0x00, 0x00};
      // static constexpr uint8_t getDay[5] = {0x0F, 0x36, 0xB7, 0x00, 0x00};
      // static constexpr uint8_t getHour[5] = {0x0F, 0x36, 0xB5, 0x00, 0x00};
      // static constexpr uint8_t getLanguageFileVersion[5] = {0x0F, 0x3A, 0xC0, 0x00, 0x00};
      // static constexpr uint8_t getMealTime[5] = {0x0F, 0x00, 0x38, 0x00, 0x00};
      // static constexpr uint8_t getMinute[5] = {0x0F, 0x36, 0xB3, 0x00, 0x00};
      // static constexpr uint8_t getMonth[5] = {0x0F, 0x36, 0xB9, 0x00, 0x00};
      // static constexpr uint8_t getSecond[5] = {0x0F, 0x36, 0xB1, 0x00, 0x00};
      // static constexpr uint8_t getSpeedLeft[5] = {0x0F, 0x24, 0xC0, 0x00, 0x00};
      // static constexpr uint8_t getSpeedRight[5] = {0x0F, 0x24, 0xBF, 0x00, 0x00};
      // static constexpr uint8_t getSquareModePercent[5] = {0x0F, 0x01, 0x34, 0x00, 0x00};
      // static constexpr uint8_t getSquareModeStatus[5] = {0x0F, 0x01, 0x37, 0x00, 0x00};
      // static constexpr uint8_t getSquareModeStatus[5] = {0x0F, 0x01, 0x38, 0x00, 0x00};
      // static constexpr uint8_t getStatus[5] = {0x0F, 0x01, 0xF1, 0x00, 0x00};
      // static constexpr uint8_t getTimerDays[5] = {0x0F, 0x4A, 0x50, 0x00, 0x00};
      // static constexpr uint8_t getTimerStatus[5] = {0x0F, 0x4A, 0x4E, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StartHour[5] = {0x0F, 0x4A, 0x3C, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StartMinute[5] = {0x0F, 0x4A, 0x3D, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StopHour[5] = {0x0F, 0x4A, 0x3E, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StopMinute[5] = {0x0F, 0x4A, 0x3F, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StartHour[5] = {0x0F, 0x4A, 0x44, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StartMinute[5] = {0x0F, 0x4A, 0x45, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StopHour[5] = {0x0F, 0x4A, 0x46, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StopMinute[5] = {0x0F, 0x4A, 0x47, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StartHour[5] = {0x0F, 0x4A, 0x38, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StartMinute[5] = {0x0F, 0x4A, 0x39, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StopHour[5] = {0x0F, 0x4A, 0x3A, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StopMinute[5] = {0x0F, 0x4A, 0x3B, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StartHour[5] = {0x0F, 0x4A, 0x40, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StartMinute[5] = {0x0F, 0x4A, 0x41, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StopHour[5] = {0x0F, 0x4A, 0x42, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StopMinute[5] = {0x0F, 0x4A, 0x43, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimerDays[5] = {0x0F, 0x4A, 0x50, 0x00, 0x00};
      // static constexpr uint8_t getYear[5] = {0x0F, 0x36, 0xBD, 0x00, 0x00};
      // static constexpr uint8_t setKey0[5] = {0x0F, 0x80, 0x5F, 0x00, 0x00};
      // static constexpr uint8_t setKey1[5] = {0x0F, 0x80, 0x5F, 0x00, 0x01};
      // static constexpr uint8_t setKey2[5] = {0x0F, 0x80, 0x5F, 0x00, 0x02};
      // static constexpr uint8_t setKey3[5] = {0x0F, 0x80, 0x5F, 0x00, 0x03};
      // static constexpr uint8_t setKey4[5] = {0x0F, 0x80, 0x5F, 0x00, 0x04};
      // static constexpr uint8_t setKey5[5] = {0x0F, 0x80, 0x5F, 0x00, 0x05};
      // static constexpr uint8_t setKey6[5] = {0x0F, 0x80, 0x5F, 0x00, 0x06};
      // static constexpr uint8_t setKey7[5] = {0x0F, 0x80, 0x5F, 0x00, 0x07};
      // static constexpr uint8_t setKey8[5] = {0x0F, 0x80, 0x5F, 0x00, 0x08};
      // static constexpr uint8_t setKey9[5] = {0x0F, 0x80, 0x5F, 0x00, 0x09};
      // static constexpr uint8_t setPRG_A[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0A};
      // static constexpr uint8_t setPRG_B[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0B};
      // static constexpr uint8_t setPRG_C[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0C};
      // static constexpr uint8_t setKeyHOME[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0D};
      // static constexpr uint8_t setKeyMANAUTO[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0E};
      // static constexpr uint8_t setKeyC[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0F};
      // static constexpr uint8_t setKeyUP[5] = {0x0F, 0x80, 0x5F, 0x00, 0x10};
      // static constexpr uint8_t setKeyDOWN[5] = {0x0F, 0x80, 0x5F, 0x00, 0x11};
      // static constexpr uint8_t setKeyYES[5] = {0x0F, 0x80, 0x5F, 0x00, 0x12};
      // static constexpr uint8_t setModeMAN[5] = {0x0F, 0x81, 0x2C, 0x00, 0x00};
      // static constexpr uint8_t setModeAUTO[5] = {0x0F, 0x81, 0x2C, 0x00, 0x01};
      // static constexpr uint8_t setModeHOME[5] = {0x0F, 0x81, 0x2C, 0x00, 0x03};
      // static constexpr uint8_t setModeDEMO[5] = {0x0F, 0x81, 0x2C, 0x00, 0x04};
      // static constexpr uint8_t setTimerActive[5] = {0x0F, 0xCA, 0x4E, 0x00, 0x00};
      // static constexpr uint8_t setTimerInactive[5] = { 0x0F,
      //                                                  0xCA,
      //                                                  0x4E,
      //                                                  0x00,
      //                                                  0x01 }

      const std::list<const uint8_t *>
          pollingCommandList = {
              getBatteryLevel,
              getBatteryTemperature,
              getBatteryUsed,
              getBatteryVoltage,
              getBladeMotorSpeed,
              getChargingTime,
              getMowingTime,
              getFirmwareVersion,
              getModeCmd,
              getStatusCode,
              READ_STOP_CMD,
              // Additional registers for sensors
              getBatteryCapacity,
              getBatteryReturn,
              getBatteryTempCharged,
              getBatteryTempLoad,
              getTempNextCheck,
              getCuttingTime,
              getLoopQuality,
              getSquareStatus,
              getSquarePercent,
              getSquareRef,
              getSpeedRight,
              getSpeedLeft,
              getLanguage,
              getTimerActive,
              getTimer1StartH,
              getTimer1StartM,
              getTimer1StopH,
              getTimer1StopM,
              getTimer2StartH,
              getTimer2StartM,
              getTimer2StopH,
              getTimer2StopM,
              getClockSec,
              getClockMin,
              getClockHour};

      void sendCommands(int index);
      void checkUartRead();
      void setStopStatusFromCode(uint16_t val);
      void publishMode(uint16_t val);
      void publishStatus(uint16_t val);
      std::string formatHex(uint16_t v);
    };

  } // namespace mower
} // namespace esphome
