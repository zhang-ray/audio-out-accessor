#include <iostream>
#include <thread>

#include "aoa/Factory.hpp"
#include "aoa/AOA.hpp"


int main(void){
    auto &myAOA = Factory::get().create(Factory::Type::PULSE);
    if (myAOA.init()){
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
