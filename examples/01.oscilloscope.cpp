#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#include "aoa/Factory.hpp"
#include "aoa/AOA.hpp"

/******************************************************************************************************************/
/* This example is a oscilloscope, ploting first channel of your system's audio-out (S16LE is required) by SDL2.  */
/******************************************************************************************************************/

int main(void){
    auto &myAOA = Factory::get().create(Factory::Type::PULSE);
    DeviceInfo deviceInfo;
    if (myAOA.init(deviceInfo)){
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        auto const width = 600;
        auto const height = 200;
        auto const S16LE_SIZE = 2;
        auto const maxAudioValue = (1<<15);

        if (SDL_Init(SDL_INIT_VIDEO) == 0){
            if (SDL_CreateWindowAndRenderer(width,height, 0, &window, &renderer) == 0) {
                SDL_SetWindowTitle(window, [=]()-> std::string {
                    std::string str("AOA oscilloscope ( ");
                    str+=deviceInfo.monitor_description_;
                    str+=" ";
                    str+=std::to_string(deviceInfo.rate_);
                    str+="Hz ";
                    str+=std::to_string(deviceInfo.channels_);
                    str+=" channels )";
                    return str.c_str();
                }().c_str());

                bool done = false;

                if (myAOA.start([=, &done](const char *myBuffer, const size_t nByte){
                    auto nSample = nByte/(deviceInfo.channels_*S16LE_SIZE);

                    if (nSample<100) return; // nByte is not constant from PulseAudio. Let's remove small fragment

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, 0x22, 0x77, 0x00, 0x33);

                    auto mapper = [=](std::array<double, 2> in) -> std::array<double, 2> {
                        return {in[0] * width/nSample, in[1]*height/2/maxAudioValue+height/2 };
                    };
                    auto mapperWrapper = [=](int i) -> std::array<double, 2> {
                        return {(double)i, (double)(myBuffer[i*deviceInfo.channels_*S16LE_SIZE+1]*256+myBuffer[deviceInfo.channels_*S16LE_SIZE])};
                    };

                    for (int i = 0; i < nSample-1; i++){
                        auto outVec0 = mapper( mapperWrapper(i));
                        auto outVec1 = mapper( mapperWrapper(i+1));
                        SDL_RenderDrawLine(renderer, outVec0[0], outVec0[1],outVec1[0], outVec1[1]);
                    }

                    SDL_RenderPresent(renderer);
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            done = true;
                        }
                    }
                })){
                    for (;;){
                        if (done){
                            std::this_thread::sleep_for(std::chrono::microseconds(100));
                            myAOA.stop();
                            break;
                        }
                    }
                    myAOA.deInit();
                }
            }
            if (renderer) SDL_DestroyRenderer(renderer);
            if (window) SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }

    return 0;
}
