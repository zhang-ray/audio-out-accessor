#pragma once

#include "ReturnType.hpp"
#include "DeviceInfo.hpp"

class AOA {
public:
    virtual ReturnType init(DeviceInfo &deviceInfo) = 0;
    virtual ReturnType start() = 0;
    virtual ReturnType stop() = 0;
    virtual ReturnType deInit() = 0;
    virtual ~AOA() { }
};
