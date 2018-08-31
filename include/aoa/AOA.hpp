#pragma once

#include "ReturnType.hpp"
#include "DeviceInfo.hpp"

#include <functional>

class AOA {
public:
    virtual ReturnType init(DeviceInfo &deviceInfo) = 0;
    virtual ReturnType start(std::function<void(const char *, const size_t nByte)> cb) = 0;
    virtual ReturnType stop() = 0;
    virtual ReturnType deInit() = 0;
    virtual ~AOA() { }
};
