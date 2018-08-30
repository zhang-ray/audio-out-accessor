#pragma once

#include "Singleton.hpp"

class AOA;

class Factory : public Singleton<Factory> {
public:
    enum class Type{
        PULSE, // Linux PulseAudio
        WASAPI, // Windows API
    };

    AOA &create(const Type type);

    Factory(){}
    Factory(const Factory&) = delete;
    Factory(const Factory&&) = delete;
    Factory& operator=(Factory const&) = delete;
    Factory& operator=(Factory &&) = delete;
};
