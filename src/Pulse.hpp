#pragma once

#include "aoa/AOA.hpp"
#include "aoa/Singleton.hpp"
#include "PulseImpl.hpp"

#include <memory>

class Pulse : public AOA, public Singleton<Pulse> {
public:
    ReturnType init(DeviceInfo &deviceInfo) override {
        if (pulseImpl_){
            return "isInited";
        }
        pulseImpl_.reset(new PulseImpl());
        return pulseImpl_->dumpDevices(deviceInfo);
    }


    ReturnType start(std::function<void(const char *, const size_t nByte)> cb) override {
        return pulseImpl_->start(cb);

    }


    ReturnType stop() override {
        return pulseImpl_->stop();
    }


    ReturnType deInit() override {
        if (!pulseImpl_){
            return "!isInited";
        }

        pulseImpl_=nullptr;
        return 0;
    }


private:
    std::shared_ptr<PulseImpl> pulseImpl_ = nullptr;
};
