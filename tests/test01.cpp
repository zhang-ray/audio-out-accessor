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
        if(true){
            for (int i = 0; i < 3; i++){
                if (myAOA.start()){
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    myAOA.stop();
                }
                else {
                    break;
                }
            }
        }
        else {
            // stress test
            for (int i = 0; i < 1000; i++){
                if (myAOA.start()){
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    myAOA.stop();
                }
                else {
                    break;
                }
            }
        }

        myAOA.deInit();
    }
    return 0;
}
