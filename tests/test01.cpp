#include <iostream>
#include <thread>
#include <fstream>

#include "aoa/Factory.hpp"
#include "aoa/AOA.hpp"


int main(void){
    auto &myAOA = Factory::get().create(Factory::Type::PULSE);
    DeviceInfo deviceInfo;
    if (myAOA.init(deviceInfo)){
        std::cout << "deviceInfo.monitor_source_name_ : "<<deviceInfo.monitor_source_name_<< std::endl;
        std::cout << "deviceInfo.monitor_description_ : "<<deviceInfo.monitor_description_<< std::endl;


        std::shared_ptr<std::ofstream> pFileDumper = nullptr;

        {
            for (int i = 0; i < 3; i++){
                pFileDumper.reset(new std::ofstream(std::string(std::to_string(i)+".pcm").c_str()));
                if (myAOA.start([=](const char *myBuffer, const size_t nByte){ /*dump file*/ pFileDumper->write(myBuffer, nByte);})){
                    std::this_thread::sleep_for(std::chrono::seconds(3));
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
