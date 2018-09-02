#include <thread>
#include <fstream>
#include <iostream>

#include "aoa/Factory.hpp"
#include "aoa/AOA.hpp"


/****************************************/
/* Dumping audio-out PCM to local file  */
/****************************************/

int main(void){
    auto &myAOA = Factory::get().create(Factory::Type::PULSE);
    DeviceInfo deviceInfo;
    if (myAOA.init(deviceInfo)){
        std::cout << deviceInfo.monitor_description_ << ": "
                  << std::to_string(deviceInfo.rate_) << "Hz, "
                  << std::to_string(deviceInfo.channels_) << " channels "
                  << std::endl;

        auto duration = std::chrono::seconds(10);
        std::string fileName = "out.pcm";
        std::cout << "Dumping " << duration.count() << " seconds PCM to " << fileName << std::endl;

        std::ofstream ofs(fileName.c_str());

        if (myAOA.start([&](const char *myBuffer, const size_t nByte){ ofs.write(myBuffer, nByte);} )){
            std::this_thread::sleep_for(duration);
            myAOA.stop();
            ofs.close();

            std::cout << "Done." << std::endl;
        }
        myAOA.deInit();
    }

    return 0;
}
