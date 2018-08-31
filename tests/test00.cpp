#include <iostream>
#include <thread>

#include "aoa/Factory.hpp"
#include "aoa/AOA.hpp"


int main(void){
    auto &myAOA = Factory::get().create(Factory::Type::PULSE);
    DeviceInfo deviceInfo;
    if (myAOA.init(deviceInfo)){
        std::cout << "deviceInfo.monitor_source_name_ : "<<deviceInfo.monitor_source_name_<< std::endl;
        std::cout << "deviceInfo.monitor_description_ : "<<deviceInfo.monitor_description_<< std::endl;
        myAOA.deInit();
    }
    return 0;
}
