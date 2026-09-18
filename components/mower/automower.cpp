#include "automower.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace mower
    {

        Automower::Automower(uart::UARTComponent *parent, uint32_t update_interval)
            : PollingComponent(update_interval), uart::UARTDevice(parent)
        {

            battery_temperature_sensor_ = new template_::TemplateSensor();
            battery_level_sensor_ = new template_::TemplateSensor();
            battery_used_sensor_ = new template_::TemplateSensor();
            battery_voltage_sensor_ = new template_::TemplateSensor();
            blade_motor_speed_sensor_ = new template_::TemplateSensor();
            charging_time_sensor_ = new template_::TemplateSensor();
            mowing_time_sensor_ = new template_::TemplateSensor();
            firmware_version_sensor_ = new template_::TemplateSensor();

            last_code_received_text_sensor_ = new template_::TemplateTextSensor();
            mode_text_sensor_ = new template_::TemplateTextSensor();
            status_text_sensor_ = new template_::TemplateTextSensor();
            status_message_text_sensor_ = new template_::TemplateTextSensor();
            pollingId_ = 0;
        }

        void Automower::set_battery_temperature_sensor(template_::TemplateSensor *s) { battery_temperature_sensor_ = s; }
        void Automower::set_battery_level_sensor(template_::TemplateSensor *s) { battery_level_sensor_ = s; }
        void Automower::set_battery_used_sensor(template_::TemplateSensor *s) { battery_used_sensor_ = s; }
        void Automower::set_battery_voltage_sensor(template_::TemplateSensor *s) { battery_voltage_sensor_ = s; }
        void Automower::set_blade_motor_speed_sensor(template_::TemplateSensor *s) { blade_motor_speed_sensor_ = s; }
        void Automower::set_charging_time_sensor(template_::TemplateSensor *s) { charging_time_sensor_ = s; }
        void Automower::set_mowing_time_sensor(template_::TemplateSensor *s) { mowing_time_sensor_ = s; }
        void Automower::set_firmware_version_sensor(template_::TemplateSensor *s) { firmware_version_sensor_ = s; }
        void Automower::set_battery_capacity_sensor(template_::TemplateSensor *s) { battery_capacity_sensor_ = s; }
        void Automower::set_battery_return_sensor(template_::TemplateSensor *s) { battery_return_sensor_ = s; }
        void Automower::set_battery_temp_charged_sensor(template_::TemplateSensor *s) { battery_temp_charged_sensor_ = s; }
        void Automower::set_battery_temp_load_sensor(template_::TemplateSensor *s) { battery_temp_load_sensor_ = s; }
        void Automower::set_temp_next_check_sensor(template_::TemplateSensor *s) { temp_next_check_sensor_ = s; }
        void Automower::set_cutting_time_sensor(template_::TemplateSensor *s) { cutting_time_sensor_ = s; }
        void Automower::set_loop_quality_sensor(template_::TemplateSensor *s) { loop_quality_sensor_ = s; }
        void Automower::set_square_status_sensor(template_::TemplateSensor *s) { square_status_sensor_ = s; }
        void Automower::set_square_percent_sensor(template_::TemplateSensor *s) { square_percent_sensor_ = s; }
        void Automower::set_square_ref_sensor(template_::TemplateSensor *s) { square_ref_sensor_ = s; }
        void Automower::set_speed_right_sensor(template_::TemplateSensor *s) { speed_right_sensor_ = s; }
        void Automower::set_speed_left_sensor(template_::TemplateSensor *s) { speed_left_sensor_ = s; }
        void Automower::set_language_sensor(template_::TemplateSensor *s) { language_sensor_ = s; }

        void Automower::set_last_code_received_text_sensor(template_::TemplateTextSensor *s) { last_code_received_text_sensor_ = s; }
        void Automower::set_mode_text_sensor(template_::TemplateTextSensor *s) { mode_text_sensor_ = s; }
        void Automower::set_status_text_sensor(template_::TemplateTextSensor *s) { status_text_sensor_ = s; }
        void Automower::set_status_message_text_sensor(template_::TemplateTextSensor *s) { status_message_text_sensor_ = s; }

        template_::TemplateSensor *Automower::get_battery_temperature_sensor() const { return battery_temperature_sensor_; }
        template_::TemplateSensor *Automower::get_blade_motor_speed_sensor() const
        {
            return blade_motor_speed_sensor_;
        }
        template_::TemplateSensor *Automower::get_battery_level_sensor() const { return battery_level_sensor_; }
        template_::TemplateSensor *Automower::get_battery_used_sensor() const { return battery_used_sensor_; }
        template_::TemplateSensor *Automower::get_battery_voltage_sensor() const { return battery_voltage_sensor_; }
        template_::TemplateSensor *Automower::get_charging_time_sensor() const { return charging_time_sensor_; }
        template_::TemplateSensor *Automower::get_mowing_time_sensor() const { return mowing_time_sensor_; }
        template_::TemplateSensor *Automower::get_firmware_version_sensor() const { return firmware_version_sensor_; }
        template_::TemplateSensor *Automower::get_battery_capacity_sensor() const { return battery_capacity_sensor_; }
        template_::TemplateSensor *Automower::get_battery_return_sensor() const { return battery_return_sensor_; }
        template_::TemplateSensor *Automower::get_battery_temp_charged_sensor() const { return battery_temp_charged_sensor_; }
        template_::TemplateSensor *Automower::get_battery_temp_load_sensor() const { return battery_temp_load_sensor_; }
        template_::TemplateSensor *Automower::get_temp_next_check_sensor() const { return temp_next_check_sensor_; }
        template_::TemplateSensor *Automower::get_cutting_time_sensor() const { return cutting_time_sensor_; }
        template_::TemplateSensor *Automower::get_loop_quality_sensor() const { return loop_quality_sensor_; }
        template_::TemplateSensor *Automower::get_square_status_sensor() const { return square_status_sensor_; }
        template_::TemplateSensor *Automower::get_square_percent_sensor() const { return square_percent_sensor_; }
        template_::TemplateSensor *Automower::get_square_ref_sensor() const { return square_ref_sensor_; }
        template_::TemplateSensor *Automower::get_speed_right_sensor() const { return speed_right_sensor_; }
        template_::TemplateSensor *Automower::get_speed_left_sensor() const { return speed_left_sensor_; }
        template_::TemplateSensor *Automower::get_language_sensor() const { return language_sensor_; }

        template_::TemplateTextSensor *Automower::get_last_code_received_text_sensor() const { return last_code_received_text_sensor_; }
        template_::TemplateTextSensor *Automower::get_mode_text_sensor() const { return mode_text_sensor_; }
        template_::TemplateTextSensor *Automower::get_status_text_sensor() const { return status_text_sensor_; }
        template_::TemplateTextSensor *Automower::get_status_message_text_sensor() const { return status_message_text_sensor_; }

        void Automower::setup()
        {
            ESP_LOGCONFIG("Automower", "Register cache initialized with %zu entries", register_values_.capacity());
        }

        void Automower::update()
        {
            // Legacy: do nothing. Pacing is now handled in loop().
        }

        void Automower::loop()
        {
            serviceBus();  // drain write queue before checking UART
            if (write_queue_.empty() && !awaiting_reply_)
            {
                uint32_t now = millis();
                if (now - last_send_time_ >= get_update_interval())
                {
                    pollingId_ = (pollingId_ + 1) % pollingCommandList.size();
                    sendCommands(pollingId_);
                }
            }
            checkUartRead();
        }

        void Automower::set_mode(const std::string &value)
        {
            if (value == "MAN")
            {
                queueFrame(MAN_DATA);
            }
            else if (value == "AUTO")
            {
                queueFrame(AUTO_DATA);
            }
            else if (value == "HOME")
            {
                queueFrame(HOME_DATA);
            }
            else if (value == "DEMO")
            {
                queueFrame(DEMO_DATA);
            }
            else
            {
                ESP_LOGE("Automower", "Unknown mode: %s", value.c_str());
            }
        }

        void Automower::set_stop(bool stop)
        {
            queueFrame(stop ? STOP_ON_DATA : STOP_OFF_DATA);
        }

        void Automower::set_left_motor(int value)
        {
            uint8_t data[5] = {0x0F, 0x92, 0x23, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
            queueFrame(data);
        }

        void Automower::set_right_motor(int value)
        {
            uint8_t data[5] = {0x0F, 0x92, 0x03, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
            queueFrame(data);
        }

        void Automower::key_back() { queueFrame(KEY_BACK); }
        void Automower::key_yes() { queueFrame(KEY_YES); }
        void Automower::key_num(uint8_t num)
        {
            uint8_t data[5] = {0x0F, 0x80, 0x5F, 0x00, num};
            queueFrame(data);
        }

        void Automower::write_register(uint16_t addr, uint8_t value)
        {
            uint8_t data[5] = {0x0F, 0xCA, static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr & 0xFF), value};
            queueFrame(data);
        }

        void Automower::queueFrame(const uint8_t *data)
        {
            write_queue_.push_back({{data[0], data[1], data[2], data[3], data[4]}});
        }

        void Automower::serviceBus()
        {
            while (!write_queue_.empty() && _writable)
            {
                auto frame = write_queue_.front();
                write_queue_.erase(write_queue_.begin());
                ESP_LOGD("Automower", "UART TX (queue): %02X %02X %02X %02X %02X",
                         frame.b[0], frame.b[1], frame.b[2], frame.b[3], frame.b[4]);
                write_array(frame.b, 5);
                // User writes (mode, stop, motor, keys) don't expect replies.
                // Release the bus immediately so poll replies and status updates
                // can be processed without waiting for a timeout.
                _writable = true;
            }
        }

        void Automower::sendCommands(int index)
        {
            if (index < (int)pollingCommandList.size())
            {
                if (!_writable)
                {
                    ESP_LOGD("Automower", "UART not writable, skipping command at index %d", index);
                    return;
                }
                auto it = pollingCommandList.begin();
                std::advance(it, index);
                ESP_LOGD("Automower", "UART TX: %02X %02X %02X %02X %02X", (*it)[0], (*it)[1], (*it)[2], (*it)[3], (*it)[4]);
                write_array(*it, 5);
                last_send_time_ = millis();
                _writable = false;
                // Record the expected reply address (bits 6-0 of bytes 1-2).
                expected_addr_ = (((*it)[1] & 0x7F) << 8) | (*it)[2];
                awaiting_reply_ = true;
            }
        }

        void Automower::checkUartRead()
        {
            while (available() > 0 && peek() != 0x0F)
                read();
            while (available() >= 5 && peek() == 0x0F)
            {
                uint8_t readData[5];
                read_array(readData, 5);
                uint16_t addr = ((readData[1] & 0x7F) << 8) | readData[2];

                // Only release the bus when the expected address arrives.
                // Keypad traffic from the mower falls through to the timeout.
                if (awaiting_reply_ && addr == expected_addr_)
                {
                    _writable = true;
                    awaiting_reply_ = false;
                }
                else if (awaiting_reply_ && millis() - last_send_time_ > UART_REPLY_TIMEOUT_MS)
                {
                    // Timeout: no reply arrived in time, release the bus.
                    ESP_LOGW("Automower", "UART reply timeout for 0x%04X", expected_addr_);
                    _writable = true;
                    awaiting_reply_ = false;
                }

                ESP_LOGD("Automower", "UART RX: %02X %02X %02X %02X %02X", readData[0], readData[1], readData[2], readData[3], readData[4]);
                uint16_t val = (readData[4] << 8) | readData[3];
                ESP_LOGD("Automower", "Decoded: addr=0x%04X val=0x%04X", addr, val);

                // Publish the raw frame as last received code.
                // Address bytes keep the write bit so 812C (ack'd write) is
                // distinguishable from 012C (read reply).
                if (last_code_received_text_sensor_)
                {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%02X%02X=%02X%02X",
                             readData[1], readData[2], readData[4], readData[3]);
                    last_code_received_text_sensor_->publish_state(buf);
                }

                switch (addr)
                {
                case 0x012C:
                    publishMode(val);
                    break;
                case 0x01F1:
                    publishStatus(val);
                    break;
                case 0x01EC:
                    if (charging_time_sensor_)
                        charging_time_sensor_->publish_state(val);
                    store_register(0x01EC, val);
                    break;
                case 0x0056:
                    if (mowing_time_sensor_)
                        mowing_time_sensor_->publish_state(val);
                    store_register(0x0056, val);
                    break;
                case 0x01EF:
                    if (battery_level_sensor_)
                        battery_level_sensor_->publish_state(val);
                    store_register(0x01EF, val);
                    break;
                case 0x0233:
                    if (battery_temperature_sensor_)
                        battery_temperature_sensor_->publish_state(val);
                    store_register(0x0233, val);
                    break;
                case 0x2EE0:
                    if (battery_used_sensor_)
                        battery_used_sensor_->publish_state(val);
                    store_register(0x2EE0, val);
                    break;
                case 0x2EEA:
                    if (blade_motor_speed_sensor_)
                        blade_motor_speed_sensor_->publish_state(val);
                    store_register(0x2EEA, val);
                    break;
                case 0x2EF4:
                    if (battery_voltage_sensor_)
                        battery_voltage_sensor_->publish_state(val / 1000.0f);
                    store_register(0x2EF4, val);
                    break;
                case 0x3390:
                    if (firmware_version_sensor_)
                        firmware_version_sensor_->publish_state(val);
                    store_register(0x3390, val);
                    break;
                case 0x012F:
                    setStopStatusFromCode(val);
                    store_register(0x012F, val);
                    break;
                // Additional registers for sensors
                case 0x00B1:
                    if (battery_capacity_sensor_)
                        battery_capacity_sensor_->publish_state(val);
                    store_register(0x00B1, val);
                    break;
                case 0x01F0:
                    if (battery_return_sensor_)
                        battery_return_sensor_->publish_state(val);
                    store_register(0x01F0, val);
                    break;
                case 0x0234:
                    if (battery_temp_charged_sensor_)
                        battery_temp_charged_sensor_->publish_state(val);
                    store_register(0x0234, val);
                    break;
                case 0x0235:
                    if (battery_temp_load_sensor_)
                        battery_temp_load_sensor_->publish_state(val);
                    store_register(0x0235, val);
                    break;
                case 0x0236:
                    if (temp_next_check_sensor_)
                        temp_next_check_sensor_->publish_state(val);
                    store_register(0x0236, val);
                    break;
                case 0x0038:
                    if (cutting_time_sensor_)
                        cutting_time_sensor_->publish_state(val);
                    store_register(0x0038, val);
                    break;
                case 0x01B9:
                    if (loop_quality_sensor_)
                        loop_quality_sensor_->publish_state(val);
                    store_register(0x01B9, val);
                    break;
                case 0x0138:
                    if (square_status_sensor_)
                        square_status_sensor_->publish_state(val);
                    store_register(0x0138, val);
                    break;
                case 0x0134:
                    if (square_percent_sensor_)
                        square_percent_sensor_->publish_state(val);
                    store_register(0x0134, val);
                    break;
                case 0x0137:
                    if (square_ref_sensor_)
                        square_ref_sensor_->publish_state(val);
                    store_register(0x0137, val);
                    break;
                case 0x24BF:
                    if (speed_right_sensor_)
                        speed_right_sensor_->publish_state(val);
                    store_register(0x24BF, val);
                    break;
                case 0x24C0:
                    if (speed_left_sensor_)
                        speed_left_sensor_->publish_state(val);
                    store_register(0x24C0, val);
                    break;
                case 0x3AC0:
                    if (language_sensor_)
                        language_sensor_->publish_state(val);
                    store_register(0x3AC0, val);
                    break;
                case 0x4A4E:
                    store_register(0x4A4E, val);
                    break;
                case 0x4A38:
                    store_register(0x4A38, val);
                    break;
                case 0x4A39:
                    store_register(0x4A39, val);
                    break;
                case 0x4A3A:
                    store_register(0x4A3A, val);
                    break;
                case 0x4A3B:
                    store_register(0x4A3B, val);
                    break;
                case 0x4A40:
                    store_register(0x4A40, val);
                    break;
                case 0x4A41:
                    store_register(0x4A41, val);
                    break;
                case 0x4A42:
                    store_register(0x4A42, val);
                    break;
                case 0x4A43:
                    store_register(0x4A43, val);
                    break;
                case 0x36B1:
                    store_register(0x36B1, val);
                    break;
                case 0x36B3:
                    store_register(0x36B3, val);
                    break;
                case 0x36B5:
                    store_register(0x36B5, val);
                    break;
                default:
                    ESP_LOGW("Automower", "Unhandled address: 0x%04X with value 0x%04X", addr, val);
                    break;
                }
            }
        }

        void Automower::setStopStatusFromCode(uint16_t val)
        {
            stopStatus = (val == 0x0002);
        }

        void Automower::publishMode(uint16_t val)
        {
            if (!mode_text_sensor_)
                return;
            std::string mode;
            switch (val)
            {
            case 0x0000:
                mode = "MAN";
                break;
            case 0x0001:
                mode = "AUTO";
                break;
            case 0x0002:
                mode = "Charging then AUTO";
                break;
            case 0x0003:
                mode = "HOME";
                break;
            case 0x0004:
                mode = "DEMO";
                break;
            default:
                mode = "MODE_" + formatHex(val);
                break;
            }
            mode_text_sensor_->publish_state(mode);
        }

        void Automower::publishStatus(uint16_t val)
        {
            if (!status_message_text_sensor_)
                return;
            if (val == 0x0410) // Collission or dodge
                return;
            std::string status;
            std::string status_message;
            switch (val)
            {
            case 0x0010:
                status = "error";
                status_message = "Outside working area";
                break;
            case 0x0012:
                status = "error";
                status_message = "LBV Low battery voltage";
                break;
            case 0x001e:
                status = "error";
                status_message = "Cutting system blocked";
                break;
            case 0x03EA:
                status = "mowing";
                status_message = "MIP Mowing in progress";
                break;
            case 0x0006:
                status = "error";
                status_message = "Left wheel motor blocked";
                break;
            case 0x0008:
                status = "error";
                status_message = "Right wheel motor blocked";
                break;
            case 0x0018:
                status = "error";
                status_message = "Wheel slip";
                break;                
            case 0x000C:
                status = "error";
                status_message = "No loop signal";
                break;
            case 0x001A:
                status = "error";
                status_message = "Station blocked";
                break;
            case 0x0022:
                status = "error";
                status_message = "Mower lifted";
                break;
            case 0x0034:
                status = "error";
                status_message = "Station no contact";
                break;
            case 0x0036:
                status = "error";
                status_message = "Pin expired";
                break;
            case 0x03E8:
                status = "mowing";
                status_message = "Leaving station";
                break;
            case 0x03EE:
                status = "mowing";
                status_message = "Start mowing";
                break;
            case 0x03F0:
                status = "mowing";
                status_message = "Mowing started";
                break;
            case 0x03F4:
                status = "mowing";
                status_message = "Start mowing2";
                break;
            case 0x03F6:
                status = "docked";
                status_message = "Charging";
                break;
            case 0x03F8:
                status = "docked";
                status_message = "Waiting timer2";
                break;
            case 0x1016:
                status = "docked";
                status_message = "Waiting timer";
                break;
            case 0x0400:
                status = "docked";
                status_message = "Parking in station";
                break;
            case 0x040C:
                status = "mowing";
                status_message = "Square mode";
                break;
            case 0x040E:
                status = "error";
                status_message = "Stuck";
                break;
            case 0x0412:
                status = "returning";
                status_message = "Searching";
                break;
            case 0x0414:
                status = "paused";
                status_message = "Stop";
                break;
            case 0x0418:
                status = "docked";
                status_message = "Docking";
                break;
            case 0x041A:
                status = "mowing";
                status_message = "Leaving station";
                break;
            case 0x041C:
                status = "error";
                status_message = "Error";
                break;
            case 0x0420:
                status = "idle";
                status_message = "Waiting for use";
                break;
            case 0x0422:
                status = "mowing";
                status_message = "Follow boundary";
                break;
            case 0x0424:
                status = "returning";
                status_message = "Found N-Signal";
                break;
            case 0x0426:
                status = "error";
                status_message = "Stuck";
                break;
            case 0x0428:
                status = "returning";
                status_message = "Searching";
                break;
            case 0x042E:
                status = "mowing/returning";
                status_message = "Follow guide line";
                break;
            case 0x0430:
                status = "returning";
                status_message = "Follow loop wire";
                break;
            default:
                status = "error";
                status_message = "STATUS_" + formatHex(val);
                break;
            }
            status_text_sensor_->publish_state(status);
            status_message_text_sensor_->publish_state(status_message);
        }

        std::string Automower::formatHex(uint16_t v)
        {
            char s[16];
            sprintf(s, "%04x", v);
            return std::string(s);
        }

        void Automower::store_register(uint16_t addr, uint16_t val)
        {
            auto it = std::lower_bound(register_values_.begin(), register_values_.end(), addr,
                                       [](const std::pair<uint16_t, uint16_t> &e, uint16_t a) {
                                           return e.first < a;
                                       });
            if (it != register_values_.end() && it->first == addr)
                it->second = val;
            else
                register_values_.insert(it, std::make_pair(addr, val));
        }

        float Automower::get_register(uint16_t addr, bool is_signed)
        {
            auto it = std::lower_bound(register_values_.begin(), register_values_.end(), addr,
                                       [](const std::pair<uint16_t, uint16_t> &e, uint16_t a) {
                                           return e.first < a;
                                       });
            if (it != register_values_.end() && it->first == addr)
            {
                uint16_t val = it->second;
                if (is_signed)
                {
                    int16_t signed_val = static_cast<int16_t>(val);
                    return static_cast<float>(signed_val);
                }
                return static_cast<float>(val);
            }
            return NAN;
        }

    } // namespace automower
} // namespace esphome
