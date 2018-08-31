#pragma once

#include <string>


class DeviceInfo {
public:
    std::string monitor_source_name_;
    std::string monitor_description_;
    uint32_t rate_;
    uint8_t channels_;
};
