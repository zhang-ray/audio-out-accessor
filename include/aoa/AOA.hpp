#pragma once

#include "ReturnType.hpp"

class AOA {
public:
    virtual ReturnType init() = 0;
    virtual ReturnType start() = 0;
    virtual ReturnType stop() = 0;
    virtual ReturnType deInit() = 0;
    virtual ~AOA() { }
};
